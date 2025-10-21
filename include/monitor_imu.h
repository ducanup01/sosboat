#ifndef __MONITOR_IMU__
#define __MONITOR_IMU__

#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BNO055.h>
#include <WiFi.h>

#define I2C_CLOCK 100000
#define BNO055_ADDRESS 0x28


// Adafruit_BNO055 bno = Adafruit_BNO055(55, BNO055_ADDRESS);
extern Adafruit_BNO055 bno;
extern WiFiClient client;

void setup_imu()
{
    
    Serial.begin(115200);

    pinMode(21, INPUT_PULLUP);
    pinMode(22, INPUT_PULLUP);
    Wire.begin(21, 22, I2C_CLOCK);
    
    Serial.println("Initializing BNO055...");
    bno.begin();
    // vTaskDelay(pdMS_TO_TICKS(5000));
    
    while (!bno.begin()) 
    {
        Serial.println("❌ Failed to find BNO055! Check wiring or address.");
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
    
    bno.setExtCrystalUse(true);
    Serial.println("BNO055 connected! ✅");
    vTaskDelay(pdMS_TO_TICKS(1000));

}

void monitor_imu(void *pvParameters)
{
    setup_imu();
    sensors_event_t acc, gyro, mag, ori;

    while (1)
    {

        bno.getEvent(&acc, Adafruit_BNO055::VECTOR_LINEARACCEL);
        bno.getEvent(&gyro, Adafruit_BNO055::VECTOR_GYROSCOPE);
        bno.getEvent(&mag, Adafruit_BNO055::VECTOR_MAGNETOMETER);
        bno.getEvent(&ori, Adafruit_BNO055::VECTOR_EULER);
        
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