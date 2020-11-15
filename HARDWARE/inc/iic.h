/*-------------------------------------------------*/
/*            ��γ����STM32ϵ�п�����              */
/*-------------------------------------------------*/
/*                                                 */
/*            ʵ��IIc���߹��ܵ�Դ�ļ�              */
/*                                                 */
/*-------------------------------------------------*/

#ifndef __IIC_H
#define __IIC_H
  		   
#define SDA_IN()       {GPIOC->CRH&=0XFFFF0FFF;GPIOC->CRH|=8<<12;GPIOC->BSRR=(1<<11);} //SDA����Ϊ��������ģʽ
#define SDA_OUT()      {GPIOC->CRH&=0XFFFF0FFF;GPIOC->CRH|=3<<12;}                     //SDA����Ϊ�������ģʽ
#define READ_SDA        GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_11)       //��ȡSDA 
#define IIC_SDA_OUT(x)  GPIO_WriteBit(GPIOC, GPIO_Pin_11, (BitAction)x) //����SDA��ƽ 

#define	IIC_SCL_H    GPIO_SetBits(GPIOC, GPIO_Pin_12)    //SCL����
#define	IIC_SDA_H    GPIO_SetBits(GPIOC, GPIO_Pin_11)    //SDA����

#define	IIC_SCL_L    GPIO_ResetBits(GPIOC, GPIO_Pin_12)  //SCL����
#define	IIC_SDA_L    GPIO_ResetBits(GPIOC, GPIO_Pin_11)  //SDA����

void IIC_BY30_Init(void);
void IIC_Start(void);
void IIC_Stop(void);
char IIC_Wait_Ack(void);
void IIC_Send_Byte(unsigned char);
unsigned char IIC_Read_Byte(unsigned char);

#endif
















