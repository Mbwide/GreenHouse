/*-------------------------------------------------*/
/*                                                 */
/*            			  ���ƶ˿�           		   	   */
/*                                                 */
/*-------------------------------------------------*/

// �豸�������ݷ��͸�������
// �Ž������ݴ���

#include "stm32f10x.h"
#include "control.h"
#include "usart1.h"
#include "rc522.h"
#include "mqtt.h"

int sta = 0; 						  								  //�Ž�����ǰ��ȡ����

int card1 = 0;															//��״̬
int card2 = 0;
int card3 = 0;

unsigned char CT[2];												//������	
unsigned char SN[4];												//��������
unsigned char card1_flag=0; 								//�����־
unsigned char card2_flag=0;
unsigned char card3_flag=0;
unsigned char card4_flag=0;
unsigned char card_1[4]={182,169,214,43};   //����
unsigned char card_2[4]={106,69,98,36};
unsigned char card_3[4]={249,245,201,194};
unsigned char card_4[4]={249,98,183,162};

extern int dataFlag;              //������״̬���������ݣ�
extern int motorFlag;						  //������״̬
extern int watarFlag;             //ˮ��״̬
extern int ledFlag;               //�����״̬                   
extern int peoFlag;     				  //��ǰ����
extern int fanFlag;     				 	//������״̬

extern int keyHuangTotal;         //�ƹϵ�ǰ��
extern int keyXiTotal;            //��������ǰ��
extern int keyQieTotal;     			//���ӵ�ǰ��

/*------------------------------------------------*/
/*���������Ž������ݴ���                           */
/*��  ������                                      */
/*����ֵ������                 									  */
/*------------------------------------------------*/
	void CardDataDeal(void)
	{
		sta = PcdRequest(PICC_REQIDL,CT);//Ѱ��
		if(sta==MI_OK)									 //Ѱ���ɹ�
		{
			u1_printf("PcdRequest_MI_OK\r\n");
			sta = MI_ERR;
			sta = PcdAnticoll(SN);				 //����ײ
			if(sta==MI_OK)  			 				 //ѡ��
			{
				u1_printf("PcdAnticoll_MI_OK\r\n");
				sta = MI_ERR;
				sta =PcdSelect(SN);
			}
				if(sta==MI_OK) //ѡ��
			{
				u1_printf("PcdSelect_MI_OK\r\n");
				sta = MI_ERR;
				sta = PcdHalt();
			}
			if(sta==MI_OK)   //ѡ��
			{
				u1_printf("PcdHalt_MI_OK\r\n");
				if(*SN == *card_1)
				{
					if(0 == card1)
					{
						card1 = 1;
						peoFlag +=1;
						u1_printf("this is card_1\r\n");
					}
					else if(1 == card1)
					{
						card1 = 0;
						peoFlag -=1;
						u1_printf("card_1 out\r\n");
					}
				}
				else if(*SN == *card_2)
				{
					if(0 == card2)
					{
						card2 = 1;
						peoFlag += 1;
						u1_printf("this is card_2\r\n");
					}
					else if(1 == card2)
					{
						card2 = 0;
						peoFlag -= 1;
						u1_printf("card_2 out\r\n");
					}
				}
				else if(*SN == *card_3)
				{
					if(*(SN+1) == *(card_3+1))
					{
						if(0 == card3)
						{
							card3 = 1;
							peoFlag +=1;
							u1_printf("this is card_3\r\n");
						}
						else if(1 == card3)
						{
							card3 = 0;
							peoFlag -=1;
							u1_printf("card_3 out\r\n");
						}
					}
					else
					{
						u1_printf("You are not allowed to enter!\r\n");
					}
				}
				else
				{
					u1_printf("You are not allowed to enter!\r\n");
				}		
				memset(SN,0,4);
				SEND_Data();
			}
		}
	}

/*-------------------------------------------------*/
/*�����������ݳ��� length ����                     */
/*��  ������                                       */
/*����ֵ������                 				             */
/*-------------------------------------------------*/
int length(int a)
{
	int i = 1;
	while(a/10 != 0)
	{
		i++;
		a = a/10;
	}
	return i;
}

/*-------------------------------------------------*/
/*�����������Ϳ����豸����                         */
/*��  ������                                       */
/*����ֵ��							                           */
/*-------------------------------------------------*/
void SEND_Data(void)
{		
	char  head1[3];
	char  temp[200];          //����һ����ʱ������1
	char  tempAll[220];       //����һ����ʱ������2

	int   Init_len = 87;      //��ų�ʼ����
		
  char  dataAll[20];        //������ݴ���״̬
	int   dataAll_len;        //���ݴ���״̬����

 	char  motorData[20];      //��ž�����״̬		
	int   motorData_len;		   //�ž�����״̬����
	
	char  waterData[20];      //���ˮ��״̬		
	int   waterData_len;		  //ˮ��״̬����

  char  ledData[20];        //��Ų����״̬
  int   ledData_len;        //�����״̬����
		
	char  peopleData[20];     //����ο�����
	int   peopleData_len;			//�ο���������
	
  char  fanData[20];        //��Ų����״̬
  int   fanData_len;        //�����״̬����

	u1_printf("dataFlag:%d\r\n",dataFlag);
	u1_printf("motorFlag :%d\r\n",motorFlag);
	u1_printf("watarFlag :%d\r\n",watarFlag);	
	u1_printf("ledFlag :%d\r\n",ledFlag);	
	u1_printf("peopleData :%d\r\n",peoFlag);	
	u1_printf("fan_flag :%d\r\n",fanFlag);

	memset(temp,       0, 200);                            //��ջ�����1
	memset(tempAll,    0, 220);                            //��ջ�����2
	memset(head1,      0, 3);                              //���MQTTͷ
	memset(dataAll,    0, 20);                             //..
	memset(motorData,  0, 20);                             
	memset(waterData,  0, 20);                             
	memset(ledData,    0, 20);                             
	memset(peopleData, 0, 20);                             
	memset(fanData, 0, 20);                             
								
	sprintf(dataAll, "%d", dataFlag);       
	dataAll_len = length(dataFlag);	
	u1_printf("dataFlag���ȣ�%d\r\n",dataFlag);
	
	sprintf(motorData, "%d", motorFlag);     
	motorData_len = length(motorFlag);
	u1_printf("motorFlag���ȣ�%d\r\n",motorData_len);
	
	sprintf(waterData, "%d", watarFlag);    					 
	waterData_len = length(watarFlag);	
	u1_printf("watarFlag���ȣ�%d\r\n",waterData_len);
	
	sprintf(ledData,  "%d", ledFlag); 						  
	ledData_len = length(ledFlag);	
	u1_printf("ledFlag���ȣ�%d\r\n",ledData_len);
	
	sprintf(fanData,  "%d", fanFlag); 						 
	fanData_len = length(fanFlag);	
	u1_printf("fanFlag���ȣ�%d\r\n",fanData_len);
	
	sprintf(peopleData,"%d", peoFlag); 					     
	peopleData_len = length(peoFlag);	
	u1_printf("peo_data���ȣ�%d\r\n",peopleData_len);


	sprintf(temp,"{\"Dataflag\":\"%d\",",dataFlag);//��������

	sprintf(motorData, "\"Motorflag\":\"%d\",", motorFlag);
	strcat(temp, motorData);
	
	sprintf(waterData, "\"Watarflag\":\"%d\",", watarFlag);
	strcat(temp, waterData);
	
	sprintf(ledData, "\"Ledflag\":\"%d\",", ledFlag);
	strcat(temp, ledData);
	
	sprintf(fanData, "\"Fanflag\":\"%d\",", fanFlag);
	strcat(temp, fanData);
	
	sprintf(peopleData, "\"Peoflag\":\"%d\"}", peoFlag);
	strcat(temp, peopleData);
	
	head1[0] = 0x03; //�̶���ͷ
	head1[1] = 0x00; //�̶���ͷ
	head1[2] = strlen(temp); //ʣ�೤��	
	sprintf(tempAll,"%c%c%c%s", head1[0], head1[1], head1[2],temp);


	Init_len = Init_len + dataAll_len + motorData_len + waterData_len + ledData_len + fanData_len + peopleData_len;
	u1_printf("%s\r\n",tempAll+3);
	u1_printf("�ܳ��ȣ�%d\r\n",Init_len);
	MQTT_PublishQs0(Data_TOPIC_NAME,tempAll,Init_len);     //������ݣ�������������		
	
}

void SEND_Veg(void)
{
	
	int   ShuCai;
	
	
	char  head1[3];
	char  temp[200];          //����һ����ʱ������1
	char  tempAll[220];       //����һ����ʱ������2
	int   Init_len = 16;      //��ų�ʼ����
	int   veg_len;            //���ݴ���״̬����

	ShuCai = keyHuangTotal*10000 + keyXiTotal*100 + keyQieTotal;
	u1_printf("ShuCai:%d\r\n",ShuCai);	
	
	memset(temp,       0, 200);                            //��ջ�����1
	memset(tempAll,    0, 220);                            //��ջ�����2
	memset(head1,      0, 3);                              //���MQTTͷ
							     
	veg_len = length(ShuCai);	
	u1_printf("ShuCai���ȣ�%d\r\n",veg_len);
	
	sprintf(temp,"{\"ShuCai\":\"%d\"}",ShuCai);//��������

	head1[0] = 0x03; //�̶���ͷ
	head1[1] = 0x00; //�̶���ͷ
	head1[2] = strlen(temp); //ʣ�೤��	
	sprintf(tempAll,"%c%c%c%s", head1[0], head1[1], head1[2],temp);

	Init_len = Init_len + veg_len;
	u1_printf("%s\r\n",tempAll+3);
	u1_printf("�ܳ��ȣ�%d\r\n",Init_len);
	MQTT_PublishQs0(Data_TOPIC_NAME,tempAll,Init_len);     //������ݣ�������������		
	
}

