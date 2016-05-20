void calibrateGyro()
{
  float gyroSums[3]={0};
  float gyroSigma[3]={0};
 
  for (int i=0;i<NUM_GYRO_SAMPLES;i++)
  {
    updateGyroValues();
    for (int j=0;j<3;j++)
    {
      gyroSums[j]+=gyroRaw[j];
      gyroSigma[j]+=gyroRaw[j]*gyroRaw[j];
    }
  }
  for (int j=0;j<3;j++)
  {
    float averageRate=gyroSums[j]/NUM_GYRO_SAMPLES;
    gyroZeroRate[j]=averageRate;    
    gyroThreshold[j]=sqrt((double(gyroSigma[j]) / NUM_GYRO_SAMPLES) - (averageRate * averageRate)) * GYRO_SIGMA_MULTIPLE;    
  }
  
}

void MPUbegin(){
  Wire.begin();
  Wire.beginTransmission(MPU);
  Wire.write(0x6B);  // PWR_MGMT_1 register
  Wire.write(0);     // set to zero (wakes up the MPU-6050)
  Wire.endTransmission(true);
  mpu6050.setFullScaleGyroRange(MPU6050_GYRO_FS_2000);
}

float updateGyroValues(){
  Wire.beginTransmission(MPU);
  Wire.write(0x45);  // starting with register 0x45 (GYRO_YOUT_H)
  Wire.endTransmission(false);
  Wire.requestFrom(MPU, 2, true); // request a total of 2 registers
  GyY = Wire.read() << 8 | Wire.read(); // 0x45 (GYRO_YOUT_H) & 0x46 (GYRO_YOUT_L)
  
  GyY -= gyroZeroRate[0]*1.8;
  gyroRaw[0] = (float)GyY;
  return (float)GyY;
}

void writeAngle(byte _angle[4]){
    byte start_byte = 0x80;
    Serial.write(start_byte);
    Serial.write(_angle, 4);
    byte end_byte = 0xFF;
    Serial.write(end_byte);
}

void serialFlush(){
  int i = Serial.available();
  for(int j=0;j<i;j++){
    char temp = Serial.read();
  }
}
