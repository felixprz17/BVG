#pragma once
#include "Arduino.h"
struct File { operator bool() const { return false; } void close() {} void print(const char*) {} size_t write(uint8_t) { return 0; } size_t write(const uint8_t*, size_t) { return 0; } int read() { return -1; } int peek() { return -1; } size_t readBytes(char*, size_t) { return 0; } };
struct FST { bool begin(bool) { return true; } File open(const char*, const char*) { return {}; } bool exists(const char*) { return false; } void remove(const char*) {} } static LittleFS;
