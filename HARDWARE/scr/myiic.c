
/*-------------------------------------------------*/
/*                                                 */
/*      	    		   IIC(BMP280)      	      	   */
/*                                                 */
/*-------------------------------------------------*/

// ��ʼ��IIC
// PB10 SCK ��ѹ �¶� ���δ�����
// PB11 SDA
// SD0  GND(LCD�ӿڵ�GND�����Ľӿ�)

#include "myiic.h"
#include "delay.h"


/*-------------------------------------------------*/
/*��������BMP280��IIC���߳�ʼ��				           	 */
/*��  ����									             		         */
/*����ֵ��                                         */
/*-------------------------------------------------*/
void IIC_BMP280_Init(void)
{					     
	GPIO_InitTypeDef GPIO_InitStructure;

	RCC_APB2PeriphClockCmd(	RCC_APB2Periph_GPIOB, ENABLE );	
	   
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_11|GPIO_Pin_10;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_Out_PP ;    //�������
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOB,&GPIO_InitStructure);
 
	IIC_SCL1=1;
	IIC_SDA1=1;

}

/*-------------------------------------------------*/
/*��������BMP280��IIC���߿�ʼ�ź�				         	 */
/*��  ����									             		         */
/*����ֵ��                                         */
/*-------------------------------------------------*/
void IIC_Start1(void)
{
	SDA_OUT1();                           //sda�����
	IIC_SDA1=1;	  	  
	IIC_SCL1=1;
	DelayUs(4);
 	IIC_SDA1=0;                           //START:when CLK is high,DATA change form high to low 
	DelayUs(4);
	IIC_SCL1=0;                           //ǯסI2C���ߣ�׼�����ͻ�������� 
}

/*-------------------------------------------------*/
/*��������BMP280��IIC���߽����ź�				         	 */
/*��  ����									             		         */
/*����ֵ��   		                                   */
/*-------------------------------------------------*/
void IIC_Stop1(void)
{
	SDA_OUT1();                           //sda�����
	IIC_SCL1=0;
	IIC_SDA1=0;                           //STOP:when CLK is high DATA change form low to high
 	DelayUs(4);
	IIC_SCL1=1; 
	IIC_SDA1=1;                           //����I2C���߽����ź�
	DelayUs(4);							   	
}

/*-------------------------------------------------*/
/*��������BMP280��IIC���ߵȴ��źŵ���				     	 */
/*��  ����									             		         */
/*����ֵ��1��0            	                         */
/*-------------------------------------------------*/
u8 IIC_Wait_Ack1(void)
{
	u8 ucErrTime=0;
	SDA_IN1();                            //SDA����Ϊ����  
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
	IIC_SCL1=0;                           //ʱ�����0 	   
	return 0;  
} 

/*-------------------------------------------------*/
/*��������BMP280��IIC����Ӧ��				  				   	 */
/*��  ����									             		         */
/*����ֵ��               	                         */
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
/*��������BMP280��IIC���߲�����Ӧ��		  			   	 */
/*��  ����									             		         */
/*����ֵ��               	                         */
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
/*��������BMP280��IIC���߷���һ���ֽ�			  	   	 */
/*��  ����txd������һ���ֽ�				    		         */
/*����ֵ��               	                         */
/*-------------------------------------------------*/			  
void IIC_Send_Byte1(u8 txd)
{                        
    u8 t;   
		SDA_OUT1(); 	    
    IIC_SCL1=0;                         //����ʱ�ӿ�ʼ���ݴ���
    for(t=0;t<8;t++)
    {              
			IIC_SDA1=(txd&0x80)>>7;
			txd<<=1; 	  
			DelayUs(2);                        //��TEA5767��������ʱ���Ǳ����
			IIC_SCL1=1;
			DelayUs(2); 
			IIC_SCL1=0;	
			DelayUs(2);
    }	 
} 

/*-------------------------------------------------*/
/*��������BMP280��IIC���߶�ȡһ���ֽ�			  	   	 */
/*��  ����ack��ack=1ʱ������ACK��ack=0������nACK	 */
/*����ֵ��һ���ֽ�         	                       */
/*-------------------------------------------------*/
u8 IIC_Read_Byte1(unsigned char ack)
{
	unsigned char i,receive=0;
	SDA_IN1();                            //SDA����Ϊ����
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
        IIC_NAck1();                    //����nACK
    else
        IIC_Ack1();                     //����ACK   
    return receive;
}

/*-------------------------------------------------*/
/*��������BMP280��IIC���ߴ�ָ����ַ����һ������	 	 */
/*��  ����ReadAddr:������ַ										     */
/*				addr�� ��ַ														   */
/*����ֵ������������         	                     */
/*-------------------------------------------------*/
u8 iicDevReadByte1(u8 devaddr,u8 addr)
{				  
	u8 temp=0;		  	    																 
	IIC_Start1();  
	IIC_Send_Byte1(devaddr);              //��������д���� 	   
	IIC_Wait_Ack1(); 
	IIC_Send_Byte1(addr);                 //���͵͵�ַ
	IIC_Wait_Ack1();	

	IIC_Start1();  	 	   
	IIC_Send_Byte1(devaddr|1);            //��������������			   
	IIC_Wait_Ack1();	 
	temp=IIC_Read_Byte1(0);			   
	IIC_Stop1();                          //����һ��ֹͣ����	    
	return temp;
}

/*-------------------------------------------------*/
/*��������BMP280��IIC���ߴ�ָ����ַ����������ֽ�	 */
/*��  ����ReadAddr:������ַ										     */
/*				addr�� ��ʼ��ַ										 	     */
/*				len��  �ݳ���												     */
/*				rbuf�� �����ݻ���									 	     */
/*����ֵ������������         	                     */
/*-------------------------------------------------*/
void iicDevRead1(u8 devaddr,u8 addr,u8 len,u8 *rbuf)
{
	int i=0;
	IIC_Start1();  
	IIC_Send_Byte1(devaddr);  
	IIC_Wait_Ack1();	
	IIC_Send_Byte1(addr);                 //��ַ����  
	IIC_Wait_Ack1();	

	IIC_Start1();  	
	IIC_Send_Byte1(devaddr|1);  	
	IIC_Wait_Ack1();	
	for(i=0; i<len; i++)
	{
		if(i==len-1)
		{
			rbuf[i]=IIC_Read_Byte1(0);        //���һ���ֽڲ�Ӧ��
		}
		else
			rbuf[i]=IIC_Read_Byte1(1);
	}
	IIC_Stop1( );	
}

/*-------------------------------------------------*/
/*��������BMP280��IIC���ߴ�ָ����ַд��һ������	   */
/*��  ����WriteAddr :д�����ݵ�Ŀ�ĵ�ַ	           */
/* 				DataToWrite :Ҫд�������	               */
/*����ֵ��                	                       */
/*-------------------------------------------------*/
void iicDevWriteByte1(u8 devaddr,u8 addr,u8 data)
{				   	  	    																 
	IIC_Start1();  
	IIC_Send_Byte1(devaddr);              //��������д���� 	 
	IIC_Wait_Ack1();	   
	IIC_Send_Byte1(addr);                 //���͵͵�ַ
	IIC_Wait_Ack1(); 	 										  		   
	IIC_Send_Byte1(data);                 //�����ֽ�							   
	IIC_Wait_Ack1();  		    	   
	IIC_Stop1();		                      //����һ��ֹͣ���� 	 
}

/*-------------------------------------------------*/
/*��������BMP280��IIC���ߴ�ָ����ַ����д����ֽ�	 */
/*��  ����ReadAddr:������ַ										     */
/*				addr�� ��ʼ��ַ										 	     */
/*				len��  �ݳ���												     */
/*				wbuf�� д���ݻ���									 	     */
/*����ֵ��					         	                     */
/*-------------------------------------------------*/
void iicDevWrite1(u8 devaddr,u8 addr,u8 len,u8 *wbuf)
{
	int i=0;
	IIC_Start1();  
	IIC_Send_Byte1(devaddr);  	
	IIC_Wait_Ack1();	
	IIC_Send_Byte1(addr);  //��ַ����
	IIC_Wait_Ack1();	
	for(i=0; i<len; i++)
	{
		IIC_Send_Byte1(wbuf[i]);  
		IIC_Wait_Ack1();		
	}
	IIC_Stop1( );	
}
