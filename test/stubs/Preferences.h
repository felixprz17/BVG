#pragma once
#include "Arduino.h"
struct Preferences { bool begin(const char*, bool) { return true; } void end() {} size_t putBytes(const char*, const void*, size_t n) { return n; }
  size_t getBytesLength(const char*) { return 0; } size_t getBytes(const char*, void*, size_t) { return 0; } void remove(const char*) {} };
