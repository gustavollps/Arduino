#include <HC_SR04.h>
#include "config.h"

HC_SR04 sensor(TRIG_PIN, ECHO_PIN, ECHO_INT);

void setup(){  
  sensor.begin();
  Serial.begin(9600); 
//  while(!Serial) continue;
  sensor.start();
}

void loop(){
  if(sensor.isFinished()){
    // Do something with the range...
    Serial.print(sensor.getRange());
    Serial.println("cm");
//    delay(100);
    sensor.start();
  }
  // Do other things...
}
