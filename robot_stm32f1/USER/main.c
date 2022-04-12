#include "sys.h"
#include "delay.h"
#include "usart.h"
#include "pwn.h"
#include "red.h"
#include "motor.h"
#include "stm32f1xx_hal.h"



int main(void)
{
	
	
    HAL_Init();                   	//��ʼ��HAL��    
    Stm32_Clock_Init(RCC_PLL_MUL9); //����ʱ��,72M
    delay_init(72);                	//��ʼ����ʱ����
	uart_init(115200);			    //��ʼ������
	TIM3_PWM_Init(2000-1,720-1);  	//72M/720=100k�ļ���Ƶ�ʣ��Զ���װ��Ϊ2000����ôPWMƵ��Ϊ100k/2000=50HZ
	RED_Init ();                  	//��������ʼ��
	motor_init();                	//�����ʼ��

	while(1)
		
	{	
		
    OUT0=0;
		OUT1=1;
		OUT2=0;
		OUT3=1;
	}
}

