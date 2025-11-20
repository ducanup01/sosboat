#include "monitor_i2c.h"
#include "global.h"

// Define sensor objects
Adafruit_BNO055 bno = Adafruit_BNO055();
Adafruit_BMP280 bmp;

unsigned long lastPrint = 0;

void setup_i2c()
{
    pinMode(21, INPUT_PULLUP);
    pinMode(22, INPUT_PULLUP);

    Wire.begin(21, 22, I2C_CLOCK);
}

void setup_imu()
{
    LoRaPrintln("Initializing ...");
    vTaskDelay(pdMS_TO_TICKS(1000));

    while (!bno.begin()) 
    {
        LoRaPrintln("❌ Failed to find BNO055! Check wiring or address.");
        vTaskDelay(pdMS_TO_TICKS(1000));
    }

    bno.setExtCrystalUse(true);
    // LoRaPrintln("BNO055 connected! ✅");
}

void setup_bmp()
{
    while (!bmp.begin())
    {
        LoRaPrintln("❌ Could not find BMP280 sensor! Check wiring or address.");
        vTaskDelay(pdMS_TO_TICKS(1000));
    }

    // LoRaPrintln("BMP280 connected! ✅");
}

void monitor_i2c(void *pvParameters)
{
    setup_i2c();
    setup_imu();
    setup_bmp();
    LoRaPrintln("Submarine is active ✅");


    vTaskDelay(pdMS_TO_TICKS(500));

    sensors_event_t acc, gyro, mag, ori;

    while (1)
    {
        bno.getEvent(&acc, Adafruit_BNO055::VECTOR_LINEARACCEL);
        bno.getEvent(&gyro, Adafruit_BNO055::VECTOR_GYROSCOPE);
        bno.getEvent(&mag, Adafruit_BNO055::VECTOR_MAGNETOMETER);
        bno.getEvent(&ori, Adafruit_BNO055::VECTOR_EULER);

        yaw = ori.orientation.x;
        // pitch = ori.orientation.y;
        // roll = ori.orientation.z;

        pressure = bmp.readPressure() / 100.0F;

        if (millis() - lastPrint >= 1000)
        {
            lastPrint = millis();

            // // Format the IMU data
            String msg = 
                "Yaw: " + String(yaw, 3) + 
                // " | Pitch: " + String(pitch, 3) + 
                // " | Roll: " + String(roll, 3) +
                " | Pressure: " + String(pressure, 3);
    
            LoRaPrintln(msg);
        }

        vTaskDelay(pdMS_TO_TICKS(300));
    }
}
