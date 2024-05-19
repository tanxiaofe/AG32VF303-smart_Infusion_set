#include "timer1.h"


/******************************************************************************
      ����˵����GPTIMER1��ʼ��
      ������ݣ�arr���Զ���װ��ֵ psc��Ԥ��Ƶֵ  ����240mhz ��1ms  arr=1e3 psc=240
      ����ֵ��  ��
******************************************************************************/
void Gptimer1_Init(uint16_t arr,uint16_t psc)
{
    /*ʹ��GPTimer1ʱ��*/
    SYS_EnableAPBClock(APB_MASK_GPTIMER1);

    /*��ʼ��GPTimer1*/
    GPTIMER_InitTypeDef tm_init;
    GPTIMER_StructInit(&tm_init);
    tm_init.Autoreload = arr;
    tm_init.Prescaler = psc;
    GPTIMER_Init(GPTIMER1,&tm_init);

    /*����GPTimer1���*/
    GPTIMER_SetOnePulseMode(GPTIMER1,GPTIMER_ONEPULSEMODE_SINGLE);

    /*ʹ��GPTimer1���*/
    GPTIMER_EnableCounter(GPTIMER1);

    /*����GPTimer1�ж�*/
    INT_EnableIRQ(GPTIMER1_IRQn,TIMER_PRIORITY);
    GPTIMER_EnableIntUpdate(GPTIMER1);

}