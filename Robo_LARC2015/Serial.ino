bool xget = false;
bool yget = false;

bool stop = false;
int tempx = 0;


bool serial_getdata() {
  char coordx[3];
  char coordy[3];
  char flag = 'n'; //dado nulo

  if (Serial3.available()) {
    unsigned long time_t = millis();
    while (Serial3.available() && millis() - time_t < 10) {
      flag = Serial3.read();

#ifdef DEBUG_S_ALL
      Serial.print(flag);
#endif
      if (flag == 'x' || flag == 'y') {

#ifdef DEBUG_S_ALL
        Serial.println();
#endif

        break;
      }
    }
    stop = false;
    if (flag == 'x') {
      for (int i = 0; i < 3; i++) {
        unsigned long time = millis();
        while (!Serial3.available()) { //waits for Serial3 data for a defined time
          if (millis() - time > T_SERIAL) {
            stop = true;
            break;
          }
        }
        if (!stop) {
          coordx[i] = Serial3.read();
#ifdef DEBUG_S_ALL
          Serial.println(coordx[i]);
#endif
          if (i == 2) {
            xget = true;
          }
        }
      }
      tempx = atoi(coordx);
      //Serial.print(tempx);
      //Serial.print("x - ");
      x_point = tempx;
    }
    else if (flag == 'y') {
      for (int i = 0; i < 3; i++) {
        unsigned long time = millis();
        while (!Serial3.available()) { //waits for Serial3 data for a defined time
          if (millis() - time > T_SERIAL) {
            stop = true;
            break;
          }
        }
        if (!stop) {
          coordy[i] = Serial3.read();
#ifdef DEBUG_S_ALL
          Serial.println(coordy[i]);
#endif
          if (i == 2) {
            yget = true;
          }
        }
      }
      y_point = atoi(coordy);
      //    Serial.print(atoi(coordy));
      //    Serial.print("y - ");
    }
    else {
      opencv = false;
    }
    if (!stop && xget && yget) {
      data_ready = true;
      xget = false;
      yget = false;
      //      x_point = tempx;
      //      y_point = atoi(coordy);
      opencv = true;
      x = x_point;
      y = y_point;
#ifdef SERIAL_DEBUG
      Serial.print(x_point);
      Serial.print(" - ");
      Serial.println(y_point);
#endif
    }
  }
  return data_ready;
}

void Get_coord(int _x_point, int _y_point) {
  data_ready = false;
  _x_point = x_point;
  _y_point = y_point;
}
