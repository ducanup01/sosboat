#include "monitor_OTA.h"
#include <WiFi.h>
#include <ArduinoOTA.h>
#include "global.h"

// ---- Define global variables (only here) ----
const char* ap_ssid = "chiec thuyen ngoai xa";
const char* ap_password = "123123123";



bool apStarted = false;

// ---------------- OTA CONFIG ----------------
void setupOTA() 
{
    ArduinoOTA.setHostname("chiecthuyenngoaixa");
    ArduinoOTA.setPassword("123123123");

    ArduinoOTA.onStart([]() {
        Serial.println("Start updating...");
    });

    ArduinoOTA.onEnd([]() {
        Serial.println("\nUpdate complete!");
    });

    ArduinoOTA.onProgress([](unsigned int progress, unsigned int total) {
        Serial.printf("Progress: %u%%\r", (progress * 100) / total);
    });

    ArduinoOTA.onError([](ota_error_t error) {
        Serial.printf("OTA Error[%u]\n", error);
    });

    ArduinoOTA.begin();
    Serial.println("OTA Ready.");
}


// ---------------- AP MODE START ----------------
void startAPMode()
{
    WiFi.disconnect(true);
    WiFi.mode(WIFI_AP);

    if (WiFi.softAP(ap_ssid, ap_password))
    {
        Serial.println("AP Mode ON");
        Serial.print("SSID: "); Serial.println(ap_ssid);
        Serial.print("Password: "); Serial.println(ap_password);
        Serial.print("IP: "); Serial.println(WiFi.softAPIP());

        setupOTA();
    }
    else
    {
        Serial.println("Failed to start AP mode");
    }
}


// ---------------- MAIN OTA TASK ----------------
void monitor_OTA(void *pvParameters)
{
    // pinMode(MODE_PIN, INPUT);
    pinMode(GREEN_LED, OUTPUT);
    digitalWrite(GREEN_LED, LOW);

    uint32_t lastBlink = millis();
    bool ledState = false;

    while (true)
    {

        if(!apStarted)
        {
            startAPMode();
            apStarted = true;
        }

        if (millis() - lastBlink > 160)
        {
            ledState = !ledState;
            digitalWrite(GREEN_LED, ledState);
            lastBlink = millis();
        }

        ArduinoOTA.handle();
        
        vTaskDelay(pdMS_TO_TICKS(60));
    }
}
