void S_direita_setup() {
  pinMode(TRIG2, OUTPUT);
  digitalWrite(TRIG2, LOW);
  pinMode(ECHO2, INPUT);
  attachInterrupt(ECHO2_INT, ECHO_ISR2, CHANGE);
}

void ECHO_ISR2() {
  if (digitalRead(ECHO2)) {
    time_direita = micros();
  }
  else {
    delay_direita = micros() - time_direita;
    S_direita_DONE = true;
  }
}

int Get_range_direita(){
  int range = delay_direita/58;
  if(range>200){
    range = 200;
  }  
  return range;
}

void S_direita_start() {
  S_direita_DONE = false;
  digitalWrite(TRIG2,1);
  delayMicroseconds(10);
  digitalWrite(TRIG2,0);
}
