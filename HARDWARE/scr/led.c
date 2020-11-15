 
/*-------------------------------------------------*/
/*         	                                       */
/*      	 		 LED������ƣ�����              		   */
/*                                                 */
/*-------------------------------------------------*/

// Ӳ�����ӣ�
// PB5 ����Ƽ̵��� 

#include "stm32f10x.h"  //������Ҫ��ͷ�ļ�
#include "led.h"        //������Ҫ��ͷ�ļ�

/*-------------------------------------------------*/
/*����������ʼ������ƺ���                       	 */
/*��  ������                                       */
/*����ֵ����                                       */
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
/*������������ƿ���                               */
/*��  ������                                       */
/*����ֵ����                                       */
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
/*������������ƹر�                               */
/*��  ������                                       */
/*����ֵ����                                       */
/*-------------------------------------------------*/
void LED_Off(void)
{		
		GPIO_InitTypeDef GPIO_InitStructure;  										
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;   
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;   		
		GPIO_Init(GPIOB, &GPIO_InitStructure);
}



