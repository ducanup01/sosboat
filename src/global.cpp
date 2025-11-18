#include "global.h"

// Define sensor objects
Adafruit_BNO055 bno = Adafruit_BNO055();
Adafruit_BMP280 bmp;

// Define variables
float yaw = 0;
float pitch = 0;
float roll = 0;
float pressure = 0;
