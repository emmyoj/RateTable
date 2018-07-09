#include "encoder.h"
#include <iostream>
#include "wiringPi.h"
#include <time.h>
using namespace std;

encoder_struct encoder;
//update the encoder with two channles and two directions

void updateEncoders()
{

	int MSB = digitalRead(encoder.pin_a);
	int LSB = digitalRead(encoder.pin_b);

	int encoded = (MSB << 1) | LSB;
	int sum = (encoder.lastEncoded << 2) | encoded;

	if(sum == 0b1101 || sum == 0b0100 || sum == 0b0010 || sum == 0b1011)
		encoder.value++;
	if(sum == 0b1110 || sum == 0b0111 || sum == 0b0001 || sum == 0b1000) encoder.value--;
	cout << encoder.lastEncoded << " " << encoded << " ";
	encoder.lastEncoded = encoded;
  encoder.position = encoder.value / 18140.79 *360;
	cout << encoder.value << endl;



}

//new motor 4535; 4535.2/360 = 50/x
/*double temp = 50*360/4535.2; //
	void updateEncoders()
	{
	static int preTime = clock();
	static int curTime = clock();
	encoder.value++;
//cout << "encoder " << encoder.value << endl;
//cout << encoder.value << endl;

if(encoder.value % 50 == 0)
{
curTime = clock();
encoder.speed = temp / (curTime - preTime) * CLOCKS_PER_SEC;
//encoder.speed = 200;
cout << "speed " <<  encoder.speed <<" degree/s" << endl;

//cout << (curTime - preTime) << endl;
//cout << CLOCKS_PER_SEC << endl;
preTime = curTime;
}

}


   two channel and both direction
void setUpEncoder(int a, int b)
{

encoder.pin_a = a;
encoder.pin_b = b;
encoder.value = 0;
encoder.lastEncoded = 0;

pinMode(encoder.pin_a, INPUT);
pinMode(encoder.pin_b, INPUT);
pullUpDnControl(encoder.pin_a, PUD_UP);
pullUpDnControl(encoder.pin_b, PUD_UP);
wiringPiISR(encoder.pin_a,INT_EDGE_BOTH, &updateEncoders);
wiringPiISR(encoder.pin_b,INT_EDGE_BOTH, &updateEncoders);

}
*/

//one channel and one direction
/*void setUpEncoder(int a, int b)
	{

	encoder.pin_a = a;
	encoder.pin_b = b;
	encoder.value = 0;
	encoder.lastEncoded = 0;


	pinMode(encoder.pin_a, INPUT);
//pinMode(encoder.pin_b, INPUT);
pullUpDnControl(encoder.pin_a, PUD_UP);
//pullUpDnControl(encoder.pin_b, PUD_UP);
wiringPiISR(encoder.pin_a,INT_EDGE_RISING, &updateEncoders);
//wiringPiISR(encoder.pin_b,INT_EDGE_BOTH, &updateEncoders);

}
*/
//two channel and two direction
void setUpEncoder(int a, int b)
{
	encoder.pin_a = a;
	encoder.pin_b = b;
	encoder.value = 0;
	encoder.lastEncoded = 0;
	encoder.position = 0;

	pinMode(encoder.pin_a, INPUT);
	pinMode(encoder.pin_b, INPUT);
	pullUpDnControl(encoder.pin_a, PUD_UP);
	pullUpDnControl(encoder.pin_b, PUD_UP);
	wiringPiISR(encoder.pin_a, INT_EDGE_BOTH, &updateEncoders);
	wiringPiISR(encoder.pin_b, INT_EDGE_BOTH, &updateEncoders);
}
