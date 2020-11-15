
/*-----------------------------------------------------*/
/*USER     �����������main��������������������        */
/*HARDWARE ��������������ֹ����������������            */
/*CORE     ������STM32�ĺ��ĳ��򣬹ٷ��ṩ�����ǲ��޸�   */
/*STLIB    ���ٷ��ṩ�Ŀ��ļ������ǲ��޸�                */
/*-----------------------------------------------------*/
/*                                                     */
/*           ����main��������ں���Դ�ļ�                */
/*                                                     */
/*-----------------------------------------------------*/

#include "stm32f10x.h"   //������Ҫ��ͷ�ļ�
#include "control.h"     //������Ҫ��ͷ�ļ�
#include "delay.h"       //������Ҫ��ͷ�ļ�
#include "usart1.h"      //������Ҫ��ͷ�ļ�
#include "usart2.h"      //������Ҫ��ͷ�ļ�
#include "timer1.h"      //������Ҫ��ͷ�ļ�
#include "timer2.h"      //������Ҫ��ͷ�ļ�
#include "timer3.h"      //������Ҫ��ͷ�ļ�
#include "timer4.h"      //������Ҫ��ͷ�ļ�
#include "wifi.h"	     //������Ҫ��ͷ�ļ�
#include "led.h"	     //������Ҫ��ͷ�ļ� ����ƿ���
#include "fan.h"	     //������Ҫ��ͷ�ļ�	�����ȿ���
#include "water.h"       //������Ҫ��ͷ�ļ� ˮ�ÿ���                                                    
#include "mqtt.h"        //������Ҫ��ͷ�ļ�
#include "key.h"         //������Ҫ��ͷ�ļ� ��ժ��
#include "exti.h"        //������Ҫ��ͷ�ļ� ��ժ���жϺ���
#include "dht11.h"       //������Ҫ��ͷ�ļ� ������ʪ��
#include "28byj48.h"     //������Ҫ��ͷ�ļ� ��������������ţ�
#include "bh1750.h"      //������Ҫ��ͷ�ļ� ���մ�����
#include "rc522.h"       //������Ҫ��ͷ�ļ� �Ž�
#include "adc.h" 		 //������Ҫ��ͷ�ļ� ����ʪ��
#include "ds18b20.h" 	 //������Ҫ��ͷ�ļ� �����¶�
#include "myiic.h"       //������Ҫ��ͷ�ļ� ���� �¶� ��ѹ I2C
#include "bmp280.h"      //������Ҫ��ͷ�ļ� �ռ��¶� ���� ��ѹ

void SEND_Data(void);

char *CMD1 = "LEDON";    //����ƴ�
char *CMD2 = "WON";  	 //ˮ�ô�
char *CMD3 = "ON";    	 //��������
char *CMD4 = "FANON";    //�����ȴ�
char *CMD5 = "MOTON";    //��������

char *CMD11 = "LEDOFF";  //����ƹر�
char *CMD21 = "WOFF";  	 //ˮ�ùر�
char *CMD31 = "OFF";     //�������ر�
char *CMD41 = "FANOFF";  //�����ȿ���
char *CMD51 = "MOTOFF";  //����������


int dataFlag = 0;        //������״̬���������ݣ�
int motorFlag = 0;	     //������״̬
int watarFlag = 0;       //ˮ��״̬
int fanFlag = 0;         //������״̬
int ledFlag = 0;         //�����״̬
int peoFlag = 0;         //��ǰ����

int keyHuangAll = 50;    //�ƹ�����
int keyXiAll = 50;       //����������
int keyQieAll = 50;      //��������
 
int keyHuangTotal = 50;  //�ƹϵ�ǰ��
int keyXiTotal = 50;     //��������ǰ��
int keyQieTotal = 50;    //���ӵ�ǰ��

int main(void) 
{	
	Delay_Init();          //��ʱ���ܳ�ʼ��              
	Usart1_Init(9600);     //����1���ܳ�ʼ����������9600
	Usart2_Init(115200);   //����2���ܳ�ʼ����������115200	
	TIM4_Init(500,7200);   //TIM4��ʼ������ʱʱ�� 500*7200*1000/72000000 = 50ms
	LED_Init();			   		 //����Ƴ�ʼ��
	FAN_Init();			       //���ȳ�ʼ��
	WAT_Init();	           //ˮ�ó�ʼ��
	KEY_Init();            //������ʼ��
	IIC_BY30_Init();       //��ʼ��IIC�ӿ� ����ǿ��
	IIC_BMP280_Init();	   //��ʼ��IIC�ӿ� ���� ��ѹ �¶�
	DHT11_Init();          //��ʼ��DHT11	
	EXTIX_Init();          //�ⲿ�жϳ�ʼ����������ʼ����
	MOTOR_Init();          //��ʼ���������
	RC522_Init();          //��ʼ���Ž���
	BMP280_Init();         //��ʼ��������ѹ�¶�ģ��
	DS18B20_Init();		     //��ʼ���������¶�
	WiFi_ResetIO_Init();   //��ʼ��WiFi�ĸ�λIO
	IoT_Parameter_Init();  //��ʼ����IoTƽ̨MQTT�������Ĳ���	

	while(1)
	{		
		// connectFlag=1ͬ���������������� 
		if(connectFlag == 1) 
		{     
			/*-------------------------------------------------------------*/
			/*                     �Ž������ݴ���                           */
			/*-------------------------------------------------------------*/	
			CardDataDeal();
			/*-------------------------------------------------------------*/
			/*                     �����ͻ���������						 */
			/*-------------------------------------------------------------*/
			if(MQTT_TxDataOutPtr != MQTT_TxDataInPtr) //if�����Ļ���˵�����ͻ�������������
			{                
				//3������ɽ���if
				//��1�֣�0x10 ���ӱ���
				//��2�֣�0x82 ���ı��ģ���connectPackFlag��λ����ʾ���ӱ��ĳɹ�
				//��3�֣�subcribePackFlag��λ��˵�����ӺͶ��ľ��ɹ����������Ŀɷ�
				if((MQTT_TxDataOutPtr[2] == 0x10)||((MQTT_TxDataOutPtr[2] == 0x82)&&(connectPackFlag == 1))||(subcribePackFlag == 1)) 
				{    
					u1_printf("��������:0x%x\r\n",MQTT_TxDataOutPtr[2]);    //������ʾ��Ϣ
					MQTT_TxData(MQTT_TxDataOutPtr);                        //��������
					MQTT_TxDataOutPtr += TBUFF_UNIT;                       //ָ������
					if(MQTT_TxDataOutPtr == MQTT_TxDataEndPtr)             //���ָ�뵽������β����
					{ 
						MQTT_TxDataOutPtr = MQTT_TxDataBuf[0];             //ָ���λ����������ͷ
					}	
				} 				
			}
			/*-------------------------------------------------------------*/
			/*                     ������ջ���������                      */
			/*-------------------------------------------------------------*/
			if(MQTT_RxDataOutPtr != MQTT_RxDataInPtr) //if�����Ļ���˵�����ջ�������������	
			{												
				u1_printf("���յ�����:");
				/*-----------------------------------------------------*/
				/*                    ����CONNACK����                  */
				/*-----------------------------------------------------*/				
				//if�жϣ������һ���ֽ���0x20����ʾ�յ�����CONNACK����
				//��������Ҫ�жϵ�4���ֽڣ�����CONNECT�����Ƿ�ɹ�
				if(MQTT_RxDataOutPtr[2] == 0x20)
				{             			
					switch(MQTT_RxDataOutPtr[5])
					{					   
						case 0x00 : u1_printf("CONNECT���ĳɹ�\r\n");                            //���������Ϣ	
										connectPackFlag = 1;                                    //CONNECT���ĳɹ������ı��Ŀɷ�
									break;                                                      //������֧case 0x00                                              
						case 0x01 : u1_printf("�����Ѿܾ�����֧�ֵ�Э��汾��׼������\r\n");        //���������Ϣ
									connectFlag = 0;                                            //connectFlag���㣬��������
									break;                                                      //������֧case 0x01   
						case 0x02 : u1_printf("�����Ѿܾ������ϸ�Ŀͻ��˱�ʶ����׼������\r\n");    //���������Ϣ
									connectFlag = 0;                                            //connectFlag���㣬��������
									break;                                                      //������֧case 0x02 
						case 0x03 : u1_printf("�����Ѿܾ�������˲����ã�׼������\r\n");           //���������Ϣ
									connectFlag = 0;                                           	//connectFlag���㣬��������
									break;                                                      //������֧case 0x03
						case 0x04 : u1_printf("�����Ѿܾ�����Ч���û��������룬׼������\r\n");     //���������Ϣ
									connectFlag = 0;                                            //connectFlag���㣬��������						
									break;                                                      //������֧case 0x04
						case 0x05 : u1_printf("�����Ѿܾ���δ��Ȩ��׼������\r\n");                //���������Ϣ
									connectFlag = 0;                                            //connectFlag���㣬��������						
									break;                                                      //������֧case 0x05 		
						default   : u1_printf("�����Ѿܾ���δ֪״̬��׼������\r\n");               //���������Ϣ 
									connectFlag = 0;                                             //connectFlag���㣬��������					
									break;                                                       //������֧case default 								
					}				
			  }			
				//if�жϣ���һ���ֽ���0x90����ʾ�յ�����SUBACK����
				//��������Ҫ�ж϶��Ļظ��������ǲ��ǳɹ�
				else if(MQTT_RxDataOutPtr[2] == 0x90)
				{ 
					switch(MQTT_RxDataOutPtr[6])
					{					
						case 0x00 :
						case 0x01 : 
									u1_printf("���ĳɹ�\r\n");            //���������Ϣ
									subcribePackFlag = 1;                //subcribePackFlag��1����ʾ���ı��ĳɹ����������Ŀɷ���
									pingFlag = 0;                        //pingFlag����
									TIM3_ENABLE_30S();                   //����30s��PING��ʱ��	
									SEND_Data();
									SEND_Veg();					
									break;                               //������֧                                             
						default: 
									u1_printf("����ʧ�ܣ�׼������\r\n");   //���������Ϣ 
									connectFlag = 0;                     //connectFlag���㣬��������
									break;                               //������֧ 								
						}					
			 }
				//if�жϣ���һ���ֽ���0xD0����ʾ�յ�����PINGRESP����
				else if(MQTT_RxDataOutPtr[2] == 0xD0)
				{ 
					u1_printf("PING���Ļظ�\r\n");                       //���������Ϣ 
					if(pingFlag == 1)
					{                   						        //���pingFlag=1����ʾ��һ�η���
						 pingFlag = 0;    				       			//Ҫ���pingFlag��־
					}
					else if(pingFlag > 1)	
					{ 				 									//���pingFlag>1����ʾ�Ƕ�η����ˣ�������2s����Ŀ��ٷ���
						pingFlag = 0;     				      			//Ҫ���pingFlag��־
						TIM3_ENABLE_30S(); 				      			//PING��ʱ���ػ�30s��ʱ��
					}				
			 }	
				//if�жϣ������һ���ֽ���0x30����ʾ�յ����Ƿ�������������������
				//����Ҫ��ȡ��������
				else if((MQTT_RxDataOutPtr[2] == 0x30))
				{ 
					u1_printf("�������ȼ�0����\r\n"); 		   	 //���������Ϣ 
					MQTT_DealPushdata_Qs0(MQTT_RxDataOutPtr);   //����ȼ�0��������
				}									
				MQTT_RxDataOutPtr +=RBUFF_UNIT;                 //ָ������
				if(MQTT_RxDataOutPtr == MQTT_RxDataEndPtr)      //���ָ�뵽������β����
				{
					MQTT_RxDataOutPtr = MQTT_RxDataBuf[0];      //ָ���λ����������ͷ              
				}		          
			}//������ջ��������ݵ�else if��֧��β
						
			/*---------------------------------------------------------------------------------------------------------------------*/
			/*                    							 ���������������                   		          			         */
			/*---------------------------------------------------------------------------------------------------------------------*/
			if(MQTT_CMDOutPtr != MQTT_CMDInPtr)							 //if�����Ļ���˵�����������������	
			{                             		       
				u1_printf("����:%s\r\n",&MQTT_CMDOutPtr[2]);              //���������Ϣ
				
				if(!memcmp(&MQTT_CMDOutPtr[2],CMD1,strlen(CMD1)))         //�ж�ָ������CMD1 
				{                                            
					ledFlag = 1;  								             
					LED_On();									  		 //����ƿ���
				}
				else if(!memcmp(&MQTT_CMDOutPtr[2],CMD11,strlen(CMD11))) //�ж�ָ������CMD11
				{ 
					ledFlag = 0;                                              
					LED_Off(); 								             //����ƹر�
				}
					
				else if(!memcmp(&MQTT_CMDOutPtr[2],CMD2,strlen(CMD2)))	 //�ж�ָ������CMD2 
				{ 
					if(0 == watarFlag)
					{
						watarFlag = 1;  									
						WAT_On();    								      //ˮ�ÿ���   
					}     	
				}
				else if(!memcmp(&MQTT_CMDOutPtr[2],CMD21,strlen(CMD21)))  //�ж�ָ������CMD21
				{ 
					if(1 == watarFlag)
					{
						watarFlag = 0;                                               
						WAT_Off();								 		 //ˮ�ùر�
					}                              	
				}
					
				else if(!memcmp(&MQTT_CMDOutPtr[2],CMD3,strlen(CMD3)))	 //�ж�ָ������CMD3
				{      
					dataFlag = 1;                //dataFlag��1����ʾ���ڲɼ�״̬��
					TIM2_ENABLE_10S();           //��ʱ��2��,10s����ɼ���ʪ��   
				}
				else if(!memcmp(&MQTT_CMDOutPtr[2],CMD31,strlen(CMD31))) //�ж�ָ������CMD3
				{     
					dataFlag = 0;                //dataFlag��0����ʾ����ֹͣ״̬��
					TIM_Cmd(TIM2,DISABLE);		 //�ж�2·����״̬�Ͳɼ�״̬����������������
				}
					
				else if(!memcmp(&MQTT_CMDOutPtr[2],CMD4,strlen(CMD4)))  //�ж�ָ������CMD4
				{ 
					if(0 == fanFlag)
					{							
						fanFlag = 1;                                      	 
						FAN_On();		 							 	 //�����ȿ���  
					}								
				}
				else if(!memcmp(&MQTT_CMDOutPtr[2],CMD41,strlen(CMD41))) //�ж�ָ������CMD41
				{   
					if(1 == fanFlag)
					{								
						fanFlag = 0;                                        
						FAN_Off();     						 			 //�����ȹر�
					}
				}
				
				else if(!memcmp(&MQTT_CMDOutPtr[2],CMD5,strlen(CMD5)))   //�ж�ָ������CMD5 k
				{    
					if(0 == motorFlag)
					{
						MotorForwardAngle(1250,1800);	                 //����������
						MotorStop();
						motorFlag = 1;
					}
				}
				else if(!memcmp(&MQTT_CMDOutPtr[2],CMD51,strlen(CMD51)))  //�ж�ָ������CMD51
				{ 
					if(1 == motorFlag)
					{
						MotorRevAngle(1250,1800); 						 //�������ر�
						MotorStop();
						motorFlag = 0;
					}			
				}	//������������ᷢ��״̬
				else u1_printf("δָ֪��\r\n");              //���������Ϣ	
				
				MQTT_CMDOutPtr += CBUFF_UNIT;               //ָ������
				if(MQTT_CMDOutPtr==MQTT_CMDEndPtr)          //���ָ�뵽������β����
				MQTT_CMDOutPtr = MQTT_CMDBuf[0];          	//ָ���λ����������ͷ	
				SEND_Data();//���Ϳ�������
			} //��������������ݵ�else if��֧��β					
		} //connectFlag=1��if��֧�Ľ�β
	
	/*--------------------------------------------------------------------*/
	/*      connectFlag=0ͬ�������Ͽ�������,����Ҫ�������ӷ�����            */
	/*--------------------------------------------------------------------*/
	 else
		{ 
			u1_printf("��Ҫ���ӷ�����\r\n");               //���������Ϣ
			TIM_Cmd(TIM4,DISABLE);                        //�ر�TIM4 
			TIM_Cmd(TIM3,DISABLE);                        //�ر�TIM3  
			WiFi_RxCounter=0;                             //WiFi������������������                        
			memset(WiFi_RX_BUF,0,WiFi_RXBUFF_SIZE);       //���WiFi���ջ����� 
			if(WiFi_Connect_IoTServer()==0)				  //���WiFi�����Ʒ�������������0����ʾ��ȷ������if
			{   			    
				u1_printf("����TCP���ӳɹ�\r\n");          //���������Ϣ
				connectFlag = 1;                          //connectFlag��1����ʾ���ӳɹ�	
				WiFi_RxCounter = 0;                       //WiFi������������������                        
				memset(WiFi_RX_BUF,0,WiFi_RXBUFF_SIZE);   //���WiFi���ջ����� 
				MQTT_Buff_Init();                         //��ʼ�����ͻ�����                    
			}				
		}
	}
}
