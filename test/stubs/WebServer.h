#pragma once
#include "WiFi.h"
enum HTTPMethod { HTTP_GET, HTTP_POST };
enum { UPLOAD_FILE_START, UPLOAD_FILE_WRITE, UPLOAD_FILE_END, UPLOAD_FILE_ABORTED };
struct HTTPUpload { int status; uint8_t* buf; size_t currentSize; };
struct WebServer { typedef std::function<void()> H; WebServer(int) {} void on(const char*, HTTPMethod, H) {} void on(const char*, HTTPMethod, H, H) {}
  void onNotFound(H) {} void begin() {} void handleClient() {} void send(int, const char*, const String&) {} void send_P(int, const char*, const char*) {}
  void sendHeader(const String&, const String&, bool = false) {} String arg(const char*) { return ""; } HTTPUpload& upload() { static HTTPUpload u; return u; } };
