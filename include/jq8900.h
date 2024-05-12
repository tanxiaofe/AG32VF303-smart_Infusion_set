#ifndef __JQ8900_H__
#define __JQ8900_H__

/*�������ͷ�ļ�*/
#include "board.h"
#include "system.h"

/*�������ź궨��*/
#define JQ_BUSY_GPIO GPIO5
#define JQ_BUSY_GPIO_MASK APB_MASK_GPIO5
#define JQ_BUSY_PORT GPIO_BIT6

/*��������*/
void JQ8900_Init(uint32_t bound); //JQ8900��ʼ��
void JQ_playNum(unsigned int num);//JQ8900����ָ����Ƶ
void JQ_volume(unsigned char volume); //JQ8900�趨����
uint8_t Get_Busy_State(void);    //��ȡBUSY����״̬

#endif
