 
/*-------------------------------------------------*/
/*         	                                       */
/*      	 					 水泵控制                 		   */
/*                                                 */
/*-------------------------------------------------*/

// 硬件连接：
// PD2 水泵继电器 

#include "stm32f10x.h"     //包含需要的头文件
#include "water.h"        //包含需要的头文件

/*-------------------------------------------------*/
/*函数名：初始化水泵电机函数                        */
/*参  数：无                                       */
/*返回值：无                                       */
/*-------------------------------------------------*/
void WAT_Init(void)
{    	 
		GPIO_InitTypeDef GPIO_InitStructure;                      
		RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOD, ENABLE);   

		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;                 
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;     
		GPIO_Init(GPIOD, &GPIO_InitStructure);            		          
}

/*-------------------------------------------------*/
/*函数名：水泵开启                                 */
/*参  数：无                                       */
/*返回值：无                                       */
/*-------------------------------------------------*/
void WAT_On(void)
{			
		GPIO_InitTypeDef GPIO_InitStructure;  								  	                  
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;                
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;        
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;   		   
		GPIO_Init(GPIOD, &GPIO_InitStructure);            	    
		GPIO_SetBits(GPIOD, GPIO_Pin_2);
}

/*-------------------------------------------------*/
/*函数名：水泵关闭                                 */
/*参  数：无                                       */
/*返回值：无                                       */
/*-------------------------------------------------*/
void WAT_Off(void)
{		
		GPIO_InitTypeDef GPIO_InitStructure;  										
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;   
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;   		
		GPIO_Init(GPIOD, &GPIO_InitStructure);
}



