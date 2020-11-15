#ifndef __MYIIC_H
#define __MYIIC_H
#include "sys.h"
  		   
//IO方向设置
#define SDA_IN1()  {GPIOB->CRH&=0XFFFF0FFF;GPIOB->CRH|=8<<12;}
#define SDA_OUT1() {GPIOB->CRH&=0XFFFF0FFF;GPIOB->CRH|=3<<12;}

//IO操作函数	 
#define IIC_SCL1    PBout(10)                                                   //SCL
#define IIC_SDA1    PBout(11)                                                   //SDA	 
#define READ_SDA1   PBin(11)                                                    //输入SDA 

//IIC所有操作函数
void IIC_BMP280_Init(void);                                                           //初始化IIC的IO口				 
void IIC_Start1(void);				                                           //发送IIC开始信号
void IIC_Stop1(void);	  			                                           //发送IIC停止信号
void IIC_Send_Byte1(u8 txd);			                                           //IIC发送一个字节
u8 IIC_Read_Byte1(unsigned char ack);                                           //IIC读取一个字节
u8 IIC_Wait_Ack1(void); 				                                           //IIC等待ACK信号
void IIC_Ack1(void);					                                           //IIC发送ACK信号
void IIC_NAck1(void);				                                           //IIC不发送ACK信号

void IIC_Write_One_Byte1(u8 daddr,u8 addr,u8 data);
u8 IIC_Read_One_Byte1(u8 daddr,u8 addr);	
			 
u8 iicDevReadByte1(u8 devaddr,u8 addr);	                                       /*读一字节*/
void iicDevWriteByte1(u8 devaddr,u8 addr,u8 data);	                           /*写一字节*/
void iicDevRead1(u8 devaddr,u8 addr,u8 len,u8 *rbuf);                           /*连续读取多个字节*/
void iicDevWrite1(u8 devaddr,u8 addr,u8 len,u8 *wbuf);                          /*连续写入多个字节*/

#endif
