#include "Arduino.h"

class PID
{
public:
	PID();
	void start();
	void setParameters(double _kp,double _ki, double _kd, double _pid_min, double _pid_max);
	void setParameters(double _kp,double _ki, double _kd);
	double Compute(double read);
	void resetI();
	double getP();
	double getI();
	double getD();
	void limitI(double _i_min, double _i_max);
	void changeSetPoint(double _setPoint);
	double P;
	double I;
	double D;
	double kp;
	double ki;
	double kd;
	double _PID;

private:
	
	unsigned long NewTime;
	unsigned long OldTime;
	double dTime;
	double dError;
	double oError;
	double setPoint;
	double pid_max;
	double pid_min;
	double i_min = 0;
	double i_max = 0;
};
