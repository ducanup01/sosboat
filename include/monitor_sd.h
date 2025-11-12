// #ifndef __MONITOR_SD__
// #define __MONITOR_SD__

// #include <Arduino.h>
// #include <SPI.h>
// #include <SD.h>

// #define SD_MOSI 23
// #define SD_MISO 19
// #define SD_SCK  18
// #define SD_CS   5

// // Create dedicated SPI instance
// SPIClass sdSPI(VSPI);

// void monitor_sd(void *pvParameters)
// {
//     Serial.println("[SD Task] Initializing SD card...");

//     // Configure CS pin safely
//     pinMode(SD_CS, OUTPUT);
//     digitalWrite(SD_CS, HIGH);
//     delay(10);

//     // Initialize the SPI bus
//     sdSPI.begin(SD_SCK, SD_MISO, SD_MOSI, SD_CS);

//     // Try mounting SD card
//     bool mounted = false;
//     for (int i = 0; i < 5; i++) {
//         if (SD.begin(SD_CS, sdSPI, 8000000U)) {  // 8 MHz
//             mounted = true;
//             break;
//         }
//         Serial.println("[SD Task] Card Mount Failed! Retrying...");
//         vTaskDelay(pdMS_TO_TICKS(1000));
//     }

//     if (!mounted) {
//         Serial.println("[SD Task] Failed to mount SD card after multiple attempts.");
//         vTaskDelete(NULL);
//         return;
//     }

//     uint8_t cardType = SD.cardType();
//     if (cardType == CARD_NONE) {
//         Serial.println("[SD Task] No SD card attached.");
//         vTaskDelete(NULL);
//         return;
//     }

//     Serial.print("[SD Task] SD Card Type: ");
//     if (cardType == CARD_MMC)      Serial.println("MMC");
//     else if (cardType == CARD_SD)  Serial.println("SDSC");
//     else if (cardType == CARD_SDHC) Serial.println("SDHC");
//     else                           Serial.println("UNKNOWN");

//     uint64_t cardSize = SD.cardSize() / (1024 * 1024);
//     Serial.printf("[SD Task] SD Card Size: %llu MB\n", cardSize);

//     // Try creating a test file
//     File testFile = SD.open("/test.txt", FILE_WRITE);
//     if (testFile) {
//         testFile.println("Hello from ESP32 SD test (FreeRTOS task)!");
//         testFile.close();
//         Serial.println("[SD Task] File written successfully.");
//     } else {
//         Serial.println("[SD Task] Failed to open file for writing.");
//     }

//     // Read back the file
//     testFile = SD.open("/test.txt");
//     if (testFile) {
//         Serial.println("[SD Task] Reading back file content:");
//         while (testFile.available()) {
//             Serial.write(testFile.read());
//             vTaskDelay(1); // yield a bit
//         }
//         testFile.close();
//     } else {
//         Serial.println("[SD Task] Failed to open file for reading.");
//     }

//     Serial.println("[SD Task] SD card test complete.");
//     vTaskDelete(NULL); // safely delete the task when done
// }

// #endif
