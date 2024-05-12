#ifndef __JQ8900_H__
#define __JQ8900_H__

/*包含相关头文件*/
#include "board.h"
#include "system.h"

/*外设引脚宏定义*/
#define JQ_BUSY_GPIO GPIO5
#define JQ_BUSY_GPIO_MASK APB_MASK_GPIO5
#define JQ_BUSY_PORT GPIO_BIT6

/*函数声明*/
void JQ8900_Init(uint32_t bound); //JQ8900初始化
void JQ_playNum(unsigned int num);//JQ8900播放指定音频
void JQ_volume(unsigned char volume); //JQ8900设定音量
uint8_t Get_Busy_State(void);    //获取BUSY引脚状态

#endif
