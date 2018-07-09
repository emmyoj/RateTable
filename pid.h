#ifndef PID_H_
#define  PID_H_


class pid
{
private:
	double dt;
	double maxVal;
	double minVal;
	double kp;
	double ki;
	double kd;
	double integral;
	double error;
	double preError;
public:
	pid(double dt1, double maxVal1, double minVal1, double kp1, double ki1, double kd1,double preError1 = 0);
	double setPWM(double target, double speed);
	bool setPosition(double targetPosition, double currentPosition, int& pwmVal, int dirPin); // in degree
};



#endif
