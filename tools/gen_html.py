#!/usr/bin/env python3
"""Bettet ui/index.html und ui/setup.html als C++-Raw-Strings in bvg_matrix/web_ui.h ein.
Nach jeder Aenderung an ui/*.html ausfuehren:  python3 tools/gen_html.py"""
import os
root = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
out = ["// AUTOMATISCH ERZEUGT von tools/gen_html.py - nicht von Hand aendern (ui/*.html bearbeiten)\n#pragma once\n#include <pgmspace.h>\n"]
for name, var in (("index.html", "INDEX_HTML"), ("setup.html", "SETUP_HTML")):
    html = open(os.path.join(root, "ui", name), encoding="utf-8").read()
    assert ')BVGHTML"' not in html
    # Raw-String-Literale sind pro Stueck begrenzt (~64 kB bei manchen Compilern) -> in Teilen aneinanderhaengen
    parts = [html[i:i + 12000] for i in range(0, len(html), 12000)]
    out.append("static const char %s[] PROGMEM =\n" % var)
    out.append("\n".join('R"BVGHTML(%s)BVGHTML"' % p for p in parts))
    out.append(";\n")
open(os.path.join(root, "bvg_matrix", "web_ui.h"), "w", encoding="utf-8").write("".join(out))
print("web_ui.h geschrieben")
