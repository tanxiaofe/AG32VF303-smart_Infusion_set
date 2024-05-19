#ifndef __PORT_H__
#define __PORT_H__

/*包含相关头文件*/
#include "board.h"
#include "main.h"

/*系统宏定义*/
#define GPIO_PIN_SET 1
#define GPIO_PIN_RESET 0

/*重定义延时函数*/
#define delay_ms(ms) UTIL_IdleMs(ms)
#define delay_us(us) UTIL_IdleUs(us)

/*中断优先级定义0到15越大优先级越高*/
#define MIN_IRQ_PRIORITY 1
#define MAX_IRQ_PRIORITY PLIC_MAX_PRIORITY

#define TIMER_PRIORITY  (MIN_IRQ_PRIORITY + 2)
#define UART_PRIORITY   (MIN_IRQ_PRIORITY + 9)

/*定义步进电机控制*/
#define A_ON  {GPIO_SetHigh(MOTOR_IN1_GPIO,MOTOR_IN1_PORT);}
#define A_OFF {GPIO_SetLow(MOTOR_IN1_GPIO,MOTOR_IN1_PORT);}

#define B_ON  {GPIO_SetHigh(MOTOR_IN2_GPIO,MOTOR_IN2_PORT);}
#define B_OFF {GPIO_SetLow(MOTOR_IN2_GPIO,MOTOR_IN2_PORT);}

#define C_ON  {GPIO_SetHigh(MOTOR_IN3_GPIO,MOTOR_IN3_PORT);}
#define C_OFF {GPIO_SetLow(MOTOR_IN3_GPIO,MOTOR_IN3_PORT);}

#define D_ON  {GPIO_SetHigh(MOTOR_IN4_GPIO,MOTOR_IN4_PORT);}
#define D_OFF {GPIO_SetLow(MOTOR_IN4_GPIO,MOTOR_IN4_PORT);}

#define Coil_8B_A  {A_ON;B_OFF;C_OFF;D_OFF;}//A相通电，其他相断电
#define Coil_8B_AB {A_ON;B_ON;C_OFF;D_OFF;}//AB相通电，其他相断电
#define Coil_8B_B  {A_OFF;B_ON;C_OFF;D_OFF;}//B相通电，其他相断电
#define Coil_8B_BC {A_OFF;B_ON;C_ON;D_OFF;}//BC相通电，其他相断电
#define Coil_8B_C  {A_OFF;B_OFF;C_ON;D_OFF;}//C相通电，其他相断电
#define Coil_8B_CD {A_OFF;B_OFF;C_ON;D_ON;}//CD相通电，其他相断电
#define Coil_8B_D  {A_OFF;B_OFF;C_OFF;D_ON;}//D相通电，其他相断电
#define Coil_8B_DA {A_ON;B_OFF;C_OFF;D_ON;}//DA相通电，其他相断电
#define  Coil_OFF  {A_OFF;B_OFF;C_OFF;D_OFF;}//全部断电

/*滴速传感器参数定义*/
#define  SampleNum       3    //取样数量
#define  SampleTimeout   20   //取样超时时间，单位（秒）

/*变量声明*/

/*滴速传感器*/
extern unsigned long times;        //取样时间
extern unsigned long total_time;   //取样总时间
extern unsigned long timeout;      //取样超时时间 
extern bool start_flag;            //开始计时标志

extern int Speed;          //滴速
extern int speed_min;      //滴速下限
extern int speed_max;     //滴速上限

/*温度设定定义*/
extern short temperature;  //温度
extern short set_tempval; //温度设置值

/*步进电机定义*/
extern uint8_t setFlag;  //设置标志
extern uint16_t direction;//方向标志

/*水位传感*/
extern bool SendFlag;
extern bool blink; //闪烁标志
extern bool update;  //刷新标志
extern bool NoWater;  //缺水标志

/*TFT显示*/
extern char display[16];
extern char Monitor[16];

/*报警*/
extern bool alarm_enable; //报警标志
extern bool set_volume; //设置音量标志
extern int volume;//音量值

/*串口1*/
extern char rxbuf[150];  //串口1接收缓存
extern unsigned int RxCounter;//串口接收Counter

/*函数声明*/
uint8_t GPIO_Read_Pin(GPIO_TypeDef *gpio, uint8_t bits);
void GPIO_Set_Pin(GPIO_TypeDef *gpio, uint8_t bits,uint8_t value);

void waterlevel_init(void); //水位传感器初始化
void displaySetValue(void); //显示设置的值
void InitDisplay(void);  //初始化显示
void keyscan(void);  //按键扫描
void UsartSendReceiveData(void);//串口接收函数
void MOTOR_init(void); //步进电机初始化
void speed_sensor_init(void); //滴速传感器初始化
void LED_init(void);  //LED初始化
void relay_init(void);//继电器初始化
void key_init(void); //按键初始化
void beep_init(void); //蜂鸣器初始化
void System_Port_Init(void);//系统外设初始化

#endif