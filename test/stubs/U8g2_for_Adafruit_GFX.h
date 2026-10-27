#pragma once
#include "Arduino.h"
static const uint8_t u8g2_font_4x6_tf[1] = {0}, u8g2_font_5x7_tf[1] = {0}, u8g2_font_6x10_tf[1] = {0};
struct Adafruit_GFX {};
struct U8G2_FOR_ADAFRUIT_GFX { void begin(Adafruit_GFX&) {} void setFontMode(int) {} void setFontDirection(int) {} void setFont(const uint8_t*) {} void setForegroundColor(uint16_t) {}
  int getFontAscent() { return 6; } int getUTF8Width(const char*) { return 0; } int drawUTF8(int, int, const char*) { return 0; } };
