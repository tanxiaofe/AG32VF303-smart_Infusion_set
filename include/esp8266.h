#ifndef __ESP8266_H__
#define __ESP8266_H__

/*包含相关头文件*/
#include "board.h"
#include "port.h"

/*外设相关宏定义*/
#define  ESP8266_BUF         rxbuf
#define  ESP8266_CNT         RxCounter

#define REV_OK		0	//接收完成标志
#define REV_WAIT	1	//接收未完成标志

/*函数声明*/
void ESP8266_Init(void);
void ESP8266_Clear(void);
void ESP8266_SendData(unsigned char *data, unsigned short len);
unsigned char *ESP8266_GetIPD(unsigned short timeOut);



#endif
