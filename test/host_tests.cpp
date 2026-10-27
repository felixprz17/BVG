// Host-Tests fuer bvg_matrix/logic.h:  ./run.sh
#include <cstdio>
#include <fstream>
#include <iostream>
#include <sstream>
#include "../bvg_matrix/logic.h"
using namespace bvg;

static int fails = 0, checks = 0;
#define CHECK_EQ(a, b) do { checks++; auto _a = (a); auto _b = (b); if (!(_a == _b)) { fails++; \
  std::cout << "FAIL " << __LINE__ << ": " #a " != " #b << "  [" << _a << " vs " << _b << "]\n"; } } while (0)

static std::vector<std::string> cols(const std::string& l) {
  std::vector<std::string> v; std::stringstream ss(l); std::string t;
  while (std::getline(ss, t, '\t')) v.push_back(t);
  return v;
}

static void test_reference_cases() {
  std::ifstream f("cases.tsv"); std::string line; int n = 0;
  while (std::getline(f, line)) {
    auto c = cols(line);
    if (c.size() < 2) continue;
    n++;
    if (c[0] == "ISO") { int64_t t = -1; bool ok = parse_iso(c[1].c_str(), t); CHECK_EQ(ok, true); CHECK_EQ((long long)t, std::stoll(c[2])); }
    else if (c[0] == "ENC") CHECK_EQ(url_encode(c[1]), c.size() > 2 ? c[2] : std::string());
    else if (c[0] == "DEC") CHECK_EQ(url_decode(c[1]), c.size() > 2 ? c[2] : std::string());
  }
  CHECK_EQ(n > 20000, true);
}

static void test_parse_iso_invalid() {
  int64_t t;
  CHECK_EQ(parse_iso("kaputt", t), false);
  CHECK_EQ(parse_iso("", t), false);
  CHECK_EQ(parse_iso(nullptr, t), false);
  CHECK_EQ(parse_iso("2026-13-03T12:00:00+02:00", t), false);
  CHECK_EQ(parse_iso("2026-10-03T12:00:00*", t), false);
}

static void test_text() {
  CHECK_EQ(clean_ziel("S+U Rathaus Spandau (Berlin)"), std::string("Rathaus S."));
  CHECK_EQ(clean_ziel("U Rudow"), std::string("Rudow"));
  CHECK_EQ(clean_ziel("S Wannsee"), std::string("Wannsee"));
  CHECK_EQ(clean_ziel("Bus Foo"), std::string("Foo"));
  CHECK_EQ(clean_ziel("Tempelhof, Ullsteinstr."), std::string("Tempelhof"));
  CHECK_EQ(clean_ziel("Grunewald"), std::string("Grunewald"));
  CHECK_EQ(clean_ziel("Schönefeld Flughafen BER", 12), std::string("Schönefeld F."));
  CHECK_EQ(clean_ziel("S Teltow Stadt", 7), std::string("Teltow."));
  CHECK_EQ(clean_ziel("Sudkreuz"), std::string("Sudkreuz"));
  CHECK_EQ(clean_ziel("Spandau"), std::string("Spandau"));
  CHECK_EQ(clean_ziel("Köpenicker Straße", 4), std::string("Köpe."));   // UTF-8 nicht mitten im Zeichen schneiden
  CHECK_EQ(fmt_zeit(0), std::string("now"));
  CHECK_EQ(fmt_zeit(7), std::string("7'"));
  CHECK_EQ(fmt_zeit(99), std::string("99'"));
  CHECK_EQ(fmt_zeit(100), std::string(">99"));
  CHECK_EQ(effective_font("6x10", 4), 1);
  CHECK_EQ(effective_font("6x10", 2), 2);
  CHECK_EQ(effective_font("5x7", 8), 0);
  CHECK_EQ(effective_font("4x6", 1), 0);
  CHECK_EQ(effective_font("blah", 1), 1);
}

static std::vector<Dep> raw_from(const char* json) {
  JsonDocument filter; departures_filter(filter);
  JsonDocument doc;
  auto err = deserializeJson(doc, json, DeserializationOption::Filter(filter));
  std::vector<Dep> out;
  if (!err) parse_raw(doc.as<JsonVariantConst>(), out);
  return out;
}

static void test_departures() {
  const char* js = R"({"departures":[
    {"tripId":"x","stop":{"id":"1"},"when":"2026-10-03T12:00:00+02:00","plannedWhen":"2026-10-03T11:58:00+02:00","direction":"S Bernau","line":{"name":"S2","id":"s2"},"remarks":[1,2,3]},
    {"when":"2026-10-03T12:01:00+02:00","direction":"S Teltow","line":{"name":"S25"}},
    {"when":null,"plannedWhen":"2026-10-03T12:02:00+02:00","direction":"S Wannsee","line":{"name":"S1"}},
    {"when":null,"direction":"nix","line":{"name":"S1"}},
    {"when":"kaputt","direction":"nix","line":{"name":"S1"}},
    {"when":"2026-10-03T12:05:00+02:00","direction":"D"},
    {"when":"2026-10-03T12:06:00+02:00","direction":"S Blankenfelde","line":{"name":"S2"}}]})";
  auto raw = raw_from(js);
  CHECK_EQ(raw.size(), (size_t)5);
  CHECK_EQ(raw[1].line, std::string("S25"));
  CHECK_EQ(raw[3].line, std::string(""));
  CHECK_EQ(raw[3].when - raw[0].when, (int64_t)300);
  auto o = filter_departures(raw, "S2, S25", "");
  CHECK_EQ(o.size(), (size_t)3);
  CHECK_EQ(filter_departures(raw, "S2", "bernau").size(), (size_t)1);
  CHECK_EQ(filter_departures(raw, "S2,S25", "Bernau|Teltow").size(), (size_t)2);      // Richtungsgruppe
  CHECK_EQ(filter_departures(raw, "S2,S25", " bernau | blanken ").size(), (size_t)2);
  CHECK_EQ(filter_departures(raw, "S2,S25", "|").size(), (size_t)3);                  // leere Gruppe = alle
  CHECK_EQ((int)raw[0].delay, 120);                                                   // when - plannedWhen
  CHECK_EQ((int)raw[1].delay, 0);
  auto canc = raw_from(R"({"departures":[{"when":null,"plannedWhen":"2026-10-03T12:00:00+02:00","cancelled":true,"direction":"X","line":{"name":"U7"}},{"when":"2026-10-03T12:03:00+02:00","plannedWhen":"2026-10-03T12:03:00+02:00","direction":"Y","line":{"name":"U7"}}]})");
  CHECK_EQ(canc.size(), (size_t)1);
  CHECK_EQ(canc[0].direction, std::string("Y"));
  CHECK_EQ(filter_departures(raw, "", "").size(), (size_t)3);   // KEEP_DEPARTURES
  // Antwort als reines Array (ohne Filter) wird von parse_raw ebenfalls verstanden
  JsonDocument arr; deserializeJson(arr, R"([{"when":"2026-10-03T12:00:00+02:00","direction":"A","line":{"name":"U7"}}])");
  std::vector<Dep> av; parse_raw(arr.as<JsonVariantConst>(), av);
  CHECK_EQ(av.size(), (size_t)1);
  CHECK_EQ(raw_from("{}").size(), (size_t)0);
  CHECK_EQ(raw_from("nonsense").size(), (size_t)0);
}

static void test_cache_and_rows() {
  JsonDocument cfg; deserializeJson(cfg, DEFAULT_CONFIG_JSON);
  JsonArray stops = cfg["stops"];
  stops[0]["stop_id"] = "100"; stops[2]["stop_id"] = "300"; stops[3]["stop_id"] = "400";
  auto q = wanted_queries(stops);
  CHECK_EQ(q.size(), (size_t)5);   // 100, 900057102, 900058103, 300, 400
  CHECK_EQ(q["900057102"].size(), (size_t)1);

  Cache c; int64_t now = 1000000;
  auto put = [&](const std::string& sid, const std::string& l, const std::string& d, std::vector<int> offs, const char* line, const char* dir) {
    CacheEntry e; e.ok = 5000;
    for (int o : offs) e.deps.push_back({line, dir, now + o});
    c.m[cache_key(sid, l, d)] = e;
  };
  put("100", "U7", "Spandau", {30, 90, 400}, "U7", "U Rathaus Spandau");
  auto d = c.departures("100", "U7", "Spandau", now, 5000);
  CHECK_EQ(d.size(), (size_t)2); CHECK_EQ(d[0].minutes, 0); CHECK_EQ(d[1].minutes, 1);
  d = c.departures("100", "U7", "Spandau", now + 100, 5100);
  CHECK_EQ(d.size(), (size_t)2); CHECK_EQ(d[0].minutes, 0); CHECK_EQ(d[1].minutes, 5);
  d = c.departures("100", "U7", "Spandau", now + 130, 5130);
  CHECK_EQ(d.size(), (size_t)1); CHECK_EQ(d[0].minutes, 4);
  CHECK_EQ(c.departures("100", "U7", "Spandau", now, 5299).size(), (size_t)2);
  CHECK_EQ(c.departures("100", "U7", "Spandau", now, 5301).size(), (size_t)0);   // veraltet
  CHECK_EQ(c.departures("zzz", "U7", "", now, 5000).size(), (size_t)0);
  CHECK_EQ(c.departures("", "U7", "", now, 5000).size(), (size_t)0);

  put("900057102", "S1", "", {240, 840}, "S1", "S Wannsee");
  put("900058103", "S2,S25", "", {120, 600}, "S25", "S Teltow Stadt");
  auto rows = build_rows(c, 0, stops, "5x7", now, 5000);
  CHECK_EQ(rows.size(), (size_t)4);
  CHECK_EQ(rows[0].label, std::string("U7")); CHECK_EQ(rows[0].ziel, std::string("Rathaus Spand."));   // 13 Zeichen + "."
  CHECK_EQ(rows[0].zeit1, std::string("now")); CHECK_EQ(rows[0].zeit2, std::string("1'"));
  CHECK_EQ(rows[1].label, std::string("S1")); CHECK_EQ(rows[1].zeit1, std::string("4'"));
  CHECK_EQ(rows[1].g, 200);                                  // gruen
  CHECK_EQ(rows[2].ziel, std::string("---")); CHECK_EQ(rows[2].label, std::string("140")); CHECK_EQ(rows[2].zeit1, std::string(""));
  CHECK_EQ(rows[0].late1, false);
  put("300", "140", "", {300, 900}, "140", "Ostbahnhof");
  c.m[cache_key("300", "140", "")].deps[0].delay = 59;
  c.m[cache_key("300", "140", "")].deps[1].delay = 60;
  rows = build_rows(c, 0, stops, "5x7", now, 5000);
  CHECK_EQ(rows[2].late1, false); CHECK_EQ(rows[2].late2, true);                // Schwelle 60 s
  stops[0]["label"] = "Spa";                                  // eigener Name wird angezeigt
  rows = build_rows(c, 0, stops, "5x7", now, 5000);
  CHECK_EQ(rows[0].label, std::string("Spa"));
  stops[0]["label"] = "U7";                                   // = Linienname -> echter Linienname
  rows = build_rows(c, 0, stops, "5x7", now, 5000);
  CHECK_EQ(rows[0].label, std::string("U7"));
  {
    JsonDocument al; deserializeJson(al, R"({"Rathaus Spandau":"Spandau","bernau":"Bernau!"})");
    rows = build_rows(c, 0, stops, "5x7", now, 5000, al.as<JsonObjectConst>());
    CHECK_EQ(rows[0].ziel, std::string("Spandau"));            // U Rathaus Spandau -> Spandau
    CHECK_EQ(rows[1].ziel, std::string("Wannsee"));            // kein Treffer: normale Kuerzung
    std::string o; CHECK_EQ(apply_alias("S Bernau", al.as<JsonObjectConst>(), o), true); CHECK_EQ(o, std::string("Bernau!"));
    CHECK_EQ(apply_alias("Grunewald", al.as<JsonObjectConst>(), o), false);
    JsonDocument d; merge_defaults(d); CHECK_EQ(std::string(d["ziel_alias"]["Rathaus Spandau"] | ""), std::string("Spandau"));
  }
  rows = build_rows(c, 1, stops, "5x7", now, 5000);          // Alternativ-Linie
  CHECK_EQ(rows[1].label, std::string("S25")); CHECK_EQ(rows[1].zeit1, std::string("2'"));
  CHECK_EQ(rows[1].b, 255);                                  // blau
  CHECK_EQ(rows[0].label, std::string("U7"));                // Zeilen ohne Alternative bleiben
}

static void test_config_merge() {
  JsonDocument c; deserializeJson(c, R"({"brightness":10,"design":"bvg"})");
  merge_defaults(c);
  CHECK_EQ((int)c["brightness"], 10);
  CHECK_EQ(std::string(c["design"] | ""), std::string("bvg"));
  CHECK_EQ((int)c["update_interval"], 20);
  CHECK_EQ(c["stops"].size(), (size_t)4);
  CHECK_EQ(c["stops"][1]["alternativ"]["line"].as<std::string>(), std::string("S2,S25"));
  JsonDocument e; merge_defaults(e);                         // leeres Dokument
  CHECK_EQ(e["stops"].size(), (size_t)4);
}

int main() {
  test_reference_cases(); test_parse_iso_invalid(); test_text(); test_departures();
  test_cache_and_rows(); test_config_merge();
  std::cout << (fails ? "FEHLER: " : "OK: ") << checks << " Pruefungen, " << fails << " fehlgeschlagen\n";
  return fails ? 1 : 0;
}
