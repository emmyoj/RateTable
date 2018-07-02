#ifndef ENCODERJUNHAO_H_
#define  ENCODERJUNHAO_H_
#include "wiringPi.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>


struct encoder_struct
{

    int pin_a;
    int pin_b;
    volatile long value;
    volatile int lastEncoded;
    double speed;

	
};

extern encoder_struct encoder; // var can be used in other files...


void setUpEncoder(int pin_a, int pin_b);
void updateEncoders();


#endif
