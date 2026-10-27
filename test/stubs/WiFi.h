#pragma once
#include "Arduino.h"
struct IPAddress { String toString() const { return "1.2.3.4"; } };
enum { WL_CONNECTED = 3, WIFI_STA = 1, WIFI_AP = 2, WIFI_AUTH_OPEN = 0 };
struct WiFiT { int status() { return 0; } String SSID(int = -1) { return ""; } int RSSI(int = -1) { return 0; } IPAddress localIP() { return {}; }
  IPAddress softAPIP() { return {}; } void mode(int) {} void disconnect() {} int scanNetworks() { return 0; } int encryptionType(int) { return 0; }
  void softAP(const char*) {} void setHostname(const char*) {} void setAutoReconnect(bool) {} void begin(const char*, const char*) {} } static WiFi;
