#include <SPI.h>
#include "nRF24L01.h"
#include "RF24.h"
#include <Servo.h>
Servo servo;
int led[2];

RF24 radio(9,10);
const uint64_t pipe = 0xE8E8F0F0E1LL;
const uint64_t pipe2 = 0xF0F0F0F0D2LL;

void setup(void){
  pinMode(4,OUTPUT); //rele
  pinMode(5,OUTPUT); //motor
  pinMode(6,OUTPUT); //motor
  pinMode(7,OUTPUT); //led
  digitalWrite(4,1);
  servo.attach(A3);
  radio.begin();
  Serial.begin(9600);
  radio.openReadingPipe(1,pipe);
  radio.openWritingPipe(pipe2);
  radio.setDataRate(RF24_250KBPS);
  radio.setCRCLength(RF24_CRC_16);
  radio.startListening();

  Serial.println("Setup");
}

void loop(void){
  led[0]=1;
  led[1]=1;
  int volt[3];
  volt[0]=map(analogRead(A0), 0, 1023, 0, 5000)*2;
  volt[1]=map(analogRead(A1), 0, 1023, 0, 5000)*2-volt[0];
  volt[2]=map(analogRead(A2), 0, 1023, 0, 5000)*3-volt[1]*2;
  digitalWrite(7,0);
  if(volt[0]<3500 || volt[1]<3500 || volt[2]<3500){
    digitalWrite(7,1);
  }
  bool done = false;
  if ( radio.available() )
  {
    while (!done)
    {
      done = radio.read( led, sizeof(led) );
    }
    /*
    Serial.print(led[0],DEC);
     Serial.print(" - ");
     Serial.println(led[1],DEC);
     */
  }


  if(led[0]==0&&led[1]==0){
    Serial.println("RECIENVING");
    radio.stopListening();
    radio.write( volt, sizeof(volt) );
    radio.startListening();
  }
  else{
    int x = led[1];
    int s = led[0];
    Serial.print(x,DEC);
    Serial.print(" - ");
    Serial.println(s,DEC);
    if(s>40&&s<130){
      servo.write(s);
    }
    if(x==30){
      motor(2,120);
    }
    else if(x>79&&x<85){
      motor(3,0);
    }
    else if(x>84){
      motor(1,x);
    }
  }
}




