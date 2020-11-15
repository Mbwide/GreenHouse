/*-------------------------------------------------*/
/*                                                 */
/*            	   中断服务函数          	     	   */
/*                                                 */
/*-------------------------------------------------*/

#include "stm32f10x.h"     //包含需要的头文件
#include "stm32f10x_it.h"  //包含需要的头文件
#include "delay.h"         //包含需要的头文件
#include "control.h"       //包含需要的头文件
#include "usart1.h"        //包含需要的头文件
#include "usart2.h"        //包含需要的头文件
#include "timer1.h"        //包含需要的头文件
#include "timer4.h"        //包含需要的头文件
#include "key.h"           //包含需要的头文件 采摘数
#include "exti.h"          //包含需要的头文件 采摘数
#include "wifi.h"	         //包含需要的头文件
#include "timer3.h"        //包含需要的头文件
#include "mqtt.h"          //包含需要的头文件
#include "dht11.h"         //包含需要的头文件
#include "bh1750.h"        //包含需要的头文件
#include "adc.h"           //包含需要的头文件
#include "myiic.h"         //包含需要的头文件 海拔 温度 气压 I2C
#include "bmp280.h" 		   //包含需要的头文件 空间温度 海拔 气压
#include "ds18b20.h" 			 //包含需要的头文件 土壤温度


extern int dataFlag;       //传感器状态（发送数据）
extern int motorFlag;			 //卷帘机状态
extern int watarFlag;      //水泵状态
extern int ledFlag;        //补光灯状态                   
extern int peoFlag;     	 //当前人数

extern int keyHuangTotal;  //黄瓜当前量
extern int keyXiTotal;     //西红柿当前量
extern int keyQieTotal;    //茄子当前量


/*-------------------------------------------------*/
/*函数名：串口2接收中断函数                        */
/*参  数：无                                       */
/*返回值：无                                       */
/*-------------------------------------------------*/
void USART2_IRQHandler(void)   
{                      
	if(USART_GetITStatus(USART2, USART_IT_RXNE) != RESET)//如果USART_IT_RXNE标志置位，表示有数据到了，进入if分支
	{  
		if(connectFlag==0) 											           //如果connectFlag等于0，当前还没有连接服务器，处于指令配置状态
		{                                     
			if(USART2->DR)
			{                                     					 //处于指令配置状态时，非零值才保存到缓冲区	
				Usart2_RxBuff[Usart2_RxCounter]=USART2->DR;    //保存到缓冲区	
				Usart2_RxCounter ++;                           //每接收1个字节的数据，Usart2_RxCounter加1，表示接收的数据总量+1 
			}		
		}
		else
		{		                                           	   //反之connectFlag等于1，连接上服务器了	
			Usart2_RxBuff[Usart2_RxCounter] = USART2->DR;    //把接收到的数据保存到Usart2_RxBuff中				
			if(Usart2_RxCounter == 0)
			{    									    											 //如果Usart2_RxCounter等于0，表示是接收的第1个数据，进入if分支				
				TIM_Cmd(TIM4,ENABLE); 
			}
			else																						 //else分支，表示果Usart2_RxCounter不等于0，不是接收的第一个数据
			{                        									    
				TIM_SetCounter(TIM4,0);  
			}	
			Usart2_RxCounter++;         				    				 //每接收1个字节的数据，Usart2_RxCounter加1，表示接收的数据总量+1 
		}
	}
} 
/*-------------------------------------------------*/
/*函数名：定时器4中断服务函数                      */
/*参  数：无                                       */
/*返回值：无                                       */
/*-------------------------------------------------*/
void TIM4_IRQHandler(void)
{
	if(TIM_GetITStatus(TIM4, TIM_IT_Update) != RESET)//如果TIM_IT_Update置位，表示TIM4溢出中断，进入if	
	{                	
		memcpy(&MQTT_RxDataInPtr[2],Usart2_RxBuff,Usart2_RxCounter); 		//拷贝数据到接收缓冲区
		MQTT_RxDataInPtr[0] = Usart2_RxCounter/256;                     //记录数据长度高字节
		MQTT_RxDataInPtr[1] = Usart2_RxCounter%256;                     //记录数据长度低字节
		MQTT_RxDataInPtr+=RBUFF_UNIT;                                   //指针下移
		if(MQTT_RxDataInPtr==MQTT_RxDataEndPtr)                         //如果指针到缓冲区尾部了
			MQTT_RxDataInPtr = MQTT_RxDataBuf[0];                         //指针归位到缓冲区开头
		Usart2_RxCounter = 0;                                           //串口2接收数据量变量清零
		TIM_SetCounter(TIM3, 0);                                        //清零定时器6计数器，重新计时ping包发送时间
		TIM_Cmd(TIM4, DISABLE);                        				  				//关闭TIM4定时器
		TIM_SetCounter(TIM4, 0);                        			          //清零定时器4计数器
		TIM_ClearITPendingBit(TIM4, TIM_IT_Update);     			          //清除TIM4溢出中断标志 	
	}
}
/*-------------------------------------------------*/
/*函数名：定时器3中断服务函数                      */
/*参  数：无                                       */
/*返回值：无                                       */
/*-------------------------------------------------*/
void TIM3_IRQHandler(void)
{
	if(TIM_GetITStatus(TIM3, TIM_IT_Update) != RESET)//如果TIM_IT_Update置位，表示TIM3溢出中断，进入if	
		{  
		switch(pingFlag) 					   //判断pingFlag的状态
		{                               
			case 0:										 //如果pingFlag等于0，表示正常状态，发送Ping报文  
					MQTT_PingREQ(); 		   //添加Ping报文到发送缓冲区  
					break;
			case 1:										 //如果pingFlag等于1，说明上一次发送到的ping报文，没有收到服务器回复，所以1没有被清除为0，可能是连接异常，我们要启动快速ping模式
					TIM3_ENABLE_2S(); 	   //我们将定时器6设置为2s定时,快速发送Ping报文
					MQTT_PingREQ();  			 //添加Ping报文到发送缓冲区  
					break;
			case 2:										 //如果pingFlag等于2，说明还没有收到服务器回复
			case 3:				             //如果pingFlag等于3，说明还没有收到服务器回复
			case 4:				             //如果pingFlag等于4，说明还没有收到服务器回复	
					MQTT_PingREQ();  			 //添加Ping报文到发送缓冲区 
					break;
			case 5:										 //如果pingFlag等于5，说明我们发送了多次ping，均无回复，应该是连接有问题，我们重启连接
					connectFlag = 0;       //连接状态置0，表示断开，没连上服务器
					TIM_Cmd(TIM3,DISABLE); //关TIM3 				
					break;			
		}
		pingFlag++;           		   //pingFlag自增1，表示又发送了一次ping，期待服务器的回复
		TIM_ClearITPendingBit(TIM3, TIM_IT_Update);      //清除TIM3溢出中断标志 	
	}
}



/*-------------------------------------------------*/
/*函数名：定时器2中断服务函数                      */
/*参  数：无                                       */
/*返回值：无                                       */
/*-------------------------------------------------*/
void TIM2_IRQHandler(void)
{
	char  humidity;           //定义一个变量，保存湿度值
	char  temperature;        //定义一个变量，保存温度值	
	float bmp280_temp;			  //定义一个变量，保存外部空气温度值
  float bmp280_press;			  //定义一个变量，保存气压值
	float bmp280_high;			  //定义一个变量，保存海拔
	int   p, t, h;						//定义三个变量，将float转换为int
	int   soil_hum;           //定义一个变量，保存土壤湿度
	int   sun_light;					//定义一个变量，保存光照
			
	char  head1[3];
	char  temp[200];          //定义一个临时缓冲区1
	char  tempAll[220];       //定义一个临时缓冲区2

	
	int   Init_len = 100;     //存放初始长度
		
  char  soilHum[20];        //存放土壤湿度
	int   soilHum_len;        //土壤湿度长度

 	char  sunLight[20];       //存放光照强度		
	int   sunLight_len;		    //光照强度长度
	 
  char  press[20];          //存放大气压
  int   press_len;          //大气压长度
		
	char  airTemp[20];        //存放密大气温度
	int   airTemp_len;			  //大气温度长度
		
	char  high[20];           //存放海拔
	int   high_len;				    //海拔长度	
	

	if(TIM_GetITStatus(TIM2, TIM_IT_Update) != RESET) //如果TIM_IT_Update置位，表示TIM2溢出中断，进入if	
	{ 
		DHT11_Read_Data(&temperature,&humidity);	              //读取温湿度值
		soil_hum = Soil_Temp();											            //读取土壤湿度值
	  sun_light = Get_Sunlight_Value();	
		
		bmp280GetData(&bmp280_press,&bmp280_temp,&bmp280_high); //读取气压、海拔、空气温度
		p = (int)bmp280_press;
		t = (int)DS18B20_Get_Temp()/10;
		h = (int)bmp280_high;
	  
		
		printf("bmp280_press:%d\r\n",p);
		printf("bmp280_temp :%d\r\n",t);
		printf("bmp280_high :%d\r\n",h);	
		u1_printf("%d\r\n",soil_hum);
		
		
	
		memset(temp,    0, 200);                            //清空缓冲区1
		memset(tempAll, 0, 220);                            //清空缓冲区2
		memset(head1,   0, 3);                              //清空MQTT头
		memset(soilHum, 0, 20);                             //清空土壤温度缓冲区 
		memset(sunLight,0, 20);                             //清空光照强度缓冲区
		memset(press,   0, 20);                             //清空压力缓冲区
		memset(airTemp, 0, 20);                             //清空空气温度缓冲区
		memset(high,    0, 20);                             //清空海拔缓冲区

									
		sprintf(soilHum, "%d", soil_hum);     						  // 土壤温度长度
		soilHum_len = length(soil_hum);	
		u1_printf("soil_hum长度：%d\r\n",soilHum_len);
		sprintf(sunLight, "%d", sun_light);   					  	// 光照强度长度
		sunLight_len = length(sun_light);
		u1_printf("sun_light长度：%d\r\n",sunLight_len);
		sprintf(press, "%d", p);    					              // 大气压长度
		press_len = length(p);	
		u1_printf("p长度：%d\r\n",press_len);
    sprintf(airTemp, "%d", t); 						              // 大气温度长度
		airTemp_len = length(t);	
		u1_printf("t长度：%d\r\n",airTemp_len);
		sprintf(high, "%d", h); 					    						  // 海拔长度
		high_len = length(h);	
		u1_printf("h长度：%d\r\n",high_len);

	  sprintf(temp,"{\"temperature\":\"%d%d\",\"humidity\":\"%d%d\",",
					  temperature/10, temperature%10, humidity/10, humidity%10);//构建报文


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
		
		head1[0] = 0x03; 					//固定报头
		head1[1] = 0x00; 					//固定报头
		head1[2] = strlen(temp);  //剩余长度	
		sprintf(tempAll,"%c%c%c%s", head1[0], head1[1], head1[2],temp);


    Init_len = Init_len + soilHum_len + sunLight_len + press_len + airTemp_len + high_len;
		u1_printf("%s\r\n",tempAll+3);
		u1_printf("总长度：%d\r\n",Init_len);
		MQTT_PublishQs0(Data_TOPIC_NAME,tempAll,Init_len);     //添加数据，发布给服务器
		
		//SEND_Data();
		//SEND_Veg();
		TIM_ClearITPendingBit(TIM2, TIM_IT_Update);            //清除TIM2溢出中断标志 	
	}
}
/*-------------------------------------------------*/
/*函数名：按键外部中断服务函数                      */
/*参  数：无                                       */
/*返回值：无                                       */
/*-------------------------------------------------*/
 
void EXTI0_IRQHandler(void)
{
  DelayMs(10);    //消抖
	if(WK_UP==1)
	{	  
		keyHuangTotal -= 2;
		SEND_Veg();
	}
	EXTI_ClearITPendingBit(EXTI_Line0);     //清除EXTI0线路挂起位
}
 void EXTI9_5_IRQHandler(void)
{			
	DelayMs(10);   //消抖			 
	if(KEY0==0)	
	{
			keyXiTotal -=2;
			SEND_Veg();
	}
 	 EXTI_ClearITPendingBit(EXTI_Line5);    //清除LINE5上的中断标志位  
}


void EXTI15_10_IRQHandler(void)
{
  DelayMs(10);    //消抖			 
  if(KEY1==0)	
	{
			keyQieTotal -=2;
			SEND_Veg();
	}
	 EXTI_ClearITPendingBit(EXTI_Line15);  //清除LINE15线路挂起位
}

/*-------------------------------------------------*/
/*函数名：不可屏蔽中断处理函数                     */
/*参  数：无                                       */
/*返回值：无                                       */
/*-------------------------------------------------*/
void NMI_Handler(void)
{

}

/*-------------------------------------------------*/
/*函数名：硬件出错后进入的中断处理函数             */
/*参  数：无                                       */
/*返回值：无                                       */
/*-------------------------------------------------*/
void HardFault_Handler(void)
{

}

/*-------------------------------------------------*/
/*函数名：内存管理中断处理函数                     */
/*参  数：无                                       */
/*返回值：无                                       */
/*-------------------------------------------------*/
void MemManage_Handler(void)
{

}

/*-------------------------------------------------*/
/*函数名：预取指失败，存储器访问失败中断处理函数   */
/*参  数：无                                       */
/*返回值：无                                       */
/*-------------------------------------------------*/
void BusFault_Handler(void)
{

}

/*-------------------------------------------------*/
/*函数名：未定义的指令或非法状态处理函数           */
/*参  数：无                                       */
/*返回值：无                                       */
/*-------------------------------------------------*/
void UsageFault_Handler(void)
{

}

/*-------------------------------------------------*/
/*函数名：软中断，SWI 指令调用的处理函数           */
/*参  数：无                                       */
/*返回值：无                                       */
/*-------------------------------------------------*/
void SVC_Handler(void)
{
	
}

/*-------------------------------------------------*/
/*函数名：调试监控器处理函数                       */
/*参  数：无                                       */
/*返回值：无                                       */
/*-------------------------------------------------*/
void DebugMon_Handler(void)
{
	
}

/*-------------------------------------------------*/
/*函数名：可挂起的系统服务处理函数                 */
/*参  数：无                                       */
/*返回值：无                                       */
/*-------------------------------------------------*/
void PendSV_Handler(void)
{
}

/*-------------------------------------------------*/
/*函数名：SysTic系统嘀嗒定时器处理函数             */
/*参  数：无                                       */
/*返回值：无                                       */
/*-------------------------------------------------*/
void SysTick_Handler(void)
{
 
}
