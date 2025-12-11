#include "global.h"
#include <LoRa.h>

volatile float Kp = 5;
volatile float Ki = 6; 
volatile float Kd = 7; 

// Define variables
float yaw = 0;
float pitch = 0;
float roll = 0;
float pressure = 0;

float targetYaw = 0;

int baseSpeed = 20; //0 to 155

// Motor states
volatile PumpCommand pumpCommand = PUMP_IDLE;
volatile bool pumpAbort = false;

void LoRaPrintln(const String &msg)
{
    LoRa.beginPacket();
    LoRa.print(msg + "\n");
    LoRa.endPacket();

    vTaskDelay(pdMS_TO_TICKS(25));
}

// String LoRaRead() {
//     String received = "";

//     int packetSize = LoRa.parsePacket();
//     if (packetSize) {
//         // Read all bytes in the packet until newline or end
//         while (LoRa.available()) {
//             char c = (char)LoRa.read();
//             if (c == '\n') break;  // stop at newline
//             received += c;
//         }
//     }

//     vTaskDelay(pdMS_TO_TICKS(25)); // small yield
//     return received; // empty string if nothing received
// }

String LoRaRead() {
    String received = "";

    int packetSize = LoRa.parsePacket();
    if (packetSize) {
        while (LoRa.available()) {
            received += (char)LoRa.read();
        }
        received.trim(); // remove any trailing newline/whitespace
    }
    vTaskDelay(pdMS_TO_TICKS(20));
    return received; // empty string if nothing received
}