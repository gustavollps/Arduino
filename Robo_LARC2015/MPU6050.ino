void Serial2_flush(){
  while(Serial2.available()){
    Serial2.read();
  }
}

float get_Angle() {
  byte temp;
  //Serial2_flush();
  Serial2.write(1);  
  unsigned long time_gyro = millis();
  while (Serial2.available() < 6 && millis() - time_gyro < 10) { //waits for the complete message (6 bytes) or a time up of 10ms
#ifdef DEBUG_ANGLES
    Serial.println(Serial2.available());
#endif
  };

#ifdef DEBUG_ANGLES
  Serial.println(Serial2.available());
#endif

  temp = Serial2.read();

#ifdef DEBUG_ANGLES
  Serial.println(temp, HEX);
#endif

  if (temp == 0x80) {
    for (int i = 0; i < 4 ; i++) {
      angle.binary[i] = Serial2.read();
    }
    if (Serial2.read() == 0xFF) {

#ifdef DEBUG_ANGLES
      Serial.println(" - OK");
#endif
    }
    else { //fix erros on serial communication by throwing away bytes until reach the end byte (0xFF) of a message
      while (Serial2.read() != 0xFF) {
        Serial2.write(1);
      }
    }
    gyro_angle = angle.floatingPoint;
    //gyro_time = millis();
#ifdef DEBUG_ANGLES
    Serial.print(" - ");
    Serial.println(gyro_angle, DEC);
#endif
    angle.floatingPoint -= angle_offset;
    if (angle.floatingPoint > 180) {
      angle.floatingPoint -= 360;
    }
    else if ( angle.floatingPoint < -180) {
      angle.floatingPoint += 360;
    }
    return angle.floatingPoint;
  }
  else {
    angle.floatingPoint -= angle_offset;
    if (angle.floatingPoint > 180.0) {
      angle.floatingPoint -= 360.0;
    }
    else if ( angle.floatingPoint < -180.0) {
      angle.floatingPoint += 360.0;
    }
    return angle.floatingPoint;
  }
}

bool reset_Angle() {
  angle_offset = angle.floatingPoint;
}