#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BNO055.h>

#define I2C_CLOCK 400000
#define BNO055_ADDRESS 0x28

Adafruit_BNO055 bno = Adafruit_BNO055(55, BNO055_ADDRESS);

void setupI2C() {
  pinMode(21, INPUT_PULLUP);
  pinMode(22, INPUT_PULLUP);
  Wire.begin(21, 22, I2C_CLOCK);
  delay(1000);
}

void setup() {
  Serial.begin(115200);
  setupI2C();

  Serial.println("Initializing BNO055...");

  if (!bno.begin()) {
    Serial.println("❌ Failed to find BNO055! Check wiring or address.");
    while (1);
  }

  bno.setExtCrystalUse(true);
  Serial.println("✅ BNO055 connected!\n");

  // Print header once
  Serial.println(F(" Ori_X  Ori_Y  Ori_Z | Acc_X  Acc_Y  Acc_Z | Gyro_X  Gyro_Y  Gyro_Z | Mag_X  Mag_Y  Mag_Z"));
  Serial.println(F("------------------------------------------------------------------------------------------"));
}

void loop() {
  sensors_event_t ori, acc, gyro, mag;

  bno.getEvent(&ori, Adafruit_BNO055::VECTOR_EULER);
  bno.getEvent(&acc, Adafruit_BNO055::VECTOR_LINEARACCEL);
  bno.getEvent(&gyro, Adafruit_BNO055::VECTOR_GYROSCOPE);
  bno.getEvent(&mag, Adafruit_BNO055::VECTOR_MAGNETOMETER);

  // Print compact, aligned table row
  Serial.printf("%6.1f %6.1f %6.1f | %6.2f %6.2f %6.2f | %7.3f %7.3f %7.3f | %6.2f %6.2f %6.2f\n",
                ori.orientation.x, ori.orientation.y, ori.orientation.z,
                acc.acceleration.x, acc.acceleration.y, acc.acceleration.z,
                gyro.gyro.x, gyro.gyro.y, gyro.gyro.z,
                mag.magnetic.x, mag.magnetic.y, mag.magnetic.z);

  delay(300);
}
