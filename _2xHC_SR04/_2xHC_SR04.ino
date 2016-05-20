#include <Loop.h>
#include "config.h"

Loop Debug(50, "Debug");
Loop Sensor_loop(30, "Sensor");

bool S_esquerda_DONE = false;
unsigned long int time_esquerda = 0;
unsigned long int delay_esquerda = 0;

bool S_direita_DONE = false;
unsigned long int time_direita = 0;
unsigned long int delay_direita = 0;

void setup() {
  S_esquerda_setup();
  S_direita_setup();
  S_esquerda_start();
  S_direita_start();
  //Debug.enable_Frequency_Monitor();
  Serial.begin(38400);
}

int Range_esquerda = 0;
int Range_direita = 0;

void loop() {
  if (Sensor_loop.ok()) {
    if (S_esquerda_DONE) {
      Range_esquerda = Get_range_esquerda();
      S_esquerda_start();
    }
    if (S_direita_DONE) {
      Range_direita = Get_range_direita();
      S_direita_start();
    }
  }
  if (Debug.ok()) {
    Serial.print("Esquerda: ");
    Serial.print(Range_esquerda);
    Serial.print("\t Direita: ");
    Serial.println(Range_direita);
  }
}
