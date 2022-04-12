#include "sys.h"
#include "delay.h"
#include "usart.h"
#include "pwn.h"
#include "red.h"
#include "motor.h"
#include "stm32f1xx_hal.h"



int main(void)
{
	
	
    HAL_Init();                   	//初始化HAL库    
    Stm32_Clock_Init(RCC_PLL_MUL9); //设置时钟,72M
    delay_init(72);                	//初始化延时函数
	uart_init(115200);			    //初始化串口
	TIM3_PWM_Init(2000-1,720-1);  	//72M/720=100k的计数频率，自动重装载为2000，那么PWM频率为100k/2000=50HZ
	RED_Init ();                  	//传感器初始化
	motor_init();                	//电机初始化

	while(1)
		
	{	
		
    OUT0=0;
		OUT1=1;
		OUT2=0;
		OUT3=1;
	}
}

