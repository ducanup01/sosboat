#ifndef __GLOBAL__
#define __GLOBAL__

#include <Adafruit_Sensor.h>
#include <Adafruit_BNO055.h>
#include <Adafruit_BMP280.h>
#include <Arduino.h>

#define MODE_PIN GPIO_NUM_36
#define GREEN_LED GPIO_NUM_17

extern volatile float Kp;
extern volatile float Ki; 
extern volatile float Kd; 

// --- Global variables (declare only) ---
extern float yaw;
extern float pitch;
extern float roll;
extern float pressure;

extern float targetYaw;

extern int baseSpeed;

// Motor state
enum PumpCommand {
    PUMP_IDLE,
    PUMP_IN,
    PUMP_OUT,
    PUMP_STOP
};

extern volatile PumpCommand pumpCommand;


// Action flags
extern volatile bool pumpAbort;

void LoRaPrintln(const String &msg);
String LoRaRead();

#endif
