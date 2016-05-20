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

void resetAngle(){
  data.floatingPoint = 0;
  angle = 0;  
}

float updateGyroValues(){
  Wire.beginTransmission(MPU);
  Wire.write(0x47);  // starting with register 0x45 (GYRO_YOUT_H)
  Wire.endTransmission(false);
  Wire.requestFrom(MPU, 2, true); // request a total of 2 registers
  GyZ = Wire.read() << 8 | Wire.read(); // 0x47 (GYRO_ZOUT_H) & 0x48 (GYRO_ZOUT_L)
  
  GyZ -= gyroZeroRate[0];
  gyroRaw[0] = (float)GyZ;
  return (float)GyZ;
}

void writeAngle(byte _angle[4]){
    
    byte start_byte = 0x80;
    Serial.write(start_byte);
    Serial.write(_angle, 4);
    byte end_byte = 0xFF;
    Serial.write(end_byte);
}

void serialFlush(){
  for(int i=0;i<Serial.available();i++){
    char temp = Serial.read();
  }
}
