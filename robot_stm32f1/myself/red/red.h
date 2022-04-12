#ifndef __RED_H
#define __RED_H
#include "sys.h"


#define red4      HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_4)  //PA4
#define red3      HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_3)  //PA3
#define red2      HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_2)  //PA2
#define red1      HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_1)  //PA1
#define red0      HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_0)  //PA0

void RED_Init(void);

#endif
