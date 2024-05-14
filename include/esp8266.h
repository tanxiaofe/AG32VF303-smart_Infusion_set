#ifndef __ESP8266_H__
#define __ESP8266_H__

/*�������ͷ�ļ�*/
#include "board.h"
#include "port.h"

/*������غ궨��*/
#define  ESP8266_BUF         rxbuf
#define  ESP8266_CNT         RxCounter

#define REV_OK		0	//������ɱ�־
#define REV_WAIT	1	//����δ��ɱ�־

/*��������*/
void ESP8266_Init(void);
void ESP8266_Clear(void);
void ESP8266_SendData(unsigned char *data, unsigned short len);
unsigned char *ESP8266_GetIPD(unsigned short timeOut);



#endif
