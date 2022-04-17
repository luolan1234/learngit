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
    float  temperature;          /**< \brief �¶����� */
	float  humidity;             /**< \brief ʪ������ */
} dht_data_t;

/**
 * \brief  DHT11��ʼ�� 
 *
 * \retval 0 - �ɹ�; 1 -  ʧ��
 */
u8  DHT_Init(void);

/**
 * \brief ��ȡDHT��ʪ�Ƚ��
 * 
 * \param [in] p_data    ��ʪ������
 * 
 * \retval 0 - �ɹ�; 1 - ʧ��
 */
u8  DHT_Read(dht_data_t *p_data);


#endif 
