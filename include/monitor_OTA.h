#ifndef __MONITOR_OTA__
#define __MONITOR_OTA__

#include <Arduino.h>

// ---- Global variables (declared, not defined) ----
extern const char* ap_ssid;
extern const char* ap_password;

// ---- OTA functions ----
void setupOTA();
void startAPMode();
void monitor_OTA(void *pvParameters);

#endif
