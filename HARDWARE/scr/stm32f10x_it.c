/*-------------------------------------------------*/
/*                                                 */
/*            	   �жϷ�����          	     	   */
/*                                                 */
/*-------------------------------------------------*/

#include "stm32f10x.h"     //������Ҫ��ͷ�ļ�
#include "stm32f10x_it.h"  //������Ҫ��ͷ�ļ�
#include "delay.h"         //������Ҫ��ͷ�ļ�
#include "control.h"       //������Ҫ��ͷ�ļ�
#include "usart1.h"        //������Ҫ��ͷ�ļ�
#include "usart2.h"        //������Ҫ��ͷ�ļ�
#include "timer1.h"        //������Ҫ��ͷ�ļ�
#include "timer4.h"        //������Ҫ��ͷ�ļ�
#include "key.h"           //������Ҫ��ͷ�ļ� ��ժ��
#include "exti.h"          //������Ҫ��ͷ�ļ� ��ժ��
#include "wifi.h"	         //������Ҫ��ͷ�ļ�
#include "timer3.h"        //������Ҫ��ͷ�ļ�
#include "mqtt.h"          //������Ҫ��ͷ�ļ�
#include "dht11.h"         //������Ҫ��ͷ�ļ�
#include "bh1750.h"        //������Ҫ��ͷ�ļ�
#include "adc.h"           //������Ҫ��ͷ�ļ�
#include "myiic.h"         //������Ҫ��ͷ�ļ� ���� �¶� ��ѹ I2C
#include "bmp280.h" 		   //������Ҫ��ͷ�ļ� �ռ��¶� ���� ��ѹ
#include "ds18b20.h" 			 //������Ҫ��ͷ�ļ� �����¶�


extern int dataFlag;       //������״̬���������ݣ�
extern int motorFlag;			 //������״̬
extern int watarFlag;      //ˮ��״̬
extern int ledFlag;        //�����״̬                   
extern int peoFlag;     	 //��ǰ����

extern int keyHuangTotal;  //�ƹϵ�ǰ��
extern int keyXiTotal;     //��������ǰ��
extern int keyQieTotal;    //���ӵ�ǰ��


/*-------------------------------------------------*/
/*������������2�����жϺ���                        */
/*��  ������                                       */
/*����ֵ����                                       */
/*-------------------------------------------------*/
void USART2_IRQHandler(void)   
{                      
	if(USART_GetITStatus(USART2, USART_IT_RXNE) != RESET)//���USART_IT_RXNE��־��λ����ʾ�����ݵ��ˣ�����if��֧
	{  
		if(connectFlag==0) 											           //���connectFlag����0����ǰ��û�����ӷ�����������ָ������״̬
		{                                     
			if(USART2->DR)
			{                                     					 //����ָ������״̬ʱ������ֵ�ű��浽������	
				Usart2_RxBuff[Usart2_RxCounter]=USART2->DR;    //���浽������	
				Usart2_RxCounter ++;                           //ÿ����1���ֽڵ����ݣ�Usart2_RxCounter��1����ʾ���յ���������+1 
			}		
		}
		else
		{		                                           	   //��֮connectFlag����1�������Ϸ�������	
			Usart2_RxBuff[Usart2_RxCounter] = USART2->DR;    //�ѽ��յ������ݱ��浽Usart2_RxBuff��				
			if(Usart2_RxCounter == 0)
			{    									    											 //���Usart2_RxCounter����0����ʾ�ǽ��յĵ�1�����ݣ�����if��֧				
				TIM_Cmd(TIM4,ENABLE); 
			}
			else																						 //else��֧����ʾ��Usart2_RxCounter������0�����ǽ��յĵ�һ������
			{                        									    
				TIM_SetCounter(TIM4,0);  
			}	
			Usart2_RxCounter++;         				    				 //ÿ����1���ֽڵ����ݣ�Usart2_RxCounter��1����ʾ���յ���������+1 
		}
	}
} 
/*-------------------------------------------------*/
/*����������ʱ��4�жϷ�����                      */
/*��  ������                                       */
/*����ֵ����                                       */
/*-------------------------------------------------*/
void TIM4_IRQHandler(void)
{
	if(TIM_GetITStatus(TIM4, TIM_IT_Update) != RESET)//���TIM_IT_Update��λ����ʾTIM4����жϣ�����if	
	{                	
		memcpy(&MQTT_RxDataInPtr[2],Usart2_RxBuff,Usart2_RxCounter); 		//�������ݵ����ջ�����
		MQTT_RxDataInPtr[0] = Usart2_RxCounter/256;                     //��¼���ݳ��ȸ��ֽ�
		MQTT_RxDataInPtr[1] = Usart2_RxCounter%256;                     //��¼���ݳ��ȵ��ֽ�
		MQTT_RxDataInPtr+=RBUFF_UNIT;                                   //ָ������
		if(MQTT_RxDataInPtr==MQTT_RxDataEndPtr)                         //���ָ�뵽������β����
			MQTT_RxDataInPtr = MQTT_RxDataBuf[0];                         //ָ���λ����������ͷ
		Usart2_RxCounter = 0;                                           //����2������������������
		TIM_SetCounter(TIM3, 0);                                        //���㶨ʱ��6�����������¼�ʱping������ʱ��
		TIM_Cmd(TIM4, DISABLE);                        				  				//�ر�TIM4��ʱ��
		TIM_SetCounter(TIM4, 0);                        			          //���㶨ʱ��4������
		TIM_ClearITPendingBit(TIM4, TIM_IT_Update);     			          //���TIM4����жϱ�־ 	
	}
}
/*-------------------------------------------------*/
/*����������ʱ��3�жϷ�����                      */
/*��  ������                                       */
/*����ֵ����                                       */
/*-------------------------------------------------*/
void TIM3_IRQHandler(void)
{
	if(TIM_GetITStatus(TIM3, TIM_IT_Update) != RESET)//���TIM_IT_Update��λ����ʾTIM3����жϣ�����if	
		{  
		switch(pingFlag) 					   //�ж�pingFlag��״̬
		{                               
			case 0:										 //���pingFlag����0����ʾ����״̬������Ping����  
					MQTT_PingREQ(); 		   //���Ping���ĵ����ͻ�����  
					break;
			case 1:										 //���pingFlag����1��˵����һ�η��͵���ping���ģ�û���յ��������ظ�������1û�б����Ϊ0�������������쳣������Ҫ��������pingģʽ
					TIM3_ENABLE_2S(); 	   //���ǽ���ʱ��6����Ϊ2s��ʱ,���ٷ���Ping����
					MQTT_PingREQ();  			 //���Ping���ĵ����ͻ�����  
					break;
			case 2:										 //���pingFlag����2��˵����û���յ��������ظ�
			case 3:				             //���pingFlag����3��˵����û���յ��������ظ�
			case 4:				             //���pingFlag����4��˵����û���յ��������ظ�	
					MQTT_PingREQ();  			 //���Ping���ĵ����ͻ����� 
					break;
			case 5:										 //���pingFlag����5��˵�����Ƿ����˶��ping�����޻ظ���Ӧ�������������⣬������������
					connectFlag = 0;       //����״̬��0����ʾ�Ͽ���û���Ϸ�����
					TIM_Cmd(TIM3,DISABLE); //��TIM3 				
					break;			
		}
		pingFlag++;           		   //pingFlag����1����ʾ�ַ�����һ��ping���ڴ��������Ļظ�
		TIM_ClearITPendingBit(TIM3, TIM_IT_Update);      //���TIM3����жϱ�־ 	
	}
}



/*-------------------------------------------------*/
/*����������ʱ��2�жϷ�����                      */
/*��  ������                                       */
/*����ֵ����                                       */
/*-------------------------------------------------*/
void TIM2_IRQHandler(void)
{
	char  humidity;           //����һ������������ʪ��ֵ
	char  temperature;        //����һ�������������¶�ֵ	
	float bmp280_temp;			  //����һ�������������ⲿ�����¶�ֵ
  float bmp280_press;			  //����һ��������������ѹֵ
	float bmp280_high;			  //����һ�����������溣��
	int   p, t, h;						//����������������floatת��Ϊint
	int   soil_hum;           //����һ����������������ʪ��
	int   sun_light;					//����һ���������������
			
	char  head1[3];
	char  temp[200];          //����һ����ʱ������1
	char  tempAll[220];       //����һ����ʱ������2

	
	int   Init_len = 100;     //��ų�ʼ����
		
  char  soilHum[20];        //�������ʪ��
	int   soilHum_len;        //����ʪ�ȳ���

 	char  sunLight[20];       //��Ź���ǿ��		
	int   sunLight_len;		    //����ǿ�ȳ���
	 
  char  press[20];          //��Ŵ���ѹ
  int   press_len;          //����ѹ����
		
	char  airTemp[20];        //����ܴ����¶�
	int   airTemp_len;			  //�����¶ȳ���
		
	char  high[20];           //��ź���
	int   high_len;				    //���γ���	
	

	if(TIM_GetITStatus(TIM2, TIM_IT_Update) != RESET) //���TIM_IT_Update��λ����ʾTIM2����жϣ�����if	
	{ 
		DHT11_Read_Data(&temperature,&humidity);	              //��ȡ��ʪ��ֵ
		soil_hum = Soil_Temp();											            //��ȡ����ʪ��ֵ
	  sun_light = Get_Sunlight_Value();	
		
		bmp280GetData(&bmp280_press,&bmp280_temp,&bmp280_high); //��ȡ��ѹ�����Ρ������¶�
		p = (int)bmp280_press;
		t = (int)DS18B20_Get_Temp()/10;
		h = (int)bmp280_high;
	  
		
		printf("bmp280_press:%d\r\n",p);
		printf("bmp280_temp :%d\r\n",t);
		printf("bmp280_high :%d\r\n",h);	
		u1_printf("%d\r\n",soil_hum);
		
		
	
		memset(temp,    0, 200);                            //��ջ�����1
		memset(tempAll, 0, 220);                            //��ջ�����2
		memset(head1,   0, 3);                              //���MQTTͷ
		memset(soilHum, 0, 20);                             //��������¶Ȼ����� 
		memset(sunLight,0, 20);                             //��չ���ǿ�Ȼ�����
		memset(press,   0, 20);                             //���ѹ��������
		memset(airTemp, 0, 20);                             //��տ����¶Ȼ�����
		memset(high,    0, 20);                             //��պ��λ�����

									
		sprintf(soilHum, "%d", soil_hum);     						  // �����¶ȳ���
		soilHum_len = length(soil_hum);	
		u1_printf("soil_hum���ȣ�%d\r\n",soilHum_len);
		sprintf(sunLight, "%d", sun_light);   					  	// ����ǿ�ȳ���
		sunLight_len = length(sun_light);
		u1_printf("sun_light���ȣ�%d\r\n",sunLight_len);
		sprintf(press, "%d", p);    					              // ����ѹ����
		press_len = length(p);	
		u1_printf("p���ȣ�%d\r\n",press_len);
    sprintf(airTemp, "%d", t); 						              // �����¶ȳ���
		airTemp_len = length(t);	
		u1_printf("t���ȣ�%d\r\n",airTemp_len);
		sprintf(high, "%d", h); 					    						  // ���γ���
		high_len = length(h);	
		u1_printf("h���ȣ�%d\r\n",high_len);

	  sprintf(temp,"{\"temperature\":\"%d%d\",\"humidity\":\"%d%d\",",
					  temperature/10, temperature%10, humidity/10, humidity%10);//��������


		sprintf(soilHum, "\"soilHum\":\"%d\",", soil_hum);
		strcat(temp, soilHum);
		
		sprintf(sunLight, "\"sunLight\":\"%d\",", sun_light);
		strcat(temp, sunLight);
		
		sprintf(press, "\"press\":\"%d\",", p);
		strcat(temp, press);
		
		sprintf(airTemp, "\"airTemp\":\"%d\",", t);
		strcat(temp, airTemp);
		
		sprintf(high, "\"high\":\"%d\"}", h);
		strcat(temp, high);
		
		head1[0] = 0x03; 					//�̶���ͷ
		head1[1] = 0x00; 					//�̶���ͷ
		head1[2] = strlen(temp);  //ʣ�೤��	
		sprintf(tempAll,"%c%c%c%s", head1[0], head1[1], head1[2],temp);


    Init_len = Init_len + soilHum_len + sunLight_len + press_len + airTemp_len + high_len;
		u1_printf("%s\r\n",tempAll+3);
		u1_printf("�ܳ��ȣ�%d\r\n",Init_len);
		MQTT_PublishQs0(Data_TOPIC_NAME,tempAll,Init_len);     //������ݣ�������������
		
		//SEND_Data();
		//SEND_Veg();
		TIM_ClearITPendingBit(TIM2, TIM_IT_Update);            //���TIM2����жϱ�־ 	
	}
}
/*-------------------------------------------------*/
/*�������������ⲿ�жϷ�����                      */
/*��  ������                                       */
/*����ֵ����                                       */
/*-------------------------------------------------*/
 
void EXTI0_IRQHandler(void)
{
  DelayMs(10);    //����
	if(WK_UP==1)
	{	  
		keyHuangTotal -= 2;
		SEND_Veg();
	}
	EXTI_ClearITPendingBit(EXTI_Line0);     //���EXTI0��·����λ
}
 void EXTI9_5_IRQHandler(void)
{			
	DelayMs(10);   //����			 
	if(KEY0==0)	
	{
			keyXiTotal -=2;
			SEND_Veg();
	}
 	 EXTI_ClearITPendingBit(EXTI_Line5);    //���LINE5�ϵ��жϱ�־λ  
}


void EXTI15_10_IRQHandler(void)
{
  DelayMs(10);    //����			 
  if(KEY1==0)	
	{
			keyQieTotal -=2;
			SEND_Veg();
	}
	 EXTI_ClearITPendingBit(EXTI_Line15);  //���LINE15��·����λ
}

/*-------------------------------------------------*/
/*�����������������жϴ�����                     */
/*��  ������                                       */
/*����ֵ����                                       */
/*-------------------------------------------------*/
void NMI_Handler(void)
{

}

/*-------------------------------------------------*/
/*��������Ӳ������������жϴ�����             */
/*��  ������                                       */
/*����ֵ����                                       */
/*-------------------------------------------------*/
void HardFault_Handler(void)
{

}

/*-------------------------------------------------*/
/*���������ڴ�����жϴ�����                     */
/*��  ������                                       */
/*����ֵ����                                       */
/*-------------------------------------------------*/
void MemManage_Handler(void)
{

}

/*-------------------------------------------------*/
/*��������Ԥȡָʧ�ܣ��洢������ʧ���жϴ�����   */
/*��  ������                                       */
/*����ֵ����                                       */
/*-------------------------------------------------*/
void BusFault_Handler(void)
{

}

/*-------------------------------------------------*/
/*��������δ�����ָ���Ƿ�״̬������           */
/*��  ������                                       */
/*����ֵ����                                       */
/*-------------------------------------------------*/
void UsageFault_Handler(void)
{

}

/*-------------------------------------------------*/
/*�����������жϣ�SWI ָ����õĴ�����           */
/*��  ������                                       */
/*����ֵ����                                       */
/*-------------------------------------------------*/
void SVC_Handler(void)
{
	
}

/*-------------------------------------------------*/
/*�����������Լ����������                       */
/*��  ������                                       */
/*����ֵ����                                       */
/*-------------------------------------------------*/
void DebugMon_Handler(void)
{
	
}

/*-------------------------------------------------*/
/*���������ɹ����ϵͳ��������                 */
/*��  ������                                       */
/*����ֵ����                                       */
/*-------------------------------------------------*/
void PendSV_Handler(void)
{
}

/*-------------------------------------------------*/
/*��������SysTicϵͳ��શ�ʱ��������             */
/*��  ������                                       */
/*����ֵ����                                       */
/*-------------------------------------------------*/
void SysTick_Handler(void)
{
 
}
