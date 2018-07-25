#include <string.h>
#include <errno.h>
#include <stdlib.h>
#include <wiringPi.h> 
#include <iostream>
#include <iomanip>
#include <softPwm.h>
#include <time.h>
#include "pid.h"
#include "encoder.h"
using namespace std;

#define channelA 4 //4 in wiringPi = 16 in RasPi. Define as CLK (blue line)
#define channelB 5 //5 in wiringPi = 18 in RasPi Define as DT (white line)

int main(int argc, char** argv)
{
	const int PWM_RANGE = 3;//4
	const double MAX_VAL = 15;
	const double MIN_VAL = -15;
	const double KP = 0.5;//0.5
	const double KI = 0.01;
	const double KD = 0;
	int pwmPin = 0; // 0 in wiringPi = 11 in RasPi
	int range = 100; //10000 / frequency. change based on frequency
	int dirPin = 2; // 2 in wiringPi = 13 in RasPi
	int pwmVal = 0; // (duty cycle * range)/100
	double pos_error;
	float input_range = 0.1; //estimated error range for final position. User input
	double error; // Target position - current position
	double sampleTime = 0.1; // sec? or micro seconds?
	double startTime = clock();
	int target_position = 90;//rpm ? 180
	double controlTime;


	cout << "start the program" << endl;

	cout << "You have entered " << argc << " arguments:" << "\n";
	cout << argv[1][2] << "\n";

	target_position = atof(argv[1]);
	input_range = atof(argv[2]); 
	cout << "target_position: "<< target_position << "\n";
	cout << "error range:  " << input_range << "\n";
	cout << "encoder position: " << encoder.position << "\n";
	//if argc is empty, set speed to default value

	//	exp_time = atoi(argv[2])*1000000;

	//	cout << "experiment time: " << exp_time << "\n";
	if(wiringPiSetup() < 0){
		//cout << "fail now" << endl;
		fprintf(stderr, "Unable to initialize wiringPi:%s\n",strerror(errno));
		return 1;
	}
	setUpEncoder(channelA, channelB);
	softPwmCreate (pwmPin, pwmVal, range);
	digitalWrite(dirPin, LOW) ;
	pinMode(dirPin, OUTPUT) ;

	//double dt, double maxVal, double minVal, double kp, double ki, double kd,double preError = 0

	pid pid_(sampleTime, MAX_VAL, MIN_VAL, KP, KI, KD);


	error = target_position - encoder.position;
	while (abs(error) >= input_range)
	{
		controlTime = clock() - startTime;
		if(controlTime >= sampleTime * CLOCKS_PER_SEC)
		{
			startTime += controlTime;
			pwmVal = pid_.setPWM(target_position, encoder.position);
			softPwmWrite (pwmPin, abs(pwmVal) + PWM_RANGE);
			cout << "pwm " << pwmVal << endl;
			cout << "angle: " << encoder.position << "\n";
			cout << "target angle: " << target_position <<"\n";
			if(pwmVal > 0)
			{
				digitalWrite(dirPin, LOW);
				cout << "direction: CW" << endl;
				//error = target_position - encoder.position;
			}
			else
			{
				digitalWrite(dirPin, HIGH);
				cout << "direction: CCW" << endl;
				//error = target_position - encoder.position;
			}
			error  = target_position - encoder.position;
		}
		pwmVal = 0;
	}
	cout <<"\n" << "final position: " << encoder.position << "\n";
	//	cout << "target position: " << target_position + input_range << "\n";
	if (encoder.position >= target_position)
	{
		pos_error = (target_position + input_range) - encoder.position;
		cout << "target position: " << target_position + input_range << "\n";
	}
	else
	{
		pos_error = (target_position - input_range) - encoder.position;
		cout << "target position: " << target_position - input_range << "\n";
	}
	cout << fixed << setprecision(6) << "positional error = " << pos_error << "\n";
	return 0;
}

