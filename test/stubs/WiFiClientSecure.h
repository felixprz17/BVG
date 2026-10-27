#pragma once
#include "Arduino.h"
struct WiFiClient {}; struct WiFiClientSecure : WiFiClient { void setInsecure() {} void setTimeout(int) {} };
