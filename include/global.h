#ifndef __GLOBAL__
#define __GLOBAL__

#include <Adafruit_Sensor.h>
#include <Adafruit_BNO055.h>
#include <Adafruit_BMP280.h>

Adafruit_BNO055 bno;
Adafruit_BMP280 bmp;

float yaw;
float pitch;
float roll;
float pressure;

#endif