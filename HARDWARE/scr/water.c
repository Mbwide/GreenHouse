 
/*-------------------------------------------------*/
/*         	                                       */
/*      	 					 ˮ�ÿ���                 		   */
/*                                                 */
/*-------------------------------------------------*/

// Ӳ�����ӣ�
// PD2 ˮ�ü̵��� 

#include "stm32f10x.h"     //������Ҫ��ͷ�ļ�
#include "water.h"        //������Ҫ��ͷ�ļ�

/*-------------------------------------------------*/
/*����������ʼ��ˮ�õ������                        */
/*��  ������                                       */
/*����ֵ����                                       */
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
/*��������ˮ�ÿ���                                 */
/*��  ������                                       */
/*����ֵ����                                       */
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
/*��������ˮ�ùر�                                 */
/*��  ������                                       */
/*����ֵ����                                       */
/*-------------------------------------------------*/
void WAT_Off(void)
{		
		GPIO_InitTypeDef GPIO_InitStructure;  										
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;   
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;   		
		GPIO_Init(GPIOD, &GPIO_InitStructure);
}



