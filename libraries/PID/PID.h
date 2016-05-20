#include "Arduino.h"

class PID
{
public:
	PID();
	void start();
	void setParameters(float _kp,float _ki, float _kd, float _pid_min, float _pid_max);
	void setParameters(float _kp,float _ki, float _kd);
	float Compute(float read);
	void resetI();
	float getP();
	float getI();
	float getD();
	void limitI(float _i_min, float _i_max);
	void changeSetPoint(float _setPoint);
	float P;
	float I;
	float D;
	float kp;
	float ki;
	float kd;
	float _PID;

private:
	
	unsigned long NewTime;
	unsigned long OldTime;
	double dTime;
	double dError;
	double oError;
	float setPoint;
	float pid_max;
	float pid_min;
	float i_min = 0;
	float i_max = 0;
};
