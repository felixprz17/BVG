#pragma once
#include "Arduino.h"
#define UPDATE_SIZE_UNKNOWN 0xFFFFFFFF
struct UpdateT { bool hasError() { return false; } String errorString() { return ""; } bool begin(uint32_t) { return true; } size_t write(uint8_t*, size_t) { return 0; } bool end(bool) { return true; } void abort() {} } static Update;
