#!/usr/bin/env python3

# сделано Цезарем
# made by Tsezar
"""
Convert a TTF font file into a C header for embedding.
Usage: python tools/embed_font.py font.ttf
Output: font_embedded.h (place next to ui.c)
"""
import sys
import os

FONT_PATH = sys.argv[1] if len(sys.argv) > 1 else "font.ttf"
OUT_PATH = "font_embedded.h"

if not os.path.exists(FONT_PATH):
    print(f"Error: {FONT_PATH} not found.")
    print("Place a .ttf file (e.g. JetBrainsMono-Regular.ttf) in the project root,")
    print("rename it to font.ttf, then run:")
    print("  python tools/embed_font.py")
    sys.exit(1)

with open(FONT_PATH, "rb") as f:
    data = f.read()

lines = ["#ifndef FONT_EMBEDDED_H", "#define FONT_EMBEDDED_H", "", f"#define FONT_TTF_SIZE {len(data)}", "", "static const unsigned char font_ttf_data[FONT_TTF_SIZE] = {"]
row = []
for i, byte in enumerate(data):
    row.append(f"0x{byte:02x}")
    if len(row) == 16:
        lines.append("    " + ", ".join(row) + ",")
        row = []
if row:
    lines.append("    " + ", ".join(row) + ",")
lines[-1] = lines[-1].rstrip(",")
lines.append("};")
lines.append("")
lines.append("#endif")

with open(OUT_PATH, "w", encoding="ascii") as f:
    f.write("\n".join(lines) + "\n")

print(f"Created {OUT_PATH} ({len(data)} bytes) from {FONT_PATH}")
print("Now rebuild the project to embed the font.")
