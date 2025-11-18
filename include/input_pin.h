#ifndef __INPUT_PIN__
#define __INPUT_PIN__

#include <Arduino.h>

#define PIN_36 GPIO_NUM_36
#define PIN_39 GPIO_NUM_39
#define PIN_34 GPIO_NUM_34
#define PIN_35 GPIO_NUM_35

void monitor_input(void *pvParameters)
{
    pinMode(PIN_36, INPUT);
    pinMode(PIN_39, INPUT);
    pinMode(PIN_34, INPUT);
    pinMode(PIN_35, INPUT);

    Serial.println("Monitoring pins 36, 39, 34, 35...");

    while (1)
    {
        int val36 = gpio_get_level(PIN_36);
        int val39 = gpio_get_level(PIN_39);
        int val34 = gpio_get_level(PIN_34);
        int val35 = gpio_get_level(PIN_35);

        Serial.printf("36:%d | 39:%d | 34:%d | 35:%d\n", val36, val39, val34, val35);

        vTaskDelay(pdMS_TO_TICKS(500)); // delay 500 ms
    }
}

#endif
