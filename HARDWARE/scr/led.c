 
/*-------------------------------------------------*/
/*         	                                       */
/*      	 		 LED（补光灯）控制              		   */
/*                                                 */
/*-------------------------------------------------*/

// 硬件连接：
// PB5 补光灯继电器 

#include "stm32f10x.h"  //包含需要的头文件
#include "led.h"        //包含需要的头文件

/*-------------------------------------------------*/
/*函数名：初始化补光灯函数                       	 */
/*参  数：无                                       */
/*返回值：无                                       */
/*-------------------------------------------------*/
void LED_Init(void)
{    	 
		GPIO_InitTypeDef GPIO_InitStructure;                     
		RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOB, ENABLE);    

		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;                
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;     
		GPIO_Init(GPIOB, &GPIO_InitStructure);            		          
}

/*-------------------------------------------------*/
/*函数名：补光灯开启                               */
/*参  数：无                                       */
/*返回值：无                                       */
/*-------------------------------------------------*/
void LED_On(void)
{			
		GPIO_InitTypeDef GPIO_InitStructure;  								  	                  
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;                
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;       
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;   		   
		GPIO_Init(GPIOB, &GPIO_InitStructure);            	    
		GPIO_SetBits(GPIOB, GPIO_Pin_5);
}

/*-------------------------------------------------*/
/*函数名：补光灯关闭                               */
/*参  数：无                                       */
/*返回值：无                                       */
/*-------------------------------------------------*/
void LED_Off(void)
{		
		GPIO_InitTypeDef GPIO_InitStructure;  										
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;   
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;   		
		GPIO_Init(GPIOB, &GPIO_InitStructure);
}



