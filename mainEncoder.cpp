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

/*
int main(int argc, char** argv)
{
	int pwmPin = 0; // 0 in wiringPi = 11 in RasPi
	int range = 100; //10000 / frequency. change based on frequency
	int dirPin = 2; // 2 in wiringPi = 13 in RasPi
	int pwmVal = 0; // (duty cycle * range)/100
	//int speed = encoder.speed;
	double sampleTime = 0.1; // sec? or micro seconds?
	double startTime = clock();
	double target_speed = 0;//rpm ? 180
	int exp_time = 0;// user input time experiment will run... without conversion, user input needs to be in microseconds >= 1000000
	int target_time = 0; //exp_time + clock()
	double controlTime;


	//wiringPiSetupGpio();
	cout << "start the program" << endl;

	cout << "You have entered " << argc << " arguments:" << "\n";
	cout << argv[1][2] << "\n";

	target_speed = atoi(argv[1]);
	//if argc is empty, set speed to default value
	//if(argc = 0){
	//	target speed = 30;
	//	}
	exp_time = atoi(argv[2])*1000000;

	cout << "experiment time: " << exp_time << "\n";
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
	
	pid pid_(sampleTime, 100, 0, 0.5, 0.1, 0);

	target_time = exp_time + clock();
	cout << "target time: " << target_time << "\n";
	while(clock() < target_time) //while (clock() < target_time)
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
*/
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
	//int speed = encoder.speed;
	double sampleTime = 0.1; // sec? or micro seconds?
	double startTime = clock();
	double target_position = 0;//rpm ? 180
	int exp_time = 0;// user input time experiment will run... without conversion, user input needs to be in microseconds >= 1000000
	int target_time = 0; //exp_time + clock()
	double controlTime;


	//wiringPiSetupGpio();
	cout << "start the program" << endl;

	cout << "You have entered " << argc << " arguments:" << "\n";
	//cout << argv[1][2] << "\n";

	target_position = atoi(argv[1]);
	//if argc is empty, set speed to default value
	//if(argc = 0){
	//	target speed = 30;
	//	}
	exp_time = atoi(argv[2])*1000000;

	cout << "experiment time: " << exp_time << "\n";
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

	target_time = exp_time + clock();
	cout << "target time: " << target_time << "\n";
	while(clock() < target_time) //while (current encoder value is not wthin range of target encoder value +/- error)
	{
		controlTime = clock() - startTime;
		//cout << controlTime << endl;
		if(controlTime >= sampleTime * CLOCKS_PER_SEC)
		{
			//cout << "control time " << controlTime / CLOCKS_PER_SEC << endl;
			startTime += controlTime;
			pwmVal = pid_.setPWM(target_position, encoder.position);
			softPwmWrite (pwmPin, abs(pwmVal) + PWM_RANGE);
			cout << "pwm " << pwmVal << endl;
			cout << "position " << target_position << " " << encoder.position << endl; //encoder.speed << endl;
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
    return 0;
}

