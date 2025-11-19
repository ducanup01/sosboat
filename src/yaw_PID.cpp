#include "yaw_PID.h"

// ----- PID global variables -----
bool isRunning = false;
unsigned long lastTime = 0;
double integral = 0;
float lastError = 0;

// ----- Helper functions -----
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

// void sendPIDLoRa(
//     float currentYaw, float targetYaw, float error, float integral, float derivative, float output,
//     int leftSpeed, int rightSpeed, float Kp, float Ki, float Kd
// ) {
//     String msg = "Yaw: " + String(currentYaw, 2) +
//                  " | Target: " + String(targetYaw, 2) +
//                  " | Error: " + String(error, 2) +
//                  " | Int: " + String(integral, 2) +
//                  " | Der: " + String(derivative, 2) +
//                  " | Out: " + String(output, 2) +
//                  " | L:" + String(leftSpeed) +
//                  " R:" + String(rightSpeed) +
//                  " | Kp: " + String(Kp, 2) +
//                  " Ki: " + String(Ki, 2) +
//                  " Kd: " + String(Kd, 2);

//     LoRaPrintln(msg); // send via LoRa
// }

int constrainMotorSpeed(int value, int minAbs, int maxVal) {
    if (abs(value) < minAbs) return 0;
    value = constrain(value, -maxVal, maxVal);
    if (value > 0) return value + 155;
    if (value < 0) return value - 155;
    return 0;
}

int computeBaseSpeed(float error)
{
    float absError = fabs(error);
    if (absError >= 80.0) return 0;
    int speed = 100 * (1.0 - absError / 80.0);
    return constrain(abs(speed), 0, 100);
}

// ----- Main PID task -----
void yaw_PID(void *pvParameters)
{
    Serial.println("Yaw PID control started");
    initialize_motors();
    
    vTaskDelay(pdMS_TO_TICKS(1000));
    
    setup_imu();
    sensors_event_t acc, gyro, mag, ori;
    
    while (1)    
    {
        if (isRunning)
        {
            float currentYaw = yaw;
            float error = yawError(targetYaw, currentYaw);

            unsigned long now = micros();
            float dt = (now - lastTime)/1000.0;
            lastTime = now;

            integral += error * dt;
            integral = integral / 100.0;
            float derivative = (error - lastError)/dt;
            lastError = error;

            float output = Kp*error + Ki*integral + Kd*derivative;

            baseSpeed = computeBaseSpeed(error);

            int leftSpeed = constrainMotorSpeed(baseSpeed + output, 15, 100);
            int rightSpeed = constrainMotorSpeed(baseSpeed - output, 15, 100);

            motorControl(leftSpeed, rightSpeed);

            // sendPIDLoRa(currentYaw, targetYaw, error, integral, derivative, output,
            //             leftSpeed, rightSpeed, Kp, Ki, Kd);
        }
        else
        {
            stopMotors();
        }
        vTaskDelay(pdMS_TO_TICKS(60));
    }
}
