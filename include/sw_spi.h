#ifndef __SW_SPI_H__
#define __SW_SPI_H__		  	 

/*包含AG32板级头文件*/
#include "board.h"
#include "tft.h"

/*使用软硬件SPI*/
#define USING_HARDWARE_SPI 0
#define USING_SOLFTWARE_SPI 1

/*函数声明*/
void SPI0_DMA_Init(void);
void SPI_Cmd(SPI_TypeDef* Spi, uint8_t NewState);
uint8_t SPI0_ReadWriteByte(uint8_t TxData);


#endif
