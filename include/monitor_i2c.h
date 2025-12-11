#ifndef __MONITOR_I2C__
#define __MONITOR_I2C__

#include <Wire.h>
#include <Arduino.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BNO055.h>
#include <Adafruit_BMP280.h>
#include "global.h"

// I2C clock speed
#define I2C_CLOCK 100000

// --- Sensor objects (declare only) ---
extern Adafruit_BNO055 bno;
extern Adafruit_BMP280 bmp;


// Function declarations
void setup_i2c();
void setup_imu();
void setup_bmp();
void monitor_i2c(void *pvParameters);

#endif
