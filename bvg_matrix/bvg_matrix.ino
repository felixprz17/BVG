// BVG Abfahrtsanzeige - 128x32 HUB75 (2x 64x32) auf dem Adafruit Matrix Portal S3
// Arduino-Version: Das Display wird per DMA/Hardware angesteuert und ist damit voellig
// unabhaengig von WLAN/HTTPS -> kein Flackern beim Abfragen.
//
// Bibliotheken: ESP32 HUB75 LED MATRIX PANEL DMA Display (mrfaptastic), Adafruit GFX Library,
//               ArduinoJson (>= 7)   (U8g2 wird nicht mehr gebraucht)
// Board:        "Adafruit MatrixPortal ESP32-S3" (esp32 Core von Espressif), Partition mit OTA
//
// Aufbau:  logic.h    reine Logik (am PC getestet)
//          bvgfont.h  eigene Bitmap-Schriften inkl. fetter BVG-Schrift (am PC getestet)
//          web_ui.h   Weboberflaeche (erzeugt aus ui/*.html mit tools/gen_html.py)

#include <Arduino.h>
#include <DNSServer.h>
#include <ESPmDNS.h>
#include <HTTPClient.h>
#include <Preferences.h>
#include <Update.h>
#include <WebServer.h>
#include <WiFi.h>
#include <WiFiClientSecure.h>
#include <ESP32-HUB75-MatrixPanel-I2S-DMA.h>
#include <time.h>

#include <map>
#include <set>

#include "bvgfont.h"
#include "logic.h"
#include "web_ui.h"

// TLS-Handshakes (auch aus Web-Handlern) brauchen mehr Stack als die 8 kB-Vorgabe
#ifdef SET_LOOP_TASK_STACK_SIZE
SET_LOOP_TASK_STACK_SIZE(16 * 1024);
#endif

// ------------------------------------------------------------------ Konfiguration
static const char* FW_VERSION = "arduino-1.0";
static const char* HOSTNAME = "bvg-matrix";
static const char* AP_SSID = "BVG-Matrix-Setup";
// Die oeffentlichen transport.rest-Server liefern zeitweise 503 -> zweiter Server als Ausweichziel
static const char* BVG_HOSTS[] = {"https://v6.bvg.transport.rest", "https://v6.vbb.transport.rest"};
static int hostIdx = 0;
// Einstellungen liegen im NVS-Flash (Preferences) - funktioniert mit jedem Partitionsschema
static const char* NVS_NS = "bvg";
// Feinjustierung (Pixel) - einstellbar in der Weboberflaeche (Anzeige -> Feinjustierung)
int gShiftY = 1;   // gilt fuer allen Text; positiv = nach unten
static const char* TZ_BERLIN = "CET-1CEST,M3.5.0,M10.5.0/3";

// Matrix Portal S3: HUB75-Pins (Adafruit-Boarddefinition)
static const int PIN_R1 = 42, PIN_G1 = 41, PIN_B1 = 40, PIN_R2 = 38, PIN_G2 = 39, PIN_B2 = 37;
static const int PIN_A = 45, PIN_B = 36, PIN_C = 48, PIN_D = 35;
static const int PIN_LAT = 47, PIN_OE = 14, PIN_CLK = 2;
static const int PIN_BTN_DOWN = 7;

static const int PANEL_W = 64, PANEL_H = 32, PANEL_CHAIN = 2;
static const int WIDTH = PANEL_W * PANEL_CHAIN, HEIGHT = PANEL_H;

// Spalten (128 px breit)
static const int X_LABEL = 0, X_ZIEL = 20, TIME_GAP = 3;
static const uint8_t LATE_RGB[3] = {255, 40, 40};   // verspaetete Abfahrten

struct Design {
  const char* name;
  uint8_t ziel[3], zeit1[3], zeit2[3], now[3], divider[3];
  bool override_label;
  uint8_t label[3];
  bool bold;   // fette Schrift (2 Pixel dicke Striche) wie auf den Zugzielanzeigern der Berliner U-Bahn
};
static const Design DESIGNS[] = {
    {"modern", {200, 200, 200}, {255, 200, 0}, {130, 130, 130}, {0, 255, 80}, {25, 25, 35}, false, {0, 0, 0}, false},
    {"bvg", {255, 130, 0}, {255, 130, 0}, {255, 130, 0}, {255, 130, 0}, {45, 22, 0}, true, {255, 130, 0}, true},   // alles im BVG-Orange, fett
    {"minimal", {180, 180, 180}, {255, 255, 255}, {110, 110, 110}, {255, 255, 255}, {20, 20, 20}, true, {255, 255, 255}, false},
    {"night", {0, 80, 60}, {0, 100, 80}, {0, 60, 45}, {0, 120, 60}, {0, 10, 8}, true, {0, 60, 120}, false},
    {"amber", {200, 100, 0}, {255, 160, 0}, {150, 80, 0}, {255, 200, 0}, {30, 15, 0}, true, {255, 140, 0}, false},
};

// ------------------------------------------------------------------ Zustand
MatrixPanel_I2S_DMA* dma = nullptr;
WebServer server(80);

SemaphoreHandle_t mtx;  // schuetzt cfgDoc und cache
JsonDocument cfgDoc;
bvg::Cache cache;
std::vector<bvg::Row> lastRows;

volatile bool running = true;
volatile bool fetchForce = true;
volatile bool newData = false;
volatile bool redrawNow = false;
volatile bool rebootRequested = false;
volatile bool cfgDirty = false;
volatile bool otaBusy = false;
uint32_t nextFetchMs = 0;
int tick = 0;

// Diagnose (Weboberflaeche: /api/debug)
String dbgLast = "noch keine Abfrage";
int dbgCode = 0, dbgOk = 0, dbgFail = 0;
uint32_t dbgBytes = 0;

struct Lock {
  Lock() { xSemaphoreTake(mtx, portMAX_DELAY); }
  ~Lock() { xSemaphoreGive(mtx); }
};

// ------------------------------------------------------------------ Dateien & Konfiguration
bool nvsPut(const char* key, const String& v) {
  Preferences p;
  if (!p.begin(NVS_NS, false)) return false;
  size_t n = p.putBytes(key, v.c_str(), v.length());
  p.end();
  return n == v.length();
}

bool nvsGet(const char* key, String& out) {
  Preferences p;
  if (!p.begin(NVS_NS, true)) return false;
  size_t n = p.getBytesLength(key);
  bool ok = n > 0 && n < 8192;
  if (ok) {
    std::vector<char> buf(n + 1, 0);
    p.getBytes(key, buf.data(), n);
    out = String(buf.data());
  }
  p.end();
  return ok;
}

void nvsRemove(const char* key) {
  Preferences p;
  if (p.begin(NVS_NS, false)) {
    p.remove(key);
    p.end();
  }
}

void saveConfigLocked() {
  String out;
  serializeJson(cfgDoc, out);
  if (!nvsPut("config", out)) Serial.println("Konfiguration nicht gespeichert");
}

void loadConfig() {
  Lock l;
  cfgDoc.clear();
  String txt;
  if (nvsGet("config", txt)) {
    if (deserializeJson(cfgDoc, txt) || !cfgDoc.is<JsonObject>()) cfgDoc.clear();
  }
  bvg::merge_defaults(cfgDoc);
}

String cfgString(const char* key, const char* def) {
  Lock l;
  return String((const char*)(cfgDoc[key] | def));
}
int cfgInt(const char* key, int def) {
  Lock l;
  return cfgDoc[key] | def;
}
bool cfgBool(const char* key, bool def) {
  Lock l;
  return cfgDoc[key] | def;
}

bool loadWifi(String& ssid, String& pass) {
  String txt;
  if (!nvsGet("wifi", txt)) return false;
  JsonDocument d;
  if (deserializeJson(d, txt) || strlen(d["ssid"] | "") == 0) return false;
  ssid = (const char*)d["ssid"];
  pass = String((const char*)(d["password"] | ""));
  return true;
}

// ------------------------------------------------------------------ Display
static uint16_t c565(const uint8_t* c) { return dma->color565(c[0], c[1], c[2]); }

// Eigene Bitmap-Schriften (bvgfont.h): size 0/1/2 = Klein/Normal/Gross, bold = fette BVG-Schrift.
static void putPx(int x, int y, uint16_t color) {
  if (x >= 0 && x < WIDTH && y >= 0 && y < HEIGHT) dma->drawPixel(x, y, color);
}

// Text links bei x: die erste Tintenspalte liegt genau auf x (damit z. B. "140" nicht 1 Pixel spaeter beginnt als "U7").
// top = oberste Zeile der Grossbuchstaben
void drawText(int size, bool bold, const std::string& text, int x, int top, uint16_t color) {
  if (text.empty()) return;
  const bvgfont::Font& f = bvgfont::pick(size, bold);
  if (x < 0) x = 0;   // negative Koordinaten wuerden am rechten Rand wieder auftauchen
  bvgfont::draw(f, text, x, top + gShiftY + f.asc, [&](int px, int py) { putPx(px, py, color); });
}

// rechtsbuendig: rightX = Pixelspalte direkt hinter der letzten Tintenspalte
void drawTextRight(int size, bool bold, const std::string& text, int rightX, int top, uint16_t color) {
  if (text.empty()) return;
  const bvgfont::Font& f = bvgfont::pick(size, bold);
  int x = rightX - bvgfont::inkExtent(f, text).width;
  if (x < 0) x = 0;
  bvgfont::draw(f, text, x, top + gShiftY + f.asc, [&](int px, int py) { putPx(px, py, color); });
}

// Index in DESIGNS (int statt Design&, sonst scheitert der Arduino-Prototyp vor der struct-Definition)
int currentDesignIdx() {
  String dname = cfgString("design", "modern");
  int idx = 0;
  for (int i = 0; i < (int)(sizeof(DESIGNS) / sizeof(DESIGNS[0])); i++)
    if (dname == DESIGNS[i].name) idx = i;
  return idx;
}

// fett, wenn das Design es vorgibt oder "Fette Schrift" in der Weboberflaeche aktiv ist
bool wantBold() { return DESIGNS[currentDesignIdx()].bold || cfgBool("font_bold", false); }

void showBrightness(int pct) {
  if (pct < 1) pct = 1;
  if (pct > 100) pct = 100;
  dma->setBrightness8((uint8_t)(pct * 255 / 100));
}

void showMessage(std::initializer_list<String> lines, uint8_t r = 200, uint8_t g = 200, uint8_t b = 200) {
  if (!dma) return;
  gShiftY = cfgInt("offset_y", 1);
  dma->clearScreen();
  showBrightness(50);
  int i = 0;
  for (const String& s : lines) {
    if (i >= 4) break;
    drawText(1, false, s.c_str(), 0, i * 8, dma->color565(r, g, b));
    i++;
  }
  dma->flipDMABuffer();
}

int currentBrightness() {
  if (cfgBool("auto_brightness", false) && time(nullptr) > 1700000000) {
    struct tm t;
    time_t now = time(nullptr);
    localtime_r(&now, &t);
    int ds = cfgInt("auto_brightness_day_start", 7), ns = cfgInt("auto_brightness_night_start", 22);
    return (t.tm_hour >= ds && t.tm_hour < ns) ? cfgInt("auto_brightness_day", 80)
                                              : cfgInt("auto_brightness_night", 30);
  }
  return cfgInt("brightness", 80);
}

// Zeichnet die Zeilen einer Seite. Alle Spaltenbreiten richten sich nach der Schrift (normal: 3 Zeichen, fett: gemessen).
void drawRows(const std::vector<bvg::Row>& rows, int size, bool bold) {
  const Design* d = &DESIGNS[currentDesignIdx()];
  const bvgfont::Font& f = bvgfont::pick(size, bold);
  int fh = bvg::FONT_H[size];
  bool z2 = cfgBool("show_second_time", true);
  int offLabel = cfgInt("offset_label_x", 0), offZiel = cfgInt("offset_x", -2), offTime = cfgInt("offset_time_x", -1);
  gShiftY = cfgInt("offset_y", 1);

  dma->clearScreen();
  showBrightness(currentBrightness());
  int n = (int)rows.size();
  if (n > 0) {
    int rowH = HEIGHT / n;
    // Zeitspalte: normale Schrift = 3 Zeichen breit, fette Schrift (proportional) = breitester Wert
    int cw = 3 * bvgfont::glyphFor(f, '8').adv;
    if (bold) {
      cw = 0;
      static const char* sample[] = {"now", "88'", ">99", "00'"};
      for (const char* s : sample) cw = max(cw, bvgfont::inkExtent(f, s).width);
    }
    int right2 = WIDTH + offTime;
    int right1 = (z2 ? WIDTH - cw - TIME_GAP : WIDTH) + offTime;
    // Label-Spalte: das Ziel beginnt nach dem breitesten Label (mindestens bei X_ZIEL)
    int labelW = 0;
    for (const bvg::Row& r : rows) labelW = max(labelW, bvgfont::inkExtent(f, r.label).width);
    int zielX = max(X_ZIEL + offZiel, X_LABEL + offLabel + labelW + 3);
    int zielMaxW = (right1 - cw) - 2 - zielX;

    for (int i = 0; i < n; i++) {
      const bvg::Row& r = rows[i];
      int yRow = i * rowH;
      int y = yRow + max(0, (rowH - fh) / 2);
      uint16_t lc = d->override_label ? c565(d->label) : dma->color565(r.r, r.g, r.b);
      drawText(size, bold, r.label, X_LABEL + offLabel, y, lc);
      drawText(size, bold, bvgfont::fit(f, r.ziel, zielMaxW), zielX, y, c565(d->ziel));
      if (!r.zeit1.empty())
        drawTextRight(size, bold, r.zeit1, right1, y, r.late1 ? c565(LATE_RGB) : c565(r.minutes <= 1 ? d->now : d->zeit1));
      if (z2 && !r.zeit2.empty())
        drawTextRight(size, bold, r.zeit2, right2, y, r.late2 ? c565(LATE_RGB) : c565(d->zeit2));
    }
  }
  dma->flipDMABuffer();   // Doppelpuffer: Bild wird erst komplett fertig gezeichnet, dann umgeschaltet
}

void redraw(bool advanceTick) {
  // Einstellungen zuerst lesen: cfgString/cfgBool nehmen selbst den Mutex
  bool bold = wantBold();
  bool hideBr = cfgBool("hide_brackets", false), hideBerlin = cfgBool("hide_berlin", false);
  String fontKey = cfgString("font_size", "5x7");
  int size = bvg::effective_font(fontKey.c_str(), bold);
  std::vector<bvg::Row> rows;
  int pages = 1;
  {
    Lock l;
    JsonArray stops = cfgDoc["stops"].as<JsonArray>();
    pages = bvg::page_count((int)stops.size(), size);
    // tick: Seitenwechsel bei jedem Schritt, Haupt-/Alternativ-Linie wechselt nach einem kompletten Durchlauf
    rows = bvg::build_rows(cache, tick / pages, stops, fontKey.c_str(), (int64_t)time(nullptr), millis() / 1000.0,
                           cfgDoc["ziel_alias"].as<JsonObject>(), bold, hideBr, hideBerlin);
  }
  // Diagnose direkt auf dem Display, solange noch gar keine Daten da sind
  bool anyData = false;
  for (auto& r : rows)
    if (!r.zeit1.empty()) anyData = true;
  if (!anyData) {
    std::string msg;
    if (time(nullptr) < 1700000000) msg = "keine Zeit";
    else if (dbgOk + dbgFail == 0) msg = "wartet...";
    else if (dbgFail > 0 && dbgOk == 0) msg = "E" + std::to_string(dbgCode);
    if (!msg.empty())
      for (auto& r : rows) r.ziel = msg;
  }
  lastRows = rows;
  // Passen nicht alle Zeilen in die gewaehlte Schrift, wird seitenweise gezeigt
  int per = bvg::rows_per_page(size);
  int first = (tick % pages) * per;
  int last = min((int)rows.size(), first + per);
  std::vector<bvg::Row> pageRows;
  if (first < last) pageRows.assign(rows.begin() + first, rows.begin() + last);
  drawRows(pageRows, size, bold);
  if (advanceTick) tick++;
}

// ------------------------------------------------------------------ BVG-API
// GET -> JSON (gefiltert). Liefert true bei HTTP 200 + gueltigem JSON.

bool apiGetOnce(const String& url, bool http10, JsonDocument& doc, JsonDocument& filter) {
  WiFiClientSecure client;
  client.setInsecure();   // kein Zertifikats-Pinning (oeffentliche Abfahrtsdaten)
  HTTPClient http;
  http.setConnectTimeout(8000);
  http.setTimeout(12000);
  http.setReuse(false);
  http.setUserAgent("bvg-matrix/1.0");
  if (http10) http.useHTTP10(true);   // keine "chunked"-Antwort -> Stream direkt parsbar
  if (!http.begin(client, url)) {
    dbgLast = "http.begin fehlgeschlagen";
    return false;
  }
  int code = http.GET();
  dbgCode = code;
  bool ok = false;
  if (code == 200) {
    DeserializationError err;
    if (http10) {
      err = deserializeJson(doc, http.getStream(), DeserializationOption::Filter(filter));
    } else {
      String body = http.getString();   // HTTP/1.1: Chunk-Dekodierung uebernimmt HTTPClient
      dbgBytes = body.length();
      err = deserializeJson(doc, body, DeserializationOption::Filter(filter));
    }
    ok = !err;
    dbgLast = ok ? String("ok (HTTP ") + (http10 ? "1.0" : "1.1") + ")" : String("JSON: ") + err.c_str();
  } else {
    dbgLast = String("HTTP ") + String(code) + (code < 0 ? String(" ") + HTTPClient::errorToString(code) : String(""));
  }
  Serial.println("API " + url + " -> " + dbgLast);
  http.end();
  return ok;
}

// GET -> JSON (gefiltert). Je Server erst HTTP/1.0 (Stream), bei Verbindungs-/JSON-Fehler HTTP/1.1;
// bei Fehlern (z. B. 503) der Reihe nach der andere Server.
bool apiGet(const String& path, const std::string& query, JsonDocument& doc, JsonDocument& filter) {
  const int nHosts = sizeof(BVG_HOSTS) / sizeof(BVG_HOSTS[0]);
  bool ok = false;
  for (int a = 0; a < nHosts && !ok; a++) {
    int hi = (hostIdx + a) % nHosts;
    String url = String(BVG_HOSTS[hi]) + path + "?" + query.c_str();
    doc.clear();
    ok = apiGetOnce(url, true, doc, filter);
    if (!ok && (dbgCode <= 0 || dbgCode == 200)) {   // kein HTTP-Fehlerstatus -> anderes Protokoll probieren
      doc.clear();
      ok = apiGetOnce(url, false, doc, filter);
    }
    if (ok) hostIdx = hi;
  }
  if (ok) dbgOk++;
  else dbgFail++;
  return ok;
}

std::string findStopId(const std::string& name) {
  JsonDocument filter, doc;
  filter[0]["id"] = true;
  std::string q = "query=" + bvg::url_encode(name) + "&results=3&poi=false&addresses=false";
  if (apiGet("/locations", q, doc, filter) && doc.is<JsonArray>() && doc.size() > 0)
    return doc[0]["id"] | "";
  return "";
}

bool fetchDepartures(const std::string& sid, int results, int duration, std::vector<bvg::Dep>& out) {
  JsonDocument filter, doc;
  bvg::departures_filter(filter);
  std::string q = "results=" + std::to_string(results) + "&duration=" + std::to_string(duration) +
                  "&remarks=false&linesOfStops=false&stopovers=false";
  if (!apiGet(String("/stops/") + sid.c_str() + "/departures", q, doc, filter)) return false;
  out.clear();
  bvg::parse_raw(doc.as<JsonVariantConst>(), out);
  return true;
}

bool refreshCache() {
  bool anyOk = false;
  JsonDocument snap;
  {
    Lock l;
    snap.set(cfgDoc);
  }
  // Haltestellen ohne ID nachtraeglich suchen
  JsonArray stops = snap["stops"].as<JsonArray>();
  for (size_t i = 0; i < stops.size(); i++) {
    std::string sid = stops[i]["stop_id"] | "";
    std::string name = stops[i]["stop_name"] | "";
    if (!sid.empty() || name.empty()) continue;
    std::string found = findStopId(name);
    Serial.printf("  %s -> %s\n", name.c_str(), found.c_str());
    if (found.empty()) continue;
    stops[i]["stop_id"] = found;
    Lock l;
    JsonArray cs = cfgDoc["stops"].as<JsonArray>();
    if (i < cs.size() && name == (const char*)(cs[i]["stop_name"] | "")) {
      cs[i]["stop_id"] = found;
      cfgDirty = true;
    }
  }
  auto by = bvg::wanted_queries(stops);
  {
    Lock l;
    std::set<std::string> keep;
    for (auto& kv : by)
      for (auto& q : kv.second) keep.insert(bvg::cache_key(kv.first, q.first, q.second));
    for (auto it = cache.m.begin(); it != cache.m.end();)
      it = keep.count(it->first) ? std::next(it) : cache.m.erase(it);
  }
  for (auto& kv : by) {
    std::vector<bvg::Dep> raw;
    if (fetchDepartures(kv.first, 30, 90, raw)) {
      anyOk = true;
      Lock l;
      double ok = millis() / 1000.0;
      for (auto& q : kv.second) {
        bvg::CacheEntry e;
        e.deps = bvg::filter_departures(raw, q.first, q.second);
        e.ok = ok;
        cache.m[bvg::cache_key(kv.first, q.first, q.second)] = e;
      }
    }
    vTaskDelay(100 / portTICK_PERIOD_MS);
  }
  return anyOk;
}

void fetchTask(void*) {
  for (;;) {
    if (running && !otaBusy && WiFi.status() == WL_CONNECTED && time(nullptr) > 1700000000 &&
        (fetchForce || (int32_t)(millis() - nextFetchMs) >= 0)) {
      fetchForce = false;
      bool ok = refreshCache();
      int fi = cfgInt("fetch_interval", 30);
      // bei Fehlschlag (Server down) schon nach 15 s erneut versuchen
      nextFetchMs = millis() + (ok ? (uint32_t)max(15, fi) * 1000UL : 15000UL);
      newData = true;
    }
    vTaskDelay(200 / portTICK_PERIOD_MS);
  }
}

// ------------------------------------------------------------------ Web-API
void sendJson(const JsonDocument& d, int code = 200) {
  String out;
  serializeJson(d, out);
  server.send(code, "application/json", out);
}
void sendOk() { server.send(200, "application/json", "{\"ok\":true}"); }
void sendErr(const String& msg) {
  JsonDocument d;
  d["ok"] = false;
  d["error"] = msg;
  sendJson(d);
}

void setupRoutes() {
  server.on("/", HTTP_GET, [] { server.send_P(200, "text/html; charset=utf-8", INDEX_HTML); });

  server.on("/api/status", HTTP_GET, [] {
    JsonDocument d;
    d["status"] = running ? "active" : "inactive";
    d["ssid"] = WiFi.SSID();
    d["rssi"] = WiFi.RSSI();
    d["ip"] = WiFi.localIP().toString();
    d["fw"] = FW_VERSION;
    d["heap"] = ESP.getFreeHeap();
    sendJson(d);
  });

  server.on("/api/debug", HTTP_GET, [] {
    JsonDocument d;
    d["fw"] = FW_VERSION;
    d["wifi"] = WiFi.status() == WL_CONNECTED;
    d["time_ok"] = time(nullptr) > 1700000000;
    d["epoch"] = (uint32_t)time(nullptr);
    d["heap_free"] = ESP.getFreeHeap();
    d["heap_max_block"] = ESP.getMaxAllocHeap();
    d["psram_free"] = ESP.getFreePsram();
    d["running"] = (bool)running;
    d["last_api"] = dbgLast;
    d["api_host"] = BVG_HOSTS[hostIdx];
    d["last_http_code"] = dbgCode;
    d["api_ok"] = dbgOk;
    d["api_fail"] = dbgFail;
    {
      Lock l;
      d["cache_entries"] = (int)cache.m.size();
      JsonArray st = d["stops"].to<JsonArray>();
      for (JsonObject s : cfgDoc["stops"].as<JsonArray>()) st.add(String((const char*)(s["stop_id"] | "(keine ID)")) + " " + (const char*)(s["line"] | ""));
      JsonArray ce = d["cache"].to<JsonArray>();
      for (auto& kv : cache.m) ce.add(String(kv.first.c_str()) + " -> " + String((int)kv.second.deps.size()) + " Abfahrten");
    }
    sendJson(d);
  });

  server.on("/api/config", HTTP_GET, [] {
    String out;
    {
      Lock l;
      serializeJson(cfgDoc, out);
    }
    server.send(200, "application/json", out);
  });

  server.on("/api/config", HTTP_POST, [] {
    JsonDocument data;
    if (deserializeJson(data, server.arg("plain")) || !data.is<JsonObject>()) {
      sendErr("Ungültige Daten");
      return;
    }
    {
      Lock l;
      for (JsonPair kv : data.as<JsonObject>()) cfgDoc[kv.key()] = kv.value();
      bvg::merge_defaults(cfgDoc);
      saveConfigLocked();
    }
    fetchForce = true;
    redrawNow = true;
    sendOk();
  });

  server.on("/api/restart", HTTP_POST, [] {
    running = true;
    fetchForce = true;
    redrawNow = true;
    sendOk();
  });

  server.on("/api/stop", HTTP_POST, [] {
    running = false;
    dma->clearScreen();
    dma->flipDMABuffer();
    sendOk();
  });

  server.on("/api/reboot", HTTP_POST, [] {
    rebootRequested = true;
    sendOk();
  });

  server.on("/api/wifi-setup", HTTP_POST, [] {
    if (!nvsPut("setupflag", "1")) {
      sendErr("Speichern nicht möglich");
      return;
    }
    rebootRequested = true;
    sendOk();
  });

  server.on("/api/search-stops", HTTP_GET, [] {
    String q = server.arg("q");   // WebServer dekodiert URL-Parameter selbst
    JsonDocument out;
    out.to<JsonArray>();
    if (q.length() < 2) return sendJson(out);
    JsonDocument filter, doc;
    filter[0]["id"] = true;
    filter[0]["name"] = true;
    filter[0]["type"] = true;
    std::string query = "query=" + bvg::url_encode(q.c_str()) + "&results=8&poi=false&addresses=false";
    if (!apiGet("/locations", query, doc, filter)) {
      JsonDocument e;
      e["error"] = "BVG-API nicht erreichbar";
      return sendJson(e, 500);
    }
    for (JsonVariant v : doc.as<JsonArray>()) {
      if (strcmp(v["type"] | "", "stop") != 0 || !v["id"].is<const char*>()) continue;
      JsonObject o = out.add<JsonObject>();
      o["id"] = v["id"];
      o["name"] = v["name"] | "";
    }
    sendJson(out);
  });

  server.on("/api/stop-lines", HTTP_GET, [] {
    String id = server.arg("id");
    JsonDocument out;
    out.to<JsonArray>();
    if (id.length() == 0) return sendJson(out);
    std::vector<bvg::Dep> raw;
    if (!fetchDepartures(id.c_str(), 50, 60, raw)) {
      JsonDocument e;
      e["error"] = "BVG-API nicht erreichbar";
      return sendJson(e, 500);
    }
    std::map<std::string, std::set<std::string>> lines;
    for (auto& d : raw)
      if (!d.line.empty()) {
        auto& s = lines[d.line];
        if (!d.direction.empty()) s.insert(d.direction);
      }
    for (auto& kv : lines) {
      JsonObject o = out.add<JsonObject>();
      o["line"] = kv.first;
      JsonArray dirs = o["directions"].to<JsonArray>();
      for (auto& dname : kv.second) dirs.add(dname);
    }
    sendJson(out);
  });

  // Firmware-Update im Browser (.bin hochladen)
  server.on(
      "/api/update", HTTP_POST,
      [] {
        bool ok = !Update.hasError();
        otaBusy = false;
        if (ok) {
          sendOk();
          rebootRequested = true;
        } else {
          sendErr(Update.errorString());
        }
      },
      [] {
        HTTPUpload& up = server.upload();
        if (up.status == UPLOAD_FILE_START) {
          otaBusy = true;
          showMessage({"Firmware-Update", "bitte warten..."}, 255, 200, 0);
          Update.begin(UPDATE_SIZE_UNKNOWN);
        } else if (up.status == UPLOAD_FILE_WRITE) {
          Update.write(up.buf, up.currentSize);
        } else if (up.status == UPLOAD_FILE_END) {
          Update.end(true);
        } else if (up.status == UPLOAD_FILE_ABORTED) {
          Update.abort();
          otaBusy = false;
        }
      });

  server.onNotFound([] { server.send(404, "text/plain", "Nicht gefunden"); });
}

// ------------------------------------------------------------------ WLAN-Setup (Handy-Portal)
void runSetupPortal(bool hasCreds) {
  showMessage({"Suche WLANs..."});
  WiFi.mode(WIFI_STA);
  WiFi.disconnect();
  delay(100);
  int n = WiFi.scanNetworks();
  JsonDocument nets;
  JsonArray arr = nets["networks"].to<JsonArray>();
  std::set<String> seen;
  // WiFi.scanNetworks sortiert nach Signal (stärkstes zuerst)
  for (int i = 0; i < n && (int)arr.size() < 20; i++) {
    String s = WiFi.SSID(i);
    if (s.length() == 0 || seen.count(s)) continue;
    seen.insert(s);
    JsonObject o = arr.add<JsonObject>();
    o["ssid"] = s;
    o["rssi"] = WiFi.RSSI(i);
    o["secure"] = WiFi.encryptionType(i) != WIFI_AUTH_OPEN;
  }
  nets["has_creds"] = hasCreds;
  String netsJson;
  serializeJson(nets, netsJson);

  WiFi.mode(WIFI_AP);
  WiFi.softAP(AP_SSID);   // offenes WLAN, nur waehrend des Setups
  delay(200);
  IPAddress ip = WiFi.softAPIP();
  String ipStr = ip.toString();

  DNSServer dns;
  dns.start(53, "*", ip);   // jede Adresse -> Portal (Captive Portal)
  WebServer portal(80);
  uint32_t savedAt = 0;

  portal.on("/", HTTP_GET, [&] { portal.send_P(200, "text/html; charset=utf-8", SETUP_HTML); });
  portal.on("/api/wifi-networks", HTTP_GET, [&] { portal.send(200, "application/json", netsJson); });
  portal.on("/api/wifi-save", HTTP_POST, [&] {
    JsonDocument d;
    String err;
    if (deserializeJson(d, portal.arg("plain"))) err = "Ungültige Daten";
    String ssid = String((const char*)(d["ssid"] | ""));
    String pass = String((const char*)(d["password"] | ""));
    ssid.trim();
    if (err.length() == 0 && (ssid.length() == 0 || ssid.length() > 32)) err = "WLAN-Name fehlt";
    if (err.length() == 0 && pass.length() > 0 && (pass.length() < 8 || pass.length() > 63))
      err = "Passwort muss 8–63 Zeichen haben";
    JsonDocument res;
    if (err.length() == 0) {
      JsonDocument w;
      w["ssid"] = ssid;
      w["password"] = pass;
      String txt;
      serializeJson(w, txt);
      if (!nvsPut("wifi", txt)) err = "Speichern nicht möglich";
    }
    res["ok"] = err.length() == 0;
    if (err.length()) res["error"] = err;
    else savedAt = millis() + 2000;
    String out;
    serializeJson(res, out);
    portal.send(200, "application/json", out);
  });
  // alles andere (Pruefadressen von Android/iOS/Windows) -> Setup-Seite oeffnet sich automatisch
  portal.onNotFound([&] {
    portal.sendHeader("Location", "http://" + ipStr + "/", true);
    portal.send(302, "text/plain", "");
  });
  portal.begin();

  showMessage({"WLAN-Setup:", "1 Mit WLAN", AP_SSID, "2 " + ipStr + " oeffnen"}, 0, 200, 80);
  uint32_t started = millis();
  for (;;) {
    dns.processNextRequest();
    portal.handleClient();
    if (savedAt && (int32_t)(millis() - savedAt) >= 0) {
      showMessage({"Gespeichert", "Neustart..."}, 0, 200, 80);
      delay(500);
      ESP.restart();
    }
    if (hasCreds && millis() - started > 600000UL) ESP.restart();   // Router war evtl. nur kurz weg
    delay(2);
  }
}

void connectWifi(bool allowPortal) {
  String ssid, pass;
  if (!loadWifi(ssid, pass)) runSetupPortal(false);
  WiFi.mode(WIFI_STA);
  WiFi.setHostname(HOSTNAME);
  WiFi.setAutoReconnect(true);
  int attempt = 0;
  while (WiFi.status() != WL_CONNECTED) {
    attempt++;
    if (allowPortal && attempt > 3) runSetupPortal(true);
    showMessage({"WLAN...", ssid.substring(0, 20), "Versuch " + String(attempt)});
    WiFi.disconnect();
    WiFi.begin(ssid.c_str(), pass.c_str());
    uint32_t t0 = millis();
    while (WiFi.status() != WL_CONNECTED && millis() - t0 < 15000) delay(100);
  }
}

// ------------------------------------------------------------------ Start
void setup() {
  Serial.begin(115200);
  mtx = xSemaphoreCreateMutex();

  // Display zuerst: ab hier laeuft die Ausgabe per DMA unabhaengig vom Rest
  HUB75_I2S_CFG mxconfig(PANEL_W, PANEL_H, PANEL_CHAIN);
  mxconfig.gpio.r1 = PIN_R1;
  mxconfig.gpio.g1 = PIN_G1;
  mxconfig.gpio.b1 = PIN_B1;
  mxconfig.gpio.r2 = PIN_R2;
  mxconfig.gpio.g2 = PIN_G2;
  mxconfig.gpio.b2 = PIN_B2;
  mxconfig.gpio.a = PIN_A;
  mxconfig.gpio.b = PIN_B;
  mxconfig.gpio.c = PIN_C;
  mxconfig.gpio.d = PIN_D;
  mxconfig.gpio.e = -1;   // 64x32-Panels haben keine E-Zeile
  mxconfig.gpio.lat = PIN_LAT;
  mxconfig.gpio.oe = PIN_OE;
  mxconfig.gpio.clk = PIN_CLK;
  mxconfig.double_buff = true;
  dma = new MatrixPanel_I2S_DMA(mxconfig);
  dma->begin();
  dma->clearScreen();
  dma->flipDMABuffer();

  loadConfig();

  pinMode(PIN_BTN_DOWN, INPUT_PULLUP);
  String flag;
  bool wantSetup = nvsGet("setupflag", flag) || digitalRead(PIN_BTN_DOWN) == LOW;
  nvsRemove("setupflag");
  if (wantSetup) {
    String s, p;
    runSetupPortal(loadWifi(s, p));
  }
  connectWifi(true);

  String ip = WiFi.localIP().toString();
  Serial.println("IP: " + ip);
  if (MDNS.begin(HOSTNAME)) MDNS.addService("http", "tcp", 80);

  showMessage({"Zeit (NTP)..."});
  configTzTime(TZ_BERLIN, "pool.ntp.org", "time.google.com", "time.cloudflare.com");
  for (int i = 0; i < 40 && time(nullptr) < 1700000000; i++) delay(250);

  setupRoutes();
  server.begin();
  showMessage({String(HOSTNAME) + ".local", ip, "Starte..."}, 0, 200, 80);
  delay(1500);

  xTaskCreatePinnedToCore(fetchTask, "fetch", 24576, nullptr, 1, nullptr, 0);   // Netzwerk auf Core 0
  nextFetchMs = millis();
}

uint32_t nextUpdateMs = 0, nextBriMs = 0, wifiLostSince = 0;

void loop() {
  server.handleClient();

  if (rebootRequested) {
    delay(600);
    ESP.restart();
  }
  if (cfgDirty) {
    cfgDirty = false;
    Lock l;
    saveConfigLocked();
  }

  // WLAN-Ueberwachung (Auto-Reconnect ist aktiv; bei laengerem Ausfall neu starten)
  if (WiFi.status() != WL_CONNECTED) {
    if (!wifiLostSince) wifiLostSince = millis();
    if (millis() - wifiLostSince > 300000UL) ESP.restart();
  } else {
    wifiLostSince = 0;
  }

  uint32_t now = millis();
  static uint32_t nextNtpMs = 0;
  if (time(nullptr) < 1700000000 && (int32_t)(now - nextNtpMs) >= 0) {   // NTP erneut anstossen
    nextNtpMs = now + 20000;
    configTzTime(TZ_BERLIN, "pool.ntp.org", "time.google.com", "time.cloudflare.com");
  }
  if (running && !otaBusy) {
    if (newData || redrawNow) {
      newData = redrawNow = false;
      redraw(false);
    } else if ((int32_t)(now - nextUpdateMs) >= 0) {
      redraw(true);   // Anzeige auffrischen (Minuten laufen weiter) + Alternativ-Linie wechseln
    }
    if ((int32_t)(now - nextUpdateMs) >= 0) {
      int ui = cfgInt("update_interval", 20);
      nextUpdateMs = now + (uint32_t)max(5, ui) * 1000UL;
    }
    if (cfgBool("auto_brightness", false) && (int32_t)(now - nextBriMs) >= 0) {
      nextBriMs = now + 30000;
      showBrightness(currentBrightness());   // nur Helligkeit, kein Neuzeichnen noetig
    }
  }
  delay(2);
}
