#ifndef __PORT_H__
#define __PORT_H__

/*�������ͷ�ļ�*/
#include "board.h"
#include "main.h"

/*ϵͳ�궨��*/
#define GPIO_PIN_SET 1
#define GPIO_PIN_RESET 0

/*�ض�����ʱ����*/
#define delay_ms(ms) UTIL_IdleMs(ms)
#define delay_us(us) UTIL_IdleUs(us)

/*�ж����ȼ�����0��15Խ�����ȼ�Խ��*/
#define MIN_IRQ_PRIORITY 1
#define MAX_IRQ_PRIORITY PLIC_MAX_PRIORITY

#define TIMER_PRIORITY  (MIN_IRQ_PRIORITY + 2)
#define UART_PRIORITY   (MIN_IRQ_PRIORITY + 9)

/*���岽���������*/
#define A_ON  {GPIO_SetHigh(MOTOR_IN1_GPIO,MOTOR_IN1_PORT);}
#define A_OFF {GPIO_SetLow(MOTOR_IN1_GPIO,MOTOR_IN1_PORT);}

#define B_ON  {GPIO_SetHigh(MOTOR_IN2_GPIO,MOTOR_IN2_PORT);}
#define B_OFF {GPIO_SetLow(MOTOR_IN2_GPIO,MOTOR_IN2_PORT);}

#define C_ON  {GPIO_SetHigh(MOTOR_IN3_GPIO,MOTOR_IN3_PORT);}
#define C_OFF {GPIO_SetLow(MOTOR_IN3_GPIO,MOTOR_IN3_PORT);}

#define D_ON  {GPIO_SetHigh(MOTOR_IN4_GPIO,MOTOR_IN4_PORT);}
#define D_OFF {GPIO_SetLow(MOTOR_IN4_GPIO,MOTOR_IN4_PORT);}

#define Coil_8B_A  {A_ON;B_OFF;C_OFF;D_OFF;}//A��ͨ�磬������ϵ�
#define Coil_8B_AB {A_ON;B_ON;C_OFF;D_OFF;}//AB��ͨ�磬������ϵ�
#define Coil_8B_B  {A_OFF;B_ON;C_OFF;D_OFF;}//B��ͨ�磬������ϵ�
#define Coil_8B_BC {A_OFF;B_ON;C_ON;D_OFF;}//BC��ͨ�磬������ϵ�
#define Coil_8B_C  {A_OFF;B_OFF;C_ON;D_OFF;}//C��ͨ�磬������ϵ�
#define Coil_8B_CD {A_OFF;B_OFF;C_ON;D_ON;}//CD��ͨ�磬������ϵ�
#define Coil_8B_D  {A_OFF;B_OFF;C_OFF;D_ON;}//D��ͨ�磬������ϵ�
#define Coil_8B_DA {A_ON;B_OFF;C_OFF;D_ON;}//DA��ͨ�磬������ϵ�
#define  Coil_OFF  {A_OFF;B_OFF;C_OFF;D_OFF;}//ȫ���ϵ�

/*���ٴ�������������*/
#define  SampleNum       3    //ȡ������
#define  SampleTimeout   20   //ȡ����ʱʱ�䣬��λ���룩

/*��������*/

/*���ٴ�����*/
extern unsigned long times;        //ȡ��ʱ��
extern unsigned long total_time;   //ȡ����ʱ��
extern unsigned long timeout;      //ȡ����ʱʱ�� 
extern bool start_flag;            //��ʼ��ʱ��־

extern int Speed;          //����
extern int speed_min;      //��������
extern int speed_max;     //��������

/*�¶��趨����*/
extern short temperature;  //�¶�
extern short set_tempval; //�¶�����ֵ

/*�����������*/
extern uint8_t setFlag;  //���ñ�־
extern uint16_t direction;//�����־

/*ˮλ����*/
extern bool SendFlag;
extern bool blink; //��˸��־
extern bool update;  //ˢ�±�־
extern bool NoWater;  //ȱˮ��־

/*TFT��ʾ*/
extern char display[16];
extern char Monitor[16];

/*����*/
extern bool alarm_enable; //������־
extern bool set_volume; //����������־
extern int volume;//����ֵ

/*����1*/
extern char rxbuf[150];  //����1���ջ���
extern unsigned int RxCounter;//���ڽ���Counter

/*��������*/
uint8_t GPIO_Read_Pin(GPIO_TypeDef *gpio, uint8_t bits);
void GPIO_Set_Pin(GPIO_TypeDef *gpio, uint8_t bits,uint8_t value);

void waterlevel_init(void); //ˮλ��������ʼ��
void displaySetValue(void); //��ʾ���õ�ֵ
void InitDisplay(void);  //��ʼ����ʾ
void keyscan(void);  //����ɨ��
void UsartSendReceiveData(void);//���ڽ��պ���
void MOTOR_init(void); //���������ʼ��
void speed_sensor_init(void); //���ٴ�������ʼ��
void LED_init(void);  //LED��ʼ��
void relay_init(void);//�̵�����ʼ��
void key_init(void); //������ʼ��
void beep_init(void); //��������ʼ��
void System_Port_Init(void);//ϵͳ�����ʼ��

#endif