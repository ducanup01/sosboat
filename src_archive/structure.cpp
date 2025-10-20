#include <Arduino.h>
// #include "monitor_OTA.h"
// #include "motor_control.h"
#include "monitor_bluetooth.h"

void setup()
{
  Serial.begin(115200);
  delay(3000);
  Serial.println("Hello World!");


  // xTaskCreate(monitor_OTA, "Monitor OTA", 2048, NULL, 3, NULL);
  xTaskCreate(monitor_bluetooth, "Monitor BT", 4096, NULL, 2, NULL);
  // xTaskCreate(motor_control, "Motor control", 1024, NULL, 2, NULL);
}


void loop(){}

