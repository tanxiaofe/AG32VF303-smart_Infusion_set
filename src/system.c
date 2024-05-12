#include "system.h"

/*����ȡ������*/
unsigned long times=0;        //ȡ��ʱ��
unsigned long total_time=0;   //ȡ����ʱ��
unsigned long timeout=0;      //ȡ����ʱʱ�� 
bool start_flag=0;            //��ʼ��ʱ��־

int PulseCount = 0;     //��¼����
int Speed = 0;          //����
int speed_min = 60;      //��������
int speed_max = 180;     //��������

/*�¶��趨����*/
short temperature=0;  //�¶�
short set_tempval=200; //�¶�����ֵ

/*�����������*/
uint8_t setFlag = 0;  //���ñ�־
uint16_t direction = 0;//�����־

/*ˮλ����*/
bool SendFlag = 0;
bool blink = 0;    //��˸��־
bool update = 0;  //ˢ�±�־
bool NoWater = 0;  //ȱˮ  ����־

/*lcd��ʾ����*/
char display[16];

/*������ض���*/
volatile char isRecv = 0;
unsigned int RxCounter = 0;
char rxbuf[150];


/******************************************************************************
      ����˵������ȡGPIO����
      ������ݣ�GPIO���˿� bit������
      ����ֵ��  bit״̬ SET��RESET
******************************************************************************/
uint8_t GPIO_Read_Pin(GPIO_TypeDef *gpio, uint8_t bits)
{
    /*�ж�GPIO�����ƽ*/
    if(GPIO_GetValue(gpio,bits)!=0)
    {
        return GPIO_PIN_SET;   //�ж�����λΪ1ʱ����
    }
    else
    {
        return GPIO_PIN_RESET; //�ж�����λΪ0ʱ����
    }
}

/******************************************************************************
      ����˵������λGPIO����
      ������ݣ�GPIO���˿� bit������ value����1��0
      ����ֵ��  ��
******************************************************************************/
void GPIO_Set_Pin(GPIO_TypeDef *gpio, uint8_t bits,uint8_t value)
{
    /*GPIO������������Լ���ƽ*/
    GPIO_SetValue(gpio, bits, value*255);
}

/******************************************************************************
      ����˵������������ʼ��
      ������ݣ���
      ����ֵ��  ��
******************************************************************************/
void beep_init(void)
{   
    /*�������*/
    GPIO_SetOutput(ALARM_GPIO,ALARM_PORT);

    /*���Ͳ�����*/
    GPIO_SetLow(ALARM_GPIO,ALARM_PORT);
}

/******************************************************************************
      ����˵�����̵�����ʼ��
      ������ݣ���
      ����ֵ��  ��
******************************************************************************/
void relay_init(void)
{   
    /*�������*/
    GPIO_SetOutput(RELAY_GPIO,RELAY_PORT);

    /*���Ͳ�����*/
    GPIO_SetLow(RELAY_GPIO,RELAY_PORT);
}

/******************************************************************************
      ����˵����LED��ʼ��
      ������ݣ���
      ����ֵ��  ��
******************************************************************************/
void LED_init(void)
{   
    /*�������*/
    GPIO_SetOutput(LED_GPIO,LED_PORT);

    /*���Ͳ���*/
    GPIO_SetLow(LED_GPIO,LED_PORT);
}

/******************************************************************************
      ����˵�������������ʼ��
      ������ݣ���
      ����ֵ��  ��
******************************************************************************/
void MOTOR_init(void)
{   
    /*�������*/
    GPIO_SetOutput(MOTOR_IN1_GPIO,MOTOR_IN1_PORT);
    GPIO_SetOutput(MOTOR_IN2_GPIO,MOTOR_IN2_PORT);
    GPIO_SetOutput(MOTOR_IN3_GPIO,MOTOR_IN3_PORT);
    GPIO_SetOutput(MOTOR_IN4_GPIO,MOTOR_IN4_PORT);

    /*����*/
    GPIO_SetLow(MOTOR_IN1_GPIO,MOTOR_IN1_PORT);
    GPIO_SetLow(MOTOR_IN2_GPIO,MOTOR_IN2_PORT);
    GPIO_SetLow(MOTOR_IN3_GPIO,MOTOR_IN3_PORT);
    GPIO_SetLow(MOTOR_IN4_GPIO,MOTOR_IN4_PORT);

}

/******************************************************************************
      ����˵����ˮλ��������ʼ��
      ������ݣ���
      ����ֵ��  ��
******************************************************************************/
void waterlevel_init(void)
{   
    /*��������*/
    GPIO_SetInput(ALARM_GPIO,ALARM_PORT);
}

/******************************************************************************
      ����˵����������ʼ��
      ������ݣ���
      ����ֵ��  ��
******************************************************************************/
void key_init(void)
{   
    /*��������*/
    GPIO_SetInput(KEY_GPIO,KEY1_PORT);
	GPIO_SetInput(KEY_GPIO,KEY2_PORT);
	GPIO_SetInput(KEY_GPIO,KEY3_PORT);
}

/******************************************************************************
      ����˵����ϵͳ��ʼ��
      ������ݣ���
      ����ֵ��  ��
******************************************************************************/
void System_Port_Init(void)
{
  	TFT_GPIO_Init();        //TFT��Ļ��ʼ��
	LCD_ShowString(100, 180, "   loading...   ",BLUE,WHITE,32,0);//��ʾ����
    DS18B20_Init();         //�¶ȴ�������ʼ��
    JQ8900_Init(9600);      //��������ģ���ʼ��
	JQ_volume(99);          //JQ8900�趨����99
	JQ_playNum(2);			//��������
	while(Get_Busy_State());//�ȴ������������	
    ESP8266_Init();         //WIFIģ���ʼ��
    beep_init();            //��������ʼ��
	relay_init();			//�̵�����ʼ��
    LED_init();              //LED��ʼ��
    MOTOR_init();           //���������ʼ��
	key_init();				//������ʼ��
    waterlevel_init();      //ˮλ��������ʼ��
    Gptimer1_Init(1e3,240); //ͨ�ö�ʱ��1��ʱ1msһ���ж�
}

/******************************************************************************
      ����˵������ʾ�趨ֵ
      ������ݣ���
      ����ֵ��  ��
******************************************************************************/
void InitDisplay(void)   //��ʼ����ʾ
{
	  unsigned char i=0;
	
	  //��ʾ���ģ����ܵ��ϵͳ
	  LCD_ShowChinese32x32(62,32,"��",BLACK,WHITE,32,0);
	  LCD_ShowChinese32x32(96,32,"��",BLACK,WHITE,32,0);
	  LCD_ShowChinese32x32(130,32,"��",BLACK,WHITE,32,0);
	  LCD_ShowChinese32x32(164,32,"Һ",BLACK,WHITE,32,0);
	  LCD_ShowChinese32x32(198,32,"ϵ",BLACK,WHITE,32,0);
	  LCD_ShowChinese32x32(222,32,"ͳ",BLACK,WHITE,32,0);

	  //��ʾ���ģ�����
	  LCD_ShowChinese24x24(48,100,"��",BLACK,WHITE,24,0);
	  LCD_ShowChinese24x24(72,100,"��",BLACK,WHITE,24,0);
	  //��ʾ���ģ��¶�
	  LCD_ShowChinese24x24(48,160,"��",BLACK,WHITE,24,0);
	  LCD_ShowChinese24x24(72,160,"��",BLACK,WHITE,24,0);
	  //��ʾ���ģ�ҩˮ
	  LCD_ShowChinese24x24(48,220,"ҩ",BLACK,WHITE,24,0);
	  LCD_ShowChinese24x24(72,220,"ˮ",BLACK,WHITE,24,0);

	  LCD_ShowChar(76,220,':',BLACK,WHITE,24,0);
}

/******************************************************************************
      ����˵������ʾ�趨ֵ
      ������ݣ���
      ����ֵ��  ��
******************************************************************************/
void displaySetValue(void)  //��ʾ���õ�ֵ
{
	  if(setFlag == 1 || setFlag == 2)
		{
				sprintf(display,"%003d",(int)speed_min);
				LCD_ShowString(96, 100, (uint8_t*)display,BLUE,WHITE,24,setFlag+1-1);//��ʾ��������
				sprintf(display,"%003d",(int)speed_max);
				LCD_ShowString(96, 180, (uint8_t*)display,BLUE,WHITE,24,setFlag+1-2);//��ʾ��������
		}
		if(setFlag == 3)
		{
				sprintf(display,"%4.1f",(float)set_tempval/10);
				LCD_ShowString(96, 100, (uint8_t*)display,BLUE,WHITE,24,setFlag+1-3);//��ʾ�¶�
		}
}

/******************************************************************************
      ����˵�����������
      ������ݣ���
      ����ֵ��  ��
******************************************************************************/
void keyscan(void)   //����ɨ��
{
	 unsigned char i=0;
	
	 if(GPIO_Read_Pin(KEY_GPIO,KEY1_PORT)==0) //���ü�
	 {
			delay_ms(15);
		  if(GPIO_Read_Pin(KEY_GPIO,KEY1_PORT)==0)
			{
				  while(GPIO_Read_Pin(KEY_GPIO,KEY1_PORT)==0);
				  setFlag ++;
				  if(setFlag == 1)
					{
						LCD_Fill(0,0,LCD_W,LCD_H,WHITE);
	
						  /* ��ʾֱ�߿� */
							LCD_DrawLine(0,0,240,0,BLUE);
							LCD_DrawLine(0,0,0,240,BLUE);
							LCD_DrawLine(240,0,0,240,BLUE);

							//��ʾ���ģ����õ���
						  LCD_ShowChinese24x24(96,24,"��",BLACK,WHITE,24,0);
						  LCD_ShowChinese24x24(124,24,"��",BLACK,WHITE,24,0);
						  LCD_ShowChinese24x24(148,24,"��",BLACK,WHITE,24,0);
						  LCD_ShowChinese24x24(172,24,"��",BLACK,WHITE,24,0);
						  //��ʾ���ģ�����											
						  LCD_ShowChinese24x24(48,100,"��",BLACK,WHITE,24,0);
						  LCD_ShowChinese24x24(72,100,"��",BLACK,WHITE,24,0);
						  //��ʾ���ģ�����
						  LCD_ShowChinese24x24(48,180,"��",BLACK,WHITE,24,0);
						  LCD_ShowChinese24x24(72,180,"��",BLACK,WHITE,24,0);

						  LCD_ShowChar(76,100,':',BLACK,WHITE,24,0);
						  LCD_ShowChar(76,180,':',BLACK,WHITE,24,0);
						  LCD_ShowString(144, 100, "/min", BLACK,WHITE,24,0);
						  LCD_ShowString(144, 180, "/min", BLACK,WHITE,24,0);
					}
          			if(setFlag == 3)
					{
						   LCD_Fill(96,24,144,180,WHITE);
						  //��ʾ���ģ������¶�
						  LCD_ShowChinese24x24(96,24,"��",BLACK,WHITE,24,0);
						  LCD_ShowChinese24x24(124,24,"��",BLACK,WHITE,24,0);
						  LCD_ShowChinese24x24(148,24,"��",BLACK,WHITE,24,0);
						  LCD_ShowChinese24x24(172,24,"��",BLACK,WHITE,24,0);
						   //��ʾ���ģ�����
						  LCD_ShowChinese24x24(48,100,"��",BLACK,WHITE,24,0);
						  LCD_ShowChinese24x24(72,100,"��",BLACK,WHITE,24,0);

						  LCD_ShowChar(76,100,':',BLACK,WHITE,24,0);
						  //��ʾ���϶�
						  LCD_ShowChinese24x24(120, 100,"��",BLACK,WHITE,24,0);   
					}
					if(setFlag >= 4)
					{
						setFlag = 0;
						LCD_Fill(0,0,LCD_W,LCD_H,WHITE);    //����
						InitDisplay();
						if(NoWater==1)
						LCD_ShowChinese24x24(80,220,"��",BLACK,WHITE,24,0);//��ʾ���ģ���
						else
						LCD_ShowChinese24x24(80,220,"��",BLACK,WHITE,24,0);//��ʾ���ģ���  
					}
					displaySetValue();   //��ʾû������ֵ
			}
	 }
	 if(GPIO_Read_Pin(KEY_GPIO,KEY2_PORT)==0) //�Ӽ�
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
					displaySetValue();   //��ʾû������ֵ
			}
	 }
	 if(GPIO_Read_Pin(KEY_GPIO,KEY3_PORT)==0) //����
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
					displaySetValue();   //��ʾû������ֵ
			}
	 }
}

/******************************************************************************
      ����˵��������1���պ���
      ������ݣ���
      ����ֵ��  ��
******************************************************************************/
void UsartSendReceiveData(void)
{
		unsigned char *dataPtr = NULL;
	  bool point = 0;
		char *str1=0,i;
	  int  setValue=0;
	  char setvalue[5]={0};
	  char SEND_BUF[150];
	
	  dataPtr = ESP8266_GetIPD(1);   //��������
		if(dataPtr != NULL)
		{
			  if(strstr((char *)dataPtr,"SpeedMin:")!=NULL)
				{
					GPIO_SetHigh(ALARM_GPIO,ALARM_PORT);
					delay_ms(80);
					GPIO_SetLow(ALARM_GPIO,ALARM_PORT);
					
						str1 = strstr((char *)dataPtr,"SpeedMin:");
					  
					  while(*str1 < '0' || *str1 > '9')    //�ж��ǲ���0��9��Ч����
						{
								str1 = str1 + 1;
								delay_ms(10);
						}
						i = 0;
						while(*str1 >= '0' && *str1 <= '9')        //�ж��ǲ���0��9��Ч����
						{
								setvalue[i] = *str1;
								i ++; str1 ++;
								if(*str1 == ',')break;            //���з���ֱ���˳�whileѭ��
								delay_ms(10);
						}
						setvalue[i] = '\0';            //���Ͻ�β��
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
					  
					  while(*str1 < '0' || *str1 > '9')    //�ж��ǲ���0��9��Ч����
						{
								str1 = str1 + 1;
								delay_ms(10);
						}
						i = 0;
						while(*str1 >= '0' && *str1 <= '9')        //�ж��ǲ���0��9��Ч����
						{
								setvalue[i] = *str1;
								i ++; str1 ++;
								if(*str1 == ',')break;            //���з���ֱ���˳�whileѭ��
								delay_ms(10);
						}
						setvalue[i] = '\0';            //���Ͻ�β��
						setValue = atoi(setvalue);
						if(setValue>=0 && setValue<=999)
						{
							  speed_max=setValue;
								displaySetValue();
						}
				}
				if(strstr((char *)dataPtr,"TempMin:")!=NULL)  //���յ������¶����޵�ָ��
				{
						str1 = strstr((char *)dataPtr,"TempMin:");
					  
					  while(*str1 < '0' || *str1 > '9')        //�ж��ǲ���0��9��Ч����
						{
								str1 = str1 + 1;
								delay_ms(10);
						}
						i = 0;
						while(*str1 >= '0' && *str1 <= '9')        //�ж��ǲ���0��9��Ч����
						{
								setvalue[i] = *str1;
								i ++; str1 ++;
								if(*str1 == '.')break;            //���з���ֱ���˳�whileѭ��
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
						setvalue[i] = '\0';            //���Ͻ�β��
						setValue = atoi(setvalue);
						if(point==0)setValue=setValue*10;
						if(setValue>=0 && setValue<=999)
						{
								set_tempval = setValue;    //���õ��¶�
							  displaySetValue();
						}
				}
				ESP8266_Clear();									//��ջ���
		}
		if(SendFlag==1)    //1�����ϴ�һ������
		{
			  SendFlag = 0;		
			   
				memset(SEND_BUF,0,sizeof(SEND_BUF));   			//��ջ�����
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
      ����˵��������1�жϷ�����
      ������ݣ���
      ����ֵ��  ��
******************************************************************************/
void UART1_isr()
{   
    if(UART_IsRawIntActive(UART1,UART_INT_RX))
    {
        UART_ClearInt(UART1,UART_INT_RX);
        UART_Receive(UART1,rxbuf,8,0);
        RxCounter=UART_Receive(UART1,rxbuf,8,0);
        isRecv = 1;
    }
}

/******************************************************************************
      ����˵����GPTIMER1�жϷ�����
      ������ݣ���
      ����ֵ��  ��
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

 /********************************************  ����Ϊ���ټ���  ***************************************/
 /* ������ȡN�����壬����ÿ������������ֱ�ӵļ��ʱ�䣬���ƽ��ÿ������֮���ʱ����������ת��Ϊ����/min�� */
			  
	if(GPIO_Read_Pin(WOUT_GPIO,WOUT_PORT)==1)//�ߵ�ƽ
	{
		if(in_flag == 0)in_flag = 1;
		if(start_flag) 
		{
		    start_flag = 0;
			total_time += times;  //�����ۼƵ�ʱ��
			PulseCount ++;       //����������ɵ͵�ƽ���ߵ�ƽ��һ���������һ��
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
		times ++;     //����������֮���ʱ���������λms
	}
	if(PulseCount!=0)
	{
		timeout ++;
	}
    if(((timeout/1000) >= SampleTimeout) || ((times/1000) >= SampleTimeout))  //ȡ��ʱ��̫����,ȡ����ʱ
	{
		/* ��ز�����0 */
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
		total_time = total_time / SampleNum; //����ƽ��ÿ��ȡ������֮���ʱ��
		Speed = 60000 / total_time;         //ת��Ϊ  ��/min					
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
			if(NoWater==1)  //��ҩˮ��������һֱ����
			{
				 GPIO_SetHigh(ALARM_GPIO,ALARM_PORT);

				 JQ_playNum(5);
				 while(Get_Busy_State());				 
			}
			else
			{   
                /*�����¶�����Ѫ�����ڷ�Χ������������*/
			    if(((Speed!=0)&&((Speed<speed_min)||(Speed>speed_max)))||(temperature<set_tempval))
				{
					GPIO_Toggle(ALARM_GPIO,ALARM_PORT);
					JQ_playNum(3);
				 	while(Get_Busy_State());	
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
	if(direction == 0)       //�������ֹͣ
	{
		Coil_OFF ;
	}
	else if(direction == 1)  //���������ת
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
	else if(direction == 2)  //���������ת
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
      
