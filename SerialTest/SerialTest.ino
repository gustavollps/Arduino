void setup() {
  Serial.begin(9600);

}
char data[10];
void loop() {
  int i = 0;
  
  while(Serial.available()<6);
  
  if (Serial.available() > 6) {
    while (Serial.available()) {
      data[i] = Serial.read();
      i++;
    }
  }
  for(int j=0;j<8;j++){
    Serial.print(data[i],HEX);
  }
  Serial.println(0000000,HEX);
}