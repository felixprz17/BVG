// AUTOMATISCH ERZEUGT von tools/gen_html.py - nicht von Hand aendern (ui/*.html bearbeiten)
#pragma once
#include <pgmspace.h>
static const char INDEX_HTML[] PROGMEM =
R"BVGHTML(<!DOCTYPE html>
<html lang="de">
<head>
<meta charset="UTF-8">
<meta name="viewport" content="width=device-width,initial-scale=1,maximum-scale=1">
<title>BVG Matrix</title>
<style>
@import url('https://fonts.googleapis.com/css2?family=IBM+Plex+Mono:wght@400;600&family=IBM+Plex+Sans:wght@400;500;600&display=swap');
:root{
  --bg:#0a0a0f;--surface:#12121a;--border:#1e1e2e;
  --accent:#f0c000;--green:#00c864;--red:#e03c3c;
  --blue:#0a64ff;--orange:#ff5000;--purple:#c800c8;
  --text:#e0e0e0;--muted:#606070;
  --mono:'IBM Plex Mono',monospace;--sans:'IBM Plex Sans',sans-serif;
}
*{box-sizing:border-box;margin:0;padding:0}
body{background:var(--bg);color:var(--text);font-family:var(--sans);min-height:100vh;padding-bottom:50px}
header{background:var(--surface);border-bottom:1px solid var(--border);padding:14px 20px;display:flex;align-items:center;justify-content:space-between;position:sticky;top:0;z-index:10}
.logo{display:flex;align-items:center;gap:10px}
.logo-grid{display:grid;grid-template-columns:repeat(3,7px);gap:3px}
.logo-grid span{width:7px;height:7px;border-radius:50%;background:var(--border)}
.logo-grid span:nth-child(2){background:var(--blue)}
.logo-grid span:nth-child(5){background:var(--accent)}
.logo-grid span:nth-child(8){background:var(--green)}
.logo h1{font-family:var(--mono);font-size:15px;font-weight:600}
.logo h1 em{color:var(--accent);font-style:normal}
.badge{font-family:var(--mono);font-size:11px;padding:4px 10px;border-radius:20px;border:1px solid var(--border);display:flex;align-items:center;gap:6px;color:var(--muted)}
.badge .dot{width:7px;height:7px;border-radius:50%;background:var(--muted)}
.badge.on{border-color:var(--green);color:var(--green)}
.badge.on .dot{background:var(--green);box-shadow:0 0 6px var(--green);animation:blink 2s infinite}
.badge.off{border-color:var(--red);color:var(--red)}
.badge.off .dot{background:var(--red)}
@keyframes blink{0%,100%{opacity:1}50%{opacity:.3}}
.tabs{display:flex;background:var(--surface);border-bottom:1px solid var(--border);padding:0 16px;overflow-x:auto;gap:2px}
.tab{font-family:var(--mono);font-size:12px;padding:11px 13px;cursor:pointer;color:var(--muted);border:none;border-bottom:2px solid transparent;background:none;white-space:nowrap;transition:color .15s}
.tab.active{color:var(--accent);border-bottom-color:var(--accent)}
.page{display:none;padding:16px 20px;max-width:480px;margin:0 auto}
.page.active{display:block}
.led-screen{background:#000;border:1px solid #1a1a2a;border-radius:8px;padding:10px 8px;margin-bottom:18px;font-family:var(--mono)}
.led-row{display:flex;align-items:center;gap:5px;padding:3px 0;border-bottom:1px solid #0d0d15}
.led-row:last-child{border:none}
.led-lbl{font-size:10px;font-weight:700;min-width:30px}
.led-ziel{flex:1;font-size:10px;color:#c8c8c8}
.led-t1{font-size:10px;color:#f0c000;min-width:34px;text-align:right}
.led-t2{font-size:10px;color:#404050;min-width:28px;text-align:right}
.led-t1.now{color:#00ff50}
.card{background:var(--surface);border:1px solid var(--border);border-radius:8px;padding:16px;margin-bottom:14px}
.card-title{font-family:var(--mono);font-size:10px;color:var(--muted);text-transform:uppercase;letter-spacing:.1em;margin-bottom:14px}
.sl-row{display:flex;align-items:center;gap:10px;margin-bottom:6px}
.sl-label{font-size:13px;min-width:90px}
.sl-val{font-family:var(--mono);font-size:13px;color:var(--accent);min-width:38px;text-align:right}
input[type=range]{flex:1;-webkit-appearance:none;height:3px;background:var(--border);border-radius:2px;outline:none}
input[type=range]::-webkit-slider-thumb{-webkit-appearance:none;width:16px;height:16px;border-radius:50%;background:var(--accent);cursor:pointer}
.tog-row{display:flex;align-items:center;justify-content:space-between;padding:9px 0;border-bottom:1px solid var(--border)}
.tog-row:last-child{border:none}
.tog-info{display:flex;align-items:center;gap:10px}
.tog-desc{font-size:12px;color:var(--muted)}
.tog{position:relative;width:42px;height:24px;flex-shrink:0}
.tog input{opacity:0;width:0;height:0}
.tog-sl{position:absolute;inset:0;background:var(--border);border-radius:24px;cursor:pointer;transition:background .2s}
.tog-sl:before{content:'';position:absolute;left:3px;top:3px;width:18px;height:18px;border-radius:50%;background:var(--muted);transition:transform .2s,background .2s}
.tog input:checked+.tog-sl{background:rgba(0,200,100,.2);border:1px solid var(--green)}
.tog input:checked+.tog-sl:before{transform:translateX(18px);background:var(--green)}
.design-grid{display:grid;grid-template-columns:1fr 1fr;gap:10px;margin-bottom:4px}
.design-card{border:1px solid var(--border);border-radius:8px;padding:10px;cursor:pointer;background:var(--surface);transition:border-color .15s}
.design-card.sel{border-color:var(--accent)}
.design-card .prev{border-radius:4px;padding:6px 5px;margin-bottom:6px;font-family:var(--mono);font-size:8px}
.design-card .prev .dr{display:flex;gap:3px;padding:1px 0}
.design-card .dname{font-size:11px;text-align:center;color:var(--muted);font-family:var(--mono)}
.design-card.sel .dname{color:var(--accent)}
.btn-row{display:flex;gap:10px;margin-top:4px}
.btn{flex:1;padding:11px 0;border:1px solid var(--border);border-radius:6px;background:var(--surface);color:var(--text);font-family:var(--mono);font-size:12px;cursor:pointer;transition:border-color .15s,color .15s}
.btn:active{transform:scale(.97)}
.btn.g{border-color:var(--green);color:var(--green)}
.btn.r{border-color:var(--red);color:var(--red)}
.btn.y{border-color:var(--accent);color:var(--accent)}
.btn.b{border-color:var(--blue);color:var(--blue)}
.btn.o{border-color:var(--orange);color:var(--orange)}
textarea#editor{width:100%;height:52vh;background:#000;color:#b0ffb0;font-family:var(--mono);font-size:11px;line-height:1.6;border:1px solid var(--border);border-radius:6px;padding:12px;resize:none;outline:none;margin-bottom:10px}
.stop-list{display:flex;flex-direction:column;gap:10px;margin-bottom:14px}
.stop-card{background:var(--surface);border:1px solid var(--border);border-radius:8px;overflow:hidden}
.stop-header{display:flex;align-items:center;gap:10px;padding:12px 14px;cursor:pointer}
.stop-color-dot{width:10px;height:10px;border-radius:50%;flex-shrink:0}
.stop-title{flex:1;font-size:13px}
.stop-line-badge{font-family:var(--mono);font-size:11px;color:var(--muted)}
.stop-del{background:none;border:none;color:var(--red);font-size:16px;cursor:pointer;padding:0 4px;opacity:.6}
.stop-del:hover{opacity:1}
.stop-body{padding:0 14px 14px;display:none;border-top:1px solid var(--border)}
.stop-body.open{display:block}
.field{margin-top:12px}
.field label{display:block;font-family:var(--mono);font-size:10px;color:var(--muted);text-transform:uppercase;letter-spacing:.08em;margin-bottom:6px}
.field input,.field select{width:100%;background:#0a0a0f;border:1px solid var(--border);border-radius:6px;padding:9px 12px;color:var(--text);font-size:13px;outline:none;font-family:var(--sans)}
.field input:focus,.field select:focus{border-color:var(--accent)}
.search-results{background:#0a0a0f;border:1px solid var(--border);border-radius:6px;margin-top:4px;max-height:180px;overflow-y:auto;display:none}
.search-result{padding:9px 12px;cursor:pointer;font-size:13px;border-bottom:1px solid var(--border)}
.search-result:last-child{border:none}
.search-result:hover{background:var(--surface)}
.search-result .sr-id{font-family:var(--mono);font-size:10px;color:var(--muted);margin-top:2px}
.line-chips{display:flex;flex-wrap:wrap;gap:6px;margin-top:4px;min-height:32px;padding:4px 0}
.line-chip{font-family:var(--mono);font-size:11px;padding:3px 10px;border-radius:4px;border:1px solid var(--border);cursor:pointer;color:var(--muted);background:var(--surface)}
.line-chip.sel{border-color:var(--accent);color:var(--accent);background:rgba(240,192,0,.08)}
.lines-loading{font-size:11px;color:var(--muted);font-family:var(--mono)}
.dir-chips{display:flex;flex-wrap:wrap;gap:6px;margin-top:4px;min-height:28px}
.dir-chip{font-family:var(--mono);font-size:10)BVGHTML"
R"BVGHTML(px;padding:3px 8px;border-radius:4px;border:1px solid var(--border);cursor:pointer;color:var(--muted);background:var(--surface)}
.dir-chip.sel{border-color:var(--green);color:var(--green);background:rgba(0,200,100,.08)}
.color-row{display:flex;gap:8px;flex-wrap:wrap;margin-top:6px}
.color-dot{width:26px;height:26px;border-radius:50%;cursor:pointer;border:2px solid transparent;transition:border-color .15s}
.color-dot.sel{border-color:#fff}
.add-stop-btn{width:100%;padding:13px;border:1px dashed var(--border);border-radius:8px;background:none;color:var(--muted);font-family:var(--mono);font-size:13px;cursor:pointer;transition:border-color .15s,color .15s}
.add-stop-btn:hover{border-color:var(--accent);color:var(--accent)}
.hint{font-size:11px;color:var(--muted);margin-top:6px;line-height:1.5}
/* Alternativ Section */
.alt-section{margin-top:14px;border:1px solid var(--border);border-radius:8px;overflow:hidden}
.alt-header{display:flex;align-items:center;justify-content:space-between;padding:10px 12px;background:#0d0d18;cursor:pointer}
.alt-header-title{font-family:var(--mono);font-size:11px;color:var(--muted);text-transform:uppercase;letter-spacing:.08em}
.alt-header-right{display:flex;align-items:center;gap:8px}
.alt-body{padding:0 12px 12px;display:none}
.alt-body.open{display:block}
.alt-enabled-badge{font-family:var(--mono);font-size:10px;padding:2px 8px;border-radius:10px;border:1px solid var(--green);color:var(--green)}
.toast{position:fixed;bottom:24px;left:50%;transform:translateX(-50%) translateY(80px);background:var(--surface);border:1px solid var(--green);color:var(--green);font-family:var(--mono);font-size:12px;padding:10px 22px;border-radius:20px;transition:transform .3s;z-index:100}
.toast.show{transform:translateX(-50%) translateY(0)}
.toast.err{border-color:var(--red);color:var(--red)}
</style>
</head>
<body>
<header>
  <div class="logo">
    <div class="logo-grid"><span></span><span></span><span></span><span></span><span></span><span></span><span></span><span></span><span></span></div>
    <h1>BVG <em>Matrix</em></h1>
  </div>
  <div class="badge" id="badge"><div class="dot"></div><span id="badge-txt">…</span></div>
</header>
<div class="tabs">
  <button class="tab active" onclick="showTab('display',this)">Anzeige</button>
  <button class="tab" onclick="showTab('stops',this)">Haltestellen</button>
  <button class="tab" onclick="showTab('design',this)">Design</button>
  <button class="tab" onclick="showTab('system',this)">System</button>
  <button class="tab" onclick="showTab('fw',this)">Firmware</button>
</div>

<!-- ANZEIGE -->
<div class="page active" id="tab-display">
  <div class="led-screen" id="live-preview">
    <div class="led-row"><span class="led-lbl" style="color:#0a64ff">U7</span><span class="led-ziel">Spandau</span><span class="led-t1">2'</span><span class="led-t2">8'</span></div>
    <div class="led-row"><span class="led-lbl" style="color:#00c864">S1</span><span class="led-ziel">Oranienbur.</span><span class="led-t1 now">now</span><span class="led-t2">5'</span></div>
    <div class="led-row"><span class="led-lbl" style="color:#ff5000">140</span><span class="led-ziel">Tempelhof</span><span class="led-t1">3'</span><span class="led-t2">11'</span></div>
    <div class="led-row"><span class="led-lbl" style="color:#c800c8">M19</span><span class="led-ziel">Grunewald</span><span class="led-t1">6'</span><span class="led-t2">14'</span></div>
  </div>
  <div class="card">
    <div class="card-title">Helligkeit</div>
    <div class="sl-row"><span class="sl-label">Helligkeit</span><input type="range" min="10" max="100" id="brightness" oninput="sv('bri-v',this.value+'%')"><span class="sl-val" id="bri-v">80%</span></div>
  </div>
  <div class="card">
    <div class="card-title">Auto-Helligkeit</div>
    <div class="tog-row">
      <div class="tog-info"><span class="tog-desc">Automatisch Tag/Nacht</span></div>
      <label class="tog"><input type="checkbox" id="auto-bri" onchange="toggleAutoBri(this.checked)"><span class="tog-sl"></span></label>
    </div>
    <div id="auto-bri-box" style="display:none;margin-top:10px">
      <div class="sl-row"><span class="sl-label">Tag</span><input type="range" min="10" max="100" id="bri-day" oninput="sv('bri-day-v',this.value+'%')"><span class="sl-val" id="bri-day-v">80%</span></div>
      <div class="sl-row"><span class="sl-label">Nacht</span><input type="range" min="5" max="60" id="bri-night" oninput="sv('bri-night-v',this.value+'%')"><span class="sl-val" id="bri-night-v">30%</span></div>
      <div class="sl-row"><span class="sl-label">Tag ab</span><input type="range" min="4" max="12" id="bri-day-start" oninput="sv('bri-day-start-v',this.value+':00')"><span class="sl-val" id="bri-day-start-v">7:00</span></div>
      <div class="sl-row"><span class="sl-label">Nacht ab</span><input type="range" min="18" max="24" id="bri-night-start" oninput="sv('bri-night-start-v',this.value+':00')"><span class="sl-val" id="bri-night-start-v">22:00</span></div>
    </div>
  </div>
  <div class="card">
    <div class="card-title">Abfahrtszeiten & Intervall</div>
    <div class="tog-row">
      <div class="tog-info"><span class="tog-desc">Zweite Abfahrtszeit</span></div>
      <label class="tog"><input type="checkbox" id="show-zeit2" checked><span class="tog-sl"></span></label>
    </div>
    <div class="sl-row" style="margin-top:12px"><span class="sl-label">Anzeige</span><input type="range" min="10" max="120" step="5" id="interval" oninput="sv('interval-v',this.value+'s')"><span class="sl-val" id="interval-v">20s</span></div>
    <div class="sl-row"><span class="sl-label">Abfrage</span><input type="range" min="30" max="300" step="10" id="fetch-interval" oninput="sv('fetch-interval-v',this.value+'s')"><span class="sl-val" id="fetch-interval-v">60s</span></div>
    <p style="font-size:11px;color:var(--muted);margin-top:6px">Anzeige = neu zeichnen und Alternativ-Linie wechseln. Abfrage = Daten per WLAN holen; seltener heißt weniger Flackern.</p>
  </div>
  <div class="card">
    <div class="card-title">Schriftgröße</div>
    <div style="display:flex;gap:8px">
      <button class="btn" id="font-4x6" onclick="setFont('4x6')">4×6 Klein</button>
      <button class="btn b" id="font-5x7" onclick="setFont('5x7')">5×7 Normal</button>
      <button class="btn" id="font-6x10" onclick="setFont('6x10')">6×10 Groß</button>
    </div>
    <div class="tog-row" style="margin-top:10px">
      <div class="tog-info"><span class="tog-desc">Fette Schrift (Striche 2 Pixel dick, wie auf dem U-Bahnhof)</span></div>
      <label class="tog"><input type="checkbox" id="font-bold"><span class="tog-sl"></span></label>
    </div>
    <p class="hint">Das Design „BVG Classic“ nutzt die fette Schrift immer. Fett gibt es in Normal und Groß (Klein wird zu Normal); Groß-Fett ist eine echte Helvetica Bold.</p>
    <p class="hint">Zeilen pro Seite: Klein 5, Normal 4, Groß 3. Hast du mehr Haltestellen, bleibt die gewählte Schrift und die Anzeige blättert im eingestellten Anzeige-Intervall automatisch zur nächsten Seite.</p>
  </div>
  <div class="card">
    <div class="card-title">Zieltext kürzen</div>
    <div class="tog-row">
      <div class="tog-info"><span class="tog-desc">Zusätze in Klammern ausblenden, z. B. „Rathaus Spandau (Berlin)“</span></div>
      <label class="tog"><input type="checkbox" id="hide-brackets"><span class="tog-sl"></span></label>
    </div>
    <div class="tog-row">
      <div class="tog-info"><span class="tog-desc">„Berlin“ ausblenden, z. B. „S+U Berlin Hbf“ → „S+U Hbf“</span></div>
      <label class="tog"><input type="checkbox" id="hide-berlin"><span class="tog-sl"></span></label>
    </div>
    <p class="hint">Gilt für alle Ziele, auch wenn du sie unten in der Haltestelle umbenennst (eine Umbenennung hat Vorrang).</p>
  </div>
  <div class="card">
    <div class="card-title">Feinjustierung (Pixel)</div>
    <div class="sl-row"><span class="sl-label">Label ←→</span><input type="range" min="0" max="6" id="off-label" oninput="sv)BVGHTML"
R"BVGHTML(('off-label-v',this.value)"><span class="sl-val" id="off-label-v">0</span></div>
    <div class="sl-row"><span class="sl-label">Ziel ←→</span><input type="range" min="-6" max="6" id="off-x" oninput="sv('off-x-v',this.value)"><span class="sl-val" id="off-x-v">-2</span></div>
    <div class="sl-row"><span class="sl-label">Zeiten ←→</span><input type="range" min="-6" max="6" id="off-time" oninput="sv('off-time-v',this.value)"><span class="sl-val" id="off-time-v">-1</span></div>
    <div class="sl-row"><span class="sl-label">Alles ↑↓</span><input type="range" min="-3" max="3" id="off-y" oninput="sv('off-y-v',this.value)"><span class="sl-val" id="off-y-v">1</span></div>
    <p style="font-size:11px;color:var(--muted);margin-top:6px">Minus = nach links bzw. oben. Mit „Speichern“ übernehmen; das Display aktualisiert sich sofort.</p>
  </div>
  <div class="btn-row">
    <button class="btn y" onclick="saveConfig()">Speichern</button>
    <button class="btn b" onclick="saveAndRestart()">Speichern & Apply</button>
  </div>
</div>

<!-- HALTESTELLEN -->
<div class="page" id="tab-stops">
  <div class="stop-list" id="stop-list"></div>
  <button class="add-stop-btn" onclick="addStop()">+ Haltestelle hinzufügen</button>
  <div class="btn-row" style="margin-top:14px">
    <button class="btn y" onclick="saveConfig()">Speichern</button>
    <button class="btn b" onclick="saveAndRestart()">Speichern & Apply</button>
  </div>
</div>

<!-- DESIGN -->
<div class="page" id="tab-design">
  <div class="card-title" style="padding:4px 0 12px">Display-Design wählen</div>
  <div class="design-grid" id="design-grid"></div>
  <div class="btn-row" style="margin-top:14px">
    <button class="btn y" onclick="saveConfig()">Speichern</button>
    <button class="btn b" onclick="saveAndRestart()">Speichern & Apply</button>
  </div>
</div>

<!-- SYSTEM -->
<div class="page" id="tab-system">
  <div class="card">
    <div class="card-title">BVG Service</div>
    <div class="btn-row">
      <button class="btn g" onclick="apiPost('/api/restart','⟳ Neugestartet')">⟳ Neustarten</button>
      <button class="btn r" onclick="apiPost('/api/stop','◼ Gestoppt')">◼ Stoppen</button>
    </div>
  </div>
  <div class="card">
    <div class="card-title">WLAN</div>
    <p id="wifi-info" style="font-size:13px;margin-bottom:12px;color:var(--muted)">–</p>
    <div class="btn-row">
      <button class="btn b" onclick="confirm('Das Board öffnet das WLAN BVG-Matrix-Setup. Weiter?')&&apiPost('/api/wifi-setup','WLAN-Setup startet…')">WLAN neu einrichten</button>
    </div>
    <p style="font-size:11px;color:var(--muted);margin-top:8px">Danach am Handy mit „BVG-Matrix-Setup“ verbinden. Alternativ beim Reset die Taste DOWN gedrückt halten.</p>
  </div>
  <div class="card">
    <div class="card-title">Matrix Portal S3</div>
    <div class="btn-row">
      <button class="btn o" onclick="confirm('Matrix Portal neu starten?')&&apiPost('/api/reboot','Board startet neu…')">↺ Reboot</button>
    </div>
  </div>
  <div class="btn-row">
    <button class="btn y" onclick="saveConfig()">Speichern</button>
    <button class="btn b" onclick="saveAndRestart()">Speichern & Apply</button>
  </div>
</div>

<!-- FIRMWARE -->
<div class="page" id="tab-fw">
  <div class="card">
    <div class="card-title">Firmware aktualisieren</div>
    <p style="font-size:12px;color:var(--muted);margin-bottom:10px">Kompilierte <code>.bin</code>-Datei wählen (Arduino IDE: Sketch → Kompilierte Binärdatei exportieren, nimm die Datei ohne „bootloader“/„partitions“; PlatformIO: <code>.pio/build/…/firmware.bin</code>). Das Board startet danach neu. Einstellungen und WLAN bleiben erhalten.</p>
    <input type="file" id="fw-file" accept=".bin" style="width:100%;margin-bottom:10px">
    <div style="height:6px;background:var(--border,#333);border-radius:3px;overflow:hidden;margin-bottom:10px"><div id="fw-bar" style="height:100%;width:0;background:#f0c000"></div></div>
    <div class="btn-row"><button class="btn y" onclick="uploadFw()">Hochladen &amp; installieren</button></div>
  </div>
</div>

<div class="toast" id="toast"></div>

<script>
const COLORS = {
  blue:'#0a64ff',green:'#00c864',orange:'#ff5000',
  purple:'#c800c8',yellow:'#f0c000',red:'#e03c3c',
  white:'#c8c8c8',cyan:'#00c8c8',pink:'#ff50b4'
};

const DESIGNS = {
  modern:{name:'Dark Modern',rows:[
    {l:'U7',lc:'#0a64ff',z:'Spandau',t1:"2'",t2:"8'",tc:'#f0c000'},
    {l:'S1',lc:'#00c864',z:'Oranienbur.',t1:'now',t2:"5'",tc:'#00ff50'},
    {l:'140',lc:'#ff5000',z:'Tempelhof',t1:"3'",t2:"11'",tc:'#f0c000'},
    {l:'M19',lc:'#c800c8',z:'Grunewald',t1:"6'",t2:"14'",tc:'#f0c000'},
  ]},
  bvg:{name:'BVG Classic (fett)',zc:'#ff8200',bold:true,rows:[
    {l:'U7',lc:'#ff8200',z:'Spandau',t1:"2'",t2:"8'",tc:'#ff8200'},
    {l:'S1',lc:'#ff8200',z:'Oranienbur.',t1:'now',t2:"5'",tc:'#ff8200'},
    {l:'140',lc:'#ff8200',z:'Tempelhof',t1:"3'",t2:"11'",tc:'#ff8200'},
    {l:'M19',lc:'#ff8200',z:'Grunewald',t1:"6'",t2:"14'",tc:'#ff8200'},
  ]},
  minimal:{name:'Minimal White',rows:[
    {l:'U7',lc:'#fff',z:'Spandau',t1:"2'",t2:"8'",tc:'#fff'},
    {l:'S1',lc:'#fff',z:'Oranienbur.',t1:'now',t2:"5'",tc:'#fff'},
    {l:'140',lc:'#fff',z:'Tempelhof',t1:"3'",t2:"11'",tc:'#fff'},
    {l:'M19',lc:'#fff',z:'Grunewald',t1:"6'",t2:"14'",tc:'#fff'},
  ]},
  night:{name:'Night Mode',rows:[
    {l:'U7',lc:'#006090',z:'Spandau',t1:"2'",t2:"8'",tc:'#006480'},
    {l:'S1',lc:'#006090',z:'Oranienbur.',t1:'now',t2:"5'",tc:'#007860'},
    {l:'140',lc:'#006090',z:'Tempelhof',t1:"3'",t2:"11'",tc:'#006480'},
    {l:'M19',lc:'#006090',z:'Grunewald',t1:"6'",t2:"14'",tc:'#006480'},
  ]},
  amber:{name:'Retro Amber',rows:[
    {l:'U7',lc:'#ff8c00',z:'Spandau',t1:"2'",t2:"8'",tc:'#ffa000'},
    {l:'S1',lc:'#ff8c00',z:'Oranienbur.',t1:'now',t2:"5'",tc:'#ffc000'},
    {l:'140',lc:'#ff8c00',z:'Tempelhof',t1:"3'",t2:"11'",tc:'#ffa000'},
    {l:'M19',lc:'#ff8c00',z:'Grunewald',t1:"6'",t2:"14'",tc:'#ffa000'},
  ]},
};

let cfg = {};
let selectedDesign = 'modern';
let selectedFont   = '5x7';
let stops = [];

// ── Stop Card ─────────────────────────────────
function buildStopList() {
  const list = document.getElementById('stop-list');
  list.innerHTML = '';
  stops.forEach((s,i) => list.appendChild(buildStopCard(s,i)));
}

function buildStopCard(s, idx) {
  const card = document.createElement('div');
  card.className = 'stop-card';
  card.id = `stop-card-${idx}`;
  const color = COLORS[s.color] || '#808080';
  const altBadge = s.alternativ ? `<span class="alt-enabled-badge">↔ Alt</span>` : '';

  card.innerHTML = `
    <div class="stop-header" onclick="toggleStop(${idx})">
      <div class="stop-color-dot" style="background:${color}"></div>
      <div class="stop-title">${s.stop_name||'Neue Haltestelle'}</div>
      <span class="stop-line-badge">${s.line||''}</span>
      ${altBadge}
      <button class="stop-del" onclick="event.stopPropagation();deleteStop(${idx})">✕</button>
    </div>
    <div class="stop-body" id="stop-body-${idx}">

      <div class="field">
        <label>Haltestelle suchen</label>
        <input type="text" id="search-${idx}" placeholder="z.B. Hermannplatz" value="${s.stop_name||''}"
               oninput="searchStop(${idx},this.value)">
        <div class="search-results" id="sr-${idx}"></div>
      </div>
      <div class="field">
        <label>Linien <span style="color:var(--muted)">(mehrere möglich)</span></label>
        <div class="line-chips" id="lchips-${idx}"><span class="lines-loading">Haltestelle wählen…</span></div>
      </div>
      <div class="field">
        <label>Richtungen <span style="color:var(--muted)">(mehrere möglich, optional)</span></label>
        <div class="dir-chips" id="dchips-${idx}"></div>
        <input type="text" id="dir-${idx}" placeholder="leer = alle · mehrere mit | trennen"
               value="${s.direction||''}" oninput="stops[${idx}].direction=this.value">
      </div>
      <p class="hint">✎ an einer Richtung ändert den Namen, der auf dem Display steht (z. B. „Rathaus Spa)BVGHTML"
R"BVGHTML(ndau“ → „Spandau“).</p>
      <div class="field">
        <label>Farbe</label>
        <div class="color-row" id="color-row-${idx}"></div>
      </div>

      <!-- ALTERNATIV SECTION -->
      <div class="alt-section" id="alt-section-${idx}">
        <div class="alt-header" onclick="toggleAlt(${idx})">
          <span class="alt-header-title">↔ Alternativ-Linie</span>
          <div class="alt-header-right">
            <span id="alt-badge-${idx}" style="display:none" class="alt-enabled-badge">Aktiv</span>
            <label class="tog" onclick="event.stopPropagation()">
              <input type="checkbox" id="alt-enabled-${idx}"
                     onchange="toggleAltEnabled(${idx},this.checked)">
              <span class="tog-sl"></span>
            </label>
          </div>
        </div>
        <div class="alt-body" id="alt-body-${idx}">
          <div class="field">
            <label>Haltestelle (leer = gleiche)</label>
            <input type="text" id="alt-search-${idx}" placeholder="Haltestelle suchen oder leer lassen"
                   value="${s.alternativ?.stop_name||''}"
                   oninput="searchAltStop(${idx},this.value)">
            <div class="search-results" id="alt-sr-${idx}"></div>
          </div>
          <div class="field">
            <label>Alternativ-Linien <span style="color:var(--muted)">(mehrere möglich)</span></label>
            <div class="line-chips" id="alt-lchips-${idx}"><span class="lines-loading">Haltestelle wählen…</span></div>
          </div>
          <div class="field">
            <label>Richtungen <span style="color:var(--muted)">(mehrere möglich, optional)</span></label>
            <div class="dir-chips" id="alt-dchips-${idx}"></div>
            <input type="text" id="alt-dir-${idx}" placeholder="leer = alle · mehrere mit | trennen"
                   value="${s.alternativ?.direction||''}"
                   oninput="if(stops[${idx}].alternativ)stops[${idx}].alternativ.direction=this.value">
          </div>
          <div class="field">
            <label>Farbe der Alternativ-Linie</label>
            <div class="color-row" id="alt-color-row-${idx}"></div>
          </div>
          <p style="font-size:11px;color:var(--muted);margin-top:10px">
            Wechselt alle ${cfg.update_interval||20}s zwischen Haupt- und Alternativ-Linie
          </p>
        </div>
      </div>

    </div>`;

  setTimeout(() => {
    // Haupt-Farbe
    buildColorDots(`color-row-${idx}`, s.color, (name) => {
      stops[idx].color = name; refreshCard(idx);
    });
    // Alternativ-Farbe
    buildColorDots(`alt-color-row-${idx}`, s.alternativ?.color||'blue', (name) => {
      if (!stops[idx].alternativ) stops[idx].alternativ = {};
      stops[idx].alternativ.color = name;
    });
    // Alt enabled toggle
    const altCb = document.getElementById(`alt-enabled-${idx}`);
    if (altCb) altCb.checked = !!s.alternativ;
    if (s.alternativ) {
      document.getElementById(`alt-badge-${idx}`).style.display = 'inline';
      // Load alt lines
      const altStopId = s.alternativ.stop_id || s.stop_id;
      if (altStopId) loadAltLines(idx, altStopId, s.alternativ.line||'', s.alternativ.direction||'');
    }
    // Load main lines
    if (s.stop_id) loadLines(idx, s.stop_id, s.line, s.direction);
  }, 0);

  return card;
}

// ── Ziele umbenennen (je Haltestelle) ─────────
// stops[idx]._al = [[teilstring, anzeige], ...]; beim Speichern wird daraus ziel_alias {teilstring: anzeige}.
// Bearbeitet wird direkt an der Richtung (✎ am Chip). Die Firmware vergleicht ohne Gross/Klein als Teilstring.
function findAlias(idx, dir) {
  const d = dir.toLowerCase();
  return (stops[idx]._al || []).find(a => a[0].trim() && d.includes(a[0].trim().toLowerCase()));
}
function renameDir(idx, dir) {
  const list = stops[idx]._al || (stops[idx]._al = []);
  const cur = findAlias(idx, dir);
  const v = prompt('Anzeigename für „' + dir + '“\n(leer lassen = Originalname)', cur ? cur[1] : '');
  if (v === null) return;
  const name = v.trim();
  if (cur) {
    if (name) cur[1] = name; else list.splice(list.indexOf(cur), 1);
  } else if (name) {
    list.push([dir, name]);
  }
}

function buildColorDots(containerId, selected, onChange) {
  const cr = document.getElementById(containerId);
  if (!cr) return;
  cr.innerHTML = '';
  Object.entries(COLORS).forEach(([name,hex]) => {
    const dot = document.createElement('div');
    dot.className = 'color-dot' + (selected===name?' sel':'');
    dot.style.background = hex;
    dot.title = name;
    dot.onclick = () => {
      cr.querySelectorAll('.color-dot').forEach(d=>d.classList.remove('sel'));
      dot.classList.add('sel');
      onChange(name);
    };
    cr.appendChild(dot);
  });
}

function toggleAlt(idx) {
  document.getElementById(`alt-body-${idx}`).classList.toggle('open');
}

function toggleAltEnabled(idx, enabled) {
  const badge = document.getElementById(`alt-badge-${idx}`);
  if (enabled) {
    if (!stops[idx].alternativ) {
      stops[idx].alternativ = {stop_id: stops[idx].stop_id, stop_name: '', line:'', direction:'', color:'blue'};
    }
    badge.style.display = 'inline';
    document.getElementById(`alt-body-${idx}`).classList.add('open');
    // Load alt lines from same stop by default
    if (stops[idx].stop_id) loadAltLines(idx, stops[idx].stop_id, '', '');
  } else {
    stops[idx].alternativ = null;
    badge.style.display = 'none';
    document.getElementById(`alt-body-${idx}`).classList.remove('open');
  }
  refreshCard(idx);
}

function toggleStop(idx) {
  document.getElementById(`stop-body-${idx}`).classList.toggle('open');
}

function refreshCard(idx) {
  const s = stops[idx];
  const card = document.getElementById(`stop-card-${idx}`);
  if (!card) return;
  card.querySelector('.stop-color-dot').style.background = COLORS[s.color]||'#808080';
  card.querySelector('.stop-title').textContent = s.stop_name||'Neue Haltestelle';
  card.querySelector('.stop-line-badge').textContent = s.line||'';
}

function deleteStop(idx) { stops.splice(idx,1); buildStopList(); }

function addStop() {
  stops.push({stop_id:null,stop_name:'',line:'',direction:'',label:'',color:'blue',alternativ:null,_al:[]});
  buildStopList();
  setTimeout(()=>toggleStop(stops.length-1),50);
}

// ── Haltestellensuche ─────────────────────────
let searchTimers = {};

function searchStop(idx, q) {
  clearTimeout(searchTimers[`m${idx}`]);
  const sr = document.getElementById(`sr-${idx}`);
  if (q.length < 2) { sr.style.display='none'; return; }
  searchTimers[`m${idx}`] = setTimeout(async () => {
    const data = await fetchStops(q);
    buildSearchResults(`sr-${idx}`, data, (s) => selectStop(idx, s));
  }, 350);
}

function searchAltStop(idx, q) {
  clearTimeout(searchTimers[`a${idx}`]);
  const sr = document.getElementById(`alt-sr-${idx}`);
  if (q.length < 2) { sr.style.display='none'; return; }
  searchTimers[`a${idx}`] = setTimeout(async () => {
    const data = await fetchStops(q);
    buildSearchResults(`alt-sr-${idx}`, data, (s) => selectAltStop(idx, s));
  }, 350);
}

async function fetchStops(q) {
  const r = await fetch(`/api/search-stops?q=${encodeURIComponent(q)}`);
  return await r.json();
}

function buildSearchResults(containerId, data, onSelect) {
  const sr = document.getElementById(containerId);
  sr.innerHTML = '';
  if (!data.length) { sr.style.display='none'; return; }
  data.forEach(s => {
    const el = document.createElement('div');
    el.className = 'search-result';
    el.innerHTML = `${s.name}<div class="sr-id">${s.id}</div>`;
    el.onclick = () => onSelect(s);
    sr.appendChild(el);
  });
  sr.style.display = 'block';
}

function selectStop(idx, s) {
  stops[idx].stop_id   = s.id;
  stops[idx].stop_name = s.name;
  stops[idx].line      = '';
  stops[idx].direction = '';
  document.getElementById(`search-${idx}`).value = s.name;
  document.getElementById(`sr-${idx}`).style.display = 'none';
  document.getElementById(`dir-${idx}`).value = '';
  stops[idx].label = '';
  refreshCar)BVGHTML"
R"BVGHTML(d(idx);
  loadLines(idx, s.id, '', '');
  // Update alt stop if using same stop
  if (stops[idx].alternativ && !stops[idx].alternativ.stop_id) {
    stops[idx].alternativ.stop_id = s.id;
    loadAltLines(idx, s.id, '', '');
  }
}

function selectAltStop(idx, s) {
  if (!stops[idx].alternativ) stops[idx].alternativ = {};
  stops[idx].alternativ.stop_id   = s.id;
  stops[idx].alternativ.stop_name = s.name;
  stops[idx].alternativ.line      = '';
  stops[idx].alternativ.direction = '';
  document.getElementById(`alt-search-${idx}`).value = s.name;
  document.getElementById(`alt-sr-${idx}`).style.display = 'none';
  document.getElementById(`alt-dir-${idx}`).value = '';
  loadAltLines(idx, s.id, '', '');
}

// Linien & Richtungen: Mehrfachauswahl. Linien kommagetrennt ("S2,S25,S26"), Richtungen mit | getrennt.
const splitSel = (v, sep) => (v||'').split(sep).map(x=>x.trim()).filter(Boolean);

async function loadLinesGeneric(idx, stop_id, which) {
  const pre = which==='alt' ? 'alt-' : '';
  const lc = document.getElementById(`${pre}lchips-${idx}`);
  const dc = document.getElementById(`${pre}dchips-${idx}`);
  if (!lc) return;
  lc.innerHTML = '<span class="lines-loading">Lade…</span>';
  dc.innerHTML = '';
  const obj = () => which==='alt' ? (stops[idx].alternativ || (stops[idx].alternativ = {})) : stops[idx];
  let data = [];
  try {
    const r = await fetch(`/api/stop-lines?id=${encodeURIComponent(stop_id)}`);
    data = await r.json();
    if (!Array.isArray(data)) throw new Error('api');
  } catch(e) { lc.innerHTML = '<span class="lines-loading">Fehler</span>'; return; }
  if (!data.length) { lc.innerHTML = '<span class="lines-loading">Keine Linien</span>'; return; }

  const setDirInput = () => {
    const inp = document.getElementById(`${pre}dir-${idx}`);
    if (inp) inp.value = obj().direction || '';
  };
  const render = () => {
    const selL = new Set(splitSel(obj().line, ','));
    const selD = new Set(splitSel(obj().direction, '|'));
    lc.innerHTML = '';
    data.forEach(item => {
      const chip = document.createElement('div');
      chip.className = 'line-chip' + (selL.has(item.line) ? ' sel' : '');
      chip.textContent = item.line;
      chip.onclick = () => {
        selL.has(item.line) ? selL.delete(item.line) : selL.add(item.line);
        const lines = data.map(d=>d.line).filter(l=>selL.has(l));
        obj().line = lines.join(',');
        if (which==='main') {
          stops[idx].label = (lines[0] || '').slice(0,4);
          refreshCard(idx);
        }
        render();
      };
      lc.appendChild(chip);
    });
    // Richtungen aller gewaehlten Linien (ohne Doppelte)
    const dirs = [];
    data.filter(i => selL.has(i.line)).forEach(i => (i.directions||[]).forEach(d => { if (!dirs.includes(d)) dirs.push(d); }));
    dc.innerHTML = '';
    if (selL.size && !dirs.length) dc.innerHTML = '<span class="lines-loading">Keine Richtungen bekannt</span>';
    dirs.forEach(dir => {
      const chip = document.createElement('div');
      chip.className = 'dir-chip' + (selD.has(dir) ? ' sel' : '');
      const al = findAlias(idx, dir);
      const shown = al ? al[1] : (dir.length>18 ? dir.slice(0,17)+'.' : dir);
      chip.textContent = shown;
      chip.title = dir;
      chip.onclick = () => {
        selD.has(dir) ? selD.delete(dir) : selD.add(dir);
        obj().direction = [...selD].join('|');
        setDirInput();
        render();
      };
      const pen = document.createElement('span');   // ✎ = Anzeigename für diese Richtung ändern
      pen.textContent = ' ✎';
      pen.style.opacity = al ? '1' : '.5';
      pen.title = 'Anzeigename ändern';
      pen.onclick = (e) => { e.stopPropagation(); renameDir(idx, dir); render(); };
      chip.appendChild(pen);
      dc.appendChild(chip);
    });
    if (dirs.length) {   // Auswahl zuruecksetzen = alle Richtungen
      const all = document.createElement('div');
      all.className = 'dir-chip' + (selD.size ? '' : ' sel');
      all.textContent = 'alle';
      all.onclick = () => { obj().direction = ''; setDirInput(); render(); };
      dc.insertBefore(all, dc.firstChild);
    }
  };
  render();
}
const loadLines = (idx, stop_id) => loadLinesGeneric(idx, stop_id, 'main');
const loadAltLines = (idx, stop_id) => loadLinesGeneric(idx, stop_id, 'alt');

// ── Design Grid ───────────────────────────────
function buildDesignGrid() {
  const g = document.getElementById('design-grid');
  g.innerHTML = '';
  Object.entries(DESIGNS).forEach(([key,d]) => {
    const card = document.createElement('div');
    card.className = 'design-card'+(key===selectedDesign?' sel':'');
    card.onclick = () => { selectedDesign=key; buildDesignGrid(); };
    const fw = d.bold ? 800 : 400;
    let rows = d.rows.map(r=>`<div class="dr" style="font-weight:${fw}"><span style="color:${r.lc};font-weight:${d.bold?800:700};min-width:22px;font-size:8px">${r.l}</span><span style="flex:1;color:${d.zc||'#909090'};font-size:8px">${r.z}</span><span style="color:${r.tc};font-size:8px">${r.t1}</span></div>`).join('');
    card.innerHTML = `<div class="prev" style="background:#000">${rows}</div><div class="dname">${d.name}</div>`;
    g.appendChild(card);
  });
}

// ── Helpers ───────────────────────────────────
function sv(id,v){document.getElementById(id).textContent=v}
function toggleAutoBri(on){document.getElementById('auto-bri-box').style.display=on?'block':'none'}
function setFont(f){
  selectedFont=f;
  ['4x6','5x7','6x10'].forEach(x=>{
    const el=document.getElementById('font-'+x);
    el.style.borderColor=x===f?'var(--accent)':'';
    el.style.color=x===f?'var(--accent)':'';
  });
}

function getConfig() {
  return {
    ziel_alias:                  {},   // global nicht mehr genutzt - Umbenennungen liegen je Haltestelle
    offset_label_x:              parseInt(document.getElementById('off-label').value),
    offset_x:                    parseInt(document.getElementById('off-x').value),
    offset_time_x:               parseInt(document.getElementById('off-time').value),
    offset_y:                    parseInt(document.getElementById('off-y').value),
    brightness:                  parseInt(document.getElementById('brightness').value),
    update_interval:             parseInt(document.getElementById('interval').value),
    fetch_interval:              parseInt(document.getElementById('fetch-interval').value),
    design:                      selectedDesign,
    font_size:                   selectedFont,
    font_bold:                   document.getElementById('font-bold').checked,
    hide_brackets:               document.getElementById('hide-brackets').checked,
    hide_berlin:                 document.getElementById('hide-berlin').checked,
    show_second_time:            document.getElementById('show-zeit2').checked,
    auto_brightness:             document.getElementById('auto-bri').checked,
    auto_brightness_day:         parseInt(document.getElementById('bri-day').value),
    auto_brightness_night:       parseInt(document.getElementById('bri-night').value),
    auto_brightness_day_start:   parseInt(document.getElementById('bri-day-start').value),
    auto_brightness_night_start: parseInt(document.getElementById('bri-night-start').value),
    stops: stops.map(s => {
      const o = {...s, alternativ: s.alternativ || null};
      const za = {};
      (s._al || []).forEach(([f, t]) => { if (f.trim() && t.trim()) za[f.trim()] = t.trim(); });
      o.ziel_alias = za;
      delete o._al;
      return o;
    })
  };
}

async function loadConfig() {
  const r = await fetch('/api/config');
  cfg = await r.json();
  document.getElementById('brightness').value=cfg.brightness||80; sv('bri-v',(cfg.brightness||80)+'%');
  document.getElementById('interval').value=cfg.update_interval||20; sv('interval-v',(cfg.update_interval||20)+'s');
  document.getElementById('fetch-interval').value=cfg.fetch_interval||60; sv('fetch-interval-v',(cfg.fetch_interval||60)+'s');
  document.getElementById('show-zeit2').checked=cfg.s)BVGHTML"
R"BVGHTML(how_second_time!==false;
  document.getElementById('font-bold').checked=!!cfg.font_bold;
  document.getElementById('hide-brackets').checked=!!cfg.hide_brackets;
  document.getElementById('hide-berlin').checked=!!cfg.hide_berlin;
  document.getElementById('auto-bri').checked=!!cfg.auto_brightness;
  toggleAutoBri(!!cfg.auto_brightness);
  document.getElementById('bri-day').value=cfg.auto_brightness_day||80; sv('bri-day-v',(cfg.auto_brightness_day||80)+'%');
  document.getElementById('bri-night').value=cfg.auto_brightness_night||30; sv('bri-night-v',(cfg.auto_brightness_night||30)+'%');
  document.getElementById('bri-day-start').value=cfg.auto_brightness_day_start||7; sv('bri-day-start-v',(cfg.auto_brightness_day_start||7)+':00');
  document.getElementById('bri-night-start').value=cfg.auto_brightness_night_start||22; sv('bri-night-start-v',(cfg.auto_brightness_night_start||22)+':00');
  if(cfg.design){selectedDesign=cfg.design; buildDesignGrid();}
  if(cfg.font_size) setFont(cfg.font_size);
  const setOff=(id,v,d)=>{v=(v===undefined||v===null)?d:v; document.getElementById(id).value=v; sv(id+'-v',v);};
  setOff('off-label',cfg.offset_label_x,0); setOff('off-x',cfg.offset_x,-2); setOff('off-time',cfg.offset_time_x,-1); setOff('off-y',cfg.offset_y,1);
  // Migration: fruehere globale Umbenennungen in jede Haltestelle uebernehmen (eigene Eintraege haben Vorrang)
  const globalAl = Object.entries(cfg.ziel_alias||{});
  stops = (cfg.stops||[]).map(s => {
    const al = Object.entries(s.ziel_alias||{});
    globalAl.forEach(([f,t]) => { if (!al.some(a => a[0].toLowerCase()===f.toLowerCase())) al.push([f,t]); });
    return {...s, alternativ: s.alternativ||null, _al: al};
  });
  buildStopList();
}

async function saveConfig() {
  try {
    const r = await fetch('/api/config',{method:'POST',headers:{'Content-Type':'application/json'},body:JSON.stringify(getConfig())});
    const d = await r.json();
    toast(d.ok?'Gespeichert ✓':'Fehler: '+(d.error||'?'),!d.ok);
    return !!d.ok;
  } catch(e) { toast('Board nicht erreichbar',true); return false; }
}
async function saveAndRestart(){if(await saveConfig())await apiPost('/api/restart','⟳ Angewendet');}

async function apiPost(url,msg){
  let d;
  try { const r=await fetch(url,{method:'POST'}); d=await r.json(); }
  catch(e) { toast('Board nicht erreichbar',true); return; }
  toast(d.ok?msg:'Fehler: '+(d.error||'?'),!d.ok);
  if(url.includes('restart')||url.includes('stop'))setTimeout(loadStatus,2000);
}

async function loadStatus(){
  const b=document.getElementById('badge');
  let d;
  try { const r=await fetch('/api/status'); d=await r.json(); }
  catch(e) { d={status:'offline'}; }
  document.getElementById('badge-txt').textContent=d.status;
  const wi=document.getElementById('wifi-info');
  if(wi) wi.textContent = d.ssid ? `${d.ssid} · ${d.ip} · ${d.rssi} dBm` : '–';
  b.className='badge '+(d.status==='active'?'on':'off');
}

function uploadFw(){
  const f=document.getElementById('fw-file').files[0];
  if(!f){toast('Bitte zuerst eine .bin-Datei wählen',true);return;}
  if(!confirm('Firmware '+f.name+' ('+Math.round(f.size/1024)+' kB) installieren?'))return;
  const fd=new FormData();fd.append('firmware',f,f.name);
  const x=new XMLHttpRequest();x.open('POST','/api/update');
  x.upload.onprogress=e=>{if(e.lengthComputable)document.getElementById('fw-bar').style.width=(100*e.loaded/e.total)+'%';};
  x.onload=()=>{let d={};try{d=JSON.parse(x.responseText);}catch(e){}
    toast(d.ok?'Installiert – Board startet neu ✓':'Fehler: '+(d.error||x.status),!d.ok);};
  x.onerror=()=>toast('Verbindung unterbrochen (Board startet evtl. schon neu)',true);
  x.send(fd);
}

function showTab(name,el){
  document.querySelectorAll('.page').forEach(p=>p.classList.remove('active'));
  document.querySelectorAll('.tab').forEach(t=>t.classList.remove('active'));
  document.getElementById('tab-'+name).classList.add('active');
  el.classList.add('active');
}

let toastTimer;
function toast(msg,err=false){
  const t=document.getElementById('toast');
  t.textContent=msg;
  t.className='toast show'+(err?' err':'');
  clearTimeout(toastTimer);
  toastTimer=setTimeout(()=>t.className='toast'+(err?' err':''),2500);
}

buildDesignGrid();
loadConfig();
loadStatus();
setInterval(loadStatus,5000);
</script>
</body>
</html>
)BVGHTML";
static const char SETUP_HTML[] PROGMEM =
R"BVGHTML(<!DOCTYPE html>
<html lang="de">
<head>
<meta charset="UTF-8">
<meta name="viewport" content="width=device-width,initial-scale=1,maximum-scale=1">
<title>BVG Matrix – WLAN einrichten</title>
<style>
:root{--bg:#0a0a0f;--surface:#12121a;--border:#1e1e2e;--accent:#f0c000;--green:#00c864;--red:#e03c3c;--text:#e0e0e0;--muted:#707080;--mono:ui-monospace,Menlo,Consolas,monospace}
*{box-sizing:border-box;margin:0;padding:0}
body{background:var(--bg);color:var(--text);font-family:system-ui,-apple-system,Segoe UI,Roboto,sans-serif;min-height:100vh;padding:20px 16px 40px}
.wrap{max-width:440px;margin:0 auto}
h1{font-family:var(--mono);font-size:17px;font-weight:600;margin-bottom:4px}
h1 em{color:var(--accent);font-style:normal}
.sub{font-size:13px;color:var(--muted);margin-bottom:20px;line-height:1.5}
.card{background:var(--surface);border:1px solid var(--border);border-radius:10px;padding:14px;margin-bottom:14px}
.title{font-family:var(--mono);font-size:10px;color:var(--muted);text-transform:uppercase;letter-spacing:.1em;margin-bottom:10px}
.net{display:flex;align-items:center;gap:10px;padding:13px 10px;border:1px solid var(--border);border-radius:8px;margin-bottom:8px;cursor:pointer;background:#0a0a0f}
.net.sel{border-color:var(--accent);background:rgba(240,192,0,.07)}
.net .n{flex:1;font-size:15px;overflow:hidden;text-overflow:ellipsis;white-space:nowrap}
.net .lock{font-size:12px;color:var(--muted)}
.bars{display:flex;align-items:flex-end;gap:2px;height:14px}
.bars i{display:block;width:3px;background:var(--border);border-radius:1px}
.bars i.on{background:var(--green)}
.empty{font-size:13px;color:var(--muted);padding:6px 2px}
label{display:block;font-family:var(--mono);font-size:10px;color:var(--muted);text-transform:uppercase;letter-spacing:.08em;margin:12px 0 6px}
input{width:100%;background:#0a0a0f;border:1px solid var(--border);border-radius:8px;padding:13px 12px;color:var(--text);font-size:16px;outline:none}
input:focus{border-color:var(--accent)}
.pw{position:relative}
.pw button{position:absolute;right:6px;top:50%;transform:translateY(-50%);background:none;border:none;color:var(--muted);font-size:13px;padding:8px;cursor:pointer}
.btn{width:100%;padding:15px;border:1px solid var(--accent);border-radius:8px;background:rgba(240,192,0,.08);color:var(--accent);font-family:var(--mono);font-size:14px;cursor:pointer;margin-top:16px}
.btn:disabled{opacity:.5}
.msg{font-size:13px;margin-top:12px;line-height:1.5;min-height:18px}
.msg.err{color:var(--red)}
.msg.ok{color:var(--green)}
.link{background:none;border:none;color:var(--muted);font-size:13px;text-decoration:underline;cursor:pointer;padding:6px 0}
.hide{display:none}
</style>
</head>
<body>
<div class="wrap">
  <h1>BVG <em>Matrix</em> · WLAN</h1>
  <p class="sub">Wähle dein WLAN und gib das Passwort ein. Danach startet die Anzeige neu und verbindet sich.</p>

  <div id="form">
    <div class="card">
      <div class="title">Gefundene Netzwerke</div>
      <div id="list"><div class="empty">Lade…</div></div>
      <button class="link" id="manual-toggle" type="button">Netzwerk nicht dabei? Name selbst eingeben</button>
      <div id="manual" class="hide">
        <label for="ssid-manual">WLAN-Name (SSID)</label>
        <input id="ssid-manual" autocapitalize="off" autocorrect="off" spellcheck="false" placeholder="Name des WLANs">
      </div>
    </div>

    <div class="card">
      <div class="title">Ausgewählt: <span id="chosen" style="color:var(--accent);text-transform:none;letter-spacing:0">–</span></div>
      <label for="pw">Passwort</label>
      <div class="pw">
        <input id="pw" type="password" autocapitalize="off" autocorrect="off" spellcheck="false" placeholder="WLAN-Passwort" style="padding-right:70px">
        <button type="button" id="pw-toggle">zeigen</button>
      </div>
      <button class="btn" id="save" type="button">Speichern &amp; verbinden</button>
      <div class="msg" id="msg"></div>
    </div>
  </div>

  <div id="done" class="card hide">
    <div class="title">Gespeichert</div>
    <p style="font-size:14px;line-height:1.6">Die Anzeige startet neu und verbindet sich mit <b id="done-ssid"></b>.<br><br>
    1. Verbinde dein Handy wieder mit deinem normalen WLAN.<br>
    2. Die IP-Adresse erscheint kurz auf der Matrix.<br>
    3. Öffne dann <b>http://bvg-matrix.local/</b> oder die angezeigte IP, um Haltestellen einzustellen.<br><br>
    Klappt die Verbindung nicht (falsches Passwort), öffnet sich dieses Setup-WLAN nach etwa einer Minute erneut.</p>
  </div>
</div>

<script>
let selected = '';
let useManual = false;
const $ = id => document.getElementById(id);

function bars(rssi){
  const lvl = rssi > -55 ? 4 : rssi > -65 ? 3 : rssi > -75 ? 2 : 1;
  let h = '<div class="bars">';
  for (let i = 1; i <= 4; i++) h += `<i class="${i <= lvl ? 'on' : ''}" style="height:${3 + i * 3}px"></i>`;
  return h + '</div>';
}

function choose(ssid){
  selected = ssid;
  useManual = false;
  $('chosen').textContent = ssid || '–';
  document.querySelectorAll('.net').forEach(e => e.classList.toggle('sel', e.dataset.ssid === ssid));
}

async function loadNetworks(){
  const list = $('list');
  try {
    const d = await (await fetch('/api/wifi-networks')).json();
    list.innerHTML = '';
    if (!d.networks.length) { list.innerHTML = '<div class="empty">Keine Netzwerke gefunden – Namen selbst eingeben.</div>'; showManual(); return; }
    d.networks.forEach(n => {
      const el = document.createElement('div');
      el.className = 'net';
      el.dataset.ssid = n.ssid;
      el.innerHTML = `<span class="n"></span><span class="lock">${n.secure ? '🔒' : 'offen'}</span>${bars(n.rssi)}`;
      el.querySelector('.n').textContent = n.ssid;
      el.onclick = () => { choose(n.ssid); $('pw').focus(); };
      list.appendChild(el);
    });
  } catch (e) {
    list.innerHTML = '<div class="empty">Liste nicht ladbar – Namen selbst eingeben.</div>';
    showManual();
  }
}

function showManual(){
  $('manual').classList.remove('hide');
  $('manual-toggle').classList.add('hide');
  useManual = true;
  $('chosen').textContent = $('ssid-manual').value || '–';
}

$('manual-toggle').onclick = () => { showManual(); $('ssid-manual').focus(); };
$('ssid-manual').oninput = e => { useManual = true; $('chosen').textContent = e.target.value || '–'; document.querySelectorAll('.net').forEach(x => x.classList.remove('sel')); };
$('pw-toggle').onclick = () => {
  const p = $('pw'), show = p.type === 'password';
  p.type = show ? 'text' : 'password';
  $('pw-toggle').textContent = show ? 'verbergen' : 'zeigen';
};

$('save').onclick = async () => {
  const ssid = (useManual ? $('ssid-manual').value : selected).trim();
  const password = $('pw').value;
  const msg = $('msg');
  msg.className = 'msg';
  if (!ssid) { msg.className = 'msg err'; msg.textContent = 'Bitte ein WLAN auswählen.'; return; }
  $('save').disabled = true;
  msg.textContent = 'Speichere…';
  try {
    const r = await fetch('/api/wifi-save', {method: 'POST', headers: {'Content-Type': 'application/json'}, body: JSON.stringify({ssid, password})});
    const d = await r.json();
    if (d.ok) {
      $('done-ssid').textContent = ssid;
      $('form').classList.add('hide');
      $('done').classList.remove('hide');
    } else {
      msg.className = 'msg err'; msg.textContent = d.error || 'Fehler';
      $('save').disabled = false;
    }
  } catch (e) {
    msg.className = 'msg err'; msg.textContent = 'Keine Verbindung zum Board.';
    $('save').disabled = false;
  }
};

loadNetworks();
</script>
</body>
</html>
)BVGHTML";
