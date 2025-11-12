#ifndef __LED__
#define __LED__

#include <Arduino.h>

#define LED_PIN GPIO_NUM_17
#define LORA_NSS GPIO_NUM_15
#define SD_CS GPIO_NUM_5

void led_control(void *pvParameters)
{
    pinMode(LED_PIN, OUTPUT);
    pinMode(LORA_NSS, OUTPUT);
    pinMode(SD_CS, OUTPUT);

    while (1)
    {
        gpio_set_level(LED_PIN, 1);
        gpio_set_level(LORA_NSS, 1);
        gpio_set_level(SD_CS, 1);
        vTaskDelay(pdMS_TO_TICKS(1000));

        gpio_set_level(LED_PIN, 0);
        gpio_set_level(LORA_NSS, 0);
        gpio_set_level(SD_CS, 0);
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}


#endif