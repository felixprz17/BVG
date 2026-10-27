# BVG-Abfahrtsanzeige â€“ LED-Matrix mit Matrix Portal S3

Eine Abfahrtsanzeige fÃ¼r Berliner Bus, U-Bahn, S-Bahn und Tram auf einer **128Ã—32 LED-Matrix** (2Ã— P4-Panel).
Sie lÃ¤uft auf dem **Adafruit Matrix Portal S3** (ESP32-S3), holt die Echtzeit-Abfahrten per WLAN und wird bequem
Ã¼ber eine WeboberflÃ¤che im Handy-Browser eingestellt â€“ ganz ohne Code anzufassen.

![Status](https://img.shields.io/badge/Plattform-ESP32--S3-blue) ![Arduino](https://img.shields.io/badge/Arduino-IDE%20%2F%20PlatformIO-green)

## Features

- **Flackerfrei**: Das Display wird per DMA/Hardware angesteuert, unabhÃ¤ngig von WLAN und HTTPS. Bilder sind doppelt gepuffert.
- **WeboberflÃ¤che** (`http://bvg-matrix.local/`): Haltestellen suchen, Linien und Richtungen per Klick wÃ¤hlen, Helligkeit, Designs usw.
- **WLAN-Einrichtung per Handy**: Beim ersten Start Ã¶ffnet das Board ein eigenes WLAN mit Einrichtungsseite (Captive Portal).
- **Mehrere Linien und Richtungen** je Haltestelle (z. B. S2 + S25 + S26, Richtung SÃ¼den).
- **Alternativ-Linie**: Eine Zeile wechselt automatisch zwischen zwei Linien/Haltestellen.
- **5 Designs** (Dark Modern, BVG Classic, Minimal White, Night Mode, Retro Amber), **3 SchriftgrÃ¶ÃŸen** plus **fette BVG-Schrift**.
- **VerspÃ¤tungen** (ab 1 min) werden rot angezeigt, ausgefallene Fahrten werden ausgeblendet.
- **Auto-Helligkeit** nach Tageszeit (Tag/Nacht), Helligkeit auch manuell einstellbar.
- **Ziele umbenennen** (z. B. â€žRathaus Spandauâ€œ â†’ â€žSpandauâ€œ) und ZusÃ¤tze wie â€ž(Berlin)â€œ ausblenden.
- **Mehr Haltestellen als Zeilen**: Die Anzeige blÃ¤ttert automatisch seitenweise.
- **Firmware-Update im Browser** (OTA): `.bin` hochladen, fertig. Einstellungen und WLAN bleiben erhalten.
- **Diagnose-Seite** unter `/api/debug` zur Fehlersuche.

---

## 1. Was du brauchst (Hardware)

| Teil | Hinweis |
|---|---|
| **Adafruit Matrix Portal S3** (ESP32-S3) | Das Controller-Board; versorgt Ã¼ber die Schraubklemmen auch die Panels |
| **2Ã— P4 LED-Matrix, 64Ã—32 Pixel, HUB75** | Je 256Ã—128 mm, also zusammen 128Ã—32 Pixel (512Ã—128 mm). Wichtig: **64Ã—32** und **HUB75** |
| **5 V Netzteil** | Empfohlen 5 V / 10 A (siehe unten). Weniger geht auch, wenn du die Helligkeit begrenzt |
| **2Ã— HUB75-Flachbandkabel** | Meist bei den Panels dabei; eins vom Matrix Portal zu Panel 1 (IN), eins von Panel 1 (OUT) zu Panel 2 (IN) |
| **Stromkabel fÃ¼r die Panels** | Die Panels haben normalerweise ein rotes/schwarzes Kabel mit Kabelschuhen oder einen Stecker; sie werden an die Schraubklemmen des Matrix Portals angeschlossen |
| **USB-C-Kabel (Daten!)** | Zum Flashen. Ein reines Ladekabel funktioniert nicht |
| **GehÃ¤use** | Case von **steelbuddy** (siehe Abschnitt GehÃ¤use) |
| Optional | Schraubendreher, Kabelbinder, ggf. Sicherung/Schalter im 230-V-Zweig |

### Netzteil â€“ Wie viel Strom brauchst du?

Ein 64Ã—32-Panel zieht bei **voller Helligkeit und komplett weiÃŸem Bild** bis zu ca. **4 A**, zwei Panels also bis zu **8 A**.
Eine Abfahrtsanzeige mit Text auf schwarzem Grund und ca. 80 % Helligkeit braucht in der Praxis aber **deutlich weniger**
(oft nur 1â€“2 A insgesamt), weil die meisten Pixel dunkel bleiben.

- **Empfehlung: 5 V / 10 A.** Dann hast du Reserve fÃ¼r jeden Fall.
- **Es geht auch weniger** (z. B. 5 V / 4 A oder 5 V / 5 A), wenn du die Helligkeit in der WeboberflÃ¤che niedrig hÃ¤ltst und keine weiÃŸen FlÃ¤chen anzeigst.
- **Das muss jeder selbst entscheiden.** Zu schwache Netzteile fÃ¼hren zu Flackern, Neustarts oder Ã¼berhitzten Kabeln. Im Zweifel grÃ¶ÃŸer wÃ¤hlen.
- Nimm ein **qualitativ ordentliches Netzteil** (CE-geprÃ¼ft, stabilisiert, 5 V fest oder auf genau 5,0 V einstellbar). Mehr als 5 V zerstÃ¶rt die Panels.

> âš ï¸ **Sicherheit:** Du arbeitest mit 230 V am Netzteil. GehÃ¤use geschlossen halten, Netzteil nicht ohne BerÃ¼hrungsschutz betreiben,
> Leitungsquerschnitte passend zur StromstÃ¤rke wÃ¤hlen (bei 5 V / 10 A mindestens 1,5 mmÂ²). Wenn du dir unsicher bist, nimm ein fertiges Steckernetzteil
> mit Hohlstecker + Schraubklemmen-Adapter. Der Aufbau erfolgt auf eigene Verantwortung.

---

## 2. GehÃ¤use (3D-Druck)

Das GehÃ¤use stammt von **steelbuddy**: *\<Link zum Modell hier eintragen\>*

- Druckt es laut Modellbeschreibung (Material, WandstÃ¤rke, Supports) und beachtet die Hinweise des Autors.
- Plane eine **LÃ¼ftungsmÃ¶glichkeit** ein; 5-V-Netzteil und Panels werden warm.
- Ein **Diffusor / eine dunkle Frontscheibe** (z. B. getÃ¶ntes Acryl) verbessert Kontrast und Lesbarkeit deutlich.
- Bitte die Lizenz des Originalmodells beachten. Dieses Repository enthÃ¤lt das GehÃ¤use **nicht**.

---

## 3. Verkabelung

```
 Strom:
 230 V â”€â”€â–º [5 V Netzteil] â”€â”€â–º Matrix Portal S3 (Schraubklemmen 5 V / GND)
                                   â”œâ”€â”€â–º Panel 1 (5 V / GND)
                                   â””â”€â”€â–º Panel 2 (5 V / GND)

 Daten (Flachbandkabel):
 [Matrix Portal S3] â”€â”€â–º Panel 1 (HUB75 IN)
 Panel 1 (HUB75 OUT) â”€â”€â–º Panel 2 (HUB75 IN)
```

**Schritt fÃ¼r Schritt:**

1. **Alles stromlos** lassen, Netzteil nicht angeschlossen.
2. **Strom vom Netzteil zum Matrix Portal S3**: Netzteil-**Plus (5 V)** und **Minus (GND)** an die **Schraubklemmen** des Matrix Portals anschlieÃŸen.
3. **Strom vom Matrix Portal zu den Panels**: Von denselben Schraubklemmen je ein Stromkabel (5 V / GND) zu **Panel 1** und zu **Panel 2** fÃ¼hren. Das Board verteilt den Strom also an beide Panels.
4. **Datenkabel Matrix Portal â†’ Panel 1**: Ein HUB75-Flachbandkabel vom Matrix Portal in den **Eingang (IN)** von Panel 1 stecken. Auf die Pfeile am Panel achten (die Signalrichtung lÃ¤uft von IN nach OUT) und auf die Kerbe/Codierung am Stecker.
5. **Datenkabel Panel 1 â†’ Panel 2**: Ein zweites Flachbandkabel vom **OUT** von Panel 1 in den **IN** von Panel 2 stecken.
6. **Polung dreimal prÃ¼fen** (rot = +5 V, schwarz = GND), erst dann Netzteil einschalten.
7. Das **USB-C-Kabel** brauchst du nur zum Flashen (oder zur Fehlersuche per Serial). Betreibe die Panels nie nur Ã¼ber USB.

> âš ï¸ **Wichtig bei dieser Variante:** Der gesamte Strom der beiden Panels flieÃŸt Ã¼ber die Schraubklemmen und Leiterbahnen des Matrix Portals.
> Halte deshalb die **Helligkeit moderat** (z. B. 50â€“80 %), vermeide groÃŸe weiÃŸe FlÃ¤chen und prÃ¼fe Klemmen und Kabel nach den ersten Betriebsstunden auf WÃ¤rme.
> Wird etwas spÃ¼rbar warm, Helligkeit senken oder die Panels direkt am Netzteil anschlieÃŸen (Verteiler/Wago) und nur das Matrix Portal zusÃ¤tzlich versorgen.

**Pinbelegung** (fest in der Boarddefinition, hier nur zur Info):
R1=42, G1=41, B1=40, R2=38, G2=39, B2=37, A=45, B=36, C=48, D=35, LAT=47, OE=14, CLK=2. Taste DOWN = GPIO 7.

---

## 4. Software installieren

Es gibt zwei Wege: **fertige Firmware flashen** (am einfachsten) oder **selbst kompilieren**.

### Weg A: Fertige Firmware flashen (empfohlen fÃ¼r Einsteiger)

Unter **Releases** findest du zwei Dateien:

- `bvg_matrix.ino.merged.bin`: komplettes Image (Bootloader + Partitionen + App) fÃ¼r den **Erstflash**
- `bvg_matrix.ino.bin`: nur die App, fÃ¼r **spÃ¤tere Updates** Ã¼ber die WeboberflÃ¤che

**Erstflash mit dem Browser (Chrome oder Edge):**

1. Matrix Portal per USB-C mit dem Rechner verbinden.
2. Falls kein COM-Port erscheint: **BOOT-Taste gedrÃ¼ckt halten**, dabei kurz **RESET** drÃ¼cken, dann BOOT loslassen (Bootloader-Modus).
3. Einen Web-Flasher Ã¶ffnen, z. B. <https://espressif.github.io/esptool-js/>, **Connect** klicken, den Port wÃ¤hlen.
4. Flash-Adresse **`0x0`** eintragen, `bvg_matrix.ino.merged.bin` auswÃ¤hlen und **Program** klicken.
5. Danach **RESET** drÃ¼cken.

**Alternativ mit esptool (Kommandozeile):**

```bash
pip install esptool
esptool.py --chip esp32s3 --baud 921600 write_flash 0x0 bvg_matrix.ino.merged.bin
```

### Weg B: Selbst kompilieren mit der Arduino IDE

1. **Arduino IDE 2.x** installieren.
2. **Boardverwalter** â†’ â€žesp32â€œ von **Espressif** installieren (getestet mit Version **3.3.x**).
3. **Bibliotheksverwalter** â†’ installieren:
   - `ESP32 HUB75 LED MATRIX PANEL DMA Display` (mrfaptastic)
   - `Adafruit GFX Library`
   - `ArduinoJson` (**Version 7**)
4. Ordner `bvg_matrix/` Ã¶ffnen (Datei `bvg_matrix.ino`).
5. **Tools-Einstellungen:**

   | Einstellung | Wert |
   |---|---|
   | Board | **Adafruit MatrixPortal ESP32-S3** |
   | USB CDC On Boot | **Enabled** |
   | PSRAM | **QSPI PSRAM** (enabled), wird fÃ¼r die TLS-Verbindung gebraucht |
   | Flash Size | 8 MB |
   | Flash Mode | QIO |
   | Partition Scheme | eines **mit OTA** (zwei App-Slots), z. B. â€žTinyUF2 8MBâ€œ oder â€ž8MB with spiffsâ€œ |
   | Upload Speed | 921600 |

6. Matrix Portal verbinden (ggf. BOOT + RESET, siehe oben), Port wÃ¤hlen, **Hochladen** klicken.

### Weg C: PlatformIO

```bash
pio run -t upload            # Umgebung matrixportal_s3
pio run -e generic_s3 -t upload   # falls dein PlatformIO das Board nicht kennt
```

Die fertige Datei fÃ¼r Updates liegt danach unter `.pio/build/matrixportal_s3/firmware.bin`.

---

## 5. Erster Start & WLAN einrichten

1. Stromversorgung einschalten. Ist noch kein WLAN gespeichert, zeigt die Matrix **â€žWLAN-Setupâ€œ**.
2. Mit dem **Handy** das offene WLAN **`BVG-Matrix-Setup`** verbinden.
3. Die Einrichtungsseite Ã¶ffnet sich automatisch. Falls nicht: im Browser **`192.168.4.1`** aufrufen.
4. Dein **Heim-WLAN** auswÃ¤hlen (nur **2,4 GHz** wird unterstÃ¼tzt), **Passwort** eingeben (8â€“63 Zeichen), **Speichern & verbinden**.
5. Das Board startet neu und verbindet sich. Kurz erscheint die **IP-Adresse** auf der Matrix.
6. Handy wieder mit dem normalen WLAN verbinden und **`http://bvg-matrix.local/`** (oder die angezeigte IP) Ã¶ffnen.

**WLAN spÃ¤ter Ã¤ndern:** WeboberflÃ¤che â†’ *System* â†’ â€žWLAN neu einrichtenâ€œ, oder beim Einschalten die **DOWN-Taste gedrÃ¼ckt halten**.
Klappt die Verbindung nicht (falsches Passwort), Ã¶ffnet sich das Setup-WLAN nach kurzer Zeit erneut.

---

## 6. Bedienung der WeboberflÃ¤che

| Tab | Funktion |
|---|---|
| **Anzeige** | Helligkeit, Auto-Helligkeit (Tag/Nacht), zweite Abfahrtszeit, Intervalle, SchriftgrÃ¶ÃŸe/fett, Ziele kÃ¼rzen, Pixel-Feinjustierung |
| **Haltestellen** | Haltestelle suchen, Linien und Richtungen wÃ¤hlen, Farbe, Alternativ-Linie, Anzeigenamen Ã¤ndern (âœŽ) |
| **Design** | Eines der 5 Farbdesigns wÃ¤hlen |
| **System** | Neustart, Stopp, WLAN neu einrichten, Reboot |
| **Firmware** | Neue `.bin` hochladen (OTA-Update) |

**Haltestelle einrichten:**

1. Tab *Haltestellen* â†’ **+ Haltestelle hinzufÃ¼gen**.
2. Namen eintippen (z. B. â€žHermannplatzâ€œ) und Treffer antippen.
3. **Linien** anwÃ¤hlen (mehrere mÃ¶glich) und optional **Richtungen** (mehrere mÃ¶glich, â€žalleâ€œ = keine EinschrÃ¤nkung).
4. Farbe wÃ¤hlen. Mit **âœŽ** an einer Richtung Ã¤nderst du den angezeigten Zielnamen.
5. Optional **Alternativ-Linie** aktivieren; die Zeile wechselt dann im Anzeige-Intervall zwischen Haupt- und Alternativ-Linie.
6. **Speichern & Apply** drÃ¼cken.

**Hinweise zur Anzeige:**
- Zeiten stehen rechtsbÃ¼ndig nebeneinander (3 Zeichen je Spalte, ab 100 min â€ž>99â€œ).
- Zeilen pro Seite: Klein 5, Normal 4, GroÃŸ 3. Bei mehr Haltestellen blÃ¤ttert die Anzeige automatisch.
- Rot = Abfahrt mindestens 1 Minute verspÃ¤tet.

---

## 7. Firmware aktualisieren

- **Im Browser (OTA):** WeboberflÃ¤che â†’ Tab *Firmware* â†’ `bvg_matrix.ino.bin` (die Datei **ohne** â€žbootloaderâ€œ, â€žpartitionsâ€œ oder â€žmergedâ€œ) wÃ¤hlen â†’ *Hochladen & installieren*. Einstellungen und WLAN bleiben erhalten.
- **Arduino IDE:** *Sketch â†’ Kompilierte BinÃ¤rdatei exportieren*.
- **Per USB:** wie beim Erstflash (siehe oben).

---

## 8. WeboberflÃ¤che Ã¤ndern (fÃ¼r Entwickler)

Die OberflÃ¤che wird als C++-Raw-String in die Firmware eingebettet.

1. `ui/index.html` bzw. `ui/setup.html` bearbeiten.
2. `python3 tools/gen_html.py` ausfÃ¼hren, erzeugt `bvg_matrix/web_ui.h`.
3. Neu kompilieren und flashen.

`web_ui.h` nicht von Hand Ã¤ndern, die Datei wird Ã¼berschrieben.

## Projektstruktur

```
bvg_matrix/        Arduino-Sketch (bvg_matrix.ino, logic.h, bvgfont.h, web_ui.h)
ui/                WeboberflÃ¤che (index.html, setup.html)
tools/gen_html.py  Erzeugt web_ui.h aus ui/*.html
platformio.ini     PlatformIO-Konfiguration
partitions.csv     Partitionstabelle (OTA)
```

---

## 9. Fehlersuche

| Problem | LÃ¶sung |
|---|---|
| **Keine Abfahrten / â€žE-â€¦â€œ oder â€žkeine Zeitâ€œ auf dem Display** | `http://bvg-matrix.local/api/debug` Ã¶ffnen: zeigt Zeit-Sync, Speicher, letzte API-Antwort und Cache. PrÃ¼fen, ob **PSRAM aktiviert** ist. â€žkeine Zeitâ€œ heiÃŸt: NTP nicht erreichbar (Internet/Firewall prÃ¼fen) |
| **Display bleibt schwarz** | Stromversorgung und Polung prÃ¼fen, Flachbandkabel im HUB75-**IN** von Panel 1 (nicht im OUT)? Panels haben 5 V? |
| **Bild geschoben, Farben falsch, Geisterbilder** | Anderer Panel-Chip: in `setup()` `mxconfig.driver = HUB75_I2S_CFG::FM6126A;` bzw. `mxconfig.clkphase = false;` ergÃ¤nzen |
| **Flackern, Neustarts** | Netzteil zu schwach oder Kabel zu dÃ¼nn. Helligkeit senken oder die Panels direkt vom Netzteil statt Ã¼ber das Matrix Portal versorgen |
| **Kein COM-Port** | Datenkabel verwenden, BOOT gedrÃ¼ckt halten und RESET drÃ¼cken |
| **`bvg-matrix.local` geht nicht** | IP-Adresse von der Matrix ablesen oder im Router nachsehen (manche Android-GerÃ¤te lÃ¶sen `.local` nicht auf) |
| **Text 1 px zu hoch/tief** | WeboberflÃ¤che â†’ Anzeige â†’ *Feinjustierung* |
| **Haltestellensuche meldet â€žBVG-API nicht erreichbarâ€œ** | Die Ã¶ffentliche API ist zeitweise Ã¼berlastet (HTTP 503). Die Firmware probiert automatisch einen zweiten Server |

## Technische Hinweise

- Datenquelle ist die Ã¶ffentliche **transport.rest**-API (`v6.bvg.transport.rest`, Ausweichserver `v6.vbb.transport.rest`). Es gibt keine Garantie fÃ¼r VerfÃ¼gbarkeit.
- HTTPS erfolgt ohne ZertifikatsprÃ¼fung (`setInsecure`), da nur Ã¶ffentliche Abfahrtsdaten abgefragt werden.
- Konfiguration und WLAN-Zugangsdaten liegen im NVS-Flash, kein Dateisystem nÃ¶tig. Die Zugangsdaten stehen **nicht** im Quellcode.
- Die Abfragen laufen in einem eigenen Task auf Core 0, die Anzeige ist davon entkoppelt.

## Lizenz

*\<z. B. MIT-Lizenz, Datei `LICENSE` hinzufÃ¼gen\>*

Dies ist ein privates Hobbyprojekt und steht in keiner Verbindung zur BVG. â€žBVGâ€œ und Liniensymbole sind Marken ihrer Inhaber.

## Credits

- GehÃ¤use: **steelbuddy**
- [ESP32-HUB75-MatrixPanel-DMA](https://github.com/mrfaptastic/ESP32-HUB75-MatrixPanel-DMA) von mrfaptastic
- [ArduinoJson](https://arduinojson.org/) von Benoit Blanchon
- [Adafruit GFX](https://github.com/adafruit/Adafruit-GFX-Library)
- Abfahrtsdaten: [transport.rest](https://transport.rest/)
