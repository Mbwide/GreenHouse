
/*-------------------------------------------------*/
/*                                                 */
/*      			   �����������������       					 */
/*                                                 */
/*-------------------------------------------------*/

// Ӳ�����ӣ�
// IN4: PC9   d
// IN3: PC8   c
// IN2: PC7   b
// IN1: PC6   a

#include "28byj48.h"
#include "delay.h"

#define uint unsigned int

/*-------------------------------------------------*/
/*�������������ʱ                     					  */
/*��  ����                                        */
/*����ֵ��                                         */
/*-------------------------------------------------*/
void Delay_xms(uint x)
{
 uint i,j;
 for(i=0;i<x;i++)
  for(j=0;j<112;j++);
}

/*-------------------------------------------------*/
/*�������������ʼ��                     					  */
/*��  ����                                         */
/*����ֵ��                                         */
/*-------------------------------------------------*/
void MOTOR_Init(void)
{
 GPIO_InitTypeDef GPIO_InitStructure;
 RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC,ENABLE);
 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7 | GPIO_Pin_8 | GPIO_Pin_9 ;
 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
 GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
 GPIO_Init(GPIOC,&GPIO_InitStructure);
 GPIO_ResetBits(GPIOC,GPIO_Pin_6 | GPIO_Pin_7 |GPIO_Pin_8 |GPIO_Pin_9 );

}

/*-------------------------------------------------*/
/*�������������ת                     					  */
/*��  ����                                         */
/*����ֵ��                                         */
/*-------------------------------------------------*/
void MotorForward(uint speed)  
{  
		    GPIO_SetBits(GPIOC,GPIO_Pin_9);//���θߵ�ƽ���
        GPIO_ResetBits(GPIOC,GPIO_Pin_8);
        GPIO_ResetBits(GPIOC,GPIO_Pin_7);
        GPIO_ResetBits(GPIOC,GPIO_Pin_6);
        DelayUs(speed);
        
        GPIO_ResetBits(GPIOC,GPIO_Pin_9);
        GPIO_SetBits(GPIOC,GPIO_Pin_8);
        GPIO_ResetBits(GPIOC,GPIO_Pin_7);
        GPIO_ResetBits(GPIOC,GPIO_Pin_6);
        DelayUs(speed);
        
        GPIO_ResetBits(GPIOC,GPIO_Pin_9);
        GPIO_ResetBits(GPIOC,GPIO_Pin_8);
        GPIO_SetBits(GPIOC,GPIO_Pin_7);
        GPIO_ResetBits(GPIOC,GPIO_Pin_6);
        DelayUs(speed);
        
        GPIO_ResetBits(GPIOC,GPIO_Pin_9);
        GPIO_ResetBits(GPIOC,GPIO_Pin_8);
        GPIO_ResetBits(GPIOC,GPIO_Pin_7);
        GPIO_SetBits(GPIOC,GPIO_Pin_6);
        DelayUs(speed);
}

/*-------------------------------------------------*/
/*�������������ת                     					  */
/*��  ����                                         */
/*����ֵ��                                         */
/*-------------------------------------------------*/
void MotorRev(int speed)  
{  
        GPIO_SetBits(GPIOC,GPIO_Pin_6);//���θߵ�ƽ���
        GPIO_ResetBits(GPIOC,GPIO_Pin_7);
        GPIO_ResetBits(GPIOC,GPIO_Pin_8);
        GPIO_ResetBits(GPIOC,GPIO_Pin_9);
        DelayUs(speed);
        
        GPIO_ResetBits(GPIOC,GPIO_Pin_6);
        GPIO_SetBits(GPIOC,GPIO_Pin_7);
        GPIO_ResetBits(GPIOC,GPIO_Pin_8);
        GPIO_ResetBits(GPIOC,GPIO_Pin_9);
        DelayUs(speed);
        
        GPIO_ResetBits(GPIOC,GPIO_Pin_6);
        GPIO_ResetBits(GPIOC,GPIO_Pin_7);
        GPIO_SetBits(GPIOC,GPIO_Pin_8);
        GPIO_ResetBits(GPIOC,GPIO_Pin_9);
        DelayUs(speed);
        
        GPIO_ResetBits(GPIOC,GPIO_Pin_6);
        GPIO_ResetBits(GPIOC,GPIO_Pin_7);
        GPIO_ResetBits(GPIOC,GPIO_Pin_8);
        GPIO_SetBits(GPIOC,GPIO_Pin_9);
        DelayUs(speed);
}

/*-------------------------------------------------*/
/*�����������ֹͣ                     					  */
/*��  ����                                         */
/*����ֵ��                                         */
/*-------------------------------------------------*/
void MotorStop(void)  
{ 
	 GPIO_ResetBits(GPIOC,GPIO_Pin_6 | GPIO_Pin_7 |GPIO_Pin_8 |GPIO_Pin_9 );
}

/*-------------------------------------------------*/
/*�������������ת�ٶȺͽǶȿ���        						 */
/*��  ����angel���Ƕ�                               */
/*��  ����speed���ٶ�                               */
/*����ֵ��                                         */
/*-------------------------------------------------*/

//����   *һ������*   *�����*  ת0.08789�ȣ����ʵת0.08789*64=5.625�ȣ�����������Ϊ5.625�ȡ�
//��ת��A-B-C-DΪ  *8������*  ����0.08789*8=0.70312�ȡ�����A-B-C-DΪһ�����ڣ���jΪ��Ҫ��ת��angle�Ƕ��������������
void MotorForwardAngle(int angle,int speed)
{
	int i,j;
	j=(int)(angle/0.70312);
	for(i=0;i<j;i++)
	{
		MotorForward(speed);
	}
}

/*-------------------------------------------------*/
/*�������������ת�ٶȺͽǶȿ���             				 */
/*��  ����angel���Ƕ�                               */
/*��  ����speed���ٶ�                               */
/*����ֵ��                                         */
/*-------------------------------------------------*/
void MotorRevAngle(int angle,int speed)
{
	int i,j;
	j=(int)(angle/0.70312);
	for(i=0;i<j;i++)
	{
		MotorRev(speed);
	}
}


