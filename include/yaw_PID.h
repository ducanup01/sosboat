#ifndef __YAW_PID__
#define __YAW_PID__

#include <Arduino.h>
#include "motor_control.h"
#include "monitor_OTA.h"
#include "monitor_imu.h"

#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BNO055.h>
#include <WiFi.h>

extern Adafruit_BNO055 bno;

// extern WiFiClient client;

float Kp = 2.0;
float Ki = 0.0; 
float Kd = 1.5; 

bool isRunning = false;

unsigned long lastTime = 0;
double integral = 0;
float lastError = 0;

int baseSpeed = 180;

float targetYaw = 0;


float normalizeYaw(float yaw)
{
    if (yaw < 0) yaw += 360;
    if (yaw >= 360) yaw -= 360;
    return yaw;
}

float yawError(float target, float current)
{
    float error = target - current;
    if (error > 180) error -= 360;
    if (error < -180) error += 360;
    return error;
}

int constrainMotorSpeed(int value, int minAbs, int maxVal) {
    // If within deadzone range, stop the motor
    if (abs(value) < minAbs) return 0;
    
    // Constrain to range -100 to 100
    value = constrain(value, -maxVal, maxVal);
    
    // Apply offset: +155 for positive, -155 for negative
    if (value > 0) {
        return value + 155;
    } else if (value < 0) {
        return value - 155;
    }
    return 0; // In case value is exactly 0 after constrain
}

int computeBaseSpeed(float error)
{
    float absError = fabs(error);

    if (absError >= 80.0)
        return 0;

    int speed = 100 * (1.0 - absError / 80.0);


    return constrain(abs(speed), 0, 100);
}

void yaw_PID(void *pvParameters)
{
    
    Serial.println("Yaw PID control started");
    initialize_motors();
    
    vTaskDelay(pdMS_TO_TICKS(1000));
    
    // Serial.println("Setting up BNO055...");
    setup_imu();
    sensors_event_t acc, gyro, mag, ori;
    
    
    
    while (1)    
    {
        if (isRunning)
        {
            bno.getEvent(&acc, Adafruit_BNO055::VECTOR_LINEARACCEL);
            bno.getEvent(&gyro, Adafruit_BNO055::VECTOR_GYROSCOPE);
            bno.getEvent(&mag, Adafruit_BNO055::VECTOR_MAGNETOMETER);
            bno.getEvent(&ori, Adafruit_BNO055::VECTOR_EULER);

            // float yaw = ori.orientation.x;
            float currentYaw = ori.orientation.x;
            
            float pitch = ori.orientation.y;
            float roll = ori.orientation.z;


            // float currentYaw = normalizeYaw(yaw);
            float error = yawError(targetYaw, currentYaw);

            unsigned long now = micros();
            float dt = (now - lastTime)/1000;
            lastTime = now;

            integral += error * dt;
            integral = integral/100;
            float derivative = (error - lastError)/dt;
            lastError = error;

            float output = Kp*error + Ki*integral + Kd*derivative;

            baseSpeed = computeBaseSpeed(error);

            int leftSpeed = constrainMotorSpeed(baseSpeed + output, 15, 100);
            int rightSpeed = constrainMotorSpeed(baseSpeed - output, 15, 100);

            motorControl(leftSpeed, rightSpeed);

            Serial.printf(
                "Yaw: %7.2f | Target: %7.2f | Error: %6.2f | Int: %6.2f | Der: %6.2f | Out: %6.2f |"
                "L:%3d R:%3d | Kp: %5.2f Ki: %5.2f Kd: %5.2f\n\r",
                currentYaw, targetYaw, error, integral, derivative, output,
                leftSpeed, rightSpeed, Kp, Ki, Kd
            );

            if (client && client.connected())
            {
                client.printf(
                    "Yaw: %7.2f | Target: %7.2f | Error: %6.2f | Int: %6.2f | Der: %6.2f | Out: %6.2f |"
                    "L:%3d R:%3d | Kp: %5.2f Ki: %5.2f Kd: %5.2f\n\r",
                    currentYaw, targetYaw, error, integral, derivative, output,
                    leftSpeed, rightSpeed, Kp, Ki, Kd
                );
            }
        }
        else
        {
            stopMotors();
        }
        vTaskDelay(pdMS_TO_TICKS(50));
    }


}

void handleCommand(String input)
{
    input.trim();

    if (input.startsWith("kp="))
    {
        Kp = input.substring(3).toFloat();
        Serial.printf("🔧 Updated Kp = %.4f\n", Kp);
        if (client && client.connected())
        {
            client.printf("🔧 Updated Kp = %.4f\n", Kp);
        }
    }
    else if (input.startsWith("ki="))
    {
        Ki = input.substring(3).toFloat();
        Serial.printf("🔧 Updated Ki = %.4f\n", Ki);
        if (client && client.connected())
        {
            client.printf("🔧 Updated Ki = %.4f\n", Ki);
        }
    }
    else if (input.startsWith("kd="))
    {
        Kd = input.substring(3).toFloat();
        Serial.printf("🔧 Updated Kd = %.4f\n", Kd);
        if (client && client.connected())
        {
            client.printf("🔧 Updated Kd = %.4f\n", Kd);
        }
    }
    else if (input.equalsIgnoreCase("go"))
    {
        sensors_event_t ori;
        bno.getEvent(&ori, Adafruit_BNO055::VECTOR_EULER);
        targetYaw = normalizeYaw(ori.orientation.x);
        lastError = 0;
        integral = 0;
        lastTime = millis();

        isRunning = true;
        Serial.printf("🚤 Going straight! Target yaw: %.2f°\n", targetYaw);
        if (client && client.connected())
            client.printf("🚤 Going straight! Target yaw: %.2f°\n", targetYaw);
    }
    else if (input.equalsIgnoreCase("stop"))
    {
        isRunning = false;
        stopMotors();
        Serial.println("🛑 Boat stopped.");
        if (client && client.connected())
            client.println("🛑 Boat stopped.");
    }
    else if (input.equalsIgnoreCase("left"))
    {
        targetYaw -= 90;
        if (targetYaw < 0) targetYaw += 360;
        Serial.printf("⬅️ Turn left! Target yaw: %.2f°\n", targetYaw);
        if (client && client.connected())
            client.printf("⬅️ Turn left! Target yaw: %.2f°\n", targetYaw);
    }
    else if (input.equalsIgnoreCase("right"))
    {
        targetYaw += 90;
        if (targetYaw > 360) targetYaw -= 360;
        Serial.printf("➡️ Turn right! Target yaw: %.2f°\n", targetYaw);
        if (client && client.connected())
            client.printf("➡️ Turn right! Target yaw: %.2f°\n", targetYaw);
    }
}

#endif  