#ifndef __MONITOR_OTA__
#define __MONITOR_OTA__
#include <Arduino.h>
#include <WiFi.h>
#include <ArduinoOTA.h>
#include "motor_control.h" 
#include "yaw_PID.h"

#define BOOT_BTN GPIO_NUM_0

// placeholder

#define max_duty 255

const char* ssid = "12N9";
const char* password = "dangducan";
// const char* ssid = "Fulbright_Student1";
// const char* password = "fulbright2018";
const uint16_t TCP_PORT = 69;

WiFiServer server(TCP_PORT);
WiFiClient client;

Adafruit_BNO055 bno = Adafruit_BNO055(55, 0x28);


const char* ap_ssid = "chiec thuyen ngoai xa";
const char* ap_password = "123123123";

void stopMotors();
void motorControl(int leftSpeed, int rightSpeed);

void handle_serial_input_v2()
{
    // if (!client || !client.connected())
    // {
    //     client = server.available();
    //     if (client)
    //     {
    //         Serial.println("Client connected!");
    //         client.println("Connected to ESP32 WiFi Serial Bridge!");
    //     }
    // }

    // if (Serial.available())
    // {
    //     while (Serial.available())
    //     {
    //         client.write(Serial.read());
    //         // ArduinoOTA.handle();
    //     }
    // }

    // if (client && client.available())
    // {
    //     String input = client.readStringUntil('\n');
    //     input.trim();

    //     if (input.length() > 0)
    //     {
    //         int speedValue = input.toInt();
    //         speedValue = constrain(speedValue, -max_duty, max_duty);

    //         if (speedValue == 0)
    //         {
    //             stopMotors();
    //             client.println("Motors stopped!");
    //         }
    //         else
    //         {
    //             motorControl(speedValue, speedValue);
    //             client.printf("Motors running at %d (PWM duty)\n", speedValue);
    //         }
    //     }
    // }

    if (client && client.available())
    {
        String input = client.readStringUntil('\n');
        input.trim();

        if (input.length() == 0) return; // skip empty lines

        // Check if input is numeric (possibly with a leading minus sign)
        bool isNumeric = true;
        for (size_t i = 0; i < input.length(); i++)
        {
            if (!(isDigit(input[i]) || (i == 0 && input[i] == '-')))
            {
                isNumeric = false;
                break;
            }
        }

        if (!isNumeric)
        {
            // Non-numeric command (e.g. "go" or "stop")
            handleCommand(input);
        }
        else
        {
            // Numeric command (manual speed input)
            int speedValue = input.toInt();
            speedValue = constrain(speedValue, -max_duty, max_duty);

            if (speedValue == 0)
            {
                stopMotors();
                client.println("Motors stopped!");
            }
            else
            {
                motorControl(speedValue, speedValue);
                client.printf("Motors running at %d (PWM duty)\n", speedValue);
            }
        }
    }

}

void handle_serial_input()
{
    // --- Handle USB Serial input (from PC Serial Monitor) ---
    if (Serial.available())
    {
        String input = Serial.readStringUntil('\n');
        input.trim();
        if (input.length() == 0) return;

        if (isdigit(input[0]) || input[0] == '-')
        {
            int speedValue = input.toInt();
            speedValue = constrain(speedValue, -max_duty, max_duty);

            if (speedValue == 0)
            {
                stopMotors();
                Serial.println("Motors stopped!");
            }
            else
            {
                motorControl(speedValue, speedValue);
                Serial.printf("Motors running at %d (PWM duty)\n", speedValue);
            }
        }
        else
        {
            handleCommand(input);
        }
    }

    // --- Handle TCP client input (from WiFi) ---
    if (!client || !client.connected())
    {
        client = server.available();
        if (client)
        {
            Serial.println("Client connected!");
            client.println("Setting up BNO055...");
            setup_imu();
            client.println("Connected to ESP32 WiFi Serial Bridge!");
            // sensors_event_t acc, gyro, mag, ori;
        }
    }

    if (client && client.available())
    {
        String input = client.readStringUntil('\n');
        input.trim();
        if (input.length() == 0) return;

        if (isdigit(input[0]) || input[0] == '-')
        {
            int speedValue = input.toInt();
            speedValue = constrain(speedValue, -max_duty, max_duty);

            if (speedValue == 0)
            {
                stopMotors();
                client.println("Motors stopped!");
            }
            else
            {
                motorControl(speedValue, speedValue);
                client.printf("Motors running at %d (PWM duty)\n", speedValue);
            }
        }
        else
        {
            handleCommand(input);
        }
    }
}



void switchToAPMode()
{
    Serial.println("Switching to AP mode for 5 minutes...");
    vTaskDelay(pdMS_TO_TICKS(50));
    WiFi.disconnect(true);
    WiFi.mode(WIFI_AP);

    if (WiFi.softAP(ap_ssid, ap_password))
    {
        Serial.println("Access Point started!");
        vTaskDelay(pdMS_TO_TICKS(50));
        Serial.print("SSID: "); vTaskDelay(pdMS_TO_TICKS(50));
        Serial.println(ap_ssid); vTaskDelay(pdMS_TO_TICKS(50));
        Serial.print("Password: "); vTaskDelay(pdMS_TO_TICKS(50));
        Serial.println(ap_password); vTaskDelay(pdMS_TO_TICKS(50));
        Serial.print("AP IP address: "); vTaskDelay(pdMS_TO_TICKS(50));
        Serial.println(WiFi.softAPIP()); vTaskDelay(pdMS_TO_TICKS(50));
    }
    else
    {
        Serial.println("Failed to start AP mode!"); vTaskDelay(pdMS_TO_TICKS(50));
        return;
    }

    ArduinoOTA.begin();
    unsigned long apStart = millis();

    while (millis() - apStart < 5*60*1000UL)
    {
        ArduinoOTA.handle();
        vTaskDelay(pdMS_TO_TICKS(150));
    }

    Serial.println("\n5 minutes passed, returning to STA mode..."); vTaskDelay(pdMS_TO_TICKS(50));
    WiFi.softAPdisconnect(true);

    WiFi.mode(WIFI_STA);
    WiFi.begin(ssid, password);
    Serial.println("Reconnecting to WiFi..."); vTaskDelay(pdMS_TO_TICKS(50));

    while (WiFi.waitForConnectResult() != WL_CONNECTED)
    {
        Serial.println("WiFi failed, retrying...");
        vTaskDelay(pdMS_TO_TICKS(1000));
    }

    Serial.print("WiFi connected: "); vTaskDelay(pdMS_TO_TICKS(50));
    Serial.println(ssid); vTaskDelay(pdMS_TO_TICKS(50));
    Serial.print("IP address: "); vTaskDelay(pdMS_TO_TICKS(50));
    Serial.println(WiFi.localIP()); vTaskDelay(pdMS_TO_TICKS(50));

    ArduinoOTA.begin();

}

void monitor_OTA(void *pvParameters)
{
    Serial.println("Booting..."); vTaskDelay(pdMS_TO_TICKS(50));
    // Serial.println("Setting up BNO055...");
    // setup_imu();

    WiFi.mode(WIFI_STA);
    vTaskDelay(pdMS_TO_TICKS(500));
    WiFi.begin(ssid, password);
    vTaskDelay(pdMS_TO_TICKS(500));
    Serial.println("Connecting to WiFi..."); vTaskDelay(pdMS_TO_TICKS(50));

    int retryCount = 0;

    while (WiFi.status() != WL_CONNECTED && retryCount < 12)
    {
        Serial.println("WiFi not connected, retrying...");
        WiFi.begin(ssid, password);
        retryCount++;
        vTaskDelay(pdMS_TO_TICKS(5000));
    }

    if (WiFi.status() != WL_CONNECTED)
    {
        Serial.println("No WiFi connection. Switching to AP mode...");
        switchToAPMode();
    }

    // while (WiFi.waitForConnectResult() != WL_CONNECTED)
    // {
    //     Serial.println("Rebooting...");
    //     vTaskDelay(pdMS_TO_TICKS(3000));
    //     ESP.restart();
    // }


    Serial.println("WiFi connected!"); vTaskDelay(pdMS_TO_TICKS(50));

    ArduinoOTA.setHostname("chiecthuyenngoaixa");
    ArduinoOTA.setPassword("123123123");

    ArduinoOTA
        .onStart([]() 
        {
            String type;
            if (ArduinoOTA.getCommand() == U_FLASH)
            type = "sketch";
            else
            type = "filesystem";

            Serial.println("Start updating " + type);
        })
        .onEnd([]() 
        {
            Serial.println("\nUpdate complete!");
        })
        .onProgress([](unsigned int progress, unsigned int total) 
        {
            Serial.printf("Progress: %u%%\r", (progress / (total / 100)));
        })
        .onError([](ota_error_t error) {
            Serial.printf("Error[%u]: ", error);
            if (error == OTA_AUTH_ERROR) Serial.println("Auth Failed");
            else if (error == OTA_BEGIN_ERROR) Serial.println("Begin Failed");
            else if (error == OTA_CONNECT_ERROR) Serial.println("Connect Failed");
            else if (error == OTA_RECEIVE_ERROR) Serial.println("Receive Failed");
            else if (error == OTA_END_ERROR) Serial.println("End Failed");
        });
    
    ArduinoOTA.begin();

    Serial.println("Ready for OTA updates!"); vTaskDelay(pdMS_TO_TICKS(50));
    Serial.print("IP address: "); vTaskDelay(pdMS_TO_TICKS(50));
    Serial.println(WiFi.localIP());

    server.begin();
    Serial.println("TCP server started, waiting for client...");


    pinMode(BOOT_BTN, INPUT_PULLUP);

    while (1)
    {
        if (digitalRead(BOOT_BTN) == LOW)
        {
            vTaskDelay(pdMS_TO_TICKS(50));
            if (digitalRead(BOOT_BTN) == LOW)
            {
                while (digitalRead(BOOT_BTN) == LOW)
                {
                    vTaskDelay(pdMS_TO_TICKS(50));
                }

                switchToAPMode();
            }
        }

        ArduinoOTA.handle();

        handle_serial_input();

        vTaskDelay(pdMS_TO_TICKS(50));
    }
}

#endif