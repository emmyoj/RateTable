#include <string.h>
#include <errno.h>
#include <stdlib.h>
#include <wiringPi.h>
#include <iostream>
#include <softPwm.h>
#include <time.h>
#include "pid.h"
#include "encoder.h"
#include <algorithm>
using namespace std;

pid::pid(
		double dt1, //delta time = change in time
		double maxVal1, // max PID value 
		double minVal1, // min PID value
		double kp1, //proportional gain 
		double ki1, //integral action time
		double kd1, //derivative time constant
		double preError1
		)
{
	dt = dt1;
	maxVal = maxVal1;
	minVal = minVal1;
	kp = kp1;
	ki = ki1;
	kd = kd1;
	preError = preError1;
	integral = 0;
}

double pid::setPWM(double target, double speed)
{
	error = target - speed; // error = set point - process variable

	integral = integral + error * dt; // new integral = previous integral + error * change in time
	if(integral *ki > 3 || integral * ki < -3)
		integral =0;
	double der = (error - preError) / dt; // derivative = (error - previous error) / change in time
	//D output, I output, P output
	double output = der * kd + integral * ki + kp * error; // controller output = derivative * derivative time const + integral * integral time const + proportional gain * error
	preError = error;
	if(output > maxVal)output = maxVal;
	if(output < minVal)output = minVal;
	return output;
}
//pwmval is the pwm value we use as output
//the return value represents the direction of the motor rotation
//true means CW (clockwise), false means CCW (counter-clockwise)
