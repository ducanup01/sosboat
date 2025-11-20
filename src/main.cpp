#include <Arduino.h>
#include <LoRa.h>
// #include "motor_control.h"
#include "monitor_i2c.h"
// #include "monitor_sd.h"
#include "monitor_lora.h"
#include "monitor_OTA.h"
#include "led.h"
#include "input_pin.h"
#include "global.h"
#include "monitor_motors.h"
#include "yaw_PID.h"


#include "monitor_mode.h"


void setup()
{
    
    Serial.begin(115200);
    vTaskDelay(pdMS_TO_TICKS(3000));
    Serial.println("Hello World!");
    
    pinMode(MODE_PIN, INPUT);
    pinMode(GREEN_LED, OUTPUT);

    if (gpio_get_level(MODE_PIN) == HIGH) // when button is pressed, do not change code below
    {
        /***************************************************************************
         * ████████████████████████████████████████████████████████████████████████ *
         * █   ⚠⚠⚠  DO NOT CHANGE ANYTHING IN THIS BLOCK — OTA SYSTEM CODE  ⚠⚠   █ *
         * █   This code is critical for boot/update mode.                          █
         * █   Modifying it may brick OTA, WiFi setup, or boot behavior.            █
         * ████████████████████████████████████████████████████████████████████████ *
         ***************************************************************************/
        xTaskCreatePinnedToCore(monitor_OTA, "Monitor OTA", 4096, NULL, 3, NULL, APP_CPU_NUM);
        /***************************************************************************
         * ████████████████████████████████████████████████████████████████████████ *
         ***************************************************************************/
    }
    else if (gpio_get_level(MODE_PIN) == LOW) // when button is released
    {
        // ============================ USER TASKS BEGIN ============================
        
        xTaskCreate(monitor_i2c, "Monitor IMU", 8192, NULL, 2, NULL);
        
        xTaskCreate(monitor_lora, "LoRA", 8192, NULL, 2, NULL);
        
        xTaskCreate(monitor_motors, "Monitor motors", 4096, NULL, 2, NULL);
        
        xTaskCreate(yaw_PID, "Straight YAW", 8192, NULL, 2, NULL);

        // ============================ USER TASKS END   ============================
    }

    /***************************************************************************
     * ████████████████████████████████████████████████████████████████████████ *
     * █   ⚠⚠⚠  DO NOT CHANGE THIS FUNCTION — SYSTEM BUTTON MONITOR  ⚠⚠⚠   █ *
     * ████████████████████████████████████████████████████████████████████████ *
     ***************************************************************************/
    xTaskCreate(modeManager, "Monitor Green button", 2048, NULL, 2, NULL);
    /***************************************************************************
     * ████████████████████████████████████████████████████████████████████████ *
     ***************************************************************************/
    
    
    
    
    // xTaskCreate(led_control, "LED", 1024, NULL, 2, NULL);
    // xTaskCreate(monitor_input, "INPUT PINS", 2048, NULL, 2, NULL);
    // xTaskCreate(motor_control, "Motor control", 2048, NULL, 2, NULL);


    // vTaskDelay(pdMS_TO_TICKS(5000));
    // xTaskCreate(monitor_lora, "LoRA", 2048, NULL, 2, NULL);
    // xTaskCreate(monitor_sd, "SD card", 8192, NULL, 2, NULL);
}


void loop(){}

