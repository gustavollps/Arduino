#include "PIDTM.h"

PID::PID(){
	p = 0;
	i = 0;
	d = 0;
	kp = 0;
	ki = 0;
	kd = 0;
	error = 0;
	prevError = 0;
	time = 0;
	prevTime = 0;
	min = -100;
	max = 100;
}

void PID::makeSetPoint(float _sp) {
	sp = _sp;
}

float PID::setParameters(float _kp, float _ki, float _kd) {
	kp = _kp;
	ki = _ki;
	kd = _kd;
}

float PID::setOutput(float _leitura) {
	leitura = _leitura;
	prevError = error;
	error = leitura-sp;
	prevTime = time;
  error = map(error,-7000,7000,-100,100);
	time = micros();
	p = kp * error;
	i += ki * (error + prevError) * (double)(time-prevTime)/ 2000000.0;
	i = constrain(i,min,max);
	d = kd * (error-prevError)/(time-prevTime);
	saida = constrain (p+i+d,min,max);	
	return saida;
}
