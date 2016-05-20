#include "Arduino.h"
#include "Loop.h"

Loop::Loop(int _rate) {
  rate = (float)_rate;
  frequency_monitor = false;
  first_loop = false;
  counter = 1;  
  time = micros();
  last_loop = millis();
  period_us = 1000000.0 / rate;
}

void Loop::Debug_Frequency() {
  if (first_loop && frequency_monitor) {
    //Serial.print(loop_name);
    //Serial.print(" - Frequency:");
    Serial.println(counter);
  }
}

void Loop::set_Loop_Rate(int _rate) {
  rate = _rate;
}

void Loop::enable_Frequency_Monitor() {
  frequency_monitor = true;
}

void Loop::disable_Frequency_Monitor() {
  frequency_monitor = false;
}

bool Loop::ok() {
  base_time = micros();
  if (base_time - time >= period_us) {
    time += period_us;
    if (frequency_monitor) {
      unsigned long int t_time = millis();
      if (t_time - last_loop >= 1000) {
        first_loop = true;
        counter = counter_temp;
        counter_temp = 1;
        last_loop = t_time;
      }
      else {
        counter_temp++;
      }
    }
    return true;
  }
  else {
    return false;
  }
}

unsigned int Loop::get_Frequency() {  //use only in a if(Loop.ok())
  if (frequency_monitor && first_loop) {
    return counter;
  }
  return 0;
}
