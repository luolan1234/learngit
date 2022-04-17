/**
 * \brief the driver for DHT11
 *
 * \par Modification History
 * - 22-04-17 luolan first implement
 */
 
#include "dht.h"
#include "delay.h"
#include "usart.h"

/*******************************************************************************
  Defines
*******************************************************************************/
#define  DHT_OUT		PAout(8)
#define  DHT_IN			PAin(8)

/* ������� */
#define  DHT_OUT_MODE()  \
    do {                 \
        GPIO_InitTypeDef GPIO_Initure;             \
		                                           \
		GPIO_Initure.Pin = GPIO_PIN_8;             \
		GPIO_Initure.Mode = GPIO_MODE_OUTPUT_PP;   \
		GPIO_Initure.Pull = GPIO_NOPULL;           \
		GPIO_Initure.Speed = GPIO_SPEED_FREQ_HIGH; \
		HAL_GPIO_Init(GPIOA, &GPIO_Initure);       \
	} while(0);

/* ���������� */
#define  DHT_IN_MODE()  \
	do {                \
		GPIO_InitTypeDef GPIO_Initure;             \
		                                           \
		GPIO_Initure.Pin = GPIO_PIN_8;             \
		GPIO_Initure.Mode = GPIO_MODE_INPUT;       \
		GPIO_Initure.Pull = GPIO_PULLUP;           \
		GPIO_Initure.Speed = GPIO_SPEED_FREQ_HIGH; \
		HAL_GPIO_Init(GPIOA, &GPIO_Initure);       \
	} while(0);

/*******************************************************************************
  Local functions
*******************************************************************************/

/*
	�������ܣ���ʼ�ź�
*/
static void DHT_Start(void)
{
	DHT_OUT_MODE();
	DHT_OUT = 1;
	delay_ms(1);
	DHT_OUT = 0;
	delay_ms(20);   //��������18ms
	DHT_OUT = 1;	//���ߵȴ�
	delay_us(30);
}

/*
	�������ܣ������ź�
*/
static void DHT_Stop(void)
{
	u8 t=0;
	while(!DHT_IN)		//�ȴ��ӻ��ͷ�����
	{
		t++;
		if(t>=70)
		{
			printf("�ӻ��ͷ�����ʧ��");
			return ;
		}
		delay_us(1);
	}
	DHT_OUT_MODE();		//���ģʽ
	DHT_OUT = 1;		//���ߣ��ͷ�����
}

/*
	�������ܣ��ȴ���Ӧ�ź�
*/
static u8 DHT_WaitAck(void)
{
	u8 err=0;
	DHT_IN_MODE();	//����ģʽ
	DHT_OUT = 1;
	while(!DHT_IN)	//�ȴ��͵�ƽʱ��
	{
		err++;
		delay_us(1);
		if(err>=100)	//��ʱ�����ش����
		{
			printf("Wait ACK error");
			return 1;
		}
	}
	err=0;
	while(DHT_IN)	//�ȴ��ߵ�ƽʱ��
	{
		err++;
		delay_us(1);
		if(err>=100)	//��ʱ�����ش����
		{
			printf("Wait ACK error");
			return 1;
		}
	}
	return 0;
}

/*
	�������ܣ���ȡDHT11��һλ
*/
static u8  DHT_ReadBit(void)
{
	u8 t=0;
	while(DHT_IN)	  //�ȴ��ߵ�ƽʱ��
	{
		t++;
		delay_us(1);
		if(t>=255)	return 2;
	}
	t=0;
	while(!DHT_IN)	  //�ȴ��͵�ƽʱ��
	{
		t++;
		delay_us(1);
		if(t>=255)	return 2;
	}	
	delay_us(35);		//��ʱ35us
	if(DHT_IN == 1)		return 1;
	else 	return 0;
}

/*******************************************************************************
  Public functions
*******************************************************************************/

/*
	�������ܣ���ȡDHT��ʪ�Ƚ��
*/
u8  DHT_Read(dht_data_t *p_data)
{
	u8 DHT_Result[5];
	u8 tmp,rx_data;
	u8 i,j;
	
	DHT_Start();		//��ʼ�ź�
	if(DHT_WaitAck())  return 1;
	
	DHT_IN_MODE();		//����ģʽ
	
	for(i=0;i<5;i++)
	{
		rx_data = 0;	
		for(j=0;j<8;j++)
		{
			DHT_OUT = 1;
			tmp = DHT_ReadBit();
			if(tmp == 2)	return 1;
			rx_data <<= 1;		//tmp����һλ
			rx_data |= tmp;
		}
		DHT_Result[i] = rx_data;
	}
	DHT_Stop();		//�����ź�
	//printf("%d %d %d %d %d\n",DHT_Result[0],DHT_Result[1],DHT_Result[2],DHT_Result[3],DHT_Result[4]);
	//ת�����
	if((DHT_Result[0]+DHT_Result[1]+DHT_Result[2]+DHT_Result[3])==DHT_Result[4])		//���
	{
		p_data->humidity = (float)(DHT_Result[0] + DHT_Result[1]*0.1);	//��ȡʪ�����ݣ��ٷֱȣ�
		tmp = DHT_Result[3] & (1<<7);
		if(tmp)
		{
			p_data->temperature = -(DHT_Result[2] + (DHT_Result[3] & 0x7F)*0.1);	//����¶�����
		}
		else
		{
			p_data->temperature = DHT_Result[2] + (DHT_Result[3] & 0x7F)*0.1;
		}
		return 0;
	}
	else
	{
		printf("����У��ʧ��");
		return 1;
	}
}

/*
	�������ܣ�DHT��ʼ��
*/
u8  DHT_Init(void)
{
	GPIO_InitTypeDef GPIO_Initure;
	__HAL_RCC_GPIOA_CLK_ENABLE();	            //ʹ��GPIO��
	
	GPIO_Initure.Pin = GPIO_PIN_8;              // PA8
	GPIO_Initure.Mode = GPIO_MODE_OUTPUT_PP;    // �������
	GPIO_Initure.Pull = GPIO_PULLUP;            // ����
	GPIO_Initure.Speed = GPIO_SPEED_FREQ_HIGH;  // ����
    HAL_GPIO_Init(GPIOA, &GPIO_Initure);
	
	DHT_Start();	//��ʼ�ź�
	return  DHT_WaitAck();
	
}

/* end of file */
