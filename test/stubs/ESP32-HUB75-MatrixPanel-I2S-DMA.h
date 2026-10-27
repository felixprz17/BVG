#pragma once
#include "U8g2_for_Adafruit_GFX.h"
struct HUB75_I2S_CFG { struct { int8_t r1,g1,b1,r2,g2,b2,a,b,c,d,e,lat,oe,clk; } gpio; bool double_buff; HUB75_I2S_CFG(int, int, int) {} };
struct MatrixPanel_I2S_DMA : Adafruit_GFX { MatrixPanel_I2S_DMA(HUB75_I2S_CFG&) {} bool begin() { return true; } void clearScreen() {} void flipDMABuffer() {}
  void setBrightness8(uint8_t) {} uint16_t color565(uint8_t, uint8_t, uint8_t) { return 0; } void drawFastHLine(int, int, int, uint16_t) {} };
