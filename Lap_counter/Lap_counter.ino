#include <NewPing.h>
#include <LiquidCrystal_I2C.h>
#include <Wire.h>
#include "config.h"

LiquidCrystal_I2C lcd(0x27, 16, 2);
NewPing sonar(TRIGGER, ECHO, MAX_DISTANCE);

float distance = 0.0;
unsigned long int uS = 0;
unsigned long int Lap_time = 0;
unsigned long int Actual_time = 0;
unsigned long int Offset_time = 0;
unsigned long int Min_time = 0;
unsigned long int Sec_time = 0;
unsigned long int Milis_time = 0;
unsigned long int Total_time = 0;

bool lap_start = false;
bool lap_wait = false;
bool sensor_change = false;
bool sensor_state = false;
bool sensor_state_old = false;

unsigned long int lap_last = 0;
unsigned int lap_count = 0;
unsigned long int laps[5][4];
unsigned long int best_lap = 0;
unsigned long int Start_offset = 0;


void setup() {
#ifdef DEBUG
  Serial.begin(115200);
#endif
  lcd.init();
  lcd.backlight();
  lcd.setCursor(0, 0);
#ifdef INTRO
  lcd.print("Starting...");
  delay(500);
  lcd.clear();
  delay(500);
  lcd.print("Starting...");
  delay(1000);
  lcd.clear();
  for (int i = 0; i < 16; i++) {
    lcd.setCursor(i, 0);
    lcd.print("-");
    lcd.setCursor(i, 1);
    lcd.print("-");
    delay(100);
  }
  lcd.clear();
  for (int i = 0; i < 16; i++) {
    lcd.setCursor(i, 0);
    lcd.print("-");
    lcd.setCursor(i, 1);
    lcd.print("-");
    delay(100);
  }
#endif
  lcd.clear();
  lcd.print("Current lap: ");
  lcd.print(lap_count);
  lcd.setCursor(0, 1);
  Offset_time = millis();
  Start_offset = Offset_time;
}

void loop() {
  if (lap_count != 0) {
    lcd.print("Time:  ");
    Actual_time = millis() - Offset_time;
    Sec_time = Actual_time % 1000;
    Sec_time = (Actual_time - Sec_time) / 1000;
    Min_time = Sec_time % 60;
    Min_time = (Sec_time - Min_time) / 60;
    Milis_time = Actual_time - Sec_time * 1000;
    Sec_time = Sec_time - Min_time * 60;

    //print Time --------------------------
    //set first zero if less than 10
    if (Min_time < 10) {
      lcd.print(0);
      lcd.print(Min_time);
    }
    else {
      lcd.print(Min_time);
    }
    lcd.print(":");
    if (Sec_time < 10) {
      lcd.print(0);
      lcd.print(Sec_time);
    }
    else {
      lcd.print(Sec_time);
    }
    lcd.print(":");
    lcd.print(Milis_time);
    lcd.setCursor(0, 1);
    //-------------------------------------
  }
  else {

  }
  //ultrasonic sensor read
  uS = sonar.ping();
  distance = (uS / US_ROUNDTRIP_CM);
  if (distance == 0) {
    distance = MAX_DISTANCE;
  }
  //print data to screen
  lcd.setCursor(0, 0);
  lcd.print("Current lap: ");
  lcd.print(lap_count);
  lcd.setCursor(0, 1);

  //get sensor bool state;
  if (distance < DIST) {
    sensor_state = true;
  }
  else {
    sensor_state = false;
  }

  //verify if sensor changed;
  if (sensor_state != sensor_state_old) {
    sensor_change = true;
    sensor_state_old = sensor_state;
  }
  else {
    sensor_change = false;
  }

  //count new lap
  if (millis() - lap_last > 2000 && sensor_change && sensor_state && lap_count != 0) {
    lap_wait = false;
    laps[lap_count - 1][3] = Actual_time;
    //Serial.println(Actual_time);
  }
  if (!lap_wait && distance < DIST && distance != 0) {

    //registre lap
    laps[lap_count - 1][0] = Min_time;
    laps[lap_count - 1][1] = Sec_time;
    laps[lap_count - 1][1] = Milis_time;

    lap_count++;
    lap_wait = true;
    lap_last = millis();
    Offset_time = lap_last;
    Min_time = 0;
    Sec_time = 0;
    Milis_time = 0;
  }

  //wait for ultrassonic wave to dissipate
  delay(WAIT_TIME);

  if (lap_count > LAPS_NUM) {
    //calculate total time
    while (1) {
      Total_time = 0;
      for (int i = 0; i < LAPS_NUM; i++) {
        Total_time += laps[i][3];
      }

      Sec_time = Total_time % 1000;
      Sec_time = (Total_time - Sec_time) / 1000;
      Min_time = Sec_time % 60;
      Min_time = (Sec_time - Min_time) / 60;
      Milis_time = Total_time - Sec_time * 1000;
      Sec_time = Sec_time - Min_time * 60;
      lcd.setCursor(0, 0);
      lcd.print("  ---Finish---  ");
      lcd.setCursor(0, 1);
      lcd.print("Total:  ");
      if (Min_time < 10) {
        lcd.print(0);
        lcd.print(Min_time);
      }
      else {
        lcd.print(Min_time);
      }
      lcd.print(":");
      if (Sec_time < 10) {
        lcd.print(0);
        lcd.print(Sec_time);
      }
      else {
        lcd.print(Sec_time);
      }
      lcd.print(":");
      lcd.print(Milis_time);
      delay(2000);



      best_lap = laps[0][3];
      for (int i = 1; i < LAPS_NUM; i++) {
        if (laps[i][3] < best_lap) {
          best_lap = laps[i][3];
        }
      }
      Actual_time = best_lap;
      Sec_time = Actual_time % 1000;
      Sec_time = (Actual_time - Sec_time) / 1000;
      Min_time = Sec_time % 60;
      Min_time = (Sec_time - Min_time) / 60;
      Milis_time = Actual_time - Sec_time * 1000;
      Sec_time = Sec_time - Min_time * 60;
      lcd.setCursor(0, 1);
      lcd.print("Best:  ");
      if (Min_time < 10) {
        lcd.print(0);
        lcd.print(Min_time);
      }
      else {
        lcd.print(Min_time);
      }
      lcd.print(":");
      if (Sec_time < 10) {
        lcd.print(0);
        lcd.print(Sec_time);
      }
      else {
        lcd.print(Sec_time);
      }
      lcd.print(":");
      lcd.print(Milis_time);
      delay(2000);
    }
  }
}
