float safe_asin(float v)
{
        if (isnan(v)) {
                return 0;
        }
        if (v >= 1.0) {
                return PI/2;
        }
        if (v <= -1.0) {
                return -PI/2;
        }
        return asin(v);
}

void updateAccvalues(float angles[3]){
  int buffer[3];
  double buffer2[3];
  MPU.getAxlData(buffer);
  buffer2[0]=(buffer[0]-ACC_X_OFFSET)*ACC_X_GAIN; 
  buffer2[1]=(buffer[1]-ACC_Y_OFFSET)*ACC_Y_GAIN;
  buffer2[2]=(buffer[2]-ACC_Z_OFFSET)*ACC_Z_GAIN;
  //Serial.println(buffer[0]);
  //float tany = buffer2[2]/buffer2[0];
  //float tanx = buffer2[2]/buffer2[1];
  //float tanz = buffer2[0]/buffer2[1];
  angles[0] = atan2(buffer2[2],buffer2[0]);
  angles[1] = atan2(buffer2[2],buffer2[1]);
  angles[2] = atan2(buffer2[0],buffer2[1]);
}


void updateHeadings()
{
    
  float deltaT=getDeltaTMicros();

  double gyroDPSDelta[3];

  for (int j=0;j<3;j++)
    {
      gyroDPSDelta[j]=(gyroDPS[j]*deltaT)/1000000.0;
      heading[j] -= gyroDPSDelta[j];
      headingint[j]= heading[j];

    }
    
  // get radians per second
  // z is yaw
  // y is pitch
  // x is bank
  double rps[3]={0.0};
  for (int j=0;j<3;j++)
    rps[j]=(gyroDPSDelta[j]/57.2957795);
  
}

// this simply returns the elapsed time since the last call.
unsigned long getDeltaTMicros()
{
  static unsigned long lastTime=0;
  
  unsigned long currentTime=micros();
  
  unsigned long deltaT=currentTime-lastTime;
  if (deltaT < 0.0)
     deltaT=currentTime+(4294967295-lastTime);
   
  lastTime=currentTime;
  
  return deltaT;
}

// I called this from the loop function to see what the right values were for the calibration constants.
// If you are trying to reduce the amount of time needed for calibration just try not to go so low that consecutive
// calibration calls give you completely unrelated data.  Some sensors are probably better than others.


void updateGyroValues() {

  //while (!(gyroReadI2C(0x27) & B00001000)){}      // Without this line you will get bad data occasionally
  
  //if (gyroReadI2C(0x27) & B01000000)
  //  Serial.println("Data missed!  Consider using an interrupt");
    
  //int reg=0x28;
  MPU.getGyroData(gyroRaw);
  
 
  float deltaGyro[3];
  for (int j=0;j<3;j++)
  {
    deltaGyro[j]=gyroRaw[j]-gyroZeroRate[j];      // Use the calibration data to modify the sensor value.
    if (abs(deltaGyro[j]) < gyroThreshold[j])
      deltaGyro[j]=0;
    gyroDPS[j]= deltaGyro[j];
  }
}
