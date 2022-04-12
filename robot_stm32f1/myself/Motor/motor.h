#ifndef _DIANJI_H
#define _DIANJI_H
#include "sys.h"
	

#define OUT0 PBout(0)   //LED0
#define OUT1 PBout(1)   //LED1
#define OUT2 PAout(10)
#define OUT3 PAout(11)

#define ENABLE1 PAout(7)   
#define ENABLE2 PAout(12)

void motor_init(void);
#endif
