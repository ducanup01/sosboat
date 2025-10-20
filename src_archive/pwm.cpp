#include <Arduino.h>

// Motor pin definitions
#define MOTOR_LEFT_FORWARD GPIO_NUM_16
#define MOTOR_LEFT_BACKWARDS GPIO_NUM_17
#define MOTOR_RIGHT_FORWARD GPIO_NUM_19
#define MOTOR_RIGHT_BACKWARDS GPIO_NUM_18

// PWM channel assignments
#define PWM_CHANNEL_LEFT_FWD 0
#define PWM_CHANNEL_LEFT_BWD 1
#define PWM_CHANNEL_RIGHT_FWD 2
#define PWM_CHANNEL_RIGHT_BWD 3

// PWM settings
#define PWM_FREQ 2000
#define PWM_RESOLUTION 8
#define MAX_DUTY 255

int motorSpeed = 0; // input value from Serial

void setupPWM()
{
  ledcSetup(PWM_CHANNEL_LEFT_FWD, PWM_FREQ, PWM_RESOLUTION);
  ledcSetup(PWM_CHANNEL_LEFT_BWD, PWM_FREQ, PWM_RESOLUTION);
  ledcSetup(PWM_CHANNEL_RIGHT_FWD, PWM_FREQ, PWM_RESOLUTION);
  ledcSetup(PWM_CHANNEL_RIGHT_BWD, PWM_FREQ, PWM_RESOLUTION);

  ledcAttachPin(MOTOR_LEFT_FORWARD, PWM_CHANNEL_LEFT_FWD);
  ledcAttachPin(MOTOR_LEFT_BACKWARDS, PWM_CHANNEL_LEFT_BWD);
  ledcAttachPin(MOTOR_RIGHT_FORWARD, PWM_CHANNEL_RIGHT_FWD);
  ledcAttachPin(MOTOR_RIGHT_BACKWARDS, PWM_CHANNEL_RIGHT_BWD);
}

void motorControl(int leftSpeed, int rightSpeed)
{
  leftSpeed = constrain(leftSpeed, -MAX_DUTY, MAX_DUTY);
  rightSpeed = constrain(rightSpeed, -MAX_DUTY, MAX_DUTY);

  // Left motor
  if (leftSpeed >= 0)
  {
    ledcWrite(PWM_CHANNEL_LEFT_FWD, leftSpeed);
    ledcWrite(PWM_CHANNEL_LEFT_BWD, 0);
  }
  else
  {
    ledcWrite(PWM_CHANNEL_LEFT_FWD, 0);
    ledcWrite(PWM_CHANNEL_LEFT_BWD, -leftSpeed);
  }

  // Right motor
  if (rightSpeed >= 0)
  {
    ledcWrite(PWM_CHANNEL_RIGHT_FWD, rightSpeed);
    ledcWrite(PWM_CHANNEL_RIGHT_BWD, 0);
  }
  else
  {
    ledcWrite(PWM_CHANNEL_RIGHT_FWD, 0);
    ledcWrite(PWM_CHANNEL_RIGHT_BWD, -rightSpeed);
  }
}

void stopMotors()
{
  ledcWrite(PWM_CHANNEL_LEFT_FWD, 0);
  ledcWrite(PWM_CHANNEL_LEFT_BWD, 0);
  ledcWrite(PWM_CHANNEL_RIGHT_FWD, 0);
  ledcWrite(PWM_CHANNEL_RIGHT_BWD, 0);
}

void setup()
{
  Serial.begin(115200);
  setupPWM();
  Serial.println("Enter motor speed (-255 to 255):");
  Serial.println("Positive = forward, Negative = backward, 0 = stop");
}

void loop()
{
  // If serial data available
  if (Serial.available() > 0)
  {
    String input = Serial.readStringUntil('\n');
    input.trim(); // remove extra spaces or newline

    if (input.length() > 0)
    {
      int speedValue = input.toInt();

      // Constrain speed value
      speedValue = constrain(speedValue, -MAX_DUTY, MAX_DUTY);

      if (speedValue == 0)
      {
        stopMotors();
        Serial.println("Motors stopped.");
      }
      else
      {
        motorControl(speedValue, speedValue); // same speed both motors
        Serial.printf("Motors running at %d (PWM duty)\n", speedValue);
      }
    }
  }

  vTaskDelay(pdMS_TO_TICKS(20)); // small delay for stability
}