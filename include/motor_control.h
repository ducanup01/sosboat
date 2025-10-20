#ifndef __MOTOR_CONTROL__
#define __MOTOR_CONTROL__

#include <Arduino.h>

#define motor_left_fw GPIO_NUM_16
#define motor_left_bw GPIO_NUM_17
#define motor_right_fw GPIO_NUM_19
#define motor_right_bw GPIO_NUM_18

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

void motor_control(void *pvParameters)
{
    Serial.begin(115200);
    // pinMode(motor_left_fw, OUTPUT);
    // pinMode(motor_left_bw, OUTPUT);
    // pinMode(motor_right_fw, OUTPUT);
    // pinMode(motor_right_bw, OUTPUT);

    ledcSetup(pwm_channel_left_fw, pwm_frequency, pwm_resolution);
    ledcSetup(pwm_channel_left_bw, pwm_frequency, pwm_resolution);
    ledcSetup(pwm_channel_right_fw, pwm_frequency, pwm_resolution);
    ledcSetup(pwm_channel_right_bw, pwm_frequency, pwm_resolution);

    ledcAttachPin(motor_left_fw, pwm_channel_left_fw);
    ledcAttachPin(motor_left_bw, pwm_channel_left_bw);
    ledcAttachPin(motor_right_fw, pwm_channel_right_fw);
    ledcAttachPin(motor_right_bw, pwm_channel_right_bw);

    Serial.println("Enter motor speed (-255 to 255):");
    Serial.println("Positive = forward, Negative = backward, 0 = stop");


    while (1)
    {
        // gpio_set_level(motor_left_fw, 1);
        // gpio_set_level(motor_left_bw, 0);
        // vTaskDelay(pdMS_TO_TICKS(1000));

        // gpio_set_level(motor_right_fw, 0);
        // gpio_set_level(motor_right_bw, 1);
        // vTaskDelay(pdMS_TO_TICKS(1000));

        if (Serial.available() > 0)
        {
            String input = Serial.readStringUntil('\n');
            input.trim();

            if (input.length() > 0)
            {
                int speedValue = input.toInt();

                speedValue = constrain(speedValue, -max_duty, max_duty);

                if (speedValue == 0)
                {
                    stopMotors();
                    Serial.println("Motors stopped!");
                }
                else
                {
                    motorControl(speedValue, speedValue);
                    Serial.printf("Motors running at %d (PWM duty)\n", speedValue);
                }
            }
        }
        vTaskDelay(pdMS_TO_TICKS(50));
    }
}


#endif