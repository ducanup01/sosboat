#include <Arduino.h>
#include "motor_control.h"
#include "monitor_i2c.h"
#include "monitor_sd.h"
#include "monitor_lora.h"
#include "led.h"
// #include "monitor_lora_selftest.h"



void setup()
{
  Serial.begin(115200);
  vTaskDelay(pdMS_TO_TICKS(3000));
  Serial.println("Hello World!");

  
  // xTaskCreate(monitor_OTA, "Monitor OTA", 4096, NULL, 3, NULL);
  // while (WiFi.status() != WL_CONNECTED)
  // {
  //   vTaskDelay(pdMS_TO_TICKS(2000));
  //   Serial.println("Reattempting to connect to WiFi");


  // xTaskCreate(monitor_bluetooth, "Monitor BT", 4096, NULL, 2, NULL);
  // xTaskCreate(yaw_PID, "Straight YAW", 4096, NULL, 2, NULL);
  
  xTaskCreate(led_control, "LED", 1024, NULL, 2, NULL);
  // xTaskCreate(monitor_i2c, "Monitor IMU", 4096, NULL, 2, NULL);
  // xTaskCreate(motor_control, "Motor control", 2048, NULL, 2, NULL);
  // vTaskDelay(pdMS_TO_TICKS(5000));
  // xTaskCreate(monitor_lora, "LoRA", 2048, NULL, 2, NULL);
  // xTaskCreate(monitor_sd, "SD card", 8192, NULL, 2, NULL);
}


void loop(){}

