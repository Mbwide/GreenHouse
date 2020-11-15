
/*-------------------------------------------------*/
/*                                                 */
/*      			   步进电机（卷帘机）       					 */
/*                                                 */
/*-------------------------------------------------*/

// 硬件连接：
// IN4: PC9   d
// IN3: PC8   c
// IN2: PC7   b
// IN1: PC6   a

#include "28byj48.h"
#include "delay.h"

#define uint unsigned int

/*-------------------------------------------------*/
/*函数名：电机延时                     					  */
/*参  数：                                        */
/*返回值：                                         */
/*-------------------------------------------------*/
void Delay_xms(uint x)
{
 uint i,j;
 for(i=0;i<x;i++)
  for(j=0;j<112;j++);
}

/*-------------------------------------------------*/
/*函数名：电机初始化                     					  */
/*参  数：                                         */
/*返回值：                                         */
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
/*函数名：电机正转                     					  */
/*参  数：                                         */
/*返回值：                                         */
/*-------------------------------------------------*/
void MotorForward(uint speed)  
{  
		    GPIO_SetBits(GPIOC,GPIO_Pin_9);//依次高电平输出
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
/*函数名：电机反转                     					  */
/*参  数：                                         */
/*返回值：                                         */
/*-------------------------------------------------*/
void MotorRev(int speed)  
{  
        GPIO_SetBits(GPIOC,GPIO_Pin_6);//依次高电平输出
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
/*函数名：电机停止                     					  */
/*参  数：                                         */
/*返回值：                                         */
/*-------------------------------------------------*/
void MotorStop(void)  
{ 
	 GPIO_ResetBits(GPIOC,GPIO_Pin_6 | GPIO_Pin_7 |GPIO_Pin_8 |GPIO_Pin_9 );
}

/*-------------------------------------------------*/
/*函数名：电机正转速度和角度控制        						 */
/*参  数：angel：角度                               */
/*参  数：speed：速度                               */
/*返回值：                                         */
/*-------------------------------------------------*/

//由于   *一个脉冲*   *输出轴*  转0.08789度（电机实转0.08789*64=5.625度），即步进角为5.625度。
//则转完A-B-C-D为  *8个脉冲*  ，即0.08789*8=0.70312度。若称A-B-C-D为一个周期，则j为需要的转完angle角度所需的周期数。
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
/*函数名：电机反转速度和角度控制             				 */
/*参  数：angel：角度                               */
/*参  数：speed：速度                               */
/*返回值：                                         */
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


