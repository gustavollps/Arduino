#include <kalmanFilter.h>
#include <PID.h>
#include <Loop.h>
#include <TimerOne.h>

//#define DEBUG
#define motor_pin 10
#define tacometer_pin 2

PID PID;

float a = 1.0;
float c = 1.0;
float H = 1.0;
float sigma = 1.0;
float R = 10;
float pa = 40;
float xa = 0;

kalmanFilter kalmanFilter(a,c,H,sigma);

uint8_t pid_f = 20;
Loop PIDLoop(pid_f);
Loop DebugLoop(10);

float P = 1.0;
float I = 0;
float D = 1.0;
float pid_min = -100.0;
float pid_max = 100.0;
float i_min = -100;
float i_max = 100;
float setpoint = 3500.0;

unsigned long counter = 0;
unsigned long old_counter = 0;

float speed = 0;
float pwm = 0;

typedef union {
  float floatData;
  byte byteData[4];
} binaryFloat;

binaryFloat data;

void setup() {
  Serial.begin(38400);

  PID.setParameters(P, I, D, pid_min, pid_max);
  PID.limitI(i_min, i_max);
  PID.start();

  kalmanFilter.Init(R,pa,xa);

  Timer1.initialize(40);

  //pinMode(motor_pin,OUTPUT);
  pinMode(tacometer_pin, INPUT);
  attachInterrupt(0, tacometer, CHANGE);

}

uint8_t index = 0;
uint8_t request = 0;
binaryFloat kp, ki, kd, _setpoint, min, max;

void loop() {
  Serial_get();  

  if (PIDLoop.ok()) {    
    speed = updateSpeed();  //in rotations per loop    
    speed = ((float)pid_f) * speed * 60.0; //convertion to RPM
    //speed = kalmanFilter.Compute(speed);

    speed = 3500 + 2000*sin(2*PI*millis()/1000);
    data.floatData = speed;       

    PID.changeSetPoint(setpoint * (255.0 / 6600.0));

    pwm = PID.Compute(speed * (255.0 / 6600.0));
    pwm = map(pwm, -100, 100, 0, 255);

    //Serial.println(speed);
    //Serial.println(pwm);
    Timer1.pwm(motor_pin, pwm * 4);

  }

#ifdef DEBUG
  if (DebugLoop.ok()) {
    Serial.print("PID output: ");
    Serial.print(pwm);
    Serial.print("\t Derivative: ");
    Serial.print(PID.D);
    Serial.print("\t \t");
    Serial.print(speed);
    Serial.print(" RPM");
    //Serial.print('\t');
    //Serial.print(counter);
    Serial.println();
  }
#endif

}
