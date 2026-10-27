#pragma once
#include <cstdint>
#include <cstdio>
#include <cstring>
#include <string>
#include <algorithm>
#include <initializer_list>
#include <functional>
#include <time.h>
#define PROGMEM
#define HIGH 1
#define LOW 0
#define INPUT_PULLUP 2
#define portMAX_DELAY 0xffffffff
#define portTICK_PERIOD_MS 1
using std::max; using std::min;
class String : public std::string {
 public:
  String() {} String(const char* s) : std::string(s ? s : "") {} String(const std::string& s) : std::string(s) {}
  String(int v) : std::string(std::to_string(v)) {} String(unsigned v) : std::string(std::to_string(v)) {}
  String(const String&) = default; String& operator=(const String&) = default;
  unsigned length() const { return size(); }
  size_t write(uint8_t c) { push_back((char)c); return 1; } size_t write(const uint8_t* b, size_t n) { append((const char*)b, n); return n; }
  bool concat(const char* s) { append(s); return true; } bool concat(const char* s, size_t n) { append(s, n); return true; } bool reserve(size_t n) { std::string::reserve(n); return true; }
  String substring(int a, int b) const { return String(substr(a, b - a)); }
  void trim() {}
  bool operator<(const String& o) const { return static_cast<const std::string&>(*this) < o; }
  String operator+(const String& o) const { return String(static_cast<const std::string&>(*this) + static_cast<const std::string&>(o)); }
  String operator+(const char* o) const { return String(static_cast<const std::string&>(*this) + o); }
  operator const char*() const { return c_str(); }
};
inline String operator+(const char* a, const String& b) { return String(std::string(a) + b.c_str()); }
struct SerialT { void begin(int) {} void println(const String&) {} void println(const char*) {} template <class... A> void printf(const char*, A...) {} } static Serial;
inline uint32_t millis() { return 0; } inline void delay(int) {} inline void pinMode(int, int) {} inline int digitalRead(int) { return 1; }
typedef void* SemaphoreHandle_t; inline SemaphoreHandle_t xSemaphoreCreateMutex() { return nullptr; }
inline void xSemaphoreTake(SemaphoreHandle_t, uint32_t) {} inline void xSemaphoreGive(SemaphoreHandle_t) {}
inline void vTaskDelay(int) {}
inline void xTaskCreatePinnedToCore(void (*)(void*), const char*, int, void*, int, void*, int) {}
inline void configTzTime(const char*, const char*, const char*, const char*) {}
struct EspT { uint32_t getFreeHeap() { return 0; } uint32_t getMaxAllocHeap() { return 0; } uint32_t getFreePsram() { return 0; } void restart() {} } static ESP;
