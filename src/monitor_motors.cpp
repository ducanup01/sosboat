#include "monitor_motors.h"
#include "global.h"


int max_pump_time = 50000;

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

    vTaskDelay(pdMS_TO_TICKS(20));
}

void pump_in()
{
    pumpAbort = false;  // reset


    while (!(gpio_get_level(switch_low) == LOW && // former low
             gpio_get_level(switch_high) == LOW)) // former low
    {
        
        digitalWrite(motor_pump_in, HIGH);
        digitalWrite(motor_pump_out, LOW);
        
        vTaskDelay(pdMS_TO_TICKS(20));
        if (pumpAbort) break;   // ONLY interrupts when sm/sf/sp send abort
    }

    digitalWrite(motor_pump_in, LOW);
    digitalWrite(motor_pump_out, LOW);
}

void pump_out()
{
    pumpAbort = false;  // reset

    unsigned long startTime = millis();

    digitalWrite(motor_pump_in, LOW);
    digitalWrite(motor_pump_out, HIGH);

    while (millis() - startTime < max_pump_time)
    {
        vTaskDelay(pdMS_TO_TICKS(20));
        if (pumpAbort) break;   // ONLY interrupts when sm/sf/sp send abort
    }

    digitalWrite(motor_pump_in, LOW);
    digitalWrite(motor_pump_out, LOW);
}

// ---------- Main control task ----------
void monitor_motors(void *pvParameters)
{
    initialize_motors();

    while (1)
    {
        switch (pumpCommand)
        {
            case PUMP_IN:
                pumpAbort = false;
                pump_in();       // runs until abort or condition
                pumpCommand = PUMP_IDLE;
                break;

            case PUMP_OUT:
                pumpAbort = false;
                pump_out();      // runs until abort or done
                pumpCommand = PUMP_IDLE;
                break;

            case PUMP_STOP:
                pumpAbort = true;
                pump_stop();     // instantly stop
                pumpCommand = PUMP_IDLE;
                break;

            case PUMP_IDLE:
            default:
                // Nothing to do
                break;
        }
        vTaskDelay(pdMS_TO_TICKS(100)); //former 10
    }
}
