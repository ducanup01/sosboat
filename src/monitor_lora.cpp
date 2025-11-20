#include "monitor_lora.h"
#include <LoRa.h>
#include "global.h"
#include "yaw_PID.h"

// Define SPI instance (only once here)
SPIClass loraSPI(VSPI);

void handleLoRaCommand(String input) {
    input.trim();  // remove whitespace

    // ----------------- PID tuning -----------------
    if (input.startsWith("kp=")) {
        Kp = input.substring(3).toFloat();
        LoRaPrintln("🔧 Updated Kp = " + String(Kp, 4));
    }
    else if (input.startsWith("ki=")) {
        Ki = input.substring(3).toFloat();
        LoRaPrintln("🔧 Updated Ki = " + String(Ki, 4));
    }
    else if (input.startsWith("kd=")) {
        Kd = input.substring(3).toFloat();
        LoRaPrintln("🔧 Updated Kd = " + String(Kd, 4));
    }

    // ----------------- Motion commands -----------------
    else if (input.equalsIgnoreCase("go")) {
        // sensors_event_t ori;
        // bno.getEvent(&ori, Adafruit_BNO055::VECTOR_EULER);
        // targetYaw = normalizeYaw(ori.orientation.x);
        targetYaw = yaw;
        lastError = 0;
        integral = 0;
        lastTime = micros();
        isRunning = true;
        LoRaPrintln("🚤 Going straight! Target yaw: " + String(targetYaw, 2) + "°");
    }
    else if (input.equalsIgnoreCase("stop")) {
        isRunning = false;
        stopMotors();
        LoRaPrintln("🛑 Boat stopped.");
    }

    // ----------------- Turn commands -----------------
    else if (input.startsWith("r")) { // e.g., "r90"
        int angle = input.substring(1).toInt();
        targetYaw += angle;
        if (targetYaw >= 360) targetYaw -= 360;
        LoRaPrintln("➡️ Turn right " + String(angle) + "°! Target yaw: " + String(targetYaw, 2) + "°");
    }
    else if (input.startsWith("l")) { // e.g., "l40"
        int angle = input.substring(1).toInt();
        targetYaw -= angle;
        if (targetYaw < 0) targetYaw += 360;
        LoRaPrintln("⬅️ Turn left " + String(angle) + "°! Target yaw: " + String(targetYaw, 2) + "°");
    }

    // ----------------- Pump commands -----------------
    else if (input.equalsIgnoreCase("sm")) {
        pumpAbort = true;
        pumpCommand = PUMP_IN;
        LoRaPrintln("💧 Submerging...");
    }
    else if (input.equalsIgnoreCase("sf")) {
        pumpAbort = true;
        pumpCommand = PUMP_OUT;
        LoRaPrintln("💧 Surfacing...");
    }
    else if (input.equalsIgnoreCase("sp")) {
        pumpAbort = true;
        pumpCommand = PUMP_STOP;
        LoRaPrintln("PUMP STOPPED");
    }

    // ----------------- Base speed -----------------
    else if (input.startsWith("bs")) { // e.g., "bs 180"
        int speed = input.substring(3).toInt();
        baseSpeed = constrain(speed, -255, 255); // limit speed
        LoRaPrintln("⚡ Base speed set to " + String(baseSpeed));
    }

    else if (input == "on") {
        digitalWrite(GREEN_LED, HIGH);
        LoRaPrintln("[LoRa Task] LED ON");  
    }

    else if (input == "off") {
        digitalWrite(GREEN_LED, LOW);
        LoRaPrintln("[LoRa Task] LED OFF");
    }

    else {
        LoRaPrintln("❌ Unknown command: " + input);
    }
}

void monitor_lora(void *pvParameters)
{
    // pinMode(LED_PIN, OUTPUT);
    digitalWrite(GREEN_LED, LOW);

    // Serial.println("[LoRa Task] Initializing...");

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

    Serial.println("[LoRa Task] LoRa initialized successfully!");
    LoRa.setTxPower(17);
    LoRa.setSpreadingFactor(7);
    LoRa.setSignalBandwidth(125E3);
    LoRa.setCodingRate4(5);

    while (true)
    {
        String msg = LoRaRead(); // non-blocking read
        if (msg.length() > 0) {
            handleLoRaCommand(msg); // handle command
        }

        vTaskDelay(pdMS_TO_TICKS(3));
    }
}
