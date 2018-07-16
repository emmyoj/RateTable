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
	const int PWM_RANGE = 4;
	const double MAX_VAL = 15;
	const double MIN_VAL = -15;
	const double KP = 0.5;
	const double KI = 0.01;
	const double KD = 0;
	int pwmPin = 0; // 0 in wiringPi = 11 in RasPi
	int range = 100; //10000 / frequency. change based on frequency
	int dirPin = 2; // 2 in wiringPi = 13 in RasPi
	int pwmVal = 0; // (duty cycle * range)/100
	double pos_error;
	double error = 0.1; //estimated error range for final position
	//float float_error;
	//int speed = encoder.speed;
	double sampleTime = 0.1; // sec? or micro seconds?
	double startTime = clock();
	double target_position = 90;//rpm ? 180
	//	int exp_time = 0;// user input time experiment will run... without conversion, user input needs to be in microseconds >= 1000000
	//	int target_time = 0; //exp_time + clock()
	double controlTime;


	//wiringPiSetupGpio();
	cout << "start the program" << endl;

	cout << "You have entered " << argc << " arguments:" << "\n";
	//cout << argv[1][2] << "\n";

	target_position = atof(argv[1]);
	error = atof(argv[2]); //atof?
	//float_error = (float)error / 1000;
	//	printf("%f ", error);
	cout << "target_position: "<< target_position << "\n";
	cout << "error:  " << error << "\n";
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

	//	target_time = exp_time + clock();
	//cout << "target time: " << target_time << "\n";

	while (encoder.position <= (target_position + error))//+- error 
	{
		controlTime = clock() - startTime;
		if(controlTime >= sampleTime * CLOCKS_PER_SEC)
		{
			startTime += controlTime;
			pwmVal = pid_.setPWM(target_position, encoder.position);
			softPwmWrite (pwmPin, abs(pwmVal) + PWM_RANGE);
			cout << "pwm " << pwmVal << endl;
			if(pwmVal > 0)
			{
				digitalWrite(dirPin, LOW);
				cout << "direction: CW" << endl;
			}
			else
			{
				digitalWrite(dirPin, HIGH);
				cout << "direction: CCW" << endl;
			}
			//if user input = space bar or enter, stop motor 
		}

	}
	cout <<"\n" << "final position: " << encoder.position << "\n";
	pos_error = encoder.position - (target_position+error);
	cout << fixed << setprecision(4) << "positional error = " << pos_error << "\n";
	return 0;
}

