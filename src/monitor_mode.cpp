#include "monitor_mode.h"
#include "global.h"

void modeManager(void *pvParameters)
{
    // pinMode(MODE_PIN, INPUT);

    bool lastState = gpio_get_level(MODE_PIN);

    while (1)
    {
        bool currentState = gpio_get_level(MODE_PIN);

        if (currentState != lastState)
        {
            Serial.println("[MODE] Edge detected on GPIO 36, restarting ESP32...");
            digitalWrite(GREEN_LED, HIGH);
            vTaskDelay(pdMS_TO_TICKS(100)); 

            ESP.restart();
        }

        lastState = currentState;
        vTaskDelay(pdMS_TO_TICKS(50));
    }
}
