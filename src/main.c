#include "main.h"

int main(void)
{      
    /*�弶��ʼ��*/
    board_init();  
    
    /*ϵͳ�����ʼ��*/
    System_Port_Init();
    
    while (1)
    {   
        keyscan();  //����ɨ��
	    if(setFlag == 0)   //��������״̬��
		{
			if((Speed!=0)&&((Speed<speed_min)||(Speed>speed_max))&&blink)   //���ٲ��ڷ�Χ����˸��ʾ
			{
				LCD_Fill(48,100,144,124,WHITE);//��ʾ�հ�
			}
			else
			{
				sprintf(display,": %003d/min ",(int)Speed);
                LCD_ShowString(96, 100, (uint8_t*)display,BLUE,WHITE,24,0);//��ʾ����
			}
			if(Speed!=0)
			{
				if(Speed<speed_min) direction=1;         //���ٹ����������ת��ģ��ſ����ڷ�
			    else if(Speed>speed_max) direction=2;    //���ٹ��죬�����ת��ģ���ս����ڷ�
				else direction=0;  //���������ٷ�Χ�����ֹͣ
			}
			else
			{
				 direction=0;  //ֹͣ
			}
					 
			if(blink == 1)
			{
				blink= 0;
				temperature= (DS18B20_ReadTemp()/10%100)*10;           //��ȡ�¶�
				if(temperature < set_tempval)GPIO_Set_Pin(RELAY_GPIO,RELAY_PORT,1);//�¶ȵͣ���������
				else GPIO_Set_Pin(RELAY_GPIO,RELAY_PORT,0);//�¶ȴﵽ����ֵ���رռ���
			}
            if(temperature<set_tempval && blink)      //�¶Ȳ��ڷ�Χ����˸��ʾ
		    {
				LCD_Fill(48,160,144,204,WHITE);//��ʾ�հ�
			}
			else
			{
				sprintf(display,": %4.1f",(float)temperature/10);
				LCD_ShowString(96, 150, (uint8_t*)display,BLUE,WHITE,24,0);//��ʾ�¶�
				LCD_ShowChinese24x24(120, 150,"��",BLACK,WHITE,24,0);   
			}

			if(GPIO_Read_Pin(WOUT_GPIO,WOUT_PORT) == 0)   //��ҩˮ
			{
				if(NoWater==0)
				LCD_ShowChinese24x24(80,220,"��",BLACK,WHITE,24,0);//��ʾ���ģ���
				NoWater  = 1;    //ȱˮ��־
			}
			else
			{
				if(NoWater==1)
				LCD_ShowChinese24x24(80,220,"��",BLACK,WHITE,24,0);//��ʾ���ģ���  
				 NoWater  = 0;
			}
		}
		UsartSendReceiveData();
		delay_ms(10);
        
    }  
}