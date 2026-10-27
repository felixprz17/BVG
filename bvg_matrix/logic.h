// Reine Logik (ohne Arduino-Abhaengigkeiten) - wird auch am PC mit g++ getestet (test/).
// Entspricht den gleichnamigen Funktionen aus der CircuitPython-Version (code.py).
#pragma once
#include <ArduinoJson.h>

#include <cstdint>
#include <map>
#include <string>
#include <utility>
#include <vector>

namespace bvg {

struct Dep {
  std::string line, direction;
  int64_t when = 0;  // UTC-Sekunden (Echtzeit)
  int delay = 0;     // Verspaetung in Sekunden (Echtzeit minus Fahrplan), 0 wenn unbekannt
};

struct DepOut {
  std::string line, direction;
  int minutes = 0;
  int delay = 0;
};

struct Row {
  std::string label, ziel, zeit1, zeit2;
  uint8_t r = 200, g = 200, b = 200;
  int minutes = 99;
  bool late1 = false, late2 = false;   // Abfahrt verspaetet (>= LATE_SECONDS)
};

static const int KEEP_DEPARTURES = 3;
static const int STALE_AFTER = 300;  // Sekunden
static const int LATE_SECONDS = 60;   // ab dieser Verspaetung gilt eine Abfahrt als verspaetet (rot)
static const int FONT_H[3] = {6, 7, 10};         // 4x6, 5x7, 6x10
static const int MAX_ZIEL[3] = {18, 13, 10};   // Zeichen vor dem "."; Zeiten sind rechtsbuendig
// Fette Schrift ist proportional: gekuerzt wird dort nach Pixelbreite (Firmware), nicht nach Zeichenzahl
static const int MAX_ZIEL_BOLD = 40;
static const int HEIGHT = 32;

inline int max_ziel_chars(int font, bool bold) { return bold ? MAX_ZIEL_BOLD : MAX_ZIEL[font]; }

// Zeilen, die in der Schrift uebereinander auf das Display passen (Klein 5, Normal 4, Gross 3)
inline int rows_per_page(int font) { return HEIGHT / FONT_H[font]; }
// Seiten, wenn mehr Haltestellen als Zeilen passen: die Anzeige blaettert dann automatisch
inline int page_count(int stops, int font) {
  int per = rows_per_page(font);
  return stops <= per ? 1 : (stops + per - 1) / per;
}

// ---------------------------------------------------------------- Zeit
inline int64_t days_from_civil(int y, int m, int d) {
  y -= m <= 2;
  const int64_t era = (y >= 0 ? y : y - 399) / 400;
  const int yoe = (int)(y - era * 400);
  const int doy = (153 * (m + (m > 2 ? -3 : 9)) + 2) / 5 + d - 1;
  const int doe = yoe * 365 + yoe / 4 - yoe / 100 + doy;
  return era * 146097 + doe - 719468;
}

inline bool _dig(const char* s, int n, int& out) {
  out = 0;
  for (int i = 0; i < n; i++) {
    if (s[i] < '0' || s[i] > '9') return false;
    out = out * 10 + (s[i] - '0');
  }
  return true;
}

// ISO-8601 ('2026-10-02T01:55:00+02:00', auch Z, .000, +0200) -> UTC-Epoche
inline bool parse_iso(const char* s, int64_t& out) {
  if (!s) return false;
  size_t len = 0;
  while (s[len]) len++;
  if (len < 19) return false;
  int y, mo, d, h, mi, sec;
  if (!_dig(s, 4, y) || !_dig(s + 5, 2, mo) || !_dig(s + 8, 2, d) || !_dig(s + 11, 2, h) ||
      !_dig(s + 14, 2, mi) || !_dig(s + 17, 2, sec))
    return false;
  if (mo < 1 || mo > 12 || d < 1 || d > 31 || h > 24 || mi > 59 || sec > 60) return false;
  const char* rest = s + 19;
  if (*rest == '.') {
    rest++;
    while (*rest >= '0' && *rest <= '9') rest++;
  }
  int64_t offset = 0;
  if (*rest == '+' || *rest == '-') {
    int sign = *rest == '+' ? 1 : -1;
    int oh = 0, om = 0;
    if (!_dig(rest + 1, 2, oh)) return false;
    const char* p = rest + 3;
    if (*p == ':') p++;
    if (*p) {
      if (!_dig(p, 2, om)) return false;
    }
    offset = sign * (oh * 3600 + om * 60);
  } else if (*rest != 0 && *rest != 'Z') {
    return false;
  }
  out = days_from_civil(y, mo, d) * 86400 + h * 3600 + mi * 60 + sec - offset;
  return true;
}

// ---------------------------------------------------------------- Text
inline std::string url_encode(const std::string& s) {
  static const char* hex = "0123456789ABCDEF";
  std::string out;
  for (unsigned char c : s) {
    if ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || (c >= '0' && c <= '9') || c == '-' ||
        c == '_' || c == '.' || c == '~') {
      out += (char)c;
    } else {
      out += '%';
      out += hex[c >> 4];
      out += hex[c & 15];
    }
  }
  return out;
}

inline int _hexval(char c) {
  if (c >= '0' && c <= '9') return c - '0';
  if (c >= 'a' && c <= 'f') return c - 'a' + 10;
  if (c >= 'A' && c <= 'F') return c - 'A' + 10;
  return -1;
}

inline std::string url_decode(const std::string& s) {
  std::string out;
  for (size_t i = 0; i < s.size(); i++) {
    if (s[i] == '+') {
      out += ' ';
    } else if (s[i] == '%' && i + 2 < s.size() && _hexval(s[i + 1]) >= 0 && _hexval(s[i + 2]) >= 0) {
      out += (char)(_hexval(s[i + 1]) * 16 + _hexval(s[i + 2]));
      i += 2;
    } else {
      out += s[i];
    }
  }
  return out;
}

inline size_t utf8_len(const std::string& s) {
  size_t n = 0;
  for (unsigned char c : s)
    if ((c & 0xC0) != 0x80) n++;
  return n;
}

// erste n Zeichen (nicht Bytes)
inline std::string utf8_prefix(const std::string& s, size_t n) {
  size_t chars = 0, i = 0;
  for (; i < s.size(); i++) {
    if (((unsigned char)s[i] & 0xC0) != 0x80) {
      if (chars == n) break;
      chars++;
    }
  }
  return s.substr(0, i);
}

inline bool starts_with(const std::string& s, const char* p) { return s.rfind(p, 0) == 0; }

inline std::string rstrip(std::string s) {
  while (!s.empty() && (s.back() == ' ' || s.back() == '\t')) s.pop_back();
  return s;
}

inline std::string trim(const std::string& s) {
  size_t a = 0, b = s.size();
  while (a < b && (s[a] == ' ' || s[a] == '\t')) a++;
  while (b > a && (s[b - 1] == ' ' || s[b - 1] == '\t')) b--;
  return s.substr(a, b - a);
}

inline std::string lower(std::string s) {
  for (auto& c : s)
    if (c >= 'A' && c <= 'Z') c += 32;
  return s;
}

// auf max_ziel Zeichen kuerzen, "." anhaengen
inline std::string shorten(const std::string& c, int max_ziel) {
  if ((int)utf8_len(c) > max_ziel) return rstrip(utf8_prefix(c, max_ziel)) + ".";
  return c;
}

// Ziel-Umbenennung: {"Rathaus Spandau":"Spandau"} - Teilstring (Gross/Klein egal) im Zielnamen -> Ersatz
// Gilt pro Haltestelle (stop["ziel_alias"]); das globale "ziel_alias" bleibt als Altbestand erhalten.
inline bool apply_alias(const std::string& raw, JsonObjectConst alias, std::string& out) {
  if (alias.isNull()) return false;
  std::string rl = lower(raw);
  for (JsonPairConst kv : alias) {
    std::string key = lower(trim(std::string(kv.key().c_str())));
    if (key.empty() || !kv.value().is<const char*>()) continue;
    std::string val = trim(std::string(kv.value().as<const char*>()));
    if (val.empty()) continue;
    if (rl.find(key) != std::string::npos) {
      out = val;
      return true;
    }
  }
  return false;
}

// Zusaetze im Zielnamen ausblenden: "(...)" bzw. "[...]" und/oder das Wort "Berlin"
// ("Rathaus Spandau (Berlin)" -> "Rathaus Spandau", "S+U Berlin Hbf" -> "S+U Hbf", "Berlin-Spandau" -> "Spandau")
inline std::string strip_extras(std::string s, bool brackets, bool berlin) {
  if (brackets) {
    for (;;) {
      size_t a = s.find_first_of("([");
      if (a == std::string::npos) break;
      char close = s[a] == '(' ? ')' : ']';
      size_t b = s.find(close, a);
      size_t end = b == std::string::npos ? s.size() : b + 1;
      while (a > 0 && s[a - 1] == ' ') a--;   // Leerzeichen vor der Klammer mitnehmen
      s.erase(a, end - a);
    }
  }
  if (berlin) {
    const std::string w = "Berlin";
    size_t pos = 0;
    while ((pos = s.find(w, pos)) != std::string::npos) {
      size_t e = pos + w.size();
      auto sep = [](char c) { return c == ' ' || c == '-' || c == ',' || c == '(' || c == ')' || c == '/'; };
      bool left = pos == 0 || sep(s[pos - 1]);
      bool right = e >= s.size() || sep(s[e]);
      if (!left || !right) {
        pos = e;
        continue;
      }
      // Trennzeichen nach dem Wort mitnehmen ("Berlin-", "Berlin "), sonst das davor (" Berlin", ", Berlin")
      if (e < s.size() && (s[e] == ' ' || s[e] == '-')) e++;
      else while (pos > 0 && (s[pos - 1] == ' ' || s[pos - 1] == '-' || s[pos - 1] == ',')) pos--;
      s.erase(pos, e - pos);
    }
    for (size_t a; (a = s.find("()")) != std::string::npos;) s.erase(a, 2);   // "(Berlin)" bleibt als "()" uebrig
    for (size_t a; (a = s.find("  ")) != std::string::npos;) s.erase(a, 1);
  }
  return trim(s);
}

inline std::string clean_ziel(const std::string& raw, int max_ziel = 9) {
  std::string c = raw;
  static const char* prefixes[] = {"S+U ", "S ", "U ", "Bus "};
  for (const char* p : prefixes) {
    if (starts_with(c, p)) {
      c = c.substr(std::string(p).size());
      break;
    }
  }
  size_t comma = c.find(',');
  if (comma != std::string::npos) c = c.substr(0, comma);
  if ((int)utf8_len(c) > max_ziel) return rstrip(utf8_prefix(c, max_ziel)) + ".";
  return c;
}

inline std::string fmt_zeit(int mins) {
  if (mins == 0) return "now";
  if (mins > 99) return ">99";   // Zeitspalte ist 3 Zeichen breit
  return std::to_string(mins) + "'";
}

inline std::vector<std::string> split_lines(const std::string& s) {
  std::vector<std::string> out;
  size_t start = 0;
  while (start <= s.size()) {
    size_t e = s.find(',', start);
    if (e == std::string::npos) e = s.size();
    std::string t = trim(s.substr(start, e - start));
    if (!t.empty()) out.push_back(t);
    start = e + 1;
  }
  return out;
}

// ---------------------------------------------------------------- Farben
inline void color_rgb(const std::string& name, uint8_t& r, uint8_t& g, uint8_t& b) {
  struct C { const char* n; uint8_t r, g, b; };
  static const C map[] = {{"blue", 0, 100, 255},   {"green", 0, 200, 80},  {"orange", 255, 80, 0},
                          {"purple", 200, 0, 200}, {"yellow", 255, 200, 0}, {"red", 220, 30, 30},
                          {"white", 200, 200, 200}, {"cyan", 0, 200, 200},  {"pink", 255, 80, 180}};
  for (const C& c : map)
    if (name == c.n) { r = c.r; g = c.g; b = c.b; return; }
  r = g = b = 200;
}

// ---------------------------------------------------------------- Schrift
// 0 = 4x6, 1 = 5x7, 2 = 6x10
inline int font_index(const std::string& key) {
  if (key == "4x6") return 0;
  if (key == "6x10") return 2;
  return 1;
}

// Gewaehlte Schrift wird immer verwendet (zu viele Zeilen werden auf Seiten verteilt, nicht verkleinert).
// Fett gibt es nur als Normal und Gross: Klein wird bei Fett zu Normal.
inline int effective_font(const std::string& want_key, bool bold = false) {
  int f = font_index(want_key);
  return (bold && f == 0) ? 1 : f;
}

// ---------------------------------------------------------------- BVG-Antwort
// Filter, damit nur die benoetigten Felder im RAM landen
inline void departures_filter(JsonDocument& f) {
  JsonObject d = f["departures"][0].to<JsonObject>();
  d["when"] = true;
  d["plannedWhen"] = true;
  d["cancelled"] = true;
  d["direction"] = true;
  d["line"]["name"] = true;
}

// Antwort ({"departures":[...]} oder direkt [...]) -> Rohliste (nur Eintraege mit gueltiger Zeit)
inline void parse_raw(JsonVariantConst root, std::vector<Dep>& out) {
  JsonArrayConst arr = root.is<JsonArrayConst>() ? root.as<JsonArrayConst>()
                                                  : root["departures"].as<JsonArrayConst>();
  for (JsonVariantConst v : arr) {
    if (!v.is<JsonObjectConst>()) continue;
    if (v["cancelled"] | false) continue;   // ausgefallene Fahrten nicht anzeigen
    const char* when = v["when"].is<const char*>() ? v["when"].as<const char*>() : nullptr;
    const char* planned = v["plannedWhen"].is<const char*>() ? v["plannedWhen"].as<const char*>() : nullptr;
    if (!when) when = planned;
    if (!when) continue;
    int64_t t;
    if (!parse_iso(when, t)) continue;
    Dep d;
    int64_t tp;
    if (planned && parse_iso(planned, tp)) d.delay = (int)(t - tp);
    d.line = v["line"]["name"] | "";
    d.direction = v["direction"] | "";
    d.when = t;
    out.push_back(d);
  }
}

inline std::vector<Dep> filter_departures(const std::vector<Dep>& raw, const std::string& lines_str,
                                          const std::string& direction, int keep = KEEP_DEPARTURES) {
  std::vector<std::string> lines = split_lines(lines_str);
  // Richtung: mehrere Teilstrings mit "|" getrennt (ODER), z. B. "Blankenfelde|Teltow"
  std::vector<std::string> dirs;
  {
    size_t st = 0;
    while (st <= direction.size()) {
      size_t e = direction.find('|', st);
      if (e == std::string::npos) e = direction.size();
      std::string t = lower(trim(direction.substr(st, e - st)));
      if (!t.empty()) dirs.push_back(t);
      st = e + 1;
    }
  }
  std::vector<Dep> out;
  for (const Dep& d : raw) {
    if (!lines.empty()) {
      bool ok = false;
      for (auto& l : lines)
        if (l == d.line) ok = true;
      if (!ok) continue;
    }
    if (!dirs.empty()) {
      std::string dl = lower(d.direction);
      bool ok = false;
      for (auto& x : dirs)
        if (dl.find(x) != std::string::npos) ok = true;
      if (!ok) continue;
    }
    out.push_back(d);
    if ((int)out.size() >= keep) break;
  }
  return out;
}

// ---------------------------------------------------------------- Abfragen & Cache
typedef std::pair<std::string, std::string> LineDir;

// Alle benoetigten Abfragen (Haupt- und Alternativ-Linie), je Haltestelle gruppiert
inline std::map<std::string, std::vector<LineDir>> wanted_queries(JsonArrayConst stops) {
  std::map<std::string, std::vector<LineDir>> by;
  for (JsonObjectConst s : stops) {
    std::string sid = s["stop_id"] | "";
    if (!sid.empty()) by[sid].push_back({s["line"] | "", s["direction"] | ""});
    JsonObjectConst alt = s["alternativ"];
    if (!alt.isNull()) {
      std::string asid = alt["stop_id"] | "";
      if (asid.empty()) asid = sid;
      if (!asid.empty()) by[asid].push_back({alt["line"] | "", alt["direction"] | ""});
    }
  }
  return by;
}

inline std::string cache_key(const std::string& sid, const std::string& lines, const std::string& dir) {
  return sid + "|" + lines + "|" + dir;
}

struct CacheEntry {
  std::vector<Dep> deps;
  double ok = 0;  // Monotonic-Sekunden der letzten erfolgreichen Abfrage
};

struct Cache {
  std::map<std::string, CacheEntry> m;

  // naechste zwei Abfahrten, Minuten bezogen auf now_epoch
  std::vector<DepOut> departures(const std::string& sid, const std::string& lines, const std::string& dir,
                                 int64_t now_epoch, double now_mono) const {
    std::vector<DepOut> out;
    if (sid.empty()) return out;
    auto it = m.find(cache_key(sid, lines, dir));
    if (it == m.end() || now_mono - it->second.ok > STALE_AFTER) return out;
    for (const Dep& d : it->second.deps) {
      double secs = (double)(d.when - now_epoch);
      if (secs < -20) continue;  // abgefahren
      DepOut o;
      o.line = d.line;
      o.direction = d.direction;
      o.delay = d.delay;
      o.minutes = (int)(secs / 60.0);
      if (o.minutes < 0) o.minutes = 0;
      out.push_back(o);
      if (out.size() >= 2) break;
    }
    return out;
  }
};

// Zeilen fuer die Anzeige (kein Netzwerkzugriff)
// alias = globale Umbenennungen (Altbestand); je Haltestelle gilt s["ziel_alias"] mit Vorrang.
// bold = fette Schrift (proportional -> die Firmware kuerzt nach Pixelbreite)
// tick = nur fuer den Wechsel Haupt-/Alternativ-Linie (ungerade = Alternative)
// hide_brackets / hide_berlin: Zusaetze im Zielnamen ausblenden (siehe strip_extras)
inline std::vector<Row> build_rows(const Cache& cache, int tick, JsonArrayConst stops,
                                   const std::string& font_size, int64_t now_epoch, double now_mono,
                                   JsonObjectConst alias = JsonObjectConst(), bool bold = false,
                                   bool hide_brackets = false, bool hide_berlin = false) {
  int max_ziel = max_ziel_chars(effective_font(font_size, bold), bold);
  std::vector<Row> result;
  for (JsonObjectConst s : stops) {
    JsonObjectConst alt = s["alternativ"];
    bool zeige_alt = !alt.isNull() && (tick % 2 == 1);
    std::string sid = s["stop_id"] | "";
    std::vector<DepOut> deps;
    if (zeige_alt) {
      std::string asid = alt["stop_id"] | "";
      if (asid.empty()) asid = sid;
      deps = cache.departures(asid, alt["line"] | "", alt["direction"] | "", now_epoch, now_mono);
    } else {
      deps = cache.departures(sid, s["line"] | "", s["direction"] | "", now_epoch, now_mono);
    }
    Row r;
    // Eigener Anzeige-Name (Label), falls er nicht einfach einer der Linienbezeichnungen entspricht
    std::string custom = trim(std::string(s["label"] | ""));
    bool useCustom = false;
    if (!custom.empty() && !zeige_alt) {
      useCustom = true;
      for (auto& l : split_lines(std::string(s["line"] | "")))
        if (l == custom) useCustom = false;
    }
    if (!deps.empty()) {
      r.label = useCustom ? custom : (deps[0].line.empty() ? std::string(s["label"] | "?") : deps[0].line);
      std::string al;
      JsonObjectConst stop_alias = s["ziel_alias"];
      bool aliased = apply_alias(deps[0].direction, stop_alias, al) || apply_alias(deps[0].direction, alias, al);
      r.ziel = aliased ? shorten(al, max_ziel)
                       : clean_ziel(strip_extras(deps[0].direction, hide_brackets, hide_berlin), max_ziel);
      r.zeit1 = fmt_zeit(deps[0].minutes);
      r.late1 = deps[0].delay >= LATE_SECONDS;
      if (deps.size() >= 2) {
        r.zeit2 = fmt_zeit(deps[1].minutes);
        r.late2 = deps[1].delay >= LATE_SECONDS;
      }
      r.minutes = deps[0].minutes;
    } else {
      r.label = std::string(s["label"] | "?");
      r.ziel = "---";
    }
    std::string col = "white";
    if (zeige_alt && !alt["color"].isNull()) col = alt["color"] | "white";
    else col = s["color"] | "white";
    color_rgb(col, r.r, r.g, r.b);
    result.push_back(r);
  }
  return result;
}

// ---------------------------------------------------------------- Konfiguration
static const char* DEFAULT_CONFIG_JSON =
    "{\"brightness\":80,\"update_interval\":20,\"fetch_interval\":30,\"design\":\"modern\","
    "\"font_size\":\"5x7\",\"font_bold\":false,\"hide_brackets\":false,\"hide_berlin\":false,\"offset_label_x\":0,\"offset_x\":-2,\"offset_time_x\":-1,\"offset_y\":1,"
    "\"ziel_alias\":{},\"show_second_time\":true,\"auto_brightness\":false,"
    "\"auto_brightness_day\":80,\"auto_brightness_night\":30,\"auto_brightness_day_start\":7,"
    "\"auto_brightness_night_start\":22,\"stops\":["
    "{\"stop_id\":null,\"stop_name\":\"U Yorckstraße\",\"line\":\"U7\",\"direction\":\"Spandau\","
    "\"label\":\"U7\",\"color\":\"blue\",\"ziel_alias\":{\"Rathaus Spandau\":\"Spandau\"}},"
    "{\"stop_id\":\"900057102\",\"stop_name\":\"S Yorckstr. (Großgörschenstr.)\",\"line\":\"S1\","
    "\"direction\":\"\",\"label\":\"S1\",\"color\":\"green\","
    "\"alternativ\":{\"stop_id\":\"900058103\",\"line\":\"S2,S25\",\"direction\":\"\",\"color\":\"blue\"}},"
    "{\"stop_id\":null,\"stop_name\":\"Monumentenstraße\",\"line\":\"140\",\"direction\":\"\","
    "\"label\":\"140\",\"color\":\"orange\"},"
    "{\"stop_id\":null,\"stop_name\":\"Katzbachstraße\",\"line\":\"M19\",\"direction\":\"Grunewald\","
    "\"label\":\"M19\",\"color\":\"purple\"}]}";

// fehlende Schluessel mit Standardwerten auffuellen
inline void merge_defaults(JsonDocument& cfg) {
  JsonDocument def;
  deserializeJson(def, DEFAULT_CONFIG_JSON);
  if (!cfg.is<JsonObject>()) cfg.to<JsonObject>();
  for (JsonPair kv : def.as<JsonObject>())
    if (cfg[kv.key()].isNull()) cfg[kv.key()] = kv.value();
}

}  // namespace bvg
