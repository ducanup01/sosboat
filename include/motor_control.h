#ifndef __MOTOR_CONTROL__
#define __MOTOR_CONTROL__

#include <Arduino.h>

#define motor_left_fw GPIO_NUM_32
#define motor_left_bw GPIO_NUM_33
#define motor_right_fw GPIO_NUM_25
#define motor_right_bw GPIO_NUM_27
#define motor_pump_in GPIO_NUM_4
#define motor_pump_out GPIO_NUM_2

#define pwm_channel_left_fw 0
#define pwm_channel_left_bw 1
#define pwm_channel_right_fw 2
#define pwm_channel_right_bw 3

#define pwm_frequency 2000
#define pwm_resolution 8
#define max_duty 255

int motorSpeed = 0;

void stopMotors()
{
  ledcWrite(pwm_channel_left_fw, 0);
  ledcWrite(pwm_channel_left_bw, 0);
  ledcWrite(pwm_channel_right_fw, 0);
  ledcWrite(pwm_channel_right_bw, 0);
}

void motorControl(int leftSpeed, int rightSpeed)
{
    leftSpeed = constrain(leftSpeed, -max_duty, max_duty);
    rightSpeed = constrain(rightSpeed, -max_duty, max_duty);

    if (leftSpeed >= 0)
    {
        ledcWrite(pwm_channel_left_fw, leftSpeed);
        ledcWrite(pwm_channel_left_bw, 0);
    }
    else
    {
        ledcWrite(pwm_channel_left_fw, 0);
        ledcWrite(pwm_channel_left_bw, -leftSpeed);
    }



    if (rightSpeed >= 0)
    {
        ledcWrite(pwm_channel_right_fw, rightSpeed);
        ledcWrite(pwm_channel_right_bw, 0);
    }
    else
    {
        ledcWrite(pwm_channel_right_fw, 0);
        ledcWrite(pwm_channel_right_bw, -rightSpeed);
    }
}

void initialize_motors()
{
    ledcSetup(pwm_channel_left_fw, pwm_frequency, pwm_resolution);
    ledcSetup(pwm_channel_left_bw, pwm_frequency, pwm_resolution);
    ledcSetup(pwm_channel_right_fw, pwm_frequency, pwm_resolution);
    ledcSetup(pwm_channel_right_bw, pwm_frequency, pwm_resolution);

    ledcAttachPin(motor_left_fw, pwm_channel_left_fw);
    ledcAttachPin(motor_left_bw, pwm_channel_left_bw);
    ledcAttachPin(motor_right_fw, pwm_channel_right_fw);
    ledcAttachPin(motor_right_bw, pwm_channel_right_bw);

    pinMode(motor_pump_in, OUTPUT);
    pinMode(motor_pump_out, OUTPUT);
}

void float_submarine()
{
    gpio_set_level(motor_pump_in, 0);
    gpio_set_level(motor_pump_out, 1);
}

void sink_submarine()
{
    gpio_set_level(motor_pump_in, 1);
    gpio_set_level(motor_pump_out, 0);
}


void motor_control(void *pvParameters)
{
    Serial.begin(115200);
    // pinMode(motor_left_fw, OUTPUT);
    // pinMode(motor_left_bw, OUTPUT);
    // pinMode(motor_right_fw, OUTPUT);
    // pinMode(motor_right_bw, OUTPUT);

    initialize_motors();

    Serial.println("Enter motor speed (-255 to 255):");
    Serial.println("Positive = forward, Negative = backward, 0 = stop");

    sink_submarine();
    unsigned long startTime = millis();
    Serial.println("Timer started! Press BOOT button to stop.");

    while (1)
    {
        // Button pressed (GPIO_0 goes LOW when pressed)
        if (digitalRead(GPIO_NUM_0) == LOW)
        {
            unsigned long elapsed = millis() - startTime;
            Serial.printf("Elapsed time: %lu ms (%.2f seconds)\n", elapsed, elapsed / 1000.0);

            // small debounce delay
            vTaskDelay(pdMS_TO_TICKS(300));

            // 5️⃣ Delete this task
            gpio_set_level(motor_pump_out, 0);
            gpio_set_level(motor_pump_in, 0);
        }

        vTaskDelay(pdMS_TO_TICKS(50)); // check every 50 ms

        // vTaskDelay(pdMS_TO_TICKS(3000));

        // sink_submarine();
        // vTaskDelay(pdMS_TO_TICKS(3000));

        // // LEFT motor forward
        // Serial.println("Left motor: FORWARD");
        // motorControl(255, 0);  // left forward
        // vTaskDelay(pdMS_TO_TICKS(1000));

        // // LEFT motor stop
        // Serial.println("Left motor: STOP");
        // stopMotors();
        // vTaskDelay(pdMS_TO_TICKS(1000));

        // // LEFT motor backward
        // Serial.println("Left motor: BACKWARD");
        // motorControl(-255, 0); // left backward
        // vTaskDelay(pdMS_TO_TICKS(1000));

        // // LEFT motor stop
        // Serial.println("Left motor: STOP");
        // stopMotors();
        // vTaskDelay(pdMS_TO_TICKS(1000));

        // // RIGHT motor forward
        // Serial.println("Right motor: FORWARD");
        // motorControl(0, 255);  // right forward
        // vTaskDelay(pdMS_TO_TICKS(1000));

        // // RIGHT motor stop
        // Serial.println("Right motor: STOP");
        // stopMotors();
        // vTaskDelay(pdMS_TO_TICKS(1000));

        // // RIGHT motor backward
        // Serial.println("Right motor: BACKWARD");
        // motorControl(0, -255); // right backward
        // vTaskDelay(pdMS_TO_TICKS(1000));

        // // RIGHT motor stop
        // Serial.println("Right motor: STOP");
        // stopMotors();
        // vTaskDelay(pdMS_TO_TICKS(1000));

        // if (Serial.available() > 0)
        // {
        //     String input = Serial.readStringUntil('\n');
        //     input.trim();

        //     if (input.length() > 0)
        //     {
        //         int speedValue = input.toInt();

        //         speedValue = constrain(speedValue, -max_duty, max_duty);

        //         if (speedValue == 0)
        //         {
        //             stopMotors();
        //             Serial.println("Motors stopped!");
        //         }
        //         else
        //         {
        //             motorControl(speedValue, speedValue);
        //             Serial.printf("Motors running at %d (PWM duty)\n", speedValue);
        //         }
        //     }
        // }
        vTaskDelay(pdMS_TO_TICKS(50));
    }
}


#endif