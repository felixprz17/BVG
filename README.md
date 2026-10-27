# BVG-Abfahrtsanzeige – Arduino-Version (Matrix Portal S3, 128x32)

Gleiche Funktionen wie die CircuitPython-Version (Weboberfläche, Handy-WLAN-Setup, 5 Designs, 3 Schriften,
Auto-Helligkeit, Haltestellensuche, Alternativ-Linie) – aber **ohne Flackern**: Das Display wird per DMA/Hardware
ausgegeben, unabhängig von WLAN und HTTPS. Bilder werden doppelt gepuffert (erst komplett zeichnen, dann umschalten).
Die Abfragen laufen in einem eigenen Task auf Core 0.

> **Status:** Die Logik (Zeit, BVG-Parsing, Cache, Zeilenaufbau, URL-Kodierung) ist am PC getestet
> (`test/run.sh`, ~48.000 Prüfungen gegen Python-Referenz). Die Firmware selbst konnte in meiner Umgebung **nicht für
> den ESP32-S3 kompiliert und nicht auf Hardware getestet werden** (Toolchain-Downloads gesperrt). Der Sketch besteht
> nur einen Syntax-Check gegen Attrappen. Rechne mit kleinen Anpassungen beim ersten Kompilieren – Fehlermeldung
> schicken, dann fixe ich es.

## Arduino IDE
1. Board-Paket **esp32 von Espressif** installieren (Boardverwalter), Board **Adafruit MatrixPortal ESP32-S3** wählen.
2. Bibliotheken (Bibliotheksverwalter): `ESP32 HUB75 LED MATRIX PANEL DMA Display`, `Adafruit GFX Library`,
   `U8g2_for_Adafruit_GFX`, `ArduinoJson` (Version 7).
3. Tools → Partition Scheme: eines mit **OTA** (zwei App-Slots) (z. B. „8MB with spiffs“ / „Default with spiffs“;
   bei Auswahl „TinyUF2“ passend dazu). Tools → USB CDC On Boot: Enabled.
4. Ordner `bvg_matrix/` öffnen (`bvg_matrix.ino`), hochladen. Wie bei deinem bisherigen Sketch: ggf. vorher BOOT
   gedrückt halten + RESET drücken, damit der Port erscheint.

## PlatformIO
`pio run -t upload` (Umgebung `matrixportal_s3`, bei Problemen `-e generic_s3`).

## Erster Start
Kein WLAN gespeichert → Board öffnet das WLAN **BVG-Matrix-Setup** (offen). Mit dem Handy verbinden, die Seite öffnet
sich automatisch (sonst `192.168.4.1`), WLAN wählen, Passwort eingeben, speichern → Neustart. Erneut einrichten:
Weboberfläche → System → „WLAN neu einrichten“, oder beim Einschalten die **DOWN-Taste** halten.
Danach: `http://bvg-matrix.local/` (oder die auf dem Display angezeigte IP).

## Updates
Weboberfläche → Tab **Firmware** → `.bin` hochladen (Arduino IDE: Sketch → Kompilierte Binärdatei exportieren;
PlatformIO: `.pio/build/matrixportal_s3/firmware.bin`). Einstellungen und WLAN bleiben erhalten.

## Weboberfläche ändern
`ui/index.html` / `ui/setup.html` bearbeiten, dann `python3 tools/gen_html.py` (erzeugt `bvg_matrix/web_ui.h`).

## Anzeige & Gruppen
- Zeiten stehen rechtsbündig direkt nebeneinander (3 Zeichen je Spalte, ab 100 min „>99“) – das Ziel bekommt mehr Platz.
- Verspätete Abfahrten (≥ 1 min, Echtzeit gegenüber Fahrplan) werden **rot** angezeigt; ausgefallene Fahrten werden ausgeblendet.
- Haltestellen-Tab: **mehrere Linien** und **mehrere Richtungen** wählbar (z. B. S2 + S25 + S26, Richtungen Blankenfelde + Teltow Stadt = „alle Richtung Süden“).
  Gespeichert wird `line` = `S2,S25,S26` und `direction` = `Blankenfelde|Teltow Stadt` (Teilstrings, ODER-verknüpft).

- Anzeige-Tab → **Ziele umbenennen**: z. B. „Rathaus Spandau“ → „Spandau“ (Standard ist schon eingetragen).

## Hinweise
- **Keine Abfahrten?** `http://bvg-matrix.local/api/debug` (bzw. IP) im Browser öffnen: zeigt Zeit-Sync, Speicher, letzte API-Antwort und Cache. In der Arduino IDE außerdem Tools → **PSRAM: QSPI PSRAM** aktivieren (die TLS-Verbindung braucht viel RAM).
- Pinbelegung (aus der Adafruit-Boarddefinition): R1=42 G1=41 B1=40 R2=38 G2=39 B2=37, A=45 B=36 C=48 D=35, LAT=47, OE=14, CLK=2.
- Helligkeit läuft jetzt über die Hardware (Slider/Auto-Helligkeit wie bisher). Bit-Depth-Einstellung entfällt.
- Falls die Farben falsch oder das Bild geschoben ist (andere Panel-Chips, z. B. FM6126/ICN2038): in `setup()`
  `mxconfig.driver = HUB75_I2S_CFG::FM6126A;` bzw. `mxconfig.clkphase = false;` ergänzen.
- Der Text sitzt evtl. 1 px höher/tiefer als bei der CircuitPython-Version (andere Schrift-Engine) – in `drawText()` justierbar.
- HTTPS zur BVG-API ohne Zertifikatsprüfung (`setInsecure`), da nur öffentliche Abfahrtsdaten.
- Config und WLAN-Daten liegen im NVS-Flash (kein Dateisystem nötig). Schlüssel der Config wie in der CircuitPython-Version.
- Pixel-Feinjustierung (Label, Ziel, Zeiten, vertikal): Weboberfläche → Anzeige → „Feinjustierung“.

## Tests (PC)
`test/run.sh` (g++ und Python nötig): Zeit-Parsing gegen `zoneinfo`, URL-Kodierung gegen `urllib`, Filter, Cache,
Zeilenaufbau, Config-Defaults.
"# BVG" 
