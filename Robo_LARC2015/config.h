//Definiçes de sensores ultrassonicos ------------------------
#define TRIG1 22
#define ECHO1 18
#define ECHO1_INT 5

#define TRIG2 24
#define ECHO2 19
#define ECHO2_INT 4

#define DIST_GND 14

//-------------------------Motores ----------------------------

#define M1_PWM 3
#define M1_DIR 5

#define M2_PWM 2
#define M2_DIR 4

#define STOP 0
#define LOW_SPEED 100
#define HALF_SPEED 120
#define FULL_SPEED 255

//----------------------- Serial -----------------------------

#define T_SERIAL 10 //ms


//----------------------- Servos -----------------------------

#define garra_pin 9
#define garra_open 145
#define garra_close 50

#define pitch_pin 10
#define pitch_up 0
#define pitch_mid 90
#define pitch_down 174

//----------------------- Captura -----------------------------

#define histerese_angulo 20
#define histerese_angulo_f 1
#define CAPTURE_DELAY 3000 //limite de tempo para andar para frente e fazer a captura

//------------------------ BUSCA ------------------------------

#define T_A0 700
#define T_A0_A1 3000
#define T_A0_A2	4750
#define T_A0_A3	6500
#define T_R_CENTRO 2400	
#define CENTRA_BALSA 1350
#define DIST_BALSA 7

//------------------------ OPENCV -----------------------------

#define histerese 1
#define fator_conversor_pixels 100
#define angulo_visao 60
#define fator_f angulo_visao/fator_conversor_pixels
#define OBJ_PERTO 7
#define centro_x 56

//#define SERIAL_DEBUG
//#define DEBUG
//#define DEBUG_S_ALL
//#define DEBUG_ANGLES
//#define TEST
//#define DEBUG_CAPTURA
//#define DEBUG_CAP_DEV