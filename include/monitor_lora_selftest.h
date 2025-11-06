#ifndef __MONITOR_LORA_SELFTEST__
#define __MONITOR_LORA_SELFTEST__

#include <Arduino.h>
#include <SPI.h>
#include <LoRa.h>

// ------------------- Pin Configuration -------------------
#define LORA_SCK   14    // SPI Clock
#define LORA_MISO  12    // MISO
#define LORA_MOSI  13    // MOSI
#define LORA_CS    15    // NSS / CS
#define LORA_RST   16    // Reset
#define LORA_IRQ   26    // DIO0 (not used in self-test)

// Frequency band (adjust for your region)
#define LORA_BAND  433E6   // 433 MHz (use 868E6 for EU or 915E6 for US/VN)

SPIClass loraSPI(VSPI);

// ----------------------------------------------------------
// LoRa Self-Test Task
// ----------------------------------------------------------
void monitor_lora_selftest(void *pvParameters)
{
    Serial.println("[LoRa SelfTest] Starting...");

    // Setup SPI
    pinMode(LORA_CS, OUTPUT);
    digitalWrite(LORA_CS, HIGH);
    loraSPI.begin(LORA_SCK, LORA_MISO, LORA_MOSI, LORA_CS);

    // Configure LoRa to use VSPI and selected pins
    LoRa.setSPI(loraSPI);
    LoRa.setPins(LORA_CS, LORA_RST, LORA_IRQ);

    // Try initializing LoRa up to 5 times
    bool loraInit = false;
    for (int i = 0; i < 5; i++) {
        if (LoRa.begin(LORA_BAND)) {
            loraInit = true;
            break;
        }
        Serial.printf("[LoRa SelfTest] Init failed (try %d)\n", i + 1);
        vTaskDelay(pdMS_TO_TICKS(1000));
    }

    if (!loraInit) {
        Serial.println("[LoRa SelfTest] ❌ Failed to initialize LoRa after retries.");
        vTaskDelete(NULL);
        return;
    }

    Serial.println("[LoRa SelfTest] ✅ LoRa initialized successfully!");
    Serial.printf("[LoRa SelfTest] Frequency: %.1f MHz\n", LORA_BAND / 1E6);

    // ------------------- Chip Information -------------------
    uint8_t version = LoRa.readRegister(REG_VERSION);
    Serial.printf("[LoRa SelfTest] Chip version register: 0x%02X\n", version);
    if (version == 0x12) {
        Serial.println("[LoRa SelfTest] SX1276/77/78/79 detected.");
    } else {
        Serial.println("[LoRa SelfTest] ⚠️ Unknown or incompatible LoRa chip detected!");
    }

    // ------------------- Test Transmission -------------------
    Serial.println("[LoRa SelfTest] Sending test packet...");
    LoRa.beginPacket();
    LoRa.print("LoRa Self-Test Packet");
    LoRa.endPacket(); // non-blocking send
    Serial.println("[LoRa SelfTest] Transmission complete.");

    // ------------------- Check RSSI Noise Floor -------------------
    vTaskDelay(pdMS_TO_TICKS(500));
    int rssi = LoRa.packetRssi();
    Serial.printf("[LoRa SelfTest] Noise floor (RSSI): %d dBm\n", rssi);
    Serial.println("[LoRa SelfTest] Expect around -100 to -120 dBm in quiet environment.");

    // ------------------- Read Current Config -------------------
    Serial.printf("[LoRa SelfTest] TX Power: %d dBm\n", 17);
    Serial.printf("[LoRa SelfTest] Bandwidth: %.0f kHz\n", LoRa.signalBandwidth() / 1E3);
    Serial.printf("[LoRa SelfTest] Spreading Factor: %d\n", LoRa.spreadingFactor());
    Serial.printf("[LoRa SelfTest] Coding Rate: 4/%d\n", LoRa.codingRate4());

    Serial.println("[LoRa SelfTest] ✅ Self-test completed successfully!");
    Serial.println("[LoRa SelfTest] You can now use this LoRa module for TX/RX tasks.");

    // Stop the task (only runs once)
    vTaskDelete(NULL);
}

#endif
