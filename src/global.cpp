#include "global.h"
#include <LoRa.h>

float Kp = 7.5;
float Ki = 2.5; 
float Kd = 4.0; 

// Define variables
float yaw = 0;
float pitch = 0;
float roll = 0;
float pressure = 0;

float targetYaw = 0;

int baseSpeed = 180;

void LoRaPrintln(const String &msg)
{
    LoRa.beginPacket();
    LoRa.print(msg + "\n");
    LoRa.endPacket();

    vTaskDelay(pdMS_TO_TICKS(25));
}

String LoRaRead() {
    String received = "";

    int packetSize = LoRa.parsePacket();
    if (packetSize) {
        // Read all bytes in the packet until newline or end
        while (LoRa.available()) {
            char c = (char)LoRa.read();
            if (c == '\n') break;  // stop at newline
            received += c;
        }
    }

    vTaskDelay(pdMS_TO_TICKS(25)); // small yield
    return received; // empty string if nothing received
}