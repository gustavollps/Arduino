#include <MPULib.h>
#include <math.h>
#include <Wire.h>
#include "config.h"

MPULib MPU;


typedef union {
  float floatingPoint;
  byte binary[4];
} binaryFloat;

void setup()
{
  Wire.begin();
  Serial.begin(115200);
  while(!Serial.available()){
    Serial.println("Waiting");
  }
  Serial.println("Saiu");
  setupGyro();
  Serial.println("Saiu1232321");
  calibrateGyro();
  Serial.println("Saiu Setup");
}

char count = 0;
bool invert = false;

void loop()
{   
  updateGyroValues();
  updateHeadings();
  
  binaryFloat x;
  
  if (Serial.available()) {
    char data = Serial.read();
    x.floatingPoint = heading[0];  
    Serial.println("Foi");
    byte start_byte = 0x80;
    Serial.write(start_byte);
    Serial.write(x.binary, 4);
    byte end_byte = 0xFF;
    Serial.write(end_byte);
  }

}
