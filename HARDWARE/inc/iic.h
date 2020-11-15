/*-------------------------------------------------*/
/*            超纬电子STM32系列开发板              */
/*-------------------------------------------------*/
/*                                                 */
/*            实现IIc总线功能的源文件              */
/*                                                 */
/*-------------------------------------------------*/

#ifndef __IIC_H
#define __IIC_H
  		   
#define SDA_IN()       {GPIOC->CRH&=0XFFFF0FFF;GPIOC->CRH|=8<<12;GPIOC->BSRR=(1<<11);} //SDA设置为上拉输入模式
#define SDA_OUT()      {GPIOC->CRH&=0XFFFF0FFF;GPIOC->CRH|=3<<12;}                     //SDA设置为推免输出模式
#define READ_SDA        GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_11)       //读取SDA 
#define IIC_SDA_OUT(x)  GPIO_WriteBit(GPIOC, GPIO_Pin_11, (BitAction)x) //设置SDA电平 

#define	IIC_SCL_H    GPIO_SetBits(GPIOC, GPIO_Pin_12)    //SCL拉高
#define	IIC_SDA_H    GPIO_SetBits(GPIOC, GPIO_Pin_11)    //SDA拉高

#define	IIC_SCL_L    GPIO_ResetBits(GPIOC, GPIO_Pin_12)  //SCL拉低
#define	IIC_SDA_L    GPIO_ResetBits(GPIOC, GPIO_Pin_11)  //SDA拉低

void IIC_BY30_Init(void);
void IIC_Start(void);
void IIC_Stop(void);
char IIC_Wait_Ack(void);
void IIC_Send_Byte(unsigned char);
unsigned char IIC_Read_Byte(unsigned char);

#endif
















