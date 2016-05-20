#include "I2Cdev.h"
#include "MPU6050.h"

MPU6050 mpu6050;


#include <Wire.h>
#include "config.h"


const int MPU = 0x68; // I2C address of the MPU-6050
int16_t GyY, GyX, GyZ;
float gyroThreshold[3];
float gyroZeroRate[3];
float gyroRaw[3];

typedef union {
  float floatingPoint;
  byte binary[4];
} binaryFloat;

void setup() {
  MPUbegin();
  Serial.begin(38400);
  calibrateGyro();
  Serial.println("Setup");
}

unsigned long time = micros();
unsigned long oldtime = time, deltaT = 0;
float angle = 0;
binaryFloat data;

void loop() {
  deltaT = time - oldtime;
  oldtime = time;
  time = micros();
  data.floatingPoint = updateGyroValues();

  //integrating
  angle += 4.0 * data.floatingPoint * deltaT / 100000000.0;


  data.floatingPoint = angle;
  if (Serial.available()) {
    if (Serial.read() == 0x7E) { //reset angle
      resetAngle();
      for (int i = 0; i < 6; i++) { //response that the angle was reseted
        Serial.write(0x00);
      }
    }
    else {            
      data.floatingPoint *= 1.5384;
      if (data.floatingPoint > 180.0) {
        data.floatingPoint -= 360;
      }
      else if (data.floatingPoint < -180.0) {
        data.floatingPoint += 360;
      }
      //Serial.println(data.floatingPoint);
      writeAngle(data.binary);
    }
    serialFlush();
  }

}
