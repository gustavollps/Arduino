#include "PIDTM.h"
#include <Loop.h>
#include <TimerOne.h>

#define tacometer_pin 2

Loop PIDLoop(20);
PID PID; 

unsigned long counter=0;
unsigned long old_counter=0;

void setup(){
  
  Serial.begin(9600);
  PID.makeSetPoint(2000);  
  PID.setParameters(1.1,0.00001,0);
  Timer1.initialize(40);

  pinMode(tacometer_pin, INPUT);
  attachInterrupt(0, tacometer, CHANGE);
  
}

float leitura = 0;
float saida = 0;

void loop(){
  
  if(PIDLoop.ok()){
    leitura = updateSpeed();  //in rotations per loop    
    leitura = ((float)20) * leitura * 60.0; //convertion to RPM  
    saida = PID.setOutput(leitura);
    //saida = map(saida,-100,100,0,255);
    Timer1.pwm(10,  saida*10);    
    /*Serial.print("PID: ");
    Serial.print(saida);
    Serial.print("\t - \t");    */
    Serial.println(leitura);
  } 
}


void tacometer() {
  counter++;
}

float updateSpeed() {
  float _speed = ((float)(counter - old_counter)) / 4.0;
  old_counter = counter;
  return _speed;
} 
