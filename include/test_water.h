// #ifndef __TEST_WATER__
// #define __TEST_WATER__

// #include <Arduino.h>

// // ---------- Pin Definitions ----------
// #define motor_left_fw   GPIO_NUM_32
// #define motor_left_bw   GPIO_NUM_33
// #define motor_right_fw  GPIO_NUM_25
// #define motor_right_bw  GPIO_NUM_27
// #define motor_pump_in   GPIO_NUM_4
// #define motor_pump_out  GPIO_NUM_2

// #define switch_low      GPIO_NUM_34
// #define switch_high     GPIO_NUM_35

// // ---------- PWM Config ----------
// #define pwm_channel_left_fw   0
// #define pwm_channel_left_bw   1
// #define pwm_channel_right_fw  2
// #define pwm_channel_right_bw  3

// #define pwm_frequency  2000
// #define pwm_resolution 8
// #define max_duty       255

// int motorSpeed = 0;

// // ---------- Motor Control ----------
// void stopMotors()
// {
//     ledcWrite(pwm_channel_left_fw, 0);
//     ledcWrite(pwm_channel_left_bw, 0);
//     ledcWrite(pwm_channel_right_fw, 0);
//     ledcWrite(pwm_channel_right_bw, 0);
// }

// void motorControl(int leftSpeed, int rightSpeed)
// {
//     leftSpeed  = constrain(leftSpeed,  -max_duty, max_duty);
//     rightSpeed = constrain(rightSpeed, -max_duty, max_duty);

//     if (leftSpeed >= 0) {
//         ledcWrite(pwm_channel_left_fw, leftSpeed);
//         ledcWrite(pwm_channel_left_bw, 0);
//     } else {
//         ledcWrite(pwm_channel_left_fw, 0);
//         ledcWrite(pwm_channel_left_bw, -leftSpeed);
//     }

//     if (rightSpeed >= 0) {
//         ledcWrite(pwm_channel_right_fw, rightSpeed);
//         ledcWrite(pwm_channel_right_bw, 0);
//     } else {
//         ledcWrite(pwm_channel_right_fw, 0);
//         ledcWrite(pwm_channel_right_bw, -rightSpeed);
//     }
// }

// // ---------- Initialization ----------
// void initialize_motors()
// {
//     ledcSetup(pwm_channel_left_fw,  pwm_frequency, pwm_resolution);
//     ledcSetup(pwm_channel_left_bw,  pwm_frequency, pwm_resolution);
//     ledcSetup(pwm_channel_right_fw, pwm_frequency, pwm_resolution);
//     ledcSetup(pwm_channel_right_bw, pwm_frequency, pwm_resolution);

//     ledcAttachPin(motor_left_fw,  pwm_channel_left_fw);
//     ledcAttachPin(motor_left_bw,  pwm_channel_left_bw);
//     ledcAttachPin(motor_right_fw, pwm_channel_right_fw);
//     ledcAttachPin(motor_right_bw, pwm_channel_right_bw);

//     pinMode(motor_pump_in,  OUTPUT);
//     pinMode(motor_pump_out, OUTPUT);
//     pinMode(switch_low,     INPUT);
//     pinMode(switch_high,    INPUT);

//     // Safety: pump off initially
//     digitalWrite(motor_pump_in,  LOW);
//     digitalWrite(motor_pump_out, LOW);
// }

// // ---------- Pump Control ----------
// void pump_stop()
// {
//     digitalWrite(motor_pump_in,  LOW);
//     digitalWrite(motor_pump_out, LOW);
//     Serial.println("Pump OFF");
// }

// void pump_in()   // pump water IN (float submarine)
// {
//     digitalWrite(motor_pump_in,  LOW);
//     digitalWrite(motor_pump_out, HIGH);
//     Serial.println("Pumping OUT (Float)");
// }

// void pump_out()  // pump water OUT (sink submarine)
// {
//     digitalWrite(motor_pump_in,  HIGH);
//     digitalWrite(motor_pump_out, LOW);
//     Serial.println("Pumping IN (Sink)");
// }

// // ---------- Main control task ----------
// void test_water(void *pvParameters)
// {
//     Serial.begin(115200);
//     initialize_motors();

//     Serial.println("Motor & Pump control ready.");
//     unsigned long startTime = millis();
//     // pump_out();
//     Serial.println("Timer started! Press BOOT button to stop.");

//     while (1)
//     {
//         int lowSwitch  = gpio_get_level(switch_low);
//         int highSwitch = gpio_get_level(switch_high);

//         Serial.printf("Switch34:%d | Switch35:%d\n", lowSwitch, highSwitch);

//         // If both switches HIGH → tank full → stop everything
//         if (lowSwitch == HIGH && highSwitch == HIGH)
//         {
//             pump_stop();
//             stopMotors();
//             Serial.println("Both switches HIGH → Pump & Motors stopped!");
//         }
//         else
//         {
//             // Optional: run pump if not full
//             pump_out(); // pumping water in until full
//         }

//         // BOOT button manual stop
//         if (digitalRead(GPIO_NUM_0) == LOW)
//         {
//             unsigned long elapsed = millis() - startTime;
//             Serial.printf("Manual stop! Elapsed time: %lu ms (%.2f s)\n", elapsed, elapsed / 1000.0);

//             pump_stop();
//             stopMotors();
//             vTaskDelay(pdMS_TO_TICKS(300)); // debounce delay
//         }

//         vTaskDelay(pdMS_TO_TICKS(500)); // check every 500ms
//     }
// }

// #endif