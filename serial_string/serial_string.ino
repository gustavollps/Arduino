void setup() {
  Serial.begin(9600); //serial do pc
  Serial1.begin(9600); //serial do bluetooth
  while (Serial1.read() != 'x');
  Serial.println("First Char ok");
}

char data;
char temp[8];
String x_coord = "";
unsigned int x_int;
String y_coord = "";
unsigned int y_int;
uint8_t count = 0;


void loop() {
  count  = 0;
  if (Serial1.available()) {
    while (1) { //x
      while (!Serial1.available()); //waits for serial data
      temp[count] = Serial1.read();
      //Serial.print(temp[count]);
      if (temp[count] == 'y') {
        //convert x string caracters to int;
        for (int i = 0; i < count; i++) {
          x_coord += temp[i];  //load x_coord with x coordinate caracters
        }
        x_int = x_coord.toInt();
        break;
      }
      count++;
    }
    
    /*
    Serial.print(" | ");
    Serial.print(x_coord);
    Serial.print(" | ");
    */
    x_coord = ""; //clear string;

    count = 0;

    while (1) { //y
      while (!Serial1.available());
      temp[count] = Serial1.read();
      //Serial.print(temp[count]);
      if (temp[count] == 'x') {
        //convert y string characters to int
        for (int i = 0; i < count; i++) {
          y_coord += temp[i];
        }
        y_int = y_coord.toInt();
        break;
      }
      count++;
    }
    /*
    Serial.print(" | ");
    Serial.print(y_coord);
    Serial.print(" | ");
    */
    
    y_coord = "";
    
    count = 0;
    
    Serial.print("x - ");
    Serial.print(x_int);
    Serial.print("\t y - ");
    Serial.println(y_int);
    
  }
}
