#include "HCSR04.h"
#include "Arduino.h"

HCSR04::HCSR04(int _trigger, int _echo){
  trigger = _trigger;
  pinMode(_trigger,OUTPUT);
  #ifdef ARDUINO_MEGA2560
    switch _echo{
      case 2:
        interrupt = 0;
        break;
      case 3:
        interrupt = 1;
        break;     
      case 18:
        interrupt = 5;
        break;
      case 19:
        interrupt = 4;
        break;      
      case 20:
        interrupt = 3;
        break;
      case 21:
        interrupt = 2;
        break;
      default:
        interrupt = 0;
        break;      
    }
  #endif
  #ifdef ARDUINO_UNO
    switch _echo{
      case 2:
        interrupt = 0;
        break;
      case 3:
        interrupt = 1;
        break;
      default:
        interrupt = 0;
        break;       
    }
  #endif
  attachInterrupt(interrupt,ISRoutine,CHANGE);  
}

void HCSR04::ISRoutine(){ //Interruppt sub routine
  switch(digitalRead(interrupt)){
    case HIGH:
      t_init = micros();
    case LOW:
      t_final = micros();

    default:      
      break;
  }
}


