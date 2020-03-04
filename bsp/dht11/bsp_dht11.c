/*
 * @Author: jiejie
 * @Github: https://github.com/jiejieTop
 * @Date: 2020-02-15 20:51:38
 * @LastEditTime : 2020-02-16 00:08:05
 * @Description: the code belongs to jiejie, please keep the author information and source code according to the license.
 */
#include "bsp_dht11.h"
#include "bsp_dwt.h"

void dht11_init(void)
{ 
    GPIO_InitTypeDef gpio_init;
    
    DHT11_PIN_CLK_ENABLE();               //开启时钟
    
    gpio_init.Mode = GPIO_MODE_OUTPUT_PP;
    gpio_init.Pin = DHT11_PIN;
    gpio_init.Pull = GPIO_NOPULL;
    gpio_init.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(DHT11_PORT, &gpio_init);
}

void dht11_set_io_mode(uint8_t mode)
{
    GPIO_InitTypeDef gpio_init;
    
    gpio_init.Pin = DHT11_PIN;
    gpio_init.Pull = GPIO_NOPULL;
    gpio_init.Speed = GPIO_SPEED_FREQ_HIGH;
    
    if(mode == DHT11_IO_MODE_INPUT)
        gpio_init.Mode = GPIO_MODE_INPUT;
    
    else if(mode == DHT11_IO_MODE_OUTPUT)
        gpio_init.Mode = GPIO_MODE_OUTPUT_PP;
    
    HAL_GPIO_Init(DHT11_PORT, &gpio_init);
}


uint8_t dht11_read_byte(void) 
{
	uint8_t i, temp = 0;

	for(i=0;i<8;i++) {

		while(DHT11_READ() == 0);
		
		delay_us(40);               //40us
	
		if(DHT11_READ() == 1) {

			while(DHT11_READ() == 1);

			temp |= (uint8_t)(0x01<<(7-i));
		}
		else {
			
			temp &= (uint8_t)~(0x01<<(7-i));
		}
	}	
	return temp;
}


uint8_t dht11_read(dht11_data_t *dht11_data) 
{
    double humi_temp;
    
    uint8_t retry = 0;
    
	dht11_set_io_mode(DHT11_IO_MODE_OUTPUT);            //配置为输出
	
	DHT11_OUT_L();              //拉低
	
	delay_ms(18);               //18ms

	DHT11_OUT_H();              //拉高
    
    delay_us(20);               //20us

	dht11_set_io_mode(DHT11_IO_MODE_INPUT);            //配置为输入

	if(DHT11_READ() == 0) {

		while((DHT11_READ() == 0) && (retry++ < 100)) {
            delay_us(1);
        }
        
        retry = 0;
        
		while((DHT11_READ() == 1) && (retry++ < 100)) {
            delay_us(1);
        }
        
		dht11_data->humi_int= dht11_read_byte();   //读取湿度整数

		dht11_data->humi_deci= dht11_read_byte();  //读取湿度小数

		dht11_data->temp_int= dht11_read_byte();   //读取温度整数

		dht11_data->temp_deci= dht11_read_byte();  //读取温度整数

		dht11_data->check_sum= dht11_read_byte();  //应答读取

		dht11_set_io_mode(DHT11_IO_MODE_OUTPUT);                    //配置为输出

		DHT11_OUT_H();
        
        humi_temp = (double) (dht11_data->humi_int * 100 + dht11_data->humi_deci * 10);
        dht11_data->humidity = humi_temp / 100.0;

        humi_temp = (double) (dht11_data->temp_int * 100 + dht11_data->temp_deci * 10);
        dht11_data->temperature = humi_temp / 100.0;   

		if(dht11_data->check_sum == dht11_data->humi_int + dht11_data->humi_deci + 
            dht11_data->temp_int+ dht11_data->temp_deci) {
			return SUCCESS;
		}
		else {		
			return ERROR;	
		}
	}
    return 0;
}

