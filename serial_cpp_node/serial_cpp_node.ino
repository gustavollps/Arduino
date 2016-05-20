#include <Wire.h>

typedef union {
  float floatingPoint;
  byte binary[4];
} binaryFloat;

void setup()
{
  
  Serial.begin(115200);
  Wire.begin();
  
}

char count = 0;
bool invert = false;

void loop()
{

  binaryFloat x;
  
  if (Serial.available()) {
    char data = Serial.read();
    x.floatingPoint = 33.77;
    

    byte start_byte = 0x80;
    Serial.write(start_byte);
    Serial.write(x.binary, 4);
    byte end_byte = 0xFF;
    Serial.write(end_byte);
  }

}
