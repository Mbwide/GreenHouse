
/*-------------------------------------------------*/
/*                                                 */
/*      	    		   IIC(BMP280)      	      	   */
/*                                                 */
/*-------------------------------------------------*/

// 初始化IIC
// PB10 SCK 气压 温度 海拔传感器
// PB11 SDA
// SD0  GND(LCD接口的GND，第四接口)

#include "myiic.h"
#include "delay.h"


/*-------------------------------------------------*/
/*函数名：BMP280的IIC总线初始化				           	 */
/*参  数：									             		         */
/*返回值：                                         */
/*-------------------------------------------------*/
void IIC_BMP280_Init(void)
{					     
	GPIO_InitTypeDef GPIO_InitStructure;

	RCC_APB2PeriphClockCmd(	RCC_APB2Periph_GPIOB, ENABLE );	
	   
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_11|GPIO_Pin_10;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_Out_PP ;    //推挽输出
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOB,&GPIO_InitStructure);
 
	IIC_SCL1=1;
	IIC_SDA1=1;

}

/*-------------------------------------------------*/
/*函数名：BMP280的IIC总线开始信号				         	 */
/*参  数：									             		         */
/*返回值：                                         */
/*-------------------------------------------------*/
void IIC_Start1(void)
{
	SDA_OUT1();                           //sda线输出
	IIC_SDA1=1;	  	  
	IIC_SCL1=1;
	DelayUs(4);
 	IIC_SDA1=0;                           //START:when CLK is high,DATA change form high to low 
	DelayUs(4);
	IIC_SCL1=0;                           //钳住I2C总线，准备发送或接收数据 
}

/*-------------------------------------------------*/
/*函数名：BMP280的IIC总线结束信号				         	 */
/*参  数：									             		         */
/*返回值：   		                                   */
/*-------------------------------------------------*/
void IIC_Stop1(void)
{
	SDA_OUT1();                           //sda线输出
	IIC_SCL1=0;
	IIC_SDA1=0;                           //STOP:when CLK is high DATA change form low to high
 	DelayUs(4);
	IIC_SCL1=1; 
	IIC_SDA1=1;                           //发送I2C总线结束信号
	DelayUs(4);							   	
}

/*-------------------------------------------------*/
/*函数名：BMP280的IIC总线等待信号到达				     	 */
/*参  数：									             		         */
/*返回值：1或0            	                         */
/*-------------------------------------------------*/
u8 IIC_Wait_Ack1(void)
{
	u8 ucErrTime=0;
	SDA_IN1();                            //SDA设置为输入  
	IIC_SDA1=1;DelayUs(1);	   
	IIC_SCL1=1;DelayUs(1);	 
	while(READ_SDA1)
	{
		ucErrTime++;
		if(ucErrTime>250)
		{
			IIC_Stop1();
			return 1;
		}
	}
	IIC_SCL1=0;                           //时钟输出0 	   
	return 0;  
} 

/*-------------------------------------------------*/
/*函数名：BMP280的IIC总线应答				  				   	 */
/*参  数：									             		         */
/*返回值：               	                         */
/*-------------------------------------------------*/
void IIC_Ack1(void)
{
	IIC_SCL1=0;
	SDA_OUT1();
	IIC_SDA1=0;
	DelayUs(2);
	IIC_SCL1=1;
	DelayUs(2);
	IIC_SCL1=0;
}

/*-------------------------------------------------*/
/*函数名：BMP280的IIC总线不产生应答		  			   	 */
/*参  数：									             		         */
/*返回值：               	                         */
/*-------------------------------------------------*/	    
void IIC_NAck1(void)
{
	IIC_SCL1=0;
	SDA_OUT1();
	IIC_SDA1=1;
	DelayUs(2);
	IIC_SCL1=1;
	DelayUs(2);
	IIC_SCL1=0;
}

/*-------------------------------------------------*/
/*函数名：BMP280的IIC总线发送一个字节			  	   	 */
/*参  数：txd：发送一个字节				    		         */
/*返回值：               	                         */
/*-------------------------------------------------*/			  
void IIC_Send_Byte1(u8 txd)
{                        
    u8 t;   
		SDA_OUT1(); 	    
    IIC_SCL1=0;                         //拉低时钟开始数据传输
    for(t=0;t<8;t++)
    {              
			IIC_SDA1=(txd&0x80)>>7;
			txd<<=1; 	  
			DelayUs(2);                        //对TEA5767这三个延时都是必须的
			IIC_SCL1=1;
			DelayUs(2); 
			IIC_SCL1=0;	
			DelayUs(2);
    }	 
} 

/*-------------------------------------------------*/
/*函数名：BMP280的IIC总线读取一个字节			  	   	 */
/*参  数：ack：ack=1时，发送ACK，ack=0，发送nACK	 */
/*返回值：一个字节         	                       */
/*-------------------------------------------------*/
u8 IIC_Read_Byte1(unsigned char ack)
{
	unsigned char i,receive=0;
	SDA_IN1();                            //SDA设置为输入
    for(i=0;i<8;i++ )
	{
        IIC_SCL1=0; 
        DelayUs(2);
		IIC_SCL1=1;
        receive<<=1;
        if(READ_SDA1)receive++;   
		DelayUs(1); 
    }					 
    if (!ack)
        IIC_NAck1();                    //发送nACK
    else
        IIC_Ack1();                     //发送ACK   
    return receive;
}

/*-------------------------------------------------*/
/*函数名：BMP280的IIC总线从指定地址读出一个数据	 	 */
/*参  数：ReadAddr:驱动地址										     */
/*				addr： 地址														   */
/*返回值：读到的数据         	                     */
/*-------------------------------------------------*/
u8 iicDevReadByte1(u8 devaddr,u8 addr)
{				  
	u8 temp=0;		  	    																 
	IIC_Start1();  
	IIC_Send_Byte1(devaddr);              //发送器件写命令 	   
	IIC_Wait_Ack1(); 
	IIC_Send_Byte1(addr);                 //发送低地址
	IIC_Wait_Ack1();	

	IIC_Start1();  	 	   
	IIC_Send_Byte1(devaddr|1);            //发送器件读命令			   
	IIC_Wait_Ack1();	 
	temp=IIC_Read_Byte1(0);			   
	IIC_Stop1();                          //产生一个停止条件	    
	return temp;
}

/*-------------------------------------------------*/
/*函数名：BMP280的IIC总线从指定地址连续读多个字节	 */
/*参  数：ReadAddr:驱动地址										     */
/*				addr： 起始地址										 	     */
/*				len：  据长度												     */
/*				rbuf： 读数据缓存									 	     */
/*返回值：读到的数据         	                     */
/*-------------------------------------------------*/
void iicDevRead1(u8 devaddr,u8 addr,u8 len,u8 *rbuf)
{
	int i=0;
	IIC_Start1();  
	IIC_Send_Byte1(devaddr);  
	IIC_Wait_Ack1();	
	IIC_Send_Byte1(addr);                 //地址自增  
	IIC_Wait_Ack1();	

	IIC_Start1();  	
	IIC_Send_Byte1(devaddr|1);  	
	IIC_Wait_Ack1();	
	for(i=0; i<len; i++)
	{
		if(i==len-1)
		{
			rbuf[i]=IIC_Read_Byte1(0);        //最后一个字节不应答
		}
		else
			rbuf[i]=IIC_Read_Byte1(1);
	}
	IIC_Stop1( );	
}

/*-------------------------------------------------*/
/*函数名：BMP280的IIC总线从指定地址写入一个数据	   */
/*参  数：WriteAddr :写入数据的目的地址	           */
/* 				DataToWrite :要写入的数据	               */
/*返回值：                	                       */
/*-------------------------------------------------*/
void iicDevWriteByte1(u8 devaddr,u8 addr,u8 data)
{				   	  	    																 
	IIC_Start1();  
	IIC_Send_Byte1(devaddr);              //发送器件写命令 	 
	IIC_Wait_Ack1();	   
	IIC_Send_Byte1(addr);                 //发送低地址
	IIC_Wait_Ack1(); 	 										  		   
	IIC_Send_Byte1(data);                 //发送字节							   
	IIC_Wait_Ack1();  		    	   
	IIC_Stop1();		                      //产生一个停止条件 	 
}

/*-------------------------------------------------*/
/*函数名：BMP280的IIC总线从指定地址连续写多个字节	 */
/*参  数：ReadAddr:驱动地址										     */
/*				addr： 起始地址										 	     */
/*				len：  据长度												     */
/*				wbuf： 写数据缓存									 	     */
/*返回值：					         	                     */
/*-------------------------------------------------*/
void iicDevWrite1(u8 devaddr,u8 addr,u8 len,u8 *wbuf)
{
	int i=0;
	IIC_Start1();  
	IIC_Send_Byte1(devaddr);  	
	IIC_Wait_Ack1();	
	IIC_Send_Byte1(addr);  //地址自增
	IIC_Wait_Ack1();	
	for(i=0; i<len; i++)
	{
		IIC_Send_Byte1(wbuf[i]);  
		IIC_Wait_Ack1();		
	}
	IIC_Stop1( );	
}
