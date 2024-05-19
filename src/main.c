#include "main.h"

int main(void)
{      
    /*板级初始化*/
    board_init();  
    
    /*系统外设初始化*/
    System_Port_Init();
	
    while (1)
    {   
          keyscan();  //按键扫描
	    if(setFlag == 0)   //不在设置状态下
		{
			if((Speed!=0)&&((Speed<speed_min)||(Speed>speed_max))&&blink)   //滴速不在范围，闪烁显示
			{	
				sprintf(Monitor,"%003d",(int)Speed);
				LCD_ShowString(120, 100,(uint8_t*)Monitor,RED,WHITE,24,0);//显示空白
			}
			else
			{
				sprintf(display,": %003d/min ",(int)Speed);
                LCD_ShowString(96, 100, (uint8_t*)display,BLUE,WHITE,24,0);//显示滴速
			}
			if(Speed!=0)
			{
				if(Speed<speed_min) direction=1;         //滴速过慢，电机正转，模拟放开调节阀
			    else if(Speed>speed_max) direction=2;    //滴速过快，电机反转，模拟收紧调节阀
				else direction=0;  //在正常滴速范围，电机停止
			}
			else
			{
				 direction=0;  //停止
			}
					 
			if(blink == 1)
			{
				blink= 0;
				temperature=DS18B20_ReadTemp();           //读取温度
				if(temperature < set_tempval)GPIO_Set_Pin(RELAY_GPIO,RELAY_PORT,1);//温度低，开启加热
				else GPIO_Set_Pin(RELAY_GPIO,RELAY_PORT,0);//温度达到设置值，关闭加热
			}
            if(temperature<set_tempval && blink)      //温度不在范围，闪烁显示
		    {	
				sprintf(Monitor,"%4.1f",(float)temperature/10);
				LCD_ShowString(120, 150,(uint8_t*)Monitor,RED,WHITE,24,0);//显示空白
			}
			else
			{
				sprintf(display,": %4.1f",(float)temperature/10);
				LCD_ShowString(96, 150, (uint8_t*)display,BLUE,WHITE,24,0);//显示温度
				LCD_ShowChinese24x24(180, 150,"℃",BLACK,WHITE,24,0);   
			}

			if(GPIO_Read_Pin(WOUT_GPIO,WOUT_PORT) == 0)   //无药水
			{
				if(NoWater==0)
				LCD_ShowChinese24x24(120,200,"无",BLACK,WHITE,24,0);//显示中文：无
				NoWater  = 1;    //缺水标志
			}
			else
			{
				if(NoWater==1)
	 			LCD_ShowChinese24x24(120,200,"有",BLACK,WHITE,24,0);//显示中文：有  
				 NoWater  = 0;
			}
		}
		UsartSendReceiveData();
		GPIO_Toggle(LED_GPIO,LED_PORT);
    }  
}