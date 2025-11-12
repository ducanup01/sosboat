#ifndef __MONITOR_I2C__
#define __MONITOR_I2C__

#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BNO055.h>
#include <Adafruit_BMP280.h>
#include "global.h"

#define I2C_CLOCK 100000
// #define BNO055_ADDRESS 0x28
// #define BMP280_ADDRESS 0x76


// Adafruit_BNO055 bno = Adafruit_BNO055(55, BNO055_ADDRESS);
extern Adafruit_BNO055 bno;
// extern Adafruit_BMP280 bmp;

extern float yaw;
extern float pitch;
extern float roll;
extern float pressure;

void setup_i2c()
{
    Serial.begin(115200);

    pinMode(21, INPUT_PULLUP);
    pinMode(22, INPUT_PULLUP);
    Wire.begin(21, 22, I2C_CLOCK);
}

void setup_imu()
{
    Serial.println("Initializing BNO055...");
    vTaskDelay(pdMS_TO_TICKS(1000));
    // bno.begin();
    
    while (!bno.begin()) 
    {
        Serial.println("❌ Failed to find BNO055! Check wiring or address.");
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
    
    bno.setExtCrystalUse(true);
    Serial.println("BNO055 connected! ✅");

}

void setup_bmp()
{
    while (!bmp.begin())
    {
        Serial.println("❌ Could not find BMP280 sensor! Check wiring or address.");
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
    Serial.println("BMP280 connected! ✅");
}
void monitor_i2c(void *pvParameters)
{
    setup_i2c();
    setup_imu();
    // setup_bmp();
    vTaskDelay(pdMS_TO_TICKS(500));
    sensors_event_t acc, gyro, mag, ori;

    while (1)
    {

        bno.getEvent(&acc, Adafruit_BNO055::VECTOR_LINEARACCEL);
        bno.getEvent(&gyro, Adafruit_BNO055::VECTOR_GYROSCOPE);
        bno.getEvent(&mag, Adafruit_BNO055::VECTOR_MAGNETOMETER);
        bno.getEvent(&ori, Adafruit_BNO055::VECTOR_EULER);
        
        yaw = ori.orientation.x;
        pitch = ori.orientation.y;
        roll = ori.orientation.z;

        // pressure = bmp.readPressure() / 100.0F;

        Serial.printf("Yaw: %8.3f | Pitch: %8.3f | Roll: %8.3f\n\r", yaw, pitch, roll);

        // if (client && client.connected())
        // {
        //     client.printf("Yaw: %8.3f | Pitch: %8.3f | Roll: %8.3f\n\r", yaw, pitch, roll);
        // }


        vTaskDelay(pdMS_TO_TICKS(300));
    }

}

#endif