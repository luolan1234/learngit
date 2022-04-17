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

/* 推挽输出 */
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

/* 上下拉输入 */
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
	函数功能：起始信号
*/
static void DHT_Start(void)
{
	DHT_OUT_MODE();
	DHT_OUT = 1;
	delay_ms(1);
	DHT_OUT = 0;
	delay_ms(20);   //拉低至少18ms
	DHT_OUT = 1;	//拉高等待
	delay_us(30);
}

/*
	函数功能：结束信号
*/
static void DHT_Stop(void)
{
	u8 t=0;
	while(!DHT_IN)		//等待从机释放总线
	{
		t++;
		if(t>=70)
		{
			printf("从机释放总线失败");
			return ;
		}
		delay_us(1);
	}
	DHT_OUT_MODE();		//输出模式
	DHT_OUT = 1;		//拉高，释放总线
}

/*
	函数功能：等待响应信号
*/
static u8 DHT_WaitAck(void)
{
	u8 err=0;
	DHT_IN_MODE();	//输入模式
	DHT_OUT = 1;
	while(!DHT_IN)	//等待低电平时长
	{
		err++;
		delay_us(1);
		if(err>=100)	//超时，返回错误号
		{
			printf("Wait ACK error");
			return 1;
		}
	}
	err=0;
	while(DHT_IN)	//等待高电平时长
	{
		err++;
		delay_us(1);
		if(err>=100)	//超时，返回错误号
		{
			printf("Wait ACK error");
			return 1;
		}
	}
	return 0;
}

/*
	函数功能：读取DHT11的一位
*/
static u8  DHT_ReadBit(void)
{
	u8 t=0;
	while(DHT_IN)	  //等待高电平时长
	{
		t++;
		delay_us(1);
		if(t>=255)	return 2;
	}
	t=0;
	while(!DHT_IN)	  //等待低电平时长
	{
		t++;
		delay_us(1);
		if(t>=255)	return 2;
	}	
	delay_us(35);		//延时35us
	if(DHT_IN == 1)		return 1;
	else 	return 0;
}

/*******************************************************************************
  Public functions
*******************************************************************************/

/*
	函数功能：读取DHT温湿度结果
*/
u8  DHT_Read(dht_data_t *p_data)
{
	u8 DHT_Result[5];
	u8 tmp,rx_data;
	u8 i,j;
	
	DHT_Start();		//起始信号
	if(DHT_WaitAck())  return 1;
	
	DHT_IN_MODE();		//输入模式
	
	for(i=0;i<5;i++)
	{
		rx_data = 0;	
		for(j=0;j<8;j++)
		{
			DHT_OUT = 1;
			tmp = DHT_ReadBit();
			if(tmp == 2)	return 1;
			rx_data <<= 1;		//tmp左移一位
			rx_data |= tmp;
		}
		DHT_Result[i] = rx_data;
	}
	DHT_Stop();		//结束信号
	//printf("%d %d %d %d %d\n",DHT_Result[0],DHT_Result[1],DHT_Result[2],DHT_Result[3],DHT_Result[4]);
	//转换结果
	if((DHT_Result[0]+DHT_Result[1]+DHT_Result[2]+DHT_Result[3])==DHT_Result[4])		//结果
	{
		p_data->humidity = (float)(DHT_Result[0] + DHT_Result[1]*0.1);	//获取湿度数据（百分比）
		tmp = DHT_Result[3] & (1<<7);
		if(tmp)
		{
			p_data->temperature = -(DHT_Result[2] + (DHT_Result[3] & 0x7F)*0.1);	//获得温度数据
		}
		else
		{
			p_data->temperature = DHT_Result[2] + (DHT_Result[3] & 0x7F)*0.1;
		}
		return 0;
	}
	else
	{
		printf("数据校验失败");
		return 1;
	}
}

/*
	函数功能：DHT初始化
*/
u8  DHT_Init(void)
{
	GPIO_InitTypeDef GPIO_Initure;
	__HAL_RCC_GPIOA_CLK_ENABLE();	            //使能GPIO口
	
	GPIO_Initure.Pin = GPIO_PIN_8;              // PA8
	GPIO_Initure.Mode = GPIO_MODE_OUTPUT_PP;    // 推挽输出
	GPIO_Initure.Pull = GPIO_PULLUP;            // 上拉
	GPIO_Initure.Speed = GPIO_SPEED_FREQ_HIGH;  // 高速
    HAL_GPIO_Init(GPIOA, &GPIO_Initure);
	
	DHT_Start();	//起始信号
	return  DHT_WaitAck();
	
}

/* end of file */
