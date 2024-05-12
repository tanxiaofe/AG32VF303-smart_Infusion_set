#ifndef __TFT_H__
#define __TFT_H__

#include "board.h"
#include "system.h"
#include "spi.h"

#define USE_HORIZONTAL 0  //设置横屏或者竖屏显示 0或1为竖屏 2或3为横屏
#define Chip_Selection 1   //设置芯片初始化 0为ILI9341  1为ST7789
#define USE_BGR 1   //设置颜色模式
#define USE_RGB 0   

#define TFT_SCK_GPIO GPIO4
#define TFT_SCK_PORT GPIO_BIT1

#define TFT_MOSI_GPIO GPIO4
#define TFT_MOSI_PORT GPIO_BIT2

#define TFT_RST_GPIO GPIO4
#define TFT_RST_PORT GPIO_BIT5 

#define TFT_BLK_GPIO GPIO4
#define TFT_BLK_PORT GPIO_BIT6

#define TFT_DC_GPIO GPIO4
#define TFT_DC_PORT GPIO_BIT3

#define TFT_CS_GPIO GPIO4
#define TFT_CS_PORT GPIO_BIT4

//-----------------LCD端口定义---------------- 

#define LCD_SCK_Clr()  GPIO_SetLow(GPIO4,GPIO_BIT1)//SCL=SCLK
#define LCD_SCK_Set() GPIO_SetHigh(GPIO4,GPIO_BIT1)

#define LCD_MOSI_Clr()  GPIO_SetLow(GPIO4,GPIO_BIT2)//SDA=MOSI
#define LCD_MOSI_Set()  GPIO_SetHigh(GPIO4,GPIO_BIT2)

#define LCD_RES_Clr()  GPIO_SetLow(GPIO4,GPIO_BIT5)//RES
#define LCD_RES_Set()  GPIO_SetHigh(GPIO4,GPIO_BIT5)

#define LCD_DC_Clr()   GPIO_SetLow(GPIO4,GPIO_BIT3)//DC
#define LCD_DC_Set()   GPIO_SetHigh(GPIO4,GPIO_BIT3)
 		     
#define LCD_CS_Clr()   GPIO_SetLow(GPIO4,GPIO_BIT4)//CS
#define LCD_CS_Set()   GPIO_SetHigh(GPIO4,GPIO_BIT4)

#define LCD_BLK_Clr()  GPIO_SetLow(GPIO4,GPIO_BIT6)//BLK
#define LCD_BLK_Set()  GPIO_SetHigh(GPIO4,GPIO_BIT6)

#if USE_HORIZONTAL==0||USE_HORIZONTAL==1
#define LCD_W 240
#define LCD_H 240

#else
#define LCD_W 320
#define LCD_H 240

#endif

#if USE_RGB
//画笔颜色
#define WHITE         	 0xFFFF
#define BLACK         	 0x0000	  
#define BLUE           	 0x001F  
#define BRED             0XF81F
#define GRED 			 0XFFE0
#define GBLUE			 0X07FF
#define RED           	 0xF800
#define MAGENTA       	 0xF81F
#define GREEN         	 0x07E0
#define CYAN          	 0x7FFF
#define YELLOW        	 0xFFE0
#define BROWN 			 0XBC40 //棕色
#define BRRED 			 0XFC07 //棕红色
#define GRAY  			 0X8430 //灰色
#define DARKBLUE      	 0X01CF	//深蓝色
#define LIGHTBLUE      	 0X7D7C	//浅蓝色  
#define GRAYBLUE       	 0X5458 //灰蓝色
#define LIGHTGREEN     	 0X841F //浅绿色
#define LGRAY 			 0XC618 //浅灰色(PANNEL),窗体背景色
#define LGRAYBLUE        0XA651 //浅灰蓝色(中间层颜色)
#define LBBLUE           0X2B12 //浅棕蓝色(选择条目的反色)

#endif

#if USE_BGR
//画笔颜色
#define WHITE         	 0x0000
#define BLACK         	 0xFFFF	  
#define BLUE           	 0xFFE0
#define BRED             0X07E0
#define GRED 			 0x001F  
#define GBLUE			 0XF800
#define RED           	 0x07FF
#define MAGENTA       	 0x07E0
#define GREEN         	 0xF8FF
#define CYAN          	 0x801F
#define YELLOW        	 0x001F 
#define BROWN 			 0X555B //棕色
#define BRRED 			 0X438E //棕红色
#define GRAY  			 0X4208 //灰色
#define DARKBLUE      	 0XE731	//深蓝色
#define LIGHTBLUE      	 0XFA00	//浅蓝色  
#define GRAYBLUE       	 0XBBE9 //灰蓝色
#define LIGHTGREEN     	 0X600C //浅绿色
#define LGRAY 			 0X2965 //浅灰色(PANNEL),窗体背景色
#define LGRAYBLUE        0X49C4 //浅灰蓝色(中间层颜色)
#define LBBLUE           0XE56C //浅棕蓝色(选择条目的反色)

#endif

//-----------------函数声明---------------- 
void TFT_GPIO_Init(void);//初始化GPIO
void LCD_Writ_Bus(uint8_t dat);//模拟SPI时序
void LCD_WR_DATA8(uint8_t dat);//写入一个字节
void LCD_WR_DATA(uint16_t dat);//写入两个字节
void LCD_WR_REG(uint8_t dat);//写入一个指令
void LCD_Address_Set(uint16_t x1,uint16_t y1,
                    uint16_t x2,uint16_t y2);//设置坐标函数
void LCD_Init(void);//LCD初始化

void LCD_Fill(uint16_t xsta,uint16_t ysta,uint16_t xend,uint16_t yend,uint16_t color);//指定区域填充颜色
void LCD_DrawPoint(uint16_t x,uint16_t y,uint16_t color);//在指定位置画一个点
void LCD_DrawLine(uint16_t x1,uint16_t y1,uint16_t x2,uint16_t y2,uint16_t color);//在指定位置画一条线
void LCD_DrawRectangle(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2,uint16_t color);//在指定位置画一个矩形
void Draw_Circle(uint16_t x0,uint16_t y0,uint8_t r,uint16_t color);//在指定位置画一个圆

void LCD_ShowChinese(uint16_t x,uint16_t y,uint8_t *s,uint16_t fc,uint16_t bc,uint8_t sizey,uint8_t mode);//显示汉字串
void LCD_ShowChinese12x12(uint16_t x,uint16_t y,uint8_t *s,uint16_t fc,uint16_t bc,uint8_t sizey,uint8_t mode);//显示单个12x12汉字
void LCD_ShowChinese16x16(uint16_t x,uint16_t y,uint8_t *s,uint16_t fc,uint16_t bc,uint8_t sizey,uint8_t mode);//显示单个16x16汉字
void LCD_ShowChinese24x24(uint16_t x,uint16_t y,uint8_t *s,uint16_t fc,uint16_t bc,uint8_t sizey,uint8_t mode);//显示单个24x24汉字
void LCD_ShowChinese32x32(uint16_t x,uint16_t y,uint8_t *s,uint16_t fc,uint16_t bc,uint8_t sizey,uint8_t mode);//显示单个32x32汉字

void LCD_ShowChar(uint16_t x,uint16_t y,uint8_t num,uint16_t fc,uint16_t bc,uint8_t sizey,uint8_t mode);//显示一个字符
void LCD_ShowString(uint16_t x,uint16_t y,const uint8_t *p,uint16_t fc,uint16_t bc,uint8_t sizey,uint8_t mode);//显示字符串
uint32_t mypow(uint8_t m,uint8_t n);//求幂
void LCD_ShowIntNum(uint16_t x,uint16_t y,uint16_t num,uint8_t len,uint16_t fc,uint16_t bc,uint8_t sizey);//显示整数变量
void LCD_ShowFloatNum1(uint16_t x,uint16_t y,float num,uint8_t len,uint16_t fc,uint16_t bc,uint8_t sizey);//显示两位小数变量

void LCD_ShowPicture(uint16_t x,uint16_t y,uint16_t length,uint16_t width,const uint8_t pic[]);//显示图片

#endif
