#ifndef __28byz48_H
#define __28byz48_H	 


//ULN2003����

#define uint unsigned int

void Delay_xms(uint x);						       		  //��ʱ����
void MOTOR_Init(void);  					       		  //���������ʼ��
void MotorForward(uint speed);           		  //���������ת����
void MotorRev(int speed);        				 		  //���������ת����
void MotorForwardAngle(int angle,int speed);  //���������ת�ǶȺ���
void MotorRevAngle(int angle,int speed); 			//���������ת�ǶȺ���
void MotorStop(void);                     		//�������ֹͣ����

#endif


