#define mright_1 10
#define mright_2 6
#define mleft_1 9
#define mleft_2 11
//#define DEBUG

void setup() {
  Serial.begin(9600);
  //  Serial1.begin(9600);
  pinMode(13, OUTPUT);

  pinMode(mleft_1, OUTPUT);
  pinMode(mleft_2, OUTPUT);
  pinMode(mright_1, OUTPUT);
  pinMode(mright_2, OUTPUT);
}

byte buf[4] = {0, 0, 0, 0};
byte temp = 0;
byte index = 0;
int angle = 0;
byte throttle = 0;
int reverse = 0;


void loop() {
  /*  if(Serial1.available()){
      byte temp = Serial1.read();
        Serial.print(temp);
        Serial.print(" - ");
      if(temp == 255){
        Serial.println();
      }
    }*/

  reverse = map(angle, -30, 30, -throttle, throttle);
  if (reverse < 0) {
    analogWrite(mleft_1, throttle);
    digitalWrite(mleft_2, 0);
    analogWrite(mright_1, throttle + reverse);
    digitalWrite(mright_2, 0);
  }
  else {
    analogWrite(mleft_1, throttle - reverse);
    digitalWrite(mleft_2, 0);
    analogWrite(mright_1, throttle);
    digitalWrite(mright_2, 0);
  }

  if (Serial.available()) {
    temp = Serial.read();
    if (temp == 0) {
      index = 1;
    }
    else if (temp == 255) {
      if (index == 3) {
        angle = buf[1];
        throttle = buf[2] - 1;
        angle = angle - 126;
        angle = constrain(angle, -30, 30);
        analogWrite(13, throttle);
#ifdef DEBUG
        Serial.print("Angle :");
        Serial.print(angle);
        Serial.print(" \t Throtle: ");
        Serial.print();
        Serial.print(" \t Reverse: ");
        Serial.print(reverse);
        Serial.print("\t pwm: ");
        if (reverse < 0) {
          Serial.print(throttle + reverse);
        } else {
          Serial.print(throttle - reverse);
        }
        Serial.println();
#endif
      }
      index = 0;
    }
    else {
      if (index < 3) {
        buf[index] = temp;
        index++;
      }
      else {
        index = 0;
      }
    }
  }
}
