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
float I = 1.0;
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
  if (Serial.available()) {
    while (Serial.available()) {
      byte temp = Serial.read();
      if (index == 0) {
        if (temp == 'k') {
          request++;
        }
        else {
          request = 0;
        }
        if (request == 4) {
          sendData();
          request = 0;
        }
      }

      switch (index) {
        case 0:
          if (temp == 0x00) {
            index++;
          }
          break;
        //-------------------------------------------------------------------------
        case 1:
          if (temp == 'P') {
            index++;
          }
          else {
            index = 0;
          }
          break;
        case 2:
          kp.byteData[0] = temp;
          index++;
          break;
        case 3:
          kp.byteData[1] = temp;
          index++;
          break;
        case 4:
          kp.byteData[2] = temp;
          index++;
          break;
        case 5:
          kp.byteData[3] = temp;
          index++;
          break;
        //-------------------------------------------------------------------------
        case 6:
          if (temp == 'I') {
            index++;
          }
          else {
            index = 0;
          }
          break;
        case 7:
          ki.byteData[0] = temp;
          index++;
          break;
        case 8:
          ki.byteData[1] = temp;
          index++;
          break;
        case 9:
          ki.byteData[2] = temp;
          index++;
          break;
        case 10:
          ki.byteData[3] = temp;
          index++;
          break;
        //-------------------------------------------------------------------------
        case 11:
          if (temp == 'D') {
            index++;
          } else {
            index = 0;
          }
          break;
        case 12:
          kd.byteData[0] = temp;
          index++;
          break;
        case 13:
          kd.byteData[1] = temp;
          index++;
          break;
        case 14:
          kd.byteData[2] = temp;
          index++;
          break;
        case 15:
          kd.byteData[3] = temp;
          index++;
          break;
        //-------------------------------------------------------------------------
        case 16:
          if (temp == 'S') {
            index++;
          }
          else {
            index = 0;
          }
          break;
        case 17:
          _setpoint.byteData[0] = temp;
          index++;
          break;
        case 18:
          _setpoint.byteData[1] = temp;
          index++;
          break;
        case 19:
          _setpoint.byteData[2] = temp;
          index++;
          break;
        case 20:
          _setpoint.byteData[3] = temp;
          index++;
          break;
        //-------------------------------------------------------------------------.
        case 21:
          if (temp == 'm') {
            index++;
          }
          else {
            index = 0;
          }
          break;
        case 22:
          min.byteData[0] = temp;
          index++;
          break;
        case 23:
          min.byteData[1] = temp;
          index++;
          break;
        case 24:
          min.byteData[2] = temp;
          index++;
          break;
        case 25:
          min.byteData[3] = temp;
          index++;
          break;
        //-------------------------------------------------------------------------
        case 26:
          if (temp == 'M') {
            index++;
          }
          else {
            index = 0;
          }
          break;
        case 27:
          max.byteData[0] = temp;
          index++;
          break;
        case 28:
          max.byteData[1] = temp;
          index++;
          break;
        case 29:
          max.byteData[2] = temp;
          index++;
          break;
        case 30:
          max.byteData[3] = temp;
          index++;
          break;
        //-------------------------------------------------------------------------
        case 31:
          if (temp == 0x80) {
            index = 0;
            P = kp.floatData;
            I = ki.floatData;
            D = kd.floatData;
            setpoint = _setpoint.floatData;
            pid_min = min.floatData;
            pid_max = max.floatData;
            PID.setParameters(P, I, D, pid_min, pid_max);
            PID.changeSetPoint(setpoint);
            PID.resetI();
#ifdef DEBUG
            Serial.print("kp: ");
            Serial.println(PID.kp, DEC);
            Serial.print("\t");
            Serial.print("ki: ");
            Serial.println(PID.ki, DEC);
            Serial.print("\t");
            Serial.print("kd: ");
            Serial.println(PID.kd, DEC);            
#endif
          } else {
            index = 0;
          }
          break;
        default:
          index = 0;
          break;
      }

    }   
  }

  if (PIDLoop.ok()) {
    speed = updateSpeed();  //in rotations per loop    
    speed = ((float)pid_f) * speed * 60.0; //convertion to RPM
    //speed = kalmanFilter.Compute(speed);

    data.floatData = speed;

    PID.changeSetPoint(setpoint * (255.0 / 4800.0));

    pwm = PID.Compute(speed * (255.0 / 4800.0));
    pwm = map(pwm, -100, 100, 0, 255);
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

void tacometer() {
  counter++;
}

float updateSpeed() {
  float _speed = ((float)(counter - old_counter)) / 4.0;
  old_counter = counter;
  return _speed;
}

void sendData() {  
  Serial.write(0x80);
  Serial.write(data.byteData, 4);
  Serial.write(0xFF);
  binaryFloat send;
  send.floatData = pwm;
  Serial.write(send.byteData,4);
  Serial.write(0x00);
}
