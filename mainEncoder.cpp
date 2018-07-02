#include <string.h>
#include <errno.h>
#include <stdlib.h>
#include <wiringPi.h> 
#include <iostream>
#include <softPwm.h>
#include <time.h>
#include "pid.h"
#include "encoder.h"
using namespace std;

#define channelA 4 //4 in wiringPi = 16 in RasPi. Define as CLK (blue line)
#define channelB 5 //5 in wiringPi = 18 in RasPi Define as DT (white line)


int main(int argc, char** argv)
{
	int pwmPin = 0; // 0 in wiringPi = 11 in RasPi
	int range = 100; //10000 / frequency. change based on frequency
	int dirPin = 2; // 2 in wiringPi = 13 in RasPi
	int pwmVal = 0; // (duty cycle * range)/100
	//int speed = encoder.speed;
	double sampleTime = 0.1; // sec
	double startTime = clock();
	double target_speed = 0;//rpm ? 180

	double controlTime;


	//wiringPiSetupGpio();
	cout << "start the program" << endl;

	cout << "You have entered " << argc << " arguments:" << "\n";

	target_speed = atoi(argv[1]);
	//if argc is empty, set speed to default value
	//if(argc = 0){
	//	target speed = 30;
	//	}

	if(wiringPiSetup() < 0){
		//cout << "fail now" << endl;
		fprintf(stderr, "Unable to initialize wiringPi:%s\n",strerror(errno));
		return 1;
	}
	setUpEncoder(channelA, channelB);
	softPwmCreate (pwmPin, pwmVal, range);
	digitalWrite(dirPin, LOW) ;
	pinMode(dirPin, OUTPUT) ;
	/*
		 double dt, double maxVal, double minVal, double kp, double ki, double kd,double preError = 0
		 */
	pid pid_(sampleTime, 100, 0, 0.5, 0.1, 0);
	while(1)
	{
		controlTime = clock() - startTime;
		//cout << controlTime << endl;
		if(controlTime >= sampleTime * CLOCKS_PER_SEC)
		{
			//cout << "control time " << controlTime / CLOCKS_PER_SEC << endl;
			startTime += controlTime;
			pwmVal = pid_.setPWM(target_speed, encoder.speed);
			softPwmWrite (pwmPin, pwmVal);
			cout << "pwm " << pwmVal << endl;
			cout << "speed " << target_speed << " " << encoder.speed << endl; //encoder.speed << endl;
			//if user input = space bar or enter, stop motor 
		}

	}
	return 0;

}

