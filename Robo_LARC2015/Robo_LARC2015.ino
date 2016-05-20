#include <Loop.h>
#include "config.h"
#include <Servo.h>

typedef union {
  float floatingPoint;
  byte binary[4];
} binaryFloat;

// ----------------------- OBJETOS --------------------------
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

Loop Debug(5, "Debug");
Loop Sensor_loop(20, "Sensor");
Loop Serial_Opencv(50, "Opencv");
Loop Motor_loop(50, "Motor");
Loop Gyro_loop(100, "Gyro");
Loop luz_loop(50, "Luz");
Loop Controle_loop(50, "Controle");

Servo servo_garra;
Servo servo_pitch;

// ------------------------ ULTRA ---------------------------
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

bool S_esquerda_DONE = false;
unsigned long int time_esquerda = 0;
unsigned long int delay_esquerda = 0;

bool S_direita_DONE = false;
unsigned long int time_direita = 0;
unsigned long int delay_direita = 0;

uint8_t Range_esquerda = 0;
uint8_t Range_direita = 0;

bool CAI_esquerda = false;
bool CAI_direita  = false;

// ---------------------- BLUETOOTH -------------------------
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

bool opencv = false;
bool data_ready = false;

int x_point = 0;
int y_point = 0;
int x = 0;
int y = 0;
bool catch_cube = false;

//-------------------------- GYRO ---------------------------
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

float gyro_angle;
float angle_offset;
binaryFloat angle; //armazena angulo e bytes
float angle_ = 0; //armazena angulo atual do gyro


// ------------------------ CAPTURA -------------------------
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

uint8_t n_cubo = 0;

//------------------------------------------------------------------------------------------------------------------------------------------------------------------------------


bool nao_entrega = false;
bool rr = false;  //flag para reconhecimento de objeto pela opencv
bool centralizado = false;  //flag que indica cubo centralizado durante captura
bool trig_varredura = false;  //gatilho para varredura ao primeiro objeto encontrado
bool varredura = true;  //flag de ativaço da varredura
bool captura = false;  //flag de ativaço da captura
bool capturado = false;  //flag de ativacao em meio ao processo de captura (antes da captura em si)
bool tempo_retorno = false;  //flag para  captura do momento inicial do retorno
bool trig_timer_captura = false; //gatilho para gravar tempo da primeira centralizacao
bool time_up = false;  //flag de tempo limite
bool entrega = false; //entrega dos cubos para a balsa
bool reposicionar = false; //flag reposicionar para robo quase caindo
bool balsa = false; //balsa a frente

//------------------------------------------------------------BUSCA
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

bool completo_b[10] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
unsigned long int tempo_re = 0;

bool busca = false;
bool caindo = false;
bool AREAS[4] = {0, 0, 0, 0};
uint8_t AREA_atual = 4;
bool completo[10] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0};  //varredura (etapas)
unsigned int T_AREAS = 0;

bool luz_cortada; // flag para fechamento da garra quando o cubo corta a luz no LDR
bool set_luz = true;
unsigned int limiar_luz = 350;

unsigned long int limite_captura = 500; //ms
unsigned long int decorrido_captura = 0; //ms
unsigned long int tempo_entrega = 0;

int contador = 0;

uint16_t objetos[4];
uint16_t objeto_proximo  = 255;

float angulo_objeto = 0;
void condicao_inicial(){
  rr= false;
  captura=false;
  capturado=false;
  trig_varredura=false;
  trig_timer_captura = false;
  varredura = false;
  tempo_retorno = false;
  centralizado = false;
  time_up = false;
  entrega = false;
  reposicionar = false;
  balsa = false;
  caindo = false;
  busca = false;  
  for(int i=0;i<10;i++){
    completo[i] = false;
  }
  for(int i=0;i<10;i++){
    completo_b[i] = false;
  }
}

void setup() {    

  S_esquerda_setup();
  S_direita_setup();
  S_esquerda_start();
  S_direita_start();

  Motor_setup();
  Servo_setup();

  //Gyro_loop.enable_Frequency_Monitor();

#ifdef DEBUG
  Serial.begin(38400);
#endif

  Serial2.begin(38400);
  Serial3.begin(9600);

#ifdef DEBUG_CAP_DEV
  Pitch_down();
  Garra_open();
  while(!Serial.available());
  Serial.read();
  Capturar(0);
  while(!Serial.available());
  Serial.read();
  Liberar(1);
  while(!Serial.available());
  Serial.read();
#endif

  Inicial();
#ifdef TEST
  Capturar(1);
  Liberar(1);
  Motor_test();
  while (!Serial.available());
  Serial.read();
  Capturar(0);
  while (!Serial.available());
  Serial.read();
  Capturar(1);
  while (!Serial.available());
  Serial.read();
  Liberar(2);
  while (!Serial.available());
  Serial.read();
  Liberar(1);
#endif
  
  limiar_luz = analogRead(A5)*0.8;
  while(analogRead(A5) > limiar_luz);  

}

void loop() {

//------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
//------------------------------------------------ GYRO ------------------------------------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

  //atualizaço dos dados do giroscopio
  if (Gyro_loop.ok()) {
    angle_ = get_Angle();
  }

//------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
//------------------------------------------------ LUZ -------------------------------------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
  //atualizaço do sensor de luz

  if(luz_loop.ok()){
    if(analogRead(A5) > limiar_luz){
      luz_cortada  = false;
    }
    else{
      luz_cortada = true;
    }
  }

//------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
//----------------------------------------------- OPENCV -----------------------------------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
  //processo de recebimento de dados da serial, quando uma coordenada completa eh recebida via bluetooth

  if (Serial_Opencv.ok()) {
    if (serial_getdata()) {
      Get_coord(x, y);
      rr = true;
      contador = 0;
      
      if (trig_varredura) {
        varredura = true;
        trig_varredura = false;
      }
    } 
    else {
      contador++;
      if(contador > 10){
        rr = false;        
      }      
    }
  }

  if(Controle_loop.ok()){    
  //------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
  //----------------------------------------------- VARREDURA --------------------------------------------------------------------------------------------------------------------
  //------------------------------------------------------------------------------------------------------------------------------------------------------------------------------    
    //processo em que o robo vai para o angulo -80 e faz uma varredura ate 80 (pelo campo de visao da camera de 60 graus)
    //guardando o angulo em que recebeu a menor coordenada y, portanto o cubo mais proximo;

    if(AREA_atual == 3){
      if(AREAS[0] && AREAS[1]  && AREAS[2] && AREAS[3]){        
        
      }
    }


    if (varredura && n_cubo < 2) {
      if (!completo[1] && !completo[2]) {
        completo[0] = Move_angle(-80);
      }  
      if (completo[0]) { //moveu para posicao -80
        completo[1] = true;
        completo[0] = false;
        y = 200; //y alto para pegar qualquer coisa mais baixa e verificar se viu algo
      }
      if (completo[1]) { //movendo para posicao 80
        completo[2] = Move_angle(80);
        if (y < objeto_proximo && y != 0 && x != 0) {
          Serial.println("OBJETO RECONHECIDO --------------------------------------------------");
          objeto_proximo = y;
          angulo_objeto = angle_ - ((x - 53) * fator_f);
        }
      }
      if (completo[2]) { //moveu para 80
        completo[1] = false;
        if (Move_angle(angulo_objeto)) {
          //varredura = false;
          condicao_inicial();
          if(!AREAS[AREA_atual] && y != 200){
            captura = true;
          }        
          if(y == 200){ //sem objeto reconhecido            
            AREAS[AREA_atual] = true;
            busca = true;
            if(AREA_atual == 3){
              entrega = true;
              nao_entrega = true;
              AREAS[0] = false;
              AREAS[1] = false;
              AREAS[2] = false;
              AREAS[3] = false;
            }
          }
        }
      }
      if(AREA_atual == 3 && y == 200){ //area final e sem cubos
        //ACABOU TUDO, enrolar ou nao
        //Motores_STOP();
      }

    }

  //------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
  //------------------------------------------------- BUSCA ----------------------------------------------------------------------------------------------------------------------
  //------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
    //processo de movimentacao para nova area quando nao foram encontrados novos cubos na area atual

    if (busca) {    
      if(AREAS[AREA_atual]){   //area atual livre
        if(Proxima_area(AREA_atual)){        
          if(AREAS[0]){
            AREA_atual = 1;  
          }
          if(AREAS[1]){
            AREA_atual = 2;            
          }
          if(AREAS[2]){
            AREA_atual = 3;
          }    
          if(AREAS[3]){
            AREA_atual = 0;
          }
        }      
        busca = true;      
      }
      else{
        varredura = true;   //area com cubos
        busca = false;
      }
    }

  //------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
  //------------------------------------------------- ENTREGA -------------------------------------------------------------------------------------------------------------------
  //------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
  //processo de movimentacao para nova area quando nao foram encontrados novos cubos na area atual ou foram pegos 2 cubos

    if(entrega){

      if(!completo[0]){    //vira para beirada
        if(Move_angle(-180)){
          completo[0]= true;        
        }      
      }

      if(completo[0] && !completo[1]){  //vai para beirada
        if(!caindo){
          Motor_esquerda(HALF_SPEED,1);
          Motor_direita(HALF_SPEED,1);
        }
        else{
          //ajustar();
          Motores_STOP();
          completo[1]=true;
          tempo_entrega = millis();
        }
      }

      if(completo[1] && !completo[2]){  //retorno beirada
        if(millis() - tempo_entrega < CENTRA_BALSA){
          Motor_direita(HALF_SPEED,0);
          Motor_esquerda(HALF_SPEED,0);        
        }
        else{
          Motores_STOP();
          completo[2]=true;
        }
      }

      if(completo[2] &&  !completo[3]){ //vira para a balsa
        if(Move_angle(-90)){
          completo[3]=true;
        }
      }

      if(completo[3] && !completo[4]){  //vai para beirada  
        if(!balsa && !caindo){
          Motor_esquerda(LOW_SPEED*0.8,1);
          Motor_direita(LOW_SPEED*0.8,1);
        }else if(caindo && !nao_entrega){
          Motores_STOP();        
        }
        else if(balsa && !nao_entrega){
          Motores_STOP();
          completo[4]=true;
        }      
        else{
          completo[4] = true;
          completo[5] = true;
          tempo_entrega = millis();
        }
      }

      if(completo[4] && !completo[5]){  //liberar cubos                
        completo[5] = true;      
        if(n_cubo == 2){
          Liberar(2);
          Liberar(1);
        }
        else{
          Liberar(1);        
        }
        tempo_entrega = millis();
      }

      if(completo[5]){  //retornar
        if(millis() - tempo_entrega < T_R_CENTRO){
          Motor_direita(HALF_SPEED,0);
          Motor_esquerda(HALF_SPEED,0);
        }
        else{
          Motores_STOP();
          condicao_inicial();                  
          AREA_atual = 0;
          varredura = true;
          n_cubo = 0;        
        }
        nao_entrega = false;

      }
      
    }  

  //------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
  //----------------------------------------------- CAPTURA ----------------------------------------------------------------------------------------------------------------------
  //------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

  //processo de captura de cubos executado quando um cubo esta sendo visualizado, ainda nao tem 2 cubos no robo e captura foi chamada

    if(captura){
      if (capturado || (!varredura && n_cubo <= 2 && captura)) {  //resolver problema sem objeto reconhecido e andando para frente sempre
        if (!capturado) {
    #ifdef DEBUG_CAPTURA
          Serial.println(F("CORRIGINDO"));
    #endif

          if(set_luz){
            limiar_luz = analogRead(A5)*0.8;
            set_luz = false;
          }
          if(rr && !caindo){                  
            if (x < centro_x - histerese && !luz_cortada) {
              Motor_direita(LOW_SPEED, 1);
              //Motor_esquerda(LOW_SPEED,0);
              centralizado = false;
            }
            else if (x > centro_x + histerese && !luz_cortada) {
              centralizado = false;
              Motor_esquerda(LOW_SPEED, 1);
              //Motor_direita(LOW_SPEED,0);
            }
            else {
              if (!centralizado) {                    
                Motores_STOP();
              }
              centralizado = true;        
            }

            /*if(caindo){
              //captura = false;
              decorrido_captura = millis() - limite_captura;              
              if(decorrido_captura > CAPTURE_DELAY){
                decorrido_captura = CAPTURE_DELAY;
              }
              capturado = true;
              centralizado = false;
              tempo_retorno = true;          
            }*/
          }else if(!caindo && !rr){
            Motor_direita(LOW_SPEED*0.5,1);
            Motor_esquerda(LOW_SPEED*0.5,1);        
          }
          else if(caindo){
            capturado = true;
            centralizado = false;
            tempo_retorno = true;
            reposicionar = true;
            decorrido_captura = millis() - limite_captura;              
            if(decorrido_captura > CAPTURE_DELAY){
                decorrido_captura = CAPTURE_DELAY;
            }
          }
        }

        if (centralizado) {
          if (!trig_timer_captura) {
              limite_captura = millis();
    #ifdef DEBUG_CAPTURA
              Serial.print(F("CENTRALIZOU OBJETO"));
              Serial.println(limite_captura);
    #endif
              trig_timer_captura = true;
          }
          /*if(set_luz){
            limiar_luz = analogRead(A5)*0.8;
            set_luz = false;
          }*/
    #ifdef DEBUG_CAPTURA
          Serial.print(F("CENTRALIZADO - limiar novo:"));
          Serial.println(limiar_luz);
    #endif
          if (!CAI_direita && !CAI_esquerda && !time_up && !luz_cortada) {
            Motor_direita(LOW_SPEED*0.5, 1);
            Motor_esquerda(LOW_SPEED*0.5, 1);
    #ifdef DEBUG_CAPTURA
            Serial.print(CAI_esquerda);
            Serial.print(F(" - Esquerda: "));
            Serial.print(Range_esquerda);
            Serial.print(F(" - "));
            Serial.print(CAI_direita);
            Serial.print(F("\t Direita: "));
            Serial.println(Range_direita);
    #endif
            if (millis() - limite_captura > CAPTURE_DELAY) {
              time_up = true;
              decorrido_captura = CAPTURE_DELAY;
    #ifdef DEBUG_CAPTURA
              Serial.println(F("TIMEUP"));
    #endif
            }
            else {
              decorrido_captura = millis() - limite_captura;
    #ifdef DEBUG_CAPTURA
              Serial.println(F("AVANCANDO"));
              Serial.print(F("Distancia: "));
              Serial.println(y);
    #endif
            }
          }
          else {
            set_luz = true;
            if (CAI_direita && CAI_esquerda) {          
    #ifdef DEBUG_CAPTURA
              Serial.println(F("CAINDO - RETORNO"));
    #endif
              //decorrido_captura = millis() - limite_captura;
              capturado = true;
              centralizado = false;
              tempo_retorno = true;
              reposicionar = true;
              /*capturado = false;
              trig_timer_captura = false;
              trig_varredura = false;
              tempo_retorno = false;
              time_up = false;
              objeto_proximo = 255;
              decorrido_captura = CAPTURE_DELAY;
              captura = false;
              centralizado = false;
              caindo = true;*/
            }
            else {
    #ifdef DEBUG_CAPTURA
              Serial.print(F("CAPTURAR - "));
              Serial.println(millis());        
    #endif
              decorrido_captura = millis() - limite_captura;
              Motores_STOP();
              Capturar(n_cubo);
              n_cubo++;
              capturado = true;
              centralizado = false;
              tempo_retorno = true;          
            }
          }
        }
        if (capturado) { //fazer retorno a posicao de varredura da area
    #ifdef DEBUG_CAPTURA
          Serial.println(F("RETORNO A POSICAO INICIAL DE VARREDURA"));
    #endif
          if (tempo_retorno) {
            limite_captura = millis();  //pega tempo atual
            tempo_retorno = false;
    #ifdef DEBUG_CAPTURA
            Serial.print(limite_captura);
    #endif
          }
          if (millis() - limite_captura < decorrido_captura) {
    #ifdef DEBUG_CAPTURA
            Serial.print(F("Retornando - "));
            Serial.print(decorrido_captura);
            Serial.print(F(" - "));
            Serial.println(millis() - limite_captura);
    #endif
            Motor_esquerda(LOW_SPEED * 0.6, 0);
            Motor_direita(LOW_SPEED * 0.6, 0);
          }
          else {
            #ifdef DEBUG_CAPTURA
              Serial.println(F("RETORNOU"));
            #endif
            Motores_STOP();        
            objeto_proximo = 255;
            decorrido_captura = CAPTURE_DELAY;
            condicao_inicial();                
            if(n_cubo < 2 && !AREAS[AREA_atual]){        
              varredura = true;
            }
            if(n_cubo == 2 || (AREAS[0] && AREAS[1] && AREAS[2] && AREAS[3] && n_cubo != 0)){ //2 cubos ou setores limpos

              entrega = true;
            }

            #ifdef DEBUG_CAPTURA
              Serial.println(F("RESET PARA NOVA CAPTURA"));
            #endif
          }
        }
      }
    }
  }
  
  
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
//----------------------------------------------- SENSORES ULTRA ---------------------------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
  //processo de captura dos sensores ultrassonicos (frequencia limitada para evitar interferencia das ondas)
  if (Sensor_loop.ok()) {
    if (S_esquerda_DONE) {
      Range_esquerda = Get_range_esquerda();
      S_esquerda_start();
    }
    if (S_direita_DONE) {
      Range_direita = Get_range_direita();
      S_direita_start();
    }
    if (Range_esquerda > DIST_GND) {
      CAI_esquerda = true;
    } else {
      CAI_esquerda = false;
    }
    if (Range_direita > DIST_GND) {
      CAI_direita = true;
    } else {
      CAI_direita = false;
    }
    if(Range_direita > DIST_GND || Range_esquerda > DIST_GND){      
      caindo = true;
      balsa = false;
    }
    else{
      caindo = false;
      if(Range_direita < DIST_BALSA || Range_esquerda < DIST_BALSA){
        balsa = true;
      }
    }
  }

//------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
//------------------------------------------------ DEBUG -----------------------------------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

  //print de variaveis relevantes ao processo e medidas de sensores
#ifdef DEBUG
  if (Debug.ok()) {

//------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
//------------------------------------------- Parada de emergencia -------------------------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------------  
    if (Serial.available()) {
      char temp = Serial.read();
      if (temp == '+') {
        varredura = true;
      }
      else if(temp == 'c'){
        varredura  = false;
        captura = true;
        centralizado = true; 
      }
      else{      
        Motores_STOP();
        while (!Serial.available());
      }
    }

    if (rr) {
      Serial.print(F(" X: "));
      Serial.print(x);
      Serial.print(F("\t Y: "));
      Serial.print(y);
      Serial.print(F(" - "));
    }
    else {
      Serial.print(F("Sem coord - \t"));
    }

    Serial.print(angle_);
    Serial.print(F(" - "));
    Serial.print(CAI_esquerda);
    Serial.print(F(" - Esquerda: "));
    Serial.print(Range_esquerda);
    Serial.print(F(" - "));
    Serial.print(CAI_direita);
    Serial.print(F("\t Direita: "));    
    Serial.print(Range_direita);
    Serial.print(F(" N_cubos: "));
    Serial.print(n_cubo);
    Serial.print(F(" Sensor Luz "));
    Serial.print(luz_cortada);
    Serial.print(F(" Leitura "));
    Serial.print(analogRead(A5));
    Serial.print(F(" limiar_luz "));
    Serial.print(limiar_luz);
    Serial.print(F(" GYRO_FREQ "));
    Serial.print(Gyro_loop.get_Frequency());
    Serial.println();
  }
#endif
}