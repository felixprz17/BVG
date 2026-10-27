# BVG-Abfahrtsanzeige – LED-Matrix mit Matrix Portal S3

Eine Abfahrtsanzeige für Berliner Bus, U-Bahn, S-Bahn und Tram auf einer **128×32 LED-Matrix** (2× P4-Panel).
Sie läuft auf dem **Adafruit Matrix Portal S3** (ESP32-S3), holt die Echtzeit-Abfahrten per WLAN und wird bequem
über eine Weboberfläche im Handy-Browser eingestellt – ganz ohne Code anzufassen.

![Status](https://img.shields.io/badge/Plattform-ESP32--S3-blue) ![Arduino](https://img.shields.io/badge/Arduino-IDE%20%2F%20PlatformIO-green)

## Features

- **Flackerfrei**: Das Display wird per DMA/Hardware angesteuert, unabhängig von WLAN und HTTPS. Bilder sind doppelt gepuffert.
- **Weboberfläche** (`http://bvg-matrix.local/`): Haltestellen suchen, Linien und Richtungen per Klick wählen, Helligkeit, Designs usw.
- **WLAN-Einrichtung per Handy**: Beim ersten Start öffnet das Board ein eigenes WLAN mit Einrichtungsseite (Captive Portal).
- **Mehrere Linien und Richtungen** je Haltestelle (z. B. S2 + S25 + S26, Richtung Süden).
- **Alternativ-Linie**: Eine Zeile wechselt automatisch zwischen zwei Linien/Haltestellen.
- **5 Designs** (Dark Modern, BVG Classic, Minimal White, Night Mode, Retro Amber), **3 Schriftgrößen** plus **fette BVG-Schrift**.
- **Verspätungen** (ab 1 min) werden rot angezeigt, ausgefallene Fahrten werden ausgeblendet.
- **Auto-Helligkeit** nach Tageszeit (Tag/Nacht), Helligkeit auch manuell einstellbar.
- **Ziele umbenennen** (z. B. „Rathaus Spandau“ → „Spandau“) und Zusätze wie „(Berlin)“ ausblenden.
- **Mehr Haltestellen als Zeilen**: Die Anzeige blättert automatisch seitenweise.
- **Firmware-Update im Browser** (OTA): `.bin` hochladen, fertig. Einstellungen und WLAN bleiben erhalten.
- **Diagnose-Seite** unter `/api/debug` zur Fehlersuche.

---

## 1. Was du brauchst (Hardware)

| Teil | Hinweis |
|---|---|
| **Adafruit Matrix Portal S3** (ESP32-S3) | Das Controller-Board; versorgt über die Schraubklemmen auch die Panels |
| **2× P4 LED-Matrix, 64×32 Pixel, HUB75** | Je 256×128 mm, also zusammen 128×32 Pixel (512×128 mm). Wichtig: **64×32** und **HUB75** |
| **5 V Netzteil** | Empfohlen 5 V / 10 A (siehe unten). Weniger geht auch, wenn du die Helligkeit begrenzt |
| **2× HUB75-Flachbandkabel** | Meist bei den Panels dabei; eins vom Matrix Portal zu Panel 1 (IN), eins von Panel 1 (OUT) zu Panel 2 (IN) |
| **Stromkabel für die Panels** | Die Panels haben normalerweise ein rotes/schwarzes Kabel mit Kabelschuhen oder einen Stecker; sie werden an die Schraubklemmen des Matrix Portals angeschlossen |
| **USB-C-Kabel (Daten!)** | Zum Flashen. Ein reines Ladekabel funktioniert nicht |
| **Gehäuse** | Case von **steelbuddy** (siehe Abschnitt Gehäuse) |
| Optional | Schraubendreher, Kabelbinder, ggf. Sicherung/Schalter im 230-V-Zweig |

### Netzteil – Wie viel Strom brauchst du?

Ein 64×32-Panel zieht bei **voller Helligkeit und komplett weißem Bild** bis zu ca. **4 A**, zwei Panels also bis zu **8 A**.
Eine Abfahrtsanzeige mit Text auf schwarzem Grund und ca. 80 % Helligkeit braucht in der Praxis aber **deutlich weniger**
(oft nur 1–2 A insgesamt), weil die meisten Pixel dunkel bleiben.

- **Empfehlung: 5 V / 10 A.** Dann hast du Reserve für jeden Fall.
- **Es geht auch weniger** (z. B. 5 V / 4 A oder 5 V / 5 A), wenn du die Helligkeit in der Weboberfläche niedrig hältst und keine weißen Flächen anzeigst.
- **Das muss jeder selbst entscheiden.** Zu schwache Netzteile führen zu Flackern, Neustarts oder überhitzten Kabeln. Im Zweifel größer wählen.
- Nimm ein **qualitativ ordentliches Netzteil** (CE-geprüft, stabilisiert, 5 V fest oder auf genau 5,0 V einstellbar). Mehr als 5 V zerstört die Panels.

> ⚠️ **Sicherheit:** Du arbeitest mit 230 V am Netzteil. Gehäuse geschlossen halten, Netzteil nicht ohne Berührungsschutz betreiben,
> Leitungsquerschnitte passend zur Stromstärke wählen (bei 5 V / 10 A mindestens 1,5 mm²). Wenn du dir unsicher bist, nimm ein fertiges Steckernetzteil
> mit Hohlstecker + Schraubklemmen-Adapter. Der Aufbau erfolgt auf eigene Verantwortung.

---

## 2. Gehäuse (3D-Druck)

Das Gehäuse stammt von **steelbuddy**: *\<Link zum Modell hier eintragen\>*

- Druckt es laut Modellbeschreibung (Material, Wandstärke, Supports) und beachtet die Hinweise des Autors.
- Plane eine **Lüftungsmöglichkeit** ein; 5-V-Netzteil und Panels werden warm.
- Ein **Diffusor / eine dunkle Frontscheibe** (z. B. getöntes Acryl) verbessert Kontrast und Lesbarkeit deutlich.
- Bitte die Lizenz des Originalmodells beachten. Dieses Repository enthält das Gehäuse **nicht**.

---

## 3. Verkabelung

```
 Strom:
 230 V ──► [5 V Netzteil] ──► Matrix Portal S3 (Schraubklemmen 5 V / GND)
                                   ├──► Panel 1 (5 V / GND)
                                   └──► Panel 2 (5 V / GND)

 Daten (Flachbandkabel):
 [Matrix Portal S3] ──► Panel 1 (HUB75 IN)
 Panel 1 (HUB75 OUT) ──► Panel 2 (HUB75 IN)
```

**Schritt für Schritt:**

1. **Alles stromlos** lassen, Netzteil nicht angeschlossen.
2. **Strom vom Netzteil zum Matrix Portal S3**: Netzteil-**Plus (5 V)** und **Minus (GND)** an die **Schraubklemmen** des Matrix Portals anschließen.
3. **Strom vom Matrix Portal zu den Panels**: Von denselben Schraubklemmen je ein Stromkabel (5 V / GND) zu **Panel 1** und zu **Panel 2** führen. Das Board verteilt den Strom also an beide Panels.
4. **Datenkabel Matrix Portal → Panel 1**: Ein HUB75-Flachbandkabel vom Matrix Portal in den **Eingang (IN)** von Panel 1 stecken. Auf die Pfeile am Panel achten (die Signalrichtung läuft von IN nach OUT) und auf die Kerbe/Codierung am Stecker.
5. **Datenkabel Panel 1 → Panel 2**: Ein zweites Flachbandkabel vom **OUT** von Panel 1 in den **IN** von Panel 2 stecken.
6. **Polung dreimal prüfen** (rot = +5 V, schwarz = GND), erst dann Netzteil einschalten.
7. Das **USB-C-Kabel** brauchst du nur zum Flashen (oder zur Fehlersuche per Serial). Betreibe die Panels nie nur über USB.

> ⚠️ **Wichtig bei dieser Variante:** Der gesamte Strom der beiden Panels fließt über die Schraubklemmen und Leiterbahnen des Matrix Portals.
> Halte deshalb die **Helligkeit moderat** (z. B. 50–80 %), vermeide große weiße Flächen und prüfe Klemmen und Kabel nach den ersten Betriebsstunden auf Wärme.
> Wird etwas spürbar warm, Helligkeit senken oder die Panels direkt am Netzteil anschließen (Verteiler/Wago) und nur das Matrix Portal zusätzlich versorgen.

**Pinbelegung** (fest in der Boarddefinition, hier nur zur Info):
R1=42, G1=41, B1=40, R2=38, G2=39, B2=37, A=45, B=36, C=48, D=35, LAT=47, OE=14, CLK=2. Taste DOWN = GPIO 7.

---

## 4. Software installieren

Es gibt zwei Wege: **fertige Firmware flashen** (am einfachsten) oder **selbst kompilieren**.

### Weg A: Fertige Firmware flashen (empfohlen für Einsteiger)

Unter **Releases** findest du zwei Dateien:

- `bvg_matrix.ino.merged.bin`: komplettes Image (Bootloader + Partitionen + App) für den **Erstflash**
- `bvg_matrix.ino.bin`: nur die App, für **spätere Updates** über die Weboberfläche

**Erstflash mit dem Browser (Chrome oder Edge):**

1. Matrix Portal per USB-C mit dem Rechner verbinden.
2. Falls kein COM-Port erscheint: **BOOT-Taste gedrückt halten**, dabei kurz **RESET** drücken, dann BOOT loslassen (Bootloader-Modus).
3. Einen Web-Flasher öffnen, z. B. <https://espressif.github.io/esptool-js/>, **Connect** klicken, den Port wählen.
4. Flash-Adresse **`0x0`** eintragen, `bvg_matrix.ino.merged.bin` auswählen und **Program** klicken.
5. Danach **RESET** drücken.

**Alternativ mit esptool (Kommandozeile):**

```bash
pip install esptool
esptool.py --chip esp32s3 --baud 921600 write_flash 0x0 bvg_matrix.ino.merged.bin
```

### Weg B: Selbst kompilieren mit der Arduino IDE

1. **Arduino IDE 2.x** installieren.
2. **Boardverwalter** → „esp32“ von **Espressif** installieren (getestet mit Version **3.3.x**).
3. **Bibliotheksverwalter** → installieren:
   - `ESP32 HUB75 LED MATRIX PANEL DMA Display` (mrfaptastic)
   - `Adafruit GFX Library`
   - `ArduinoJson` (**Version 7**)
4. Ordner `bvg_matrix/` öffnen (Datei `bvg_matrix.ino`).
5. **Tools-Einstellungen:**

   | Einstellung | Wert |
   |---|---|
   | Board | **Adafruit MatrixPortal ESP32-S3** |
   | USB CDC On Boot | **Enabled** |
   | PSRAM | **QSPI PSRAM** (enabled), wird für die TLS-Verbindung gebraucht |
   | Flash Size | 8 MB |
   | Flash Mode | QIO |
   | Partition Scheme | eines **mit OTA** (zwei App-Slots), z. B. „TinyUF2 8MB“ oder „8MB with spiffs“ |
   | Upload Speed | 921600 |

6. Matrix Portal verbinden (ggf. BOOT + RESET, siehe oben), Port wählen, **Hochladen** klicken.

### Weg C: PlatformIO

```bash
pio run -t upload            # Umgebung matrixportal_s3
pio run -e generic_s3 -t upload   # falls dein PlatformIO das Board nicht kennt
```

Die fertige Datei für Updates liegt danach unter `.pio/build/matrixportal_s3/firmware.bin`.

---

## 5. Erster Start & WLAN einrichten

1. Stromversorgung einschalten. Ist noch kein WLAN gespeichert, zeigt die Matrix **„WLAN-Setup“**.
2. Mit dem **Handy** das offene WLAN **`BVG-Matrix-Setup`** verbinden.
3. Die Einrichtungsseite öffnet sich automatisch. Falls nicht: im Browser **`192.168.4.1`** aufrufen.
4. Dein **Heim-WLAN** auswählen (nur **2,4 GHz** wird unterstützt), **Passwort** eingeben (8–63 Zeichen), **Speichern & verbinden**.
5. Das Board startet neu und verbindet sich. Kurz erscheint die **IP-Adresse** auf der Matrix.
6. Handy wieder mit dem normalen WLAN verbinden und **`http://bvg-matrix.local/`** (oder die angezeigte IP) öffnen.

**WLAN später ändern:** Weboberfläche → *System* → „WLAN neu einrichten“, oder beim Einschalten die **DOWN-Taste gedrückt halten**.
Klappt die Verbindung nicht (falsches Passwort), öffnet sich das Setup-WLAN nach kurzer Zeit erneut.

---

## 6. Bedienung der Weboberfläche

| Tab | Funktion |
|---|---|
| **Anzeige** | Helligkeit, Auto-Helligkeit (Tag/Nacht), zweite Abfahrtszeit, Intervalle, Schriftgröße/fett, Ziele kürzen, Pixel-Feinjustierung |
| **Haltestellen** | Haltestelle suchen, Linien und Richtungen wählen, Farbe, Alternativ-Linie, Anzeigenamen ändern (✎) |
| **Design** | Eines der 5 Farbdesigns wählen |
| **System** | Neustart, Stopp, WLAN neu einrichten, Reboot |
| **Firmware** | Neue `.bin` hochladen (OTA-Update) |

**Haltestelle einrichten:**

1. Tab *Haltestellen* → **+ Haltestelle hinzufügen**.
2. Namen eintippen (z. B. „Hermannplatz“) und Treffer antippen.
3. **Linien** anwählen (mehrere möglich) und optional **Richtungen** (mehrere möglich, „alle“ = keine Einschränkung).
4. Farbe wählen. Mit **✎** an einer Richtung änderst du den angezeigten Zielnamen.
5. Optional **Alternativ-Linie** aktivieren; die Zeile wechselt dann im Anzeige-Intervall zwischen Haupt- und Alternativ-Linie.
6. **Speichern & Apply** drücken.

**Hinweise zur Anzeige:**
- Zeiten stehen rechtsbündig nebeneinander (3 Zeichen je Spalte, ab 100 min „>99“).
- Zeilen pro Seite: Klein 5, Normal 4, Groß 3. Bei mehr Haltestellen blättert die Anzeige automatisch.
- Rot = Abfahrt mindestens 1 Minute verspätet.

---

## 7. Firmware aktualisieren

- **Im Browser (OTA):** Weboberfläche → Tab *Firmware* → `bvg_matrix.ino.bin` (die Datei **ohne** „bootloader“, „partitions“ oder „merged“) wählen → *Hochladen & installieren*. Einstellungen und WLAN bleiben erhalten.
- **Arduino IDE:** *Sketch → Kompilierte Binärdatei exportieren*.
- **Per USB:** wie beim Erstflash (siehe oben).

---

## 8. Weboberfläche ändern (für Entwickler)

Die Oberfläche wird als C++-Raw-String in die Firmware eingebettet.

1. `ui/index.html` bzw. `ui/setup.html` bearbeiten.
2. `python3 tools/gen_html.py` ausführen, erzeugt `bvg_matrix/web_ui.h`.
3. Neu kompilieren und flashen.

`web_ui.h` nicht von Hand ändern, die Datei wird überschrieben.

## Projektstruktur

```
bvg_matrix/        Arduino-Sketch (bvg_matrix.ino, logic.h, bvgfont.h, web_ui.h)
ui/                Weboberfläche (index.html, setup.html)
tools/gen_html.py  Erzeugt web_ui.h aus ui/*.html
platformio.ini     PlatformIO-Konfiguration
partitions.csv     Partitionstabelle (OTA)
```

---

## 9. Fehlersuche

| Problem | Lösung |
|---|---|
| **Keine Abfahrten / „E-…“ oder „keine Zeit“ auf dem Display** | `http://bvg-matrix.local/api/debug` öffnen: zeigt Zeit-Sync, Speicher, letzte API-Antwort und Cache. Prüfen, ob **PSRAM aktiviert** ist. „keine Zeit“ heißt: NTP nicht erreichbar (Internet/Firewall prüfen) |
| **Display bleibt schwarz** | Stromversorgung und Polung prüfen, Flachbandkabel im HUB75-**IN** von Panel 1 (nicht im OUT)? Panels haben 5 V? |
| **Bild geschoben, Farben falsch, Geisterbilder** | Anderer Panel-Chip: in `setup()` `mxconfig.driver = HUB75_I2S_CFG::FM6126A;` bzw. `mxconfig.clkphase = false;` ergänzen |
| **Flackern, Neustarts** | Netzteil zu schwach oder Kabel zu dünn. Helligkeit senken oder die Panels direkt vom Netzteil statt über das Matrix Portal versorgen |
| **Kein COM-Port** | Datenkabel verwenden, BOOT gedrückt halten und RESET drücken |
| **`bvg-matrix.local` geht nicht** | IP-Adresse von der Matrix ablesen oder im Router nachsehen (manche Android-Geräte lösen `.local` nicht auf) |
| **Text 1 px zu hoch/tief** | Weboberfläche → Anzeige → *Feinjustierung* |
| **Haltestellensuche meldet „BVG-API nicht erreichbar“** | Die öffentliche API ist zeitweise überlastet (HTTP 503). Die Firmware probiert automatisch einen zweiten Server |

## Technische Hinweise

- Datenquelle ist die öffentliche **transport.rest**-API (`v6.bvg.transport.rest`, Ausweichserver `v6.vbb.transport.rest`). Es gibt keine Garantie für Verfügbarkeit.
- HTTPS erfolgt ohne Zertifikatsprüfung (`setInsecure`), da nur öffentliche Abfahrtsdaten abgefragt werden.
- Konfiguration und WLAN-Zugangsdaten liegen im NVS-Flash, kein Dateisystem nötig. Die Zugangsdaten stehen **nicht** im Quellcode.
- Die Abfragen laufen in einem eigenen Task auf Core 0, die Anzeige ist davon entkoppelt.

## Lizenz

*\<z. B. MIT-Lizenz, Datei `LICENSE` hinzufügen\>*

Dies ist ein privates Hobbyprojekt und steht in keiner Verbindung zur BVG. „BVG“ und Liniensymbole sind Marken ihrer Inhaber.

## Credits

- Gehäuse: **steelbuddy**
- [ESP32-HUB75-MatrixPanel-DMA](https://github.com/mrfaptastic/ESP32-HUB75-MatrixPanel-DMA) von mrfaptastic
- [ArduinoJson](https://arduinojson.org/) von Benoit Blanchon
- [Adafruit GFX](https://github.com/adafruit/Adafruit-GFX-Library)
- Abfahrtsdaten: [transport.rest](https://transport.rest/)
