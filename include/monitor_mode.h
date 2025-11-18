// #ifndef __MONITOR_MODE__
// #define __MONITOR_MODE__

// #include <Arduino.h>

// #define MODE_PIN GPIO_NUM_36

// void modeManager(void *pvParameters)
// {
//     pinMode(MODE_PIN, INPUT);

//     bool lastState = gpio_get_level(MODE_PIN);

//     while (1)
//     {
//         bool currentState = gpio_get_level(MODE_PIN);

//         if (currentState != lastState)
//         {
//             Serial.println("[MODE] Edge detected on GPIO 36, restarting ESP32...");
//             vTaskDelay(pdMS_TO_TICKS(100));
//             ESP.restart();
//         }

//         lastState = currentState;

//         vTaskDelay(pdMS_TO_TICKS(50));
//     }
// }

// #endif


#ifndef __MONITOR_MODE__
#define __MONITOR_MODE__

#include <Arduino.h>

// Getter only — no extern variable
bool getModePinState();

void modeManager(void *pvParameters);

#endif
