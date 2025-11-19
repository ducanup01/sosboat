#include "test_water.h"

// ---------- Global variable definitions ----------
int motorSpeed = 0;

// ---------- Motor Control ----------
void stopMotors()
{
    ledcWrite(pwm_channel_left_fw, 0);
    ledcWrite(pwm_channel_left_bw, 0);
    ledcWrite(pwm_channel_right_fw, 0);
    ledcWrite(pwm_channel_right_bw, 0);
}

void motorControl(int leftSpeed, int rightSpeed)
{
    leftSpeed  = constrain(leftSpeed,  -max_duty, max_duty);
    rightSpeed = constrain(rightSpeed, -max_duty, max_duty);

    if (leftSpeed >= 0) {
        ledcWrite(pwm_channel_left_fw, leftSpeed);
        ledcWrite(pwm_channel_left_bw, 0);
    } else {
        ledcWrite(pwm_channel_left_fw, 0);
        ledcWrite(pwm_channel_left_bw, -leftSpeed);
    }

    if (rightSpeed >= 0) {
        ledcWrite(pwm_channel_right_fw, rightSpeed);
        ledcWrite(pwm_channel_right_bw, 0);
    } else {
        ledcWrite(pwm_channel_right_fw, 0);
        ledcWrite(pwm_channel_right_bw, -rightSpeed);
    }
}

// ---------- Initialization ----------
void initialize_motors()
{
    ledcSetup(pwm_channel_left_fw,  pwm_frequency, pwm_resolution);
    ledcSetup(pwm_channel_left_bw,  pwm_frequency, pwm_resolution);
    ledcSetup(pwm_channel_right_fw, pwm_frequency, pwm_resolution);
    ledcSetup(pwm_channel_right_bw, pwm_frequency, pwm_resolution);

    ledcAttachPin(motor_left_fw,  pwm_channel_left_fw);
    ledcAttachPin(motor_left_bw,  pwm_channel_left_bw);
    ledcAttachPin(motor_right_fw, pwm_channel_right_fw);
    ledcAttachPin(motor_right_bw, pwm_channel_right_bw);

    pinMode(motor_pump_in,  OUTPUT);
    pinMode(motor_pump_out, OUTPUT);
    pinMode(switch_low,     INPUT);
    pinMode(switch_high,    INPUT);

    // Safety: pump off initially
    digitalWrite(motor_pump_in,  LOW);
    digitalWrite(motor_pump_out, LOW);
}

// ---------- Pump Control ----------
void pump_stop()
{
    digitalWrite(motor_pump_in,  LOW);
    digitalWrite(motor_pump_out, LOW);
    Serial.println("Pump OFF");
}

void pump_in()
{
    Serial.println("Pumping IN (Float)");
    while (!(gpio_get_level(switch_low) == HIGH && gpio_get_level(switch_high) == HIGH))
    {
        digitalWrite(motor_pump_in,  HIGH);
        digitalWrite(motor_pump_out, LOW);
        vTaskDelay(pdMS_TO_TICKS(20));
    }
    digitalWrite(motor_pump_in,  LOW);
    digitalWrite(motor_pump_out, LOW);
}

void pump_out()
{
    unsigned long startTime = millis();

    digitalWrite(motor_pump_in,  LOW);
    digitalWrite(motor_pump_out, HIGH);
    while (millis() - startTime < 45000) {
        delay(10);
    }
    digitalWrite(motor_pump_in, LOW);
    digitalWrite(motor_pump_out, LOW);
}

// ---------- Main control task ----------
void test_water(void *pvParameters)
{
    initialize_motors();

    while (1)
    {
        int lowSwitch  = gpio_get_level(switch_low);
        int highSwitch = gpio_get_level(switch_high);

        // Pump IN phase
        while (!(lowSwitch == HIGH && highSwitch == HIGH))
        {
            lowSwitch  = gpio_get_level(switch_low);
            highSwitch = gpio_get_level(switch_high);

            if (lowSwitch == HIGH && highSwitch == HIGH)
            {
                pump_stop();
                break;
            }

            pump_in();
            vTaskDelay(pdMS_TO_TICKS(50));
        }

        // Pump OUT phase
        if (lowSwitch == HIGH && highSwitch == HIGH)
        {
            pump_out();
            vTaskDelay(pdMS_TO_TICKS(45000));
            pump_stop();
            vTaskDelete(NULL);  // stop task
        }

        vTaskDelay(pdMS_TO_TICKS(100));
    }
}
