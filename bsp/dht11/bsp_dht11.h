/*
 * @Author: jiejie
 * @Github: https://github.com/jiejieTop
 * @Date: 2020-02-15 20:51:38
 * @LastEditTime : 2020-02-16 00:07:58
 * @Description: the code belongs to jiejie, please keep the author information and source code according to the license.
 */
#ifndef _BSP_DHT11_H
#define _BSP_DHT11_H

#include "stm32f1xx_hal.h"

#define     DHT11_IO_MODE_INPUT         0       // 配置io为输入
#define     DHT11_IO_MODE_OUTPUT        1       // 配置io为输出

/*硬件接口*/
#define DHT11_PIN_CLK_ENABLE()      __HAL_RCC_GPIOB_CLK_ENABLE()
#define DHT11_PIN                   GPIO_PIN_3
#define DHT11_PORT                  GPIOB

#define DHT11_OUT_H()         HAL_GPIO_WritePin(DHT11_PORT, DHT11_PIN, GPIO_PIN_SET)    
#define DHT11_OUT_L()         HAL_GPIO_WritePin(DHT11_PORT, DHT11_PIN, GPIO_PIN_RESET)
#define DHT11_READ()          HAL_GPIO_ReadPin(DHT11_PORT, DHT11_PIN)

/*结构体成员定义*/
typedef struct
{
	uint8_t  humi_int;		//湿度的整数部分
	uint8_t  humi_deci;	 	//湿度的小数部分
	uint8_t  temp_int;	 	//温度的整数部分
	uint8_t  temp_deci;	 	//温度的小数部分
	uint8_t  check_sum;	 	//校验和
    double   humidity;        //实际湿度
    double   temperature;     //实际温度                   
}dht11_data_t;


/*函数声明*/
void dht11_init(void);
uint8_t dht11_read(dht11_data_t *DHT11_Data);

#endif	/* _BSP_DHT11_H */
