
#ifndef __DHT11_H
#define __DHT11_H 
 
#define DHT11_IO_IN()  {GPIOA->CRH&=0XFFFFFFF0;GPIOA->CRH|=8<<0;}       //PA8����DHT11������Ϊ���
#define DHT11_IO_OUT() {GPIOA->CRH&=0XFFFFFFF0;GPIOA->CRH|=3<<0;}       //PA8����DHT11������Ϊ����
										   
#define DHT11_OUT(x)    GPIO_WriteBit(GPIOA, GPIO_Pin_8, (BitAction)x)  //PA8����DHT11
#define DHT11_DQ_IN     GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_8)        //PA8����DHT11	
   
void DHT11_Rst(void);
char DHT11_Check(void);
char DHT11_Read_Bit(void);
char DHT11_Read_Byte(void);
char DHT11_Read_Data(char *temp, char *humi);
char DHT11_Init(void);

#endif















