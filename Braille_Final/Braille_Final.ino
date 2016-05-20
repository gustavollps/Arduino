#include <Braille_Letters.h>
#include <Servo.h>

Braille_Letters Braille_Cell;

int dif = 100;
int d1 = 1350;
int d2 = 1750;
int d3 = 1750;
int d4 = 1800;
int d5 = 1800;
int d6 = 1300;

void setup() {
  Serial.begin(9600);
  Braille_Cell.Init(  4, 1330, 1480,  //1
                      3, 1220, 1120,  //2
                      6, 1750, 1500,  //3
                      7, 1700, 1550,  //4
                      8, 1650, 1500,  //5
                      9, 1450, 1600); //6
  //PIN, PIN_DOWN, PIN_UP

#ifdef  DEBUG
  bool up = false;
  Braille_Cell.Servo_write(0, 0, 0, 0, 0, 0);
  while (1) {
    if (Serial.available()) {
      Serial.read();
      if (!up) {
        Braille_Cell.Servo_write(1, 1, 1, 1, 1, 1);
        up = true;
      }
      else {
        Braille_Cell.Servo_write(0, 0, 0, 0, 0, 0);
        up = false;
      }
    }
  }
#endif

}

void loop() {
  if (Serial.available()) {
    Braille_Cell.Print_Letter((char)Serial.read());
  }
}
