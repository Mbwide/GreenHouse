#ifndef __28byz48_H
#define __28byz48_H	 


//ULN2003驱动

#define uint unsigned int

void Delay_xms(uint x);						       		  //延时函数
void MOTOR_Init(void);  					       		  //步进电机初始化
void MotorForward(uint speed);           		  //步进电机正转函数
void MotorRev(int speed);        				 		  //步进电机反转函数
void MotorForwardAngle(int angle,int speed);  //步进电机正转角度函数
void MotorRevAngle(int angle,int speed); 			//步进电机反转角度函数
void MotorStop(void);                     		//步进电机停止函数

#endif


