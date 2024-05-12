#ifndef __DS18B20_H__
#define __DS18B20_H__

/*包含相关头文件*/
#include "board.h"
#include "system.h"

/*外设引脚宏定义*/
#define TEMP_GPIO GPIO3
#define TEMP_GPIO_MASK APB_MASK_GPIO3
#define TEMP_PORT GPIO_BIT5

/*IO状态定义*/
#define DQ_L   GPIO_SetLow(TEMP_GPIO,TEMP_PORT)//IO为低电平
#define DQ_H   GPIO_SetHigh(TEMP_GPIO,TEMP_PORT)//IO为高电平
#define DQ_Read GPIO_Read_Pin(TEMP_GPIO,TEMP_PORT)//读取IO电平

/*全局变量声明*/
extern uint8_t flag;  

/*函数声明*/
void DS18B20_Init(void);
uint16_t DS18B20_ReadTemp(void);
uint8_t DS18B20_Reset(void);


#endif
