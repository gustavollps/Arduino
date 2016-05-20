void S_esquerda_setup() {
  pinMode(TRIG1, OUTPUT);
  digitalWrite(TRIG1, LOW);
  pinMode(ECHO1, INPUT);
  attachInterrupt(ECHO1_INT, ECHO_ISR_ESQ, CHANGE);
}

void ECHO_ISR_ESQ() {
  if (digitalRead(ECHO1)) {
    time_esquerda = micros();
  }
  else {
    delay_esquerda = micros() - time_esquerda;
    S_esquerda_DONE = true;
  }
}

int Get_range_esquerda(){
  int range = delay_esquerda/58;
  constrain(range,0,200);
  return range;
}

void S_esquerda_start() {
  S_esquerda_DONE = false;  
  digitalWrite(TRIG1,1);
  delayMicroseconds(10);
  digitalWrite(TRIG1,0);
}
