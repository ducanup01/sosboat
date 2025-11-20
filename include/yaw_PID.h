#ifndef __YAW_PID_H__
#define __YAW_PID_H__

#include <Arduino.h>
#include "monitor_motors.h"
#include "global.h"
#include "monitor_i2c.h"

#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BNO055.h>

// ----- External BNO instance -----
extern Adafruit_BNO055 bno;

// ----- PID global variables -----
extern bool isRunning;
extern unsigned long lastTime;
extern double integral;
extern float lastError;

// ----- Function declarations -----
float normalizeYaw(float yaw);
float yawError(float target, float current);

void sendPIDLoRa(
    float currentYaw, float targetYaw, float error, float integral, float derivative, float output,
    int leftSpeed, int rightSpeed, float Kp, float Ki, float Kd
);

int constrainMotorSpeed(int value, int minAbs, int maxVal);
int computeBaseSpeed(float error);

void yaw_PID(void *pvParameters);

#endif
