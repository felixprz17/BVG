#pragma once
#include "Arduino.h"
struct MDNST { bool begin(const char*) { return true; } void addService(const char*, const char*, int) {} } static MDNS;
