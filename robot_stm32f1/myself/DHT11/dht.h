/**
 * \brief the driver for DHT11
 *
 * \par Modification History
 * - 22-04-17 luolan first implement
 */


#ifndef  __DHT_H
#define  __DHT_H

#include "sys.h"

typedef struct _dht_data {
    float  temperature;          /**< \brief 温度数据 */
	float  humidity;             /**< \brief 湿度数据 */
} dht_data_t;

/**
 * \brief  DHT11初始化 
 *
 * \retval 0 - 成功; 1 -  失败
 */
u8  DHT_Init(void);

/**
 * \brief 读取DHT温湿度结果
 * 
 * \param [in] p_data    温湿度数据
 * 
 * \retval 0 - 成功; 1 - 失败
 */
u8  DHT_Read(dht_data_t *p_data);


#endif 
