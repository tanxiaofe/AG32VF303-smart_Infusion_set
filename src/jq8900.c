#include "jq8900.h"

/******************************************************************************
      用法：
      在初始化后JQ_volume设定音量：一般99最大
      在需要播放设定曲目的地方调用 JQ_playNum()
      num=1:“欢迎使用智能输液系统！”
      num=2：经典开机音效
      num=3：“温度设定不在范围内。“
      num=4：”滴速设定不在范围内。“
      num=5：”医护人员请注意，输液即将完成，请及时拔针或换药。“
      随后while(Get_Busy_State());
      等待播放完毕。
******************************************************************************/


/******************************************************************************
      函数说明：初始化串口2引脚
      入口数据：无
      返回值：  无
******************************************************************************/
void JQ8900_Init(uint32_t bound)
{   
    /*使能uart1引脚*/
    GPIO_AF_ENABLE(UART2_UARTRXD);
    GPIO_AF_ENABLE(UART2_UARTTXD);

    /*开启uart1时钟*/
    SYS_EnableAPBClock(APB_MASK_UART2);

    /*开启JQ8900 BSUY引脚时钟*/
    SYS_EnableAPBClock(JQ_BUSY_GPIO_MASK);

    /*uart1初始化*/
    UART_Init(UART2,bound,UART_LCR_DATABITS_8,UART_LCR_STOPBITS_1,
    UART_LCR_PARITY_NONE,UART_LCR_FIFO_16);

    /*设置BUSY引脚为输入模式*/
    GPIO_SetInput(JQ_BUSY_GPIO,JQ_BUSY_PORT);

}

/******************************************************************************
      函数说明：获取JQ8900 BUSY引脚状态
      入口数据：无
      返回值：  无
******************************************************************************/
uint8_t Get_Busy_State(void)
{
    return GPIO_Read_Pin(JQ_BUSY_GPIO,JQ_BUSY_PORT);
}

/******************************************************************************
      函数说明：串口1发送字符
      入口数据：无
      返回值：  无
******************************************************************************/
void Usart2_SendString(unsigned char *bufs,unsigned char len)
{
    /*UART2发送数据*/
    UART_Send(UART2,bufs,len);
}

/******************************************************************************
      函数说明：JQ8900播放指定音频
      入口数据：num：音频标号1到200
      返回值：  无
******************************************************************************/
void JQ_playNum(unsigned int num)
{   // 指定曲目播放 1-200
	uint8_t data[6] = {0xAA,0x07,0x02};
	uint8_t i = 0;
	
    if(num > 200) num = 200;
    if(num < 1  ) num = 1;
    
	data[3] = num>>8;
	data[4] = num&0xff;
	for(i = 0;i < 5;i++) data[5] = (data[5] + data[i])&0xff;
	
	Usart2_SendString(data,6);
}

/******************************************************************************
      函数说明：JQ8900设定音量
      入口数据：volume：音量0到30
      返回值：  无
******************************************************************************/
void JQ_volume(unsigned char volume)
{   // 设置音量 0-30
	uint8_t data[5] = {0xAA,0x13,0x01};
	uint8_t i = 0;
	
    if(volume > 30) volume = 30;
	data[3] = volume;
	for(i = 0;i < 4;i++) data[4] = (data[4] + data[i])&0xff;
	Usart2_SendString(data,5);
}
