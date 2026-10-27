#pragma once
#include "WiFiClientSecure.h"
struct Stream : std::string { int read() { return -1; } size_t readBytes(char*, size_t) { return 0; } };
struct HTTPClient { void setConnectTimeout(int) {} void setTimeout(int) {} void useHTTP10(bool) {} void setReuse(bool) {} bool begin(WiFiClient&, const String&) { return true; }
  int GET() { return 200; } Stream& getStream() { static Stream s; return s; } void end() {} void setUserAgent(const char*) {} String getString() { return ""; } static String errorToString(int) { return ""; } };
