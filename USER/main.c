
/*-----------------------------------------------------*/
/*USER     ：包含程序的main函数，是整个程序的入口        */
/*HARDWARE ：包含开发板各种功能外设的驱动程序            */
/*CORE     ：包含STM32的核心程序，官方提供，我们不修改   */
/*STLIB    ：官方提供的库文件，我们不修改                */
/*-----------------------------------------------------*/
/*                                                     */
/*           程序main函数，入口函数源文件                */
/*                                                     */
/*-----------------------------------------------------*/

#include "stm32f10x.h"   //包含需要的头文件
#include "control.h"     //包含需要的头文件
#include "delay.h"       //包含需要的头文件
#include "usart1.h"      //包含需要的头文件
#include "usart2.h"      //包含需要的头文件
#include "timer1.h"      //包含需要的头文件
#include "timer2.h"      //包含需要的头文件
#include "timer3.h"      //包含需要的头文件
#include "timer4.h"      //包含需要的头文件
#include "wifi.h"	     //包含需要的头文件
#include "led.h"	     //包含需要的头文件 补光灯开关
#include "fan.h"	     //包含需要的头文件	换气扇开关
#include "water.h"       //包含需要的头文件 水泵开关                                                    
#include "mqtt.h"        //包含需要的头文件
#include "key.h"         //包含需要的头文件 采摘数
#include "exti.h"        //包含需要的头文件 采摘数中断函数
#include "dht11.h"       //包含需要的头文件 空气温湿度
#include "28byj48.h"     //包含需要的头文件 步进电机（卷帘门）
#include "bh1750.h"      //包含需要的头文件 光照传感器
#include "rc522.h"       //包含需要的头文件 门禁
#include "adc.h" 		 //包含需要的头文件 土壤湿度
#include "ds18b20.h" 	 //包含需要的头文件 土壤温度
#include "myiic.h"       //包含需要的头文件 海拔 温度 气压 I2C
#include "bmp280.h"      //包含需要的头文件 空间温度 海拔 气压

void SEND_Data(void);

char *CMD1 = "LEDON";    //补光灯打开
char *CMD2 = "WON";  	 //水泵打开
char *CMD3 = "ON";    	 //传感器打开
char *CMD4 = "FANON";    //换气扇打开
char *CMD5 = "MOTON";    //卷帘机打开

char *CMD11 = "LEDOFF";  //补光灯关闭
char *CMD21 = "WOFF";  	 //水泵关闭
char *CMD31 = "OFF";     //传感器关闭
char *CMD41 = "FANOFF";  //换气扇开关
char *CMD51 = "MOTOFF";  //卷帘机开关


int dataFlag = 0;        //传感器状态（发送数据）
int motorFlag = 0;	     //卷帘机状态
int watarFlag = 0;       //水泵状态
int fanFlag = 0;         //换气扇状态
int ledFlag = 0;         //补光灯状态
int peoFlag = 0;         //当前人数

int keyHuangAll = 50;    //黄瓜总量
int keyXiAll = 50;       //西红柿总量
int keyQieAll = 50;      //茄子总量
 
int keyHuangTotal = 50;  //黄瓜当前量
int keyXiTotal = 50;     //西红柿当前量
int keyQieTotal = 50;    //茄子当前量

int main(void) 
{	
	Delay_Init();          //延时功能初始化              
	Usart1_Init(9600);     //串口1功能初始化，波特率9600
	Usart2_Init(115200);   //串口2功能初始化，波特率115200	
	TIM4_Init(500,7200);   //TIM4初始化，定时时间 500*7200*1000/72000000 = 50ms
	LED_Init();			   //补光灯初始化
	FAN_Init();			   //风扇初始化
	WAT_Init();	           //水泵初始化
	KEY_Init();            //按键初始化
	IIC_BY30_Init();       //初始化IIC接口 光照强度
	IIC_BMP280_Init();	   //初始化IIC接口 海拔 气压 温度
	DHT11_Init();          //初始化DHT11	
	EXTIX_Init();          //外部中断初始化（按键初始化）
	MOTOR_Init();          //初始化步进电机
	RC522_Init();          //初始化门禁卡
	BMP280_Init();         //初始化海拔气压温度模块
	DS18B20_Init();		     //初始化门土壤温度
	WiFi_ResetIO_Init();   //初始化WiFi的复位IO
	IoT_Parameter_Init();  //初始化云IoT平台MQTT服务器的参数	

	while(1)
	{		
		// connectFlag=1同服务器建立了连接 
		if(connectFlag == 1) 
		{     
			/*-------------------------------------------------------------*/
			/*                     门禁卡数据处理                           */
			/*-------------------------------------------------------------*/	
			CardDataDeal();
			/*-------------------------------------------------------------*/
			/*                     处理发送缓冲区数据						 */
			/*-------------------------------------------------------------*/
			if(MQTT_TxDataOutPtr != MQTT_TxDataInPtr) //if成立的话，说明发送缓冲区有数据了
			{                
				//3种情况可进入if
				//第1种：0x10 连接报文
				//第2种：0x82 订阅报文，且connectPackFlag置位，表示连接报文成功
				//第3种：subcribePackFlag置位，说明连接和订阅均成功，其他报文可发
				if((MQTT_TxDataOutPtr[2] == 0x10)||((MQTT_TxDataOutPtr[2] == 0x82)&&(connectPackFlag == 1))||(subcribePackFlag == 1)) 
				{    
					u1_printf("发送数据:0x%x\r\n",MQTT_TxDataOutPtr[2]);    //串口提示信息
					MQTT_TxData(MQTT_TxDataOutPtr);                        //发送数据
					MQTT_TxDataOutPtr += TBUFF_UNIT;                       //指针下移
					if(MQTT_TxDataOutPtr == MQTT_TxDataEndPtr)             //如果指针到缓冲区尾部了
					{ 
						MQTT_TxDataOutPtr = MQTT_TxDataBuf[0];             //指针归位到缓冲区开头
					}	
				} 				
			}
			/*-------------------------------------------------------------*/
			/*                     处理接收缓冲区数据                      */
			/*-------------------------------------------------------------*/
			if(MQTT_RxDataOutPtr != MQTT_RxDataInPtr) //if成立的话，说明接收缓冲区有数据了	
			{												
				u1_printf("接收到数据:");
				/*-----------------------------------------------------*/
				/*                    处理CONNACK报文                  */
				/*-----------------------------------------------------*/				
				//if判断，如果第一个字节是0x20，表示收到的是CONNACK报文
				//接着我们要判断第4个字节，看看CONNECT报文是否成功
				if(MQTT_RxDataOutPtr[2] == 0x20)
				{             			
					switch(MQTT_RxDataOutPtr[5])
					{					   
						case 0x00 : u1_printf("CONNECT报文成功\r\n");                            //串口输出信息	
										connectPackFlag = 1;                                    //CONNECT报文成功，订阅报文可发
									break;                                                      //跳出分支case 0x00                                              
						case 0x01 : u1_printf("连接已拒绝，不支持的协议版本，准备重启\r\n");        //串口输出信息
									connectFlag = 0;                                            //connectFlag置零，重启连接
									break;                                                      //跳出分支case 0x01   
						case 0x02 : u1_printf("连接已拒绝，不合格的客户端标识符，准备重启\r\n");    //串口输出信息
									connectFlag = 0;                                            //connectFlag置零，重启连接
									break;                                                      //跳出分支case 0x02 
						case 0x03 : u1_printf("连接已拒绝，服务端不可用，准备重启\r\n");           //串口输出信息
									connectFlag = 0;                                           	//connectFlag置零，重启连接
									break;                                                      //跳出分支case 0x03
						case 0x04 : u1_printf("连接已拒绝，无效的用户名或密码，准备重启\r\n");     //串口输出信息
									connectFlag = 0;                                            //connectFlag置零，重启连接						
									break;                                                      //跳出分支case 0x04
						case 0x05 : u1_printf("连接已拒绝，未授权，准备重启\r\n");                //串口输出信息
									connectFlag = 0;                                            //connectFlag置零，重启连接						
									break;                                                      //跳出分支case 0x05 		
						default   : u1_printf("连接已拒绝，未知状态，准备重启\r\n");               //串口输出信息 
									connectFlag = 0;                                             //connectFlag置零，重启连接					
									break;                                                       //跳出分支case default 								
					}				
			  }			
				//if判断，第一个字节是0x90，表示收到的是SUBACK报文
				//接着我们要判断订阅回复，看看是不是成功
				else if(MQTT_RxDataOutPtr[2] == 0x90)
				{ 
					switch(MQTT_RxDataOutPtr[6])
					{					
						case 0x00 :
						case 0x01 : 
									u1_printf("订阅成功\r\n");            //串口输出信息
									subcribePackFlag = 1;                //subcribePackFlag置1，表示订阅报文成功，其他报文可发送
									pingFlag = 0;                        //pingFlag清零
									TIM3_ENABLE_30S();                   //启动30s的PING定时器	
									SEND_Data();
									SEND_Veg();					
									break;                               //跳出分支                                             
						default: 
									u1_printf("订阅失败，准备重启\r\n");   //串口输出信息 
									connectFlag = 0;                     //connectFlag置零，重启连接
									break;                               //跳出分支 								
						}					
			 }
				//if判断，第一个字节是0xD0，表示收到的是PINGRESP报文
				else if(MQTT_RxDataOutPtr[2] == 0xD0)
				{ 
					u1_printf("PING报文回复\r\n");                       //串口输出信息 
					if(pingFlag == 1)
					{                   						        //如果pingFlag=1，表示第一次发送
						 pingFlag = 0;    				       			//要清除pingFlag标志
					}
					else if(pingFlag > 1)	
					{ 				 									//如果pingFlag>1，表示是多次发送了，而且是2s间隔的快速发送
						pingFlag = 0;     				      			//要清除pingFlag标志
						TIM3_ENABLE_30S(); 				      			//PING定时器重回30s的时间
					}				
			 }	
				//if判断，如果第一个字节是0x30，表示收到的是服务器发来的推送数据
				//我们要提取控制命令
				else if((MQTT_RxDataOutPtr[2] == 0x30))
				{ 
					u1_printf("服务器等级0推送\r\n"); 		   	 //串口输出信息 
					MQTT_DealPushdata_Qs0(MQTT_RxDataOutPtr);   //处理等级0推送数据
				}									
				MQTT_RxDataOutPtr +=RBUFF_UNIT;                 //指针下移
				if(MQTT_RxDataOutPtr == MQTT_RxDataEndPtr)      //如果指针到缓冲区尾部了
				{
					MQTT_RxDataOutPtr = MQTT_RxDataBuf[0];      //指针归位到缓冲区开头              
				}		          
			}//处理接收缓冲区数据的else if分支结尾
						
			/*---------------------------------------------------------------------------------------------------------------------*/
			/*                    							 处理命令缓冲区数据                   		          			         */
			/*---------------------------------------------------------------------------------------------------------------------*/
			if(MQTT_CMDOutPtr != MQTT_CMDInPtr)							 //if成立的话，说明命令缓冲区有数据了	
			{                             		       
				u1_printf("命令:%s\r\n",&MQTT_CMDOutPtr[2]);              //串口输出信息
				
				if(!memcmp(&MQTT_CMDOutPtr[2],CMD1,strlen(CMD1)))         //判断指令，如果是CMD1 
				{                                            
					ledFlag = 1;  								             
					LED_On();									  		 //补光灯开启
				}
				else if(!memcmp(&MQTT_CMDOutPtr[2],CMD11,strlen(CMD11))) //判断指令，如果是CMD11
				{ 
					ledFlag = 0;                                              
					LED_Off(); 								             //补光灯关闭
				}
					
				else if(!memcmp(&MQTT_CMDOutPtr[2],CMD2,strlen(CMD2)))	 //判断指令，如果是CMD2 
				{ 
					if(0 == watarFlag)
					{
						watarFlag = 1;  									
						WAT_On();    								      //水泵开启   
					}     	
				}
				else if(!memcmp(&MQTT_CMDOutPtr[2],CMD21,strlen(CMD21)))  //判断指令，如果是CMD21
				{ 
					if(1 == watarFlag)
					{
						watarFlag = 0;                                               
						WAT_Off();								 		 //水泵关闭
					}                              	
				}
					
				else if(!memcmp(&MQTT_CMDOutPtr[2],CMD3,strlen(CMD3)))	 //判断指令，如果是CMD3
				{      
					dataFlag = 1;                //dataFlag置1，表示处于采集状态了
					TIM2_ENABLE_10S();           //定时器2，,10s间隔采集温湿度   
				}
				else if(!memcmp(&MQTT_CMDOutPtr[2],CMD31,strlen(CMD31))) //判断指令，如果是CMD3
				{     
					dataFlag = 0;                //dataFlag置0，表示处于停止状态了
					TIM_Cmd(TIM2,DISABLE);		 //判断2路开关状态和采集状态，并发布给服务器
				}
					
				else if(!memcmp(&MQTT_CMDOutPtr[2],CMD4,strlen(CMD4)))  //判断指令，如果是CMD4
				{ 
					if(0 == fanFlag)
					{							
						fanFlag = 1;                                      	 
						FAN_On();		 							 	 //换气扇开启  
					}								
				}
				else if(!memcmp(&MQTT_CMDOutPtr[2],CMD41,strlen(CMD41))) //判断指令，如果是CMD41
				{   
					if(1 == fanFlag)
					{								
						fanFlag = 0;                                        
						FAN_Off();     						 			 //换气扇关闭
					}
				}
				
				else if(!memcmp(&MQTT_CMDOutPtr[2],CMD5,strlen(CMD5)))   //判断指令，如果是CMD5 k
				{    
					if(0 == motorFlag)
					{
						MotorForwardAngle(1250,1800);	                 //卷帘机开启
						MotorStop();
						motorFlag = 1;
					}
				}
				else if(!memcmp(&MQTT_CMDOutPtr[2],CMD51,strlen(CMD51)))  //判断指令，如果是CMD51
				{ 
					if(1 == motorFlag)
					{
						MotorRevAngle(1250,1800); 						 //卷帘机关闭
						MotorStop();
						motorFlag = 0;
					}			
				}	//不做处理，后面会发送状态
				else u1_printf("未知指令\r\n");              //串口输出信息	
				
				MQTT_CMDOutPtr += CBUFF_UNIT;               //指针下移
				if(MQTT_CMDOutPtr==MQTT_CMDEndPtr)          //如果指针到缓冲区尾部了
				MQTT_CMDOutPtr = MQTT_CMDBuf[0];          	//指针归位到缓冲区开头	
				SEND_Data();//发送控制数据
			} //处理命令缓冲区数据的else if分支结尾					
		} //connectFlag=1的if分支的结尾
	
	/*--------------------------------------------------------------------*/
	/*      connectFlag=0同服务器断开了连接,我们要重启连接服务器            */
	/*--------------------------------------------------------------------*/
	 else
		{ 
			u1_printf("需要连接服务器\r\n");               //串口输出信息
			TIM_Cmd(TIM4,DISABLE);                        //关闭TIM4 
			TIM_Cmd(TIM3,DISABLE);                        //关闭TIM3  
			WiFi_RxCounter=0;                             //WiFi接收数据量变量清零                        
			memset(WiFi_RX_BUF,0,WiFi_RXBUFF_SIZE);       //清空WiFi接收缓冲区 
			if(WiFi_Connect_IoTServer()==0)				  //如果WiFi连接云服务器函数返回0，表示正确，进入if
			{   			    
				u1_printf("建立TCP连接成功\r\n");          //串口输出信息
				connectFlag = 1;                          //connectFlag置1，表示连接成功	
				WiFi_RxCounter = 0;                       //WiFi接收数据量变量清零                        
				memset(WiFi_RX_BUF,0,WiFi_RXBUFF_SIZE);   //清空WiFi接收缓冲区 
				MQTT_Buff_Init();                         //初始化发送缓冲区                    
			}				
		}
	}
}
