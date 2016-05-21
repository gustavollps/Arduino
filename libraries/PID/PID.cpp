#include "PID.h"

PID::PID(){
	//PID constructor
	setPoint = 0;
}

void PID::start(){
	OldTime = micros();
}

void PID::setParameters(double _kp,double _ki, double _kd, double _pid_min, double _pid_max){
	kp = _kp;
	ki = _ki;
	kd = _kd;	
	pid_min = _pid_min;
	pid_max = _pid_max;
}

void PID::setParameters(double _kp, double _ki, double _kd){
	kp = _kp;
	ki = _ki;
	kd = _kd;	
}

void PID::changeSetPoint(double _setPoint){
	setPoint = _setPoint;
}

void PID::limitI(double _i_min, double _i_max){
	i_min = _i_min;
	i_max = _i_max;
}

void PID::resetI(){
	I = 0.0;
}

double PID::Compute(double read){

	NewTime = micros();
	dTime = ((double)(OldTime - NewTime))/1000000.0;
	double error = setPoint - read;
	dError = oError-error;

	P = error*kp;

	I += constrain(((error+oError)/2.0*dTime)*ki,pid_min,pid_max);	
	if(i_min < i_max){
		I = constrain(I,i_min,i_max);
	}

	D = dError/dTime*kd;

	oError = error;
	_PID = P+I+D;
	_PID = constrain(_PID,pid_min,pid_max);
	return _PID;

}