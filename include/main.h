#ifndef __MAIN_H__
#define __MAIN_H__

/*����AG32�弶ͷ�ļ�*/
#include "board.h"
#include "port.h"
#include "timer1.h"

/*�����������ͷ�ļ�*/
#include "ds18b20.h"
#include "tft.h"
#include "esp8266.h"
#include "jq8900.h"

/*����C��׼���ļ�*/
#include <stdio.h>
#include <string.h>

/*�������ź궨��*/

/*����*/
#define KEY_GPIO GPIO3 
#define KEY_GPIO_MASK APB_MASK_GPIO3  

//����1����
#define KEY1_PORT GPIO_BIT1

//����2����
#define KEY2_PORT GPIO_BIT2

//����3����
#define KEY3_PORT GPIO_BIT3

//ˮλ����������
#define WOUT_GPIO GPIO3 
#define WOUT_GPIO_MASK APB_MASK_GPIO3  
#define WOUT_PORT GPIO_BIT6

//���ٴ���������
#define DOUT_GPIO GPIO3
#define DOUT_GPIO_MASK APB_MASK_GPIO3   
#define DOUT_PORT GPIO_BIT4

//�����������
#define MOTOR_GPIO_MASK APB_MASK_GPIO5

#define MOTOR_IN1_GPIO GPIO5   //IN1

#define MOTOR_IN1_PORT GPIO_BIT2

#define MOTOR_IN2_GPIO GPIO5   //IN2
#define MOTOR_IN2_PORT GPIO_BIT3

#define MOTOR_IN3_GPIO GPIO5   //IN3
#define MOTOR_IN3_PORT GPIO_BIT4

#define MOTOR_IN4_GPIO GPIO5   //IN4
#define MOTOR_IN4_PORT GPIO_BIT5

//�̵�������
#define RELAY_GPIO GPIO4  
#define RELAY_GPIO_MASK APB_MASK_GPIO4
#define RELAY_PORT GPIO_BIT4

//������·����
#define ALARM_GPIO GPIO4
#define ALARM_GPIO_MASK APB_MASK_GPIO4   
#define ALARM_PORT GPIO_BIT7

/*��������*/


#endif