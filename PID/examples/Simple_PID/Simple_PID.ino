#include <PID.h>
#include <Loop.h>

#define motor_pin 3
#define tacometer_pin A0

PID PID;
Loop PIDLoop(50,"PID");
Loop DebugLoop(10,"Debug");

float P = 1.0;
float I = 1.0;
float D = 1.0;
float pid_min = -100.0;
float pid_max = 100.0;
float i_min = -50;
float i_max = 50;
float setpoint = 30;

void setup(){
  
  Serial.begin(9600);
  PID.setParameters(P, I, D, pid_min, pid_max);
  PID.limitI(i_min,i_max);
  PID.start();

}

void loop(){
  
  if(PIDLoop.ok()){
    float velocidade = analogRead(tacometer_pin)*(pid_max-pid_min)/1024;
    
    PID.changeSetPoint(setpoint);
    
    float pwm = PID.Compute(velocidade);
    pwm = map(pwm,-100,100,0,255);
    
    analogWrite(pwm,motor_pin);          
  }
 
  if(DebugLoop.ok()){
    Serial.print("P: ");
    Serial.print(PID.P);
    Serial.print("/t I: ");
    Serial.print(PID.I);
    Serial.print("/t D: ");
    Serial.print(PID.D);
    Serial.println();        
  }
}
