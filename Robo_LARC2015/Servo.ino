void Servo_setup(){
  servo_garra.attach(garra_pin);
  servo_pitch.attach(pitch_pin);  
}

void Garra_open(){
  servo_garra.write(garra_open);
}

void Garra_close(){
  servo_garra.write(garra_close);
}

void Pitch_up(){
  servo_pitch.write(pitch_up);
}

void Pitch_down(){
  servo_pitch.write(pitch_down);
}

void Pitch_mid(){
  servo_pitch.write(pitch_mid);
}

void Inicial(){
  Garra_open();
  Pitch_down();
  delay(1000);
}
void Capturar(uint8_t _n_cubo){
  if(_n_cubo == 0){
    Garra_open();
    delay(500);
    Pitch_down();
    delay(500);
    Garra_close();
    delay(500);
    Pitch_up();
    delay(1500);
    Garra_open();
    delay(500);
    Pitch_down();
    delay(1500);  
  }
  if(_n_cubo ==1){
    Garra_open();
    delay(500);
    Garra_close();
    delay(500);
    Pitch_mid();
    delay(500);    
  }  
  
}

void Liberar(uint8_t _n_cubo){
  if(_n_cubo == 1){
    Garra_open();
    Pitch_up();
    delay(1000);
    Garra_close();
    delay(500);
    Pitch_down();
    delay(1000);
    Garra_open();
    delay(500);
    _n_cubo--;
  }else if(_n_cubo == 2){
    Pitch_mid();
    delay(500);
    Pitch_down();
    delay(500);
    Garra_open();
    delay(500);
    Pitch_mid();
    delay(500);
    _n_cubo--;
  }
}
