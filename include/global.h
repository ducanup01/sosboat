#ifndef __GLOBAL__
#define __GLOBAL__

#include <Adafruit_Sensor.h>
#include <Adafruit_BNO055.h>
#include <Adafruit_BMP280.h>
#include <Arduino.h>

#define MODE_PIN GPIO_NUM_36
#define GREEN_LED GPIO_NUM_17

// --- Sensor objects (declare only) ---
extern Adafruit_BNO055 bno;
extern Adafruit_BMP280 bmp;

// --- Global variables (declare only) ---
extern float yaw;
extern float pitch;
extern float roll;
extern float pressure;

#endif
