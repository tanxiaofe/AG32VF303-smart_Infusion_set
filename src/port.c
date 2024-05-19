#include "port.h"

/*滴速取样定义*/
unsigned long times=0;        //取样时间
unsigned long total_time=0;   //取样总时间
unsigned long timeout=0;      //取样超时时间 
bool start_flag=0;            //开始计时标志

int PulseCount = 0;     //记录脉冲
int Speed = 0;          //滴速
int speed_min = 60;      //滴速下限
int speed_max = 180;     //滴速上限

/*温度设定定义*/
short temperature=0;  //温度
short set_tempval=260; //温度设置值

/*步进电机定义*/
uint8_t setFlag = 0;  //设置标志
uint16_t direction = 0;//方向标志

/*水位传感*/
bool SendFlag = 0;
bool blink = 0;    //闪烁标志
bool update = 0;  //刷新标志
bool NoWater = 0;  //缺水标志

/*lcd显示数组*/
char display[16]={0}; //显示数组
char Monitor[16]={0}; //COPY数组

/*报警相关定义*/
bool alarm_enable =1; //报警标志
bool set_volume=0;	 //设置音量标志
int volume=90;		//音量值

/*串口相关定义*/
unsigned int RxCounter = 0; //串口接收Counter
char rxbuf[150]={0};	 //串口1接收缓存


/******************************************************************************
      函数说明：读取GPIO引脚
      入口数据：GPIO：端口 bit：引脚
      返回值：  bit状态 SET或RESET
******************************************************************************/
uint8_t GPIO_Read_Pin(GPIO_TypeDef *gpio, uint8_t bits)
{
    /*判断GPIO输入电平*/
    if(GPIO_GetValue(gpio,bits)!=0)
    {
        return GPIO_PIN_SET;   //判断数据位为1时返回
    }
    else
    {
        return GPIO_PIN_RESET; //判断数据位为0时返回
    }
}

/******************************************************************************
      函数说明：置位GPIO引脚
      入口数据：GPIO：端口 bit：引脚 value：置1或0
      返回值：  无
******************************************************************************/
void GPIO_Set_Pin(GPIO_TypeDef *gpio, uint8_t bits,uint8_t value)
{
    /*GPIO配置输出引脚以及电平*/
    GPIO_SetValue(gpio, bits, value*255);
}

/******************************************************************************
      函数说明：蜂鸣器初始化
      入口数据：无
      返回值：  无
******************************************************************************/
void beep_init(void)
{   
    /*设置输出*/
    GPIO_SetOutput(ALARM_GPIO,ALARM_PORT);

    /*拉低不报警*/
    GPIO_SetLow(ALARM_GPIO,ALARM_PORT);
}

/******************************************************************************
      函数说明：继电器初始化
      入口数据：无
      返回值：  无
******************************************************************************/
void relay_init(void)
{   
    /*设置输出*/
    GPIO_SetOutput(RELAY_GPIO,RELAY_PORT);

    /*拉低不触发*/
    GPIO_SetLow(RELAY_GPIO,RELAY_PORT);
}

/******************************************************************************
      函数说明：LED初始化
      入口数据：无
      返回值：  无
******************************************************************************/
void LED_init(void)
{   
    /*设置输出*/
    GPIO_SetOutput(LED_GPIO,LED_PORT);

    /*拉低不亮*/
    GPIO_SetLow(LED_GPIO,LED_PORT);
}

/******************************************************************************
      函数说明：步进电机初始化
      入口数据：无
      返回值：  无
******************************************************************************/
void MOTOR_init(void)
{   
    /*设置输出*/
    GPIO_SetOutput(MOTOR_IN1_GPIO,MOTOR_IN1_PORT);
    GPIO_SetOutput(MOTOR_IN2_GPIO,MOTOR_IN2_PORT);
    GPIO_SetOutput(MOTOR_IN3_GPIO,MOTOR_IN3_PORT);
    GPIO_SetOutput(MOTOR_IN4_GPIO,MOTOR_IN4_PORT);

    /*拉低*/
    GPIO_SetLow(MOTOR_IN1_GPIO,MOTOR_IN1_PORT);
    GPIO_SetLow(MOTOR_IN2_GPIO,MOTOR_IN2_PORT);
    GPIO_SetLow(MOTOR_IN3_GPIO,MOTOR_IN3_PORT);
    GPIO_SetLow(MOTOR_IN4_GPIO,MOTOR_IN4_PORT);

}

/******************************************************************************
      函数说明：水位传感器初始化
      入口数据：无
      返回值：  无
******************************************************************************/
void waterlevel_init(void)
{   
    /*设置输入*/
    GPIO_SetInput(WOUT_GPIO,WOUT_PORT);
}

/******************************************************************************
      函数说明：低速传感器初始化
      入口数据：无
      返回值：  无
******************************************************************************/
void speed_sensor_init(void)
{   
    /*设置输入*/
    GPIO_SetInput(DOUT_GPIO,DOUT_PORT);
}

/******************************************************************************
      函数说明：按键初始化
      入口数据：无
      返回值：  无
******************************************************************************/
void key_init(void)
{   
    /*设置输入*/
    GPIO_SetInput(KEY_GPIO,KEY1_PORT);
	GPIO_SetInput(KEY_GPIO,KEY2_PORT);
	GPIO_SetInput(KEY_GPIO,KEY3_PORT);
}

/******************************************************************************
      函数说明：系统初始化
      入口数据：无
      返回值：  无
******************************************************************************/
void System_Port_Init(void)
{
  	LCD_Init();        //TFT屏幕初始化
	relay_init();			//继电器初始化
	beep_init();            //蜂鸣器初始化
    LED_init();              //LED初始化
	LCD_Fill(0,0,LCD_W,LCD_H,WHITE);//填充白色
	LCD_ShowString(10, 100, "   loading...   ",BLUE,WHITE,32,0);//显示加载
    DS18B20_Init();         //温度传感器初始化
    JQ8900_Init(9600);      //语音播报模块初始化
	JQ_volume(volume);          //JQ8900设定音量
	JQ_playNum(2);			//语音播报
	while(Get_Busy_State());//等待语音播报完毕	
	delay_ms(2000);
    ESP8266_Init();         //WIFI模块初始化
    MOTOR_init();           //步进电机初始化
	key_init();				//按键初始化
    waterlevel_init();      //水位传感器初始化
	speed_sensor_init();    //低速传感器初始化
    Gptimer1_Init(1e3,240); //通用定时器1定时1ms一次中断
	LCD_Fill(0,0,LCD_W,LCD_H,WHITE);//填充白色
}

/******************************************************************************
      函数说明：显示设定值
      入口数据：无
      返回值：  无
******************************************************************************/
void InitDisplay(void)   //初始化显示
{
	  unsigned char i=0;
	
	  //显示中文：智能点滴系统
	  LCD_ShowChinese32x32(20,32,"智",BLACK,WHITE,32,0);
	  LCD_ShowChinese32x32(52,32,"能",BLACK,WHITE,32,0);
	  LCD_ShowChinese32x32(84,32,"输",BLACK,WHITE,32,0);
	  LCD_ShowChinese32x32(116,32,"液",BLACK,WHITE,32,0);
	  LCD_ShowChinese32x32(148,32,"系",BLACK,WHITE,32,0);
	  LCD_ShowChinese32x32(180,32,"统",BLACK,WHITE,32,0);

	  //显示中文：滴速
	  LCD_ShowChinese24x24(48,100,"滴",BLACK,WHITE,24,0);
	  LCD_ShowChinese24x24(72,100,"速",BLACK,WHITE,24,0);
	  //显示中文：温度
	  LCD_ShowChinese24x24(48,150,"温",BLACK,WHITE,24,0);
	  LCD_ShowChinese24x24(72,150,"度",BLACK,WHITE,24,0);
	  //显示中文：药水
	  LCD_ShowChinese24x24(48,200,"药",BLACK,WHITE,24,0);
	  LCD_ShowChinese24x24(72,200,"水",BLACK,WHITE,24,0);

	  LCD_ShowChar(96,200,':',BLUE,WHITE,24,0);
}

/******************************************************************************
      函数说明：显示设定值
      入口数据：无
      返回值：  无
******************************************************************************/
void displaySetValue(void)  //显示设置的值
{
	   if(setFlag == 1 || setFlag == 2)
		{
				sprintf(display,"%003d",(int)speed_min);
				LCD_ShowString(120, 100, (uint8_t*)display,BLUE,WHITE,24,0);//显示滴速下限
				sprintf(display,"%003d",(int)speed_max);
				LCD_ShowString(120, 180, (uint8_t*)display,BLUE,WHITE,24,0);//显示滴速上限
		}
		if(setFlag == 3)
		{
				sprintf(display,"%4.1f",(float)set_tempval/10);
				LCD_ShowString(120, 100, (uint8_t*)display,BLUE,WHITE,24,0);//显示温度
		}
		if(setFlag == 4||setFlag == 5)
		{	
			sprintf(display,"%002d",(int)volume);
			if(alarm_enable==0)
			{
				LCD_ShowChinese24x24(120,100,"关",BLACK,WHITE,24,0);	
			}
			if(alarm_enable==1)
			{
				LCD_ShowChinese24x24(120,100,"开",BLACK,WHITE,24,0);	
			}
			LCD_ShowString(120, 180, (uint8_t*)display,BLUE,WHITE,24,0);//显示音量
			JQ_volume(volume);
		}
}

/******************************************************************************
      函数说明：按键检测
      入口数据：无
      返回值：  无
******************************************************************************/
void keyscan(void)   //按键扫描
{
	 unsigned char i=0;
	
	 if(GPIO_Read_Pin(KEY_GPIO,KEY1_PORT)==0) //设置键
	 {
			delay_ms(10);
		  if(GPIO_Read_Pin(KEY_GPIO,KEY1_PORT)==0)
			{
				  while(GPIO_Read_Pin(KEY_GPIO,KEY1_PORT)==0);
				  setFlag ++;
				  if(setFlag == 1)
					{
						LCD_Fill(0,0,LCD_W,LCD_H,WHITE);
	
						  /* 显示边框 */
						  LCD_DrawRectangle(0,0, 240, 240,BLUE);

							//显示中文：设置滴速
						  LCD_ShowChinese24x24(72,24,"设",BLACK,WHITE,24,0);
						  LCD_ShowChinese24x24(100,24,"置",BLACK,WHITE,24,0);
						  LCD_ShowChinese24x24(128,24,"滴",BLACK,WHITE,24,0);
						  LCD_ShowChinese24x24(152,24,"速",BLACK,WHITE,24,0);
						  //显示中文：下限											
						  LCD_ShowChinese24x24(48,100,"下",BLACK,WHITE,24,0);
	 					  LCD_ShowChinese24x24(72,100,"限",BLACK,WHITE,24,0);
						  //显示中文：上限
						  LCD_ShowChinese24x24(48,180,"上",BLACK,WHITE,24,0);
						  LCD_ShowChinese24x24(72,180,"限",BLACK,WHITE,24,0);

						  LCD_ShowChar(96,100,':',BLACK,WHITE,24,0);
						  LCD_ShowChar(96,180,':',BLACK,WHITE,24,0);
						  LCD_ShowString(180, 100, "/min", BLACK,WHITE,24,0);
						  LCD_ShowString(180, 180, "/min", BLACK,WHITE,24,0);
					}
          			if(setFlag == 3)
					{
						   LCD_Fill(96,24,144,180,WHITE);
						  //显示中文：设置温度
		  				  LCD_ShowChinese24x24(72,24,"设",BLACK,WHITE,24,0);
						  LCD_ShowChinese24x24(100,24,"置",BLACK,WHITE,24,0);
						  LCD_ShowChinese24x24(128,24,"温",BLACK,WHITE,24,0);
						  LCD_ShowChinese24x24(152,24,"度",BLACK,WHITE,24,0);
						   //显示中文：下限
						  LCD_ShowChinese24x24(48,100,"下",BLACK,WHITE,24,0);
						  LCD_ShowChinese24x24(72,100,"限",BLACK,WHITE,24,0);

						  LCD_ShowChar(76,100,':',BLACK,WHITE,24,0);
						  //显示摄氏度
						  LCD_ShowChinese24x24(120, 100,"℃",BLACK,WHITE,24,0);   
					}
					if(setFlag == 4)
					{
						   LCD_Fill(96,24,144,180,WHITE);
						  //显示中文：设置温度
		  				  LCD_ShowChinese24x24(72,24,"设",BLACK,WHITE,24,0);
						  LCD_ShowChinese24x24(100,24,"置",BLACK,WHITE,24,0);
						  LCD_ShowChinese24x24(128,24,"报",BLACK,WHITE,24,0);
						  LCD_ShowChinese24x24(152,24,"警",BLACK,WHITE,24,0);
						   //显示中文：下限
						  LCD_ShowChinese24x24(48,100,"报",BLACK,WHITE,24,0);
						  LCD_ShowChinese24x24(72,100,"警",BLACK,WHITE,24,0);
						  LCD_ShowChinese24x24(48,180,"音",BLACK,WHITE,24,0);
						  LCD_ShowChinese24x24(72,180,"量",BLACK,WHITE,24,0);

						  LCD_ShowChar(96,100,':',BLACK,WHITE,24,0);
						  LCD_ShowChar(96,180,':',BLACK,WHITE,24,0);
					}
					if(setFlag >= 6)
					{
						setFlag = 0;
						LCD_Fill(0,0,LCD_W,LCD_H,WHITE);    //清屏
						InitDisplay();
						if(NoWater==1)
						LCD_ShowChinese24x24(120,200,"无",BLACK,WHITE,24,0);//显示中文：无
						else
						LCD_ShowChinese24x24(120,200,"有",BLACK,WHITE,24,0);//显示中文：有  
					}
					displaySetValue();   //显示没有设置值
			}
	 }
	 if(GPIO_Read_Pin(KEY_GPIO,KEY2_PORT)==0) //加键
	 {
			delay_ms(100);
		  if(GPIO_Read_Pin(KEY_GPIO,KEY2_PORT)==0)
			{
				  if(setFlag == 1)
					{
							if(speed_max-speed_min>1)speed_min++;
					}
					if(setFlag == 2)
					{
							if(speed_max<999)speed_max++;
					}
					if(setFlag == 3)
					{
							if(set_tempval<999)set_tempval+=1;
					}
					if(setFlag == 4)
					{
						alarm_enable=1;	
						set_volume=1;
					}
					if(set_volume==1)
					{
						if(setFlag == 5)
						{
							if(volume<90)volume+=10;
						}
					}
					displaySetValue();   //显示设置值
			}
	 }
	 if(GPIO_Read_Pin(KEY_GPIO,KEY3_PORT)==0) //减键
	 {
			delay_ms(100);
		  if(GPIO_Read_Pin(KEY_GPIO,KEY3_PORT)==0)
			{
				  	if(setFlag == 1)
					{
							if(speed_min>0)speed_min--;
					}
					if(setFlag == 2)
					{
							if(speed_max-speed_min>1)speed_max--;
					}
          			if(setFlag == 3)
					{
							if(set_tempval>0)set_tempval-=1;
					}
					if(setFlag == 4)
					{
							alarm_enable=0;	
							set_volume=0;
					}
					if(set_volume==1)
					{
						if(setFlag == 5)
						{	
							if(volume>0)volume-=10;
						}
					}
					displaySetValue();   //显示没有设置值
			}
	 }
}

/******************************************************************************
      函数说明：串口1接收函数
      入口数据：无
      返回值：  无
******************************************************************************/
void UsartSendReceiveData(void)
{
	  unsigned char *dataPtr = NULL;
	  bool point = 0;
	  char *str1=0,i;
	  int  setValue=0;
	  char setvalue[5]={0};
	  char SEND_BUF[150];
	
	  dataPtr = ESP8266_GetIPD(1);   //接收数据
		if(dataPtr != NULL)
		{
			  if(strstr((char *)dataPtr,"SpeedMin:")!=NULL)
				{
					if(alarm_enable==1)
				  {
					GPIO_SetHigh(ALARM_GPIO,ALARM_PORT);
					delay_ms(80);
					GPIO_SetLow(ALARM_GPIO,ALARM_PORT);
				  }
					
						str1 = strstr((char *)dataPtr,"SpeedMin:");
					  
					  while(*str1 < '0' || *str1 > '9')    //判断是不是0到9有效数字
						{
								str1 = str1 + 1;
								delay_ms(10);
						}
						i = 0;
						while(*str1 >= '0' && *str1 <= '9')        //判断是不是0到9有效数字
						{
								setvalue[i] = *str1;
								i ++; str1 ++;
								if(*str1 == ',')break;            //换行符，直接退出while循环
								delay_ms(10);
						}
						setvalue[i] = '\0';            //加上结尾符
						setValue = atoi(setvalue);
						if(setValue>=0 && setValue<=999)
						{
							  speed_min=setValue;
								displaySetValue();
						}
				} 
				if(strstr((char *)dataPtr,"SpeedMax:")!=NULL)
				{
						str1 = strstr((char *)dataPtr,"SpeedMax:");
					  
					  while(*str1 < '0' || *str1 > '9')    //判断是不是0到9有效数字
						{
								str1 = str1 + 1;
								delay_ms(10);
						}
						i = 0;
						while(*str1 >= '0' && *str1 <= '9')        //判断是不是0到9有效数字
						{
								setvalue[i] = *str1;
								i ++; str1 ++;
								if(*str1 == ',')break;            //换行符，直接退出while循环
								delay_ms(10);
						}
						setvalue[i] = '\0';            //加上结尾符
						setValue = atoi(setvalue);
						if(setValue>=0 && setValue<=999)
						{
							  speed_max=setValue;
								displaySetValue();
						}
				}
				if(strstr((char *)dataPtr,"TempMin:")!=NULL)  //接收到设置温度上限的指令
				{
						str1 = strstr((char *)dataPtr,"TempMin:");
					  
					  while(*str1 < '0' || *str1 > '9')        //判断是不是0到9有效数字
						{
								str1 = str1 + 1;
								delay_ms(10);
						}
						i = 0;
						while(*str1 >= '0' && *str1 <= '9')        //判断是不是0到9有效数字
						{
								setvalue[i] = *str1;
								i ++; str1 ++;
								if(*str1 == '.')break;            //换行符，直接退出while循环
								delay_ms(10);
						}
						if(*str1 == '.')
						{
								str1 = str1 + 1;
							  point = 1;
						}
						if(*str1 >= '0' && *str1 <= '9')
						{
								setvalue[i] = *str1;
						}
						i = i + 1;
						setvalue[i] = '\0';            //加上结尾符
						setValue = atoi(setvalue);
						if(point==0)setValue=setValue*10;
						if(setValue>=0 && setValue<=999)
						{
								set_tempval = setValue;    //设置的温度
							  displaySetValue();
						}
				}
				ESP8266_Clear();									//清空缓存
		}
		if(SendFlag==1)    //1秒钟上传一次数据
		{
			  SendFlag = 0;		
			   
				memset(SEND_BUF,0,sizeof(SEND_BUF));   			//清空缓冲区
				sprintf(SEND_BUF,"$speed:%d#,$temp:%.1f#",Speed,(float)temperature/10);
			
			  if(GPIO_Read_Pin(WOUT_GPIO,WOUT_PORT)==0)
				{
						strcat(SEND_BUF,",lack");
				}
				else
				{
						strcat(SEND_BUF,",normal");
				}
			  ESP8266_SendData((uint8_t *)SEND_BUF, strlen(SEND_BUF));
			  ESP8266_Clear();
		}
}

/******************************************************************************
      函数说明：串口1中断服务函数
      入口数据：无
      返回值：  无
******************************************************************************/
void UART1_isr()
{   
  	uint32_t tick = UTIL_GetTick();

    if(UART_IsRawIntActive(UART1,UART_INT_RX))
    {
        UART_ClearInt(UART1,UART_INT_RX);
        // RxCounter=UART_Receive(UART1,rxbuf,8,0);
		
 	while (RxCounter < 8) 
	{
    	if (UART_IsRxFifoEmpty(UART1)) 
		{
      		if (0 && UTIL_GetTick() - tick >0) 
			{
        		break;
      		} 
			else 
			{
       		    continue;
      		}
    	}
   		 rxbuf[RxCounter]= UART1->DR;
    	++RxCounter;
  	}
    }
}

/******************************************************************************
      函数说明：GPTIMER1中断服务函数
      入口数据：无
      返回值：  无
******************************************************************************/
void GPTIMER1_isr()
{   
    static uint16_t timeCount1 = 1000;
	static uint16_t timeCount2 = 4000;
	static uint16_t timeCount3 = 0;
	static uint8_t in_flag = 0;
	static uint8_t index = 0;

    GPTIMER_ClearFlagUpdate(GPTIMER1);
    GPTIMER_EnableCounter(GPTIMER1);

 /********************************************  以下为滴速计算  ***************************************/
 /* 方法：取N个脉冲，计算每个脉冲与脉冲直接的间隔时间，求出平均每个脉冲之间的时间间隔，最终转换为（滴/min） */
			  
	if(GPIO_Read_Pin(DOUT_GPIO,DOUT_PORT)==1)//高电平
	{
		if(in_flag == 0)in_flag = 1;
		if(start_flag) 
		{
		    start_flag = 0;
			total_time += times;  //计算累计的时间
			PulseCount ++;       //脉冲计数，由低电平到高电平的一个跳变才算一个
			times = 0;
		}
	}
	else
	{
		if(in_flag == 1)
		{
			in_flag = 0;
			start_flag = 1;
		}
	}
	if(start_flag)
	{
		times ++;     //脉冲与脉冲之间的时间计数，单位ms
	}
	if(PulseCount!=0)
	{
		timeout ++;
	}
    if(((timeout/1000) >= SampleTimeout) || ((times/1000) >= SampleTimeout))  //取样时间太长了,取消计时
	{
		/* 相关参数清0 */
		total_time = 0;
		PulseCount = 0;
		start_flag = 0;
		Speed = 0;
		timeout = 0;
		times = 0;
	}
				
	if(PulseCount>=SampleNum)
	{
		PulseCount = 0;
		total_time = total_time / SampleNum; //计算平均每个取样脉冲之间的时间
		Speed = 60000 / total_time;         //转换为  滴/min					
		total_time = 0;
		timeout = 0;
	}
	/********************************************   END    *********************************************/
	/**************************************************************************************************/
				
	timeCount1 ++;
	timeCount2 ++;
	timeCount3 ++;
				
	if(timeCount1>=1000)  //1s
	{
		timeCount1 = 0;
		update = 1;
		SendFlag = 1;	
	}

	if(timeCount3>=300)  //300ms
	{
		timeCount3 = 0;				  
		blink = !blink;
		if(!setFlag)
		{
			if(NoWater==1)  //无药水，蜂鸣器一直报警
			{	
				if(alarm_enable==1)
				{
				 GPIO_Toggle(ALARM_GPIO,ALARM_PORT);
				 JQ_playNum(5);
				 while(Get_Busy_State());	
				}			 
			}
			else
			{   
                /*滴速温度，滴速不在范围，蜂鸣器报警*/
			    if(((Speed!=0)&&((Speed<speed_min)||(Speed>speed_max)))||(temperature<set_tempval))
				{	
					if(alarm_enable==1)
					{
					GPIO_Toggle(ALARM_GPIO,ALARM_PORT);
					JQ_playNum(3);
				 	while(Get_Busy_State());
					}	
				}
				else
				{
					 GPIO_SetLow(ALARM_GPIO,ALARM_PORT);
				}	
			}
		}
		else
		{
			 GPIO_SetLow(ALARM_GPIO,ALARM_PORT);
		}
	}
	if(direction == 0)       //步进电机停止
	{
		Coil_OFF ;
	}
	else if(direction == 1)  //步进电机正转
	{
		switch(index) 
		{
			case 0:Coil_8B_A; break; 
			case 1:Coil_8B_AB;break;  
			case 2:Coil_8B_B; break; 
			case 3:Coil_8B_BC;break; 
			case 4:Coil_8B_C; break; 
			case 5:Coil_8B_CD;break;  
			case 6:Coil_8B_D; break; 
			case 7:Coil_8B_DA;break; 
		}
		index ++; 
		if(index == 8) index = 0;
	}
	else if(direction == 2)  //步进电机反转
	{
		switch(index) 
		{
		case 0:Coil_8B_DA; break; 
		case 1:Coil_8B_D;  break;  
		case 2:Coil_8B_CD; break; 
		case 3:Coil_8B_C;  break; 
		case 4:Coil_8B_BC; break; 
		case 5:Coil_8B_B;  break;  
		case 6:Coil_8B_AB; break; 
		case 7:Coil_8B_A;  break; 
		}
		index ++; 
		if(index == 8) index = 0;
		
	}

}
      
