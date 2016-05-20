void Motor_setup() {
  pinMode(M1_PWM, OUTPUT);
  pinMode(M1_DIR, OUTPUT);
  pinMode(M2_PWM, OUTPUT);
  pinMode(M2_DIR, OUTPUT);
}

void Motor_esquerda(int _pwm, int dir) {
  analogWrite(M2_PWM, _pwm);
  digitalWrite(M2_DIR, dir);
}

void Motor_direita(int _pwm, int dir) {
  analogWrite(M1_PWM, _pwm);
  digitalWrite(M1_DIR, dir);
}

void Motores_STOP() {
  digitalWrite(M2_PWM, 0);
  digitalWrite(M2_DIR, 0);
  digitalWrite(M1_PWM, 0);
  digitalWrite(M1_DIR, 0);
}

void Motor_test() {
  uint16_t dl = 500;
  Motor_direita(HALF_SPEED, 1);
  Motor_esquerda(FULL_SPEED, 1);
  delay(dl);
  Motor_direita(HALF_SPEED, 0);
  Motor_esquerda(HALF_SPEED, 0);
  delay(dl);
  Motor_direita(HALF_SPEED, 1);
  Motor_esquerda(HALF_SPEED, 0);
  delay(dl);
  Motor_direita(HALF_SPEED, 0);
  Motor_esquerda(HALF_SPEED, 1);
  delay(dl);
  Motores_STOP();
}

bool Move_angle(int angle_to) {  
  
  if (angle_ > angle_to + histerese_angulo) {
    Motor_direita(HALF_SPEED, 0);
    Motor_esquerda(HALF_SPEED, 1);
    return false;
  }
  else if (angle_ > angle_to + histerese_angulo_f) {
    Motor_direita(LOW_SPEED, 0);
    Motor_esquerda(LOW_SPEED, 1);
    return false;
  }
  else if (angle_ < angle_to - histerese_angulo) {
    Motor_direita(HALF_SPEED, 1);
    Motor_esquerda(HALF_SPEED, 0);
    return false;
  }
  else if (angle_ < angle_to - histerese_angulo_f){
    Motor_direita(LOW_SPEED, 1);
    Motor_esquerda(LOW_SPEED, 0);
    return false;
  }

  if ( (angle_ < angle_to + histerese_angulo_f) && (angle_ > angle_to - histerese_angulo_f) )
  {
    Motores_STOP();
    return true;
  }
}

bool Proxima_area(uint8_t _AREA_atual){

//    Serial.println("Proxima area");
  if(_AREA_atual == 0){
    T_AREAS = T_A0_A1;
  }else if(_AREA_atual == 1){
    T_AREAS = T_A0_A2;
  }else if(_AREA_atual == 2){
    T_AREAS = T_A0_A3;
  }else if(_AREA_atual == 3){
    T_AREAS = T_A0;
  }
  
//    Serial.println("---------------- AREA 0");
  if(!completo_b[0]){   //virar para borda      
    Pitch_mid();
    if(Move_angle(-180)){
//        Serial.println("---------------- Virar borda");
      completo_b[0] = true;        
      tempo_re = millis();
    }      
  }
  
  if(completo_b[0] && !completo_b[1]){  //mover ate a borda
//      Serial.println("---------------- Ate a borda");
    if(!caindo){
      Motor_direita(LOW_SPEED,1);
      Motor_esquerda(LOW_SPEED,1);        
    }else{
      Motores_STOP();
      completo_b[1]=true;
      tempo_re = millis();
    }
  }

  if(completo_b[1] && !completo_b[2]){  //re por tempo definido ate area selecionada
//      Serial.println("---------------- Re ate area");
    if(millis()- tempo_re < T_AREAS){ // MODFIQUEI PARA T_A0_A1 POIS EH A MOVIMENTACAO DA AREA 0 PARA 1. (Antes estava T_A0_A1)
      Motor_direita(LOW_SPEED, 0); // ACHO QUE SE DEIXAR LOW SPEED VAMOS PERDER MUITO TEMPO E NAO TEM PERGIGO DO ROBO CAIR
      Motor_esquerda(LOW_SPEED, 0);
    }
    else{
      Motores_STOP();
      completo_b[2] = true; //AQUI NAO SERIA completo_b[2]?
    }
  }      

  if(completo_b[2] && !completo_b[3]){      //virou para beirada      
    if(Move_angle(90)){
     completo_b[3] = true;            
    }
  }

  if(completo_b[3] && !completo_b[4]){  //avança ate beirada
    if(!caindo){
      Motor_direita(LOW_SPEED,1);
      Motor_esquerda(LOW_SPEED,1);
    }
    else{
      completo_b[4] = true;
      tempo_re = millis();
    }
  }

  if(completo_b[4]){  //retorno para centro da arena
    Pitch_down();
    if(millis()-tempo_re < T_R_CENTRO){
      Motor_esquerda(LOW_SPEED,0);
      Motor_direita(LOW_SPEED,0);
      return false;
    }else{
      Motores_STOP();            
      condicao_inicial();
      return true;
    }     
  }
  else{return false;}    
  
}