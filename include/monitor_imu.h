#ifndef __MONITOR_IMU__
#define __MONITOR_IMU__

#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BNO055.h>
#include <WiFi.h>

#define I2C_CLOCK 400000
#define BNO055_ADDRESS 0x28


Adafruit_BNO055 bno = Adafruit_BNO055(55, BNO055_ADDRESS);

void monitor_imu(void *pvParameters)
{
    Serial.begin(115200);

    pinMode(21, INPUT_PULLUP);
    pinMode(22, INPUT_PULLUP);
    Wire.begin(21, 22, I2C_CLOCK);
    vTaskDelay(pdMS_TO_TICKS(1000));

    Serial.println("Initializing BNO055...");

    if (!bno.begin()) 
    {
        Serial.println("❌ Failed to find BNO055! Check wiring or address.");
        while (1);
    }

    bno.setExtCrystalUse(true);
    Serial.println("BNO055 connected! ✅");

    while (1)
    {
        sensors_event_t ori, acc, gyro, mag;

        bno.getEvent(&ori, Adafruit_BNO055::VECTOR_EULER);
        bno.getEvent(&acc, Adafruit_BNO055::VECTOR_LINEARACCEL);
        bno.getEvent(&gyro, Adafruit_BNO055::VECTOR_GYROSCOPE);
        bno.getEvent(&mag, Adafruit_BNO055::VECTOR_MAGNETOMETER);
        
        float yaw = ori.orientation.x;
        float pitch = ori.orientation.y;
        float roll = ori.orientation.z;

        Serial.printf("Yaw: %8.3f | Pitch: %8.3f | Roll: %8.3f\n\r", yaw, pitch, roll);

        if (client && client.connected())
        {
            client.printf("Yaw: %8.3f | Pitch: %8.3f | Roll: %8.3f\n\r", yaw, pitch, roll);
        }


        vTaskDelay(pdMS_TO_TICKS(300));
    }

}

#endif