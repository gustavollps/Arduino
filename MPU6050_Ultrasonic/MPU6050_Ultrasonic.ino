#include "I2Cdev.h"
#include "MPU6050.h"

MPU6050 mpu6050;


#include <Wire.h>
#include "config.h"


const int MPU = 0x68; // I2C address of the MPU-6050
int16_t GyY,GyX,GyZ;
float gyroThreshold[3];
float gyroZeroRate[3];
float gyroRaw[3];

typedef union {
  float floatingPoint;
  byte binary[4];
} binaryFloat;

binaryFloat data;

void setup() {
  MPUbegin();
  Serial.begin(115200);
  calibrateGyro();
}

unsigned long time = micros();
unsigned long oldtime=time, deltaT=0;
float angle = 0;

void loop() {
  deltaT = time - oldtime;
  oldtime = time;
  time = micros();
  data.floatingPoint = updateGyroValues();
  
  //integraus
  angle += 4.0*data.floatingPoint*deltaT/100000000.0;
  
  data.floatingPoint = angle;
  if (Serial.available()) {
    serialFlush();
    writeAngle(data.binary);
  }
  
}
