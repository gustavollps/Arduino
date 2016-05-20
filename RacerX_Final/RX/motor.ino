void motor(int dire, int vel){
  if(dire==1){
    analogWrite(6,vel);
    digitalWrite(5,0);
  }
  if(dire==2){
    analogWrite(5,vel);
    digitalWrite(6,0);
  }
  if(dire==3){
    digitalWrite(6,0);
    digitalWrite(5,0);
  }
}

