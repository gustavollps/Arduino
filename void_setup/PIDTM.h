#include "Arduino.h"

class PID {
public:
	float kp, ki, kd, min, max, leitura, error, prevError, saida;
	unsigned long int time, prevTime;
	PID();
	void makeSetPoint(float _sp);
	float setParameters(float _kp, float _ki, float _kd);
	float setOutput(float _leitura);
private:
	float p, i, d, sp;
};