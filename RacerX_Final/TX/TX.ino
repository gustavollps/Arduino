#include <SPI.h>
#include "nRF24L01.h"
#include "RF24.h"
#include <Wire.h> 
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27,16,2);
int led[2], volt[3];


RF24 radio(9,10);
const uint64_t pipe = 0xE8E8F0F0E1LL;
const uint64_t pipe2 = 0xF0F0F0F0D2LL;

void setup(void){ 
  pinMode(5,INPUT);
  pinMode(2,OUTPUT); //led1
  pinMode(3,OUTPUT); //led2
  pinMode(4,OUTPUT); //led3
  Serial.begin(9600);
  lcd.init();
  lcd.backlight();
  radio.begin();  
  radio.openWritingPipe(pipe);
  radio.openReadingPipe(1,pipe2);
  radio.setDataRate(RF24_250KBPS);
  radio.setCRCLength(RF24_CRC_16);
}
int l=0,z=0;
void loop(void){
  if(l<10){
    led[0] = map(analogRead(A0),0,1023,62,115);
    led[1] = map(analogRead(A1),100,400,200,80);
    if(led[1]>200){
      led[1]=200;
    }
    if(led[1]<80){
      led[1]=80;
    }
    if(digitalRead(5)==1){
      led[1]=30;
    }
    radio.write( led, sizeof(led) );
    Serial.print(led[0],DEC);
    Serial.print(" - ");
    Serial.println(led[1],DEC);
    l++;
  }
  else{
    l=0;
    led[0]=0;
    led[1]=0;
    radio.write( led, sizeof(led) );
    Serial.println("--------");
    radio.startListening();
    Serial.println("Recieving");
    bool done = false;
    if(radio.available()){
      lcd.setCursor(0,1);
      lcd.print("Connected   ");
      z=0;
      while (!done){
        done = radio.read( volt, sizeof(volt) );
      }
      /*Serial.print(volt[0],DEC);
      Serial.print(" - ");
      Serial.print(volt[1],DEC);
      Serial.print(" - ");
      Serial.println(volt[2],DEC);*/
      
      lcd.setCursor(0,0);
      float A = 0.00;
      A = volt[0]/1.000;
      lcd.print(A/1000);
      
      lcd.setCursor(6,0);
      float B = 0.00;
      B = volt[1]/1.004;
      lcd.print(B/1000);
      
      lcd.setCursor(12,0);
      float C = 0.00;
      C = volt[2]/1.038;
      lcd.print(C/1000);
      
      if(volt[0]>3500){
        digitalWrite(2,0);
      }
      else{
        digitalWrite(2,1);
      }
      if(volt[1]>3500){
        digitalWrite(3,0);
      }
      else{
        digitalWrite(3,1);
      }
      if(volt[2]>3500){
        digitalWrite(4,0);
      }
      else{
        digitalWrite(4,1);
      }
      
    }
    else{
      z++;
      if(z>8){
      lcd.setCursor(0,1);
      lcd.print("Disconnected");
      }
    }
    radio.stopListening();
  }
}
