#include "timer1.h"


/******************************************************************************
      函数说明：GPTIMER1初始化
      入口数据：arr：自动重装载值 psc：预分频值  例：240mhz 定1ms  arr=1e3 psc=240
      返回值：  无
******************************************************************************/
void Gptimer1_Init(uint16_t arr,uint16_t psc)
{
    /*使能GPTimer1时钟*/
    SYS_EnableAPBClock(APB_MASK_GPTIMER1);

    /*初始化GPTimer1*/
    GPTIMER_InitTypeDef tm_init;
    GPTIMER_StructInit(&tm_init);
    tm_init.Autoreload = arr;
    tm_init.Prescaler = psc;
    GPTIMER_Init(GPTIMER1,&tm_init);

    /*设置GPTimer1输出*/
    GPTIMER_SetOnePulseMode(GPTIMER1,GPTIMER_ONEPULSEMODE_SINGLE);

    /*使能GPTimer1输出*/
    GPTIMER_EnableCounter(GPTIMER1);

    /*开启GPTimer1中断*/
    INT_EnableIRQ(GPTIMER1_IRQn,TIMER_PRIORITY);
    GPTIMER_EnableIntUpdate(GPTIMER1);

}