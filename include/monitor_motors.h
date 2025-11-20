#ifndef __MONITOR_MOTORS__
#define __MONITOR_MOTORS__

#include <Arduino.h>

// ---------- Pin Definitions ----------
#define motor_left_fw   GPIO_NUM_33
#define motor_left_bw   GPIO_NUM_32
#define motor_right_fw  GPIO_NUM_25
#define motor_right_bw  GPIO_NUM_27
#define motor_pump_in   GPIO_NUM_2
#define motor_pump_out  GPIO_NUM_4

#define switch_low      GPIO_NUM_34
#define switch_high     GPIO_NUM_35

// ---------- PWM Config ----------
#define pwm_channel_left_fw   0
#define pwm_channel_left_bw   1
#define pwm_channel_right_fw  2
#define pwm_channel_right_bw  3

#define pwm_frequency  2000
#define pwm_resolution 8
#define max_duty       255

// ---------- Global variables ----------
extern int motorSpeed;

// ---------- Motor control ----------
void stopMotors();
void motorControl(int leftSpeed, int rightSpeed);
void initialize_motors();

// ---------- Pump control ----------
void pump_stop();
void pump_in();
void pump_out();

// ---------- Main task ----------
void monitor_motors(void *pvParameters);

#endif
