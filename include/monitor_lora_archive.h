// #ifndef __MONITOR_LORA__
// #define __MONITOR_LORA__

// #include <Arduino.h>
// #include <SPI.h>
// #include <LoRa.h>

// // Adjust pin mapping to match your board
// #define LORA_SCK   14    // SPI Clock
// #define LORA_MISO  12   // MISO
// #define LORA_MOSI  13   // MOSI
// #define LORA_CS    15   // NSS / CS
// #define LORA_RST   16   // Reset
// #define LORA_IRQ   26   // DIO0 (interrupt pin)

// // Frequency (depends on your region)
// #define LORA_BAND  433E6   // 915 MHz for US/VN — use 868E6 for EU, 433E6 for CN

// SPIClass loraSPI(HSPI);

// void monitor_lora(void *pvParameters)
// {
//     Serial.println("[LoRa Task] Initializing...");

//     // Setup SPI for LoRa
//     pinMode(LORA_CS, OUTPUT);
//     digitalWrite(LORA_CS, HIGH);
//     loraSPI.begin(LORA_SCK, LORA_MISO, LORA_MOSI, LORA_CS);

//     // Initialize LoRa
//     LoRa.setSPI(loraSPI);
//     LoRa.setPins(LORA_CS, LORA_RST, LORA_IRQ);

//     bool loraInit = false;
//     for (int i = 0; i < 5; i++) {
//         if (LoRa.begin(LORA_BAND)) {
//             loraInit = true;
//             break;
//         }
//         Serial.printf("[LoRa Task] LoRa init failed (try %d)\n", i + 1);
//         vTaskDelay(pdMS_TO_TICKS(1000));
//     }

//     if (!loraInit) {
//         Serial.println("[LoRa Task] Failed to start LoRa after retries!");
//         vTaskDelete(NULL);
//         return;
//     }

//     Serial.println("[LoRa Task] LoRa initialized successfully!");
//     Serial.printf("[LoRa Task] Frequency: %.1f MHz\n", LORA_BAND / 1E6);

//     // Optional tuning
//     LoRa.setTxPower(17);         // TX power in dBm (2–20)
//     LoRa.setSpreadingFactor(7);  // SF7 for faster communication
//     LoRa.setSignalBandwidth(125E3);
//     LoRa.setCodingRate4(5);

//     unsigned long lastSendTime = 0;
//     int counter = 0;

//     while (true)
//     {
//         // Send packet every 3 seconds
//         if (millis() - lastSendTime > 3000) {
//             String msg = "Hello LoRa #" + String(counter++);
//             Serial.printf("[LoRa Task] Sending: %s\n", msg.c_str());
//             LoRa.beginPacket();
//             LoRa.print(msg);
//             LoRa.endPacket();
//             lastSendTime = millis();
//         }

//         // Receive packets
//         int packetSize = LoRa.parsePacket();
//         if (packetSize) {
//             Serial.print("[LoRa Task] Received packet: ");
//             String incoming = "";
//             while (LoRa.available()) {
//                 incoming += (char)LoRa.read();
//             }
//             Serial.println(incoming);

//             Serial.printf("[LoRa Task] RSSI: %d dBm, SNR: %.2f\n",
//                           LoRa.packetRssi(), LoRa.packetSnr());
//         }

//         vTaskDelay(pdMS_TO_TICKS(100));  // Yield CPU
//     }

//     vTaskDelete(NULL);
// }

// #endif
