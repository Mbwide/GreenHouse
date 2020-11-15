 
/*-------------------------------------------------*/
/*         	                                       */
/*      	 				   换气扇控制              		   */
/*                                                 */
/*-------------------------------------------------*/

// 硬件连接：
// PB6 换气扇继电器 

#include "stm32f10x.h"  //包含需要的头文件
#include "fan.h"        //包含需要的头文件

/*-------------------------------------------------*/
/*函数名：初始化换气扇函数                          */
/*参  数：无                                       */
/*返回值：无                                       */
/*-------------------------------------------------*/
void FAN_Init(void)
{    	 
		GPIO_InitTypeDef GPIO_InitStructure;                      
		RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOB, ENABLE);    

		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;                 
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;     
		GPIO_Init(GPIOB, &GPIO_InitStructure);            		          
}

/*-------------------------------------------------*/
/*函数名：换气扇开启                                */
/*参  数：无                                       */
/*返回值：无                                       */
/*-------------------------------------------------*/
void FAN_On(void)
{			
		GPIO_InitTypeDef GPIO_InitStructure;  								  	                  
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;                
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;        
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;   		   
		GPIO_Init(GPIOB, &GPIO_InitStructure);            	     
		GPIO_SetBits(GPIOB, GPIO_Pin_6);
}

/*-------------------------------------------------*/
/*函数名：换气扇关闭                                */
/*参  数：无                                       */
/*返回值：无                                       */
/*-------------------------------------------------*/
void FAN_Off(void)
{		
		GPIO_InitTypeDef GPIO_InitStructure;  										
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;   
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;   		
		GPIO_Init(GPIOB, &GPIO_InitStructure);
}



