#include "monitor_lora.h"
#include <LoRa.h>
#include "global.h"

// Define SPI instance (only once here)
SPIClass loraSPI(VSPI);

void monitor_lora(void *pvParameters)
{
    // pinMode(LED_PIN, OUTPUT);
    digitalWrite(GREEN_LED, LOW);

    Serial.println("[LoRa Task] Initializing...");

    // Setup SPI & CS
    // pinMode(LORA_CS, OUTPUT);
    // digitalWrite(LORA_CS, HIGH);
    loraSPI.begin(LORA_SCK, LORA_MISO, LORA_MOSI, LORA_CS);

    LoRa.setSPI(loraSPI);
    LoRa.setPins(LORA_CS, LORA_RST, LORA_DIO0);

    bool loraInit = false;
    for (int i = 0; i < 5; i++) {
        if (LoRa.begin(433E6)) {
            loraInit = true;
            break;
        }
        Serial.printf("[LoRa Task] LoRa init failed (try %d)\n", i + 1);
        vTaskDelay(pdMS_TO_TICKS(1000));
    }

    // if (!loraInit) {
    //     Serial.println("[LoRa Task] Failed to start LoRa after retries!");
    //     vTaskDelete(NULL);
    //     return;
    // }

    if (loraInit) {
        Serial.println("[LoRa Test] LoRa initialized successfully!");
    } else {
        Serial.println("[LoRa Test] LoRa failed to initialize!");
        digitalWrite(GREEN_LED, HIGH); // Turn on LED
    }

    Serial.println("[LoRa Task] LoRa initialized successfully!");
    LoRa.setTxPower(17);
    LoRa.setSpreadingFactor(7);
    LoRa.setSignalBandwidth(125E3);
    LoRa.setCodingRate4(5);

    while (true)
    {
        // int packetSize = LoRa.parsePacket();
        // if (packetSize)
        // {
        //     String incoming = "";
        //     while (LoRa.available()) {
        //         incoming += (char)LoRa.read();
        //     }
        //     incoming.trim();

        //     Serial.printf("[LoRa Task] Received: %s\n", incoming.c_str());

        //     if (incoming == "ON") {
        //         digitalWrite(GREEN_LED, HIGH);
        //         Serial.println("[LoRa Task] LED ON");
        //     } 
        //     else if (incoming == "OFF") {
        //         digitalWrite(GREEN_LED, LOW);
        //         Serial.println("[LoRa Task] LED OFF");
        //     }

        //     // Send response
        //     String response = "Received: " + incoming;
        //     LoRa.beginPacket();
        //     LoRa.print(response);
        //     LoRa.endPacket();

        //     Serial.printf("[LoRa Task] Sent response: %s\n", response.c_str());
        // }

        vTaskDelay(pdMS_TO_TICKS(100));
    }
}
