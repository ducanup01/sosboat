#include <Wire.h>
#include <Arduino.h>

#define MOTOR_LEFT_FORWARD GPIO_NUM_16
#define MOTOR_LEFT_BACKWARDS GPIO_NUM_17
#define MOTOR_RIGHT_FORWARD GPIO_NUM_19
#define MOTOR_RIGHT_BACKWARDS GPIO_NUM_18

void setup()
{
  Serial.begin(115200);
  pinMode(MOTOR_LEFT_FORWARD, OUTPUT);
  pinMode(MOTOR_LEFT_BACKWARDS, OUTPUT);
  pinMode(MOTOR_RIGHT_FORWARD, OUTPUT);
  pinMode(MOTOR_RIGHT_BACKWARDS, OUTPUT);

  Serial.println("Motor test starting...");
}

void loop()
{
  // 1. Left motor forward
  gpio_set_level(MOTOR_LEFT_FORWARD, 1);
  gpio_set_level(MOTOR_LEFT_BACKWARDS, 0);
  Serial.println("Left motor forward");
  vTaskDelay(2000);

  // Stop left motor
  gpio_set_level(MOTOR_LEFT_FORWARD, 0);
  gpio_set_level(MOTOR_LEFT_BACKWARDS, 0);
  Serial.println("Left motor stop");
  vTaskDelay(2000);

  // 2. Right motor forward
  gpio_set_level(MOTOR_RIGHT_FORWARD, 1);
  gpio_set_level(MOTOR_RIGHT_BACKWARDS, 0);
  Serial.println("Right motor forward");
  vTaskDelay(2000);

  // Stop right motor
  gpio_set_level(MOTOR_RIGHT_FORWARD, 0);
  gpio_set_level(MOTOR_RIGHT_BACKWARDS, 0);
  Serial.println("Right motor stop");
  vTaskDelay(2000);

  // 3. Left motor backward
  gpio_set_level(MOTOR_LEFT_FORWARD, 0);
  gpio_set_level(MOTOR_LEFT_BACKWARDS, 1);
  Serial.println("Left motor backward");
  vTaskDelay(2000);

  // Stop left motor
  gpio_set_level(MOTOR_LEFT_FORWARD, 0);
  gpio_set_level(MOTOR_LEFT_BACKWARDS, 0);
  Serial.println("Left motor stop");
  vTaskDelay(2000);

  // 4. Right motor backward
  gpio_set_level(MOTOR_RIGHT_FORWARD, 0);
  gpio_set_level(MOTOR_RIGHT_BACKWARDS, 1);
  Serial.println("Right motor backward");
  vTaskDelay(2000);

  // Stop right motor
  gpio_set_level(MOTOR_RIGHT_FORWARD, 0);
  gpio_set_level(MOTOR_RIGHT_BACKWARDS, 0);
  Serial.println("Right motor stop");
  vTaskDelay(2000);

  // Optional: loop indicator
  Serial.println("Cycle complete, restarting...");
}
