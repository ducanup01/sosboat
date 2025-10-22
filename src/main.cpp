#include <Arduino.h>
#include "monitor_OTA.h"
// #include "monitor_bluetooth.h"
#include "motor_control.h"
#include "monitor_imu.h"



void setup()
{
  // Serial.begin(115200);
  vTaskDelay(pdMS_TO_TICKS(3000));
  Serial.println("Hello World!");

  
  xTaskCreate(monitor_OTA, "Monitor OTA", 4096, NULL, 3, NULL);
  while (WiFi.status() != WL_CONNECTED)
  {
    vTaskDelay(pdMS_TO_TICKS(2000));
    Serial.println("Reattempting to connect to WiFi");
  }

  // xTaskCreate(monitor_bluetooth, "Monitor BT", 4096, NULL, 2, NULL);
  // xTaskCreate(motor_control, "Motor control", 2048, NULL, 2, NULL);
  // xTaskCreate(monitor_imu, "Monitor IMU", 4096, NULL, 2, NULL);
  xTaskCreate(yaw_PID, "Straight YAW", 4096, NULL, 2, NULL);
}


void loop(){}

