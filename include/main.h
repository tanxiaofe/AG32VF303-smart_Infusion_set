#ifndef __MAIN_H__
#define __MAIN_H__

/*包含AG32板级头文件*/
#include "board.h"
#include "port.h"
#include "timer1.h"

/*包含相关外设头文件*/
#include "ds18b20.h"
#include "tft.h"
#include "esp8266.h"
#include "jq8900.h"

/*包含C标准库文件*/
#include <stdio.h>
#include <string.h>

/*外设引脚宏定义*/

/*按键*/
#define KEY_GPIO GPIO3 
#define KEY_GPIO_MASK APB_MASK_GPIO3  

//按键1引脚
#define KEY1_PORT GPIO_BIT1

//按键2引脚
#define KEY2_PORT GPIO_BIT2

//按键3引脚
#define KEY3_PORT GPIO_BIT3

//水位传感器引脚
#define WOUT_GPIO GPIO3 
#define WOUT_GPIO_MASK APB_MASK_GPIO3  
#define WOUT_PORT GPIO_BIT6

//滴速传感器引脚
#define DOUT_GPIO GPIO3
#define DOUT_GPIO_MASK APB_MASK_GPIO3   
#define DOUT_PORT GPIO_BIT4

//步进电机引脚
#define MOTOR_GPIO_MASK APB_MASK_GPIO5

#define MOTOR_IN1_GPIO GPIO5   //IN1

#define MOTOR_IN1_PORT GPIO_BIT2

#define MOTOR_IN2_GPIO GPIO5   //IN2
#define MOTOR_IN2_PORT GPIO_BIT3

#define MOTOR_IN3_GPIO GPIO5   //IN3
#define MOTOR_IN3_PORT GPIO_BIT4

#define MOTOR_IN4_GPIO GPIO5   //IN4
#define MOTOR_IN4_PORT GPIO_BIT5

//继电器引脚
#define RELAY_GPIO GPIO4  
#define RELAY_GPIO_MASK APB_MASK_GPIO4
#define RELAY_PORT GPIO_BIT4

//报警电路引脚
#define ALARM_GPIO GPIO4
#define ALARM_GPIO_MASK APB_MASK_GPIO4   
#define ALARM_PORT GPIO_BIT7

/*函数声明*/


#endif