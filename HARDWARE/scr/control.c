/*-------------------------------------------------*/
/*                                                 */
/*            			  控制端口           		   	   */
/*                                                 */
/*-------------------------------------------------*/

// 设备控制数据发送给服务器
// 门禁卡数据处理

#include "stm32f10x.h"
#include "control.h"
#include "usart1.h"
#include "rc522.h"
#include "mqtt.h"

int sta = 0; 						  								  //门禁卡当前读取进度

int card1 = 0;															//卡状态
int card2 = 0;
int card3 = 0;

unsigned char CT[2];												//卡类型	
unsigned char SN[4];												//卡号数组
unsigned char card1_flag=0; 								//进入标志
unsigned char card2_flag=0;
unsigned char card3_flag=0;
unsigned char card4_flag=0;
unsigned char card_1[4]={182,169,214,43};   //卡号
unsigned char card_2[4]={106,69,98,36};
unsigned char card_3[4]={249,245,201,194};
unsigned char card_4[4]={249,98,183,162};

extern int dataFlag;              //传感器状态（发送数据）
extern int motorFlag;						  //卷帘机状态
extern int watarFlag;             //水泵状态
extern int ledFlag;               //补光灯状态                   
extern int peoFlag;     				  //当前人数
extern int fanFlag;     				 	//换气扇状态

extern int keyHuangTotal;         //黄瓜当前量
extern int keyXiTotal;            //西红柿当前量
extern int keyQieTotal;     			//茄子当前量

/*------------------------------------------------*/
/*函数名：门禁卡数据处理                           */
/*参  数：无                                      */
/*返回值：人数                 									  */
/*------------------------------------------------*/
	void CardDataDeal(void)
	{
		sta = PcdRequest(PICC_REQIDL,CT);//寻卡
		if(sta==MI_OK)									 //寻卡成功
		{
			u1_printf("PcdRequest_MI_OK\r\n");
			sta = MI_ERR;
			sta = PcdAnticoll(SN);				 //防冲撞
			if(sta==MI_OK)  			 				 //选卡
			{
				u1_printf("PcdAnticoll_MI_OK\r\n");
				sta = MI_ERR;
				sta =PcdSelect(SN);
			}
				if(sta==MI_OK) //选卡
			{
				u1_printf("PcdSelect_MI_OK\r\n");
				sta = MI_ERR;
				sta = PcdHalt();
			}
			if(sta==MI_OK)   //选卡
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
/*函数名：数据长度 length 函数                     */
/*参  数：无                                       */
/*返回值：长度                 				             */
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
/*函数名：发送控制设备数据                         */
/*参  数：无                                       */
/*返回值：							                           */
/*-------------------------------------------------*/
void SEND_Data(void)
{		
	char  head1[3];
	char  temp[200];          //定义一个临时缓冲区1
	char  tempAll[220];       //定义一个临时缓冲区2

	int   Init_len = 87;      //存放初始长度
		
  char  dataAll[20];        //存放数据传送状态
	int   dataAll_len;        //数据传送状态长度

 	char  motorData[20];      //存放卷帘机状态		
	int   motorData_len;		   //放卷帘机状态长度
	
	char  waterData[20];      //存放水泵状态		
	int   waterData_len;		  //水泵状态长度

  char  ledData[20];        //存放补光灯状态
  int   ledData_len;        //补光灯状态长度
		
	char  peopleData[20];     //存放游客数量
	int   peopleData_len;			//游客数量长度
	
  char  fanData[20];        //存放补光灯状态
  int   fanData_len;        //补光灯状态长度

	u1_printf("dataFlag:%d\r\n",dataFlag);
	u1_printf("motorFlag :%d\r\n",motorFlag);
	u1_printf("watarFlag :%d\r\n",watarFlag);	
	u1_printf("ledFlag :%d\r\n",ledFlag);	
	u1_printf("peopleData :%d\r\n",peoFlag);	
	u1_printf("fan_flag :%d\r\n",fanFlag);

	memset(temp,       0, 200);                            //清空缓冲区1
	memset(tempAll,    0, 220);                            //清空缓冲区2
	memset(head1,      0, 3);                              //清空MQTT头
	memset(dataAll,    0, 20);                             //..
	memset(motorData,  0, 20);                             
	memset(waterData,  0, 20);                             
	memset(ledData,    0, 20);                             
	memset(peopleData, 0, 20);                             
	memset(fanData, 0, 20);                             
								
	sprintf(dataAll, "%d", dataFlag);       
	dataAll_len = length(dataFlag);	
	u1_printf("dataFlag长度：%d\r\n",dataFlag);
	
	sprintf(motorData, "%d", motorFlag);     
	motorData_len = length(motorFlag);
	u1_printf("motorFlag长度：%d\r\n",motorData_len);
	
	sprintf(waterData, "%d", watarFlag);    					 
	waterData_len = length(watarFlag);	
	u1_printf("watarFlag长度：%d\r\n",waterData_len);
	
	sprintf(ledData,  "%d", ledFlag); 						  
	ledData_len = length(ledFlag);	
	u1_printf("ledFlag长度：%d\r\n",ledData_len);
	
	sprintf(fanData,  "%d", fanFlag); 						 
	fanData_len = length(fanFlag);	
	u1_printf("fanFlag长度：%d\r\n",fanData_len);
	
	sprintf(peopleData,"%d", peoFlag); 					     
	peopleData_len = length(peoFlag);	
	u1_printf("peo_data长度：%d\r\n",peopleData_len);


	sprintf(temp,"{\"Dataflag\":\"%d\",",dataFlag);//构建报文

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
	
	head1[0] = 0x03; //固定报头
	head1[1] = 0x00; //固定报头
	head1[2] = strlen(temp); //剩余长度	
	sprintf(tempAll,"%c%c%c%s", head1[0], head1[1], head1[2],temp);


	Init_len = Init_len + dataAll_len + motorData_len + waterData_len + ledData_len + fanData_len + peopleData_len;
	u1_printf("%s\r\n",tempAll+3);
	u1_printf("总长度：%d\r\n",Init_len);
	MQTT_PublishQs0(Data_TOPIC_NAME,tempAll,Init_len);     //添加数据，发布给服务器		
	
}

void SEND_Veg(void)
{
	
	int   ShuCai;
	
	
	char  head1[3];
	char  temp[200];          //定义一个临时缓冲区1
	char  tempAll[220];       //定义一个临时缓冲区2
	int   Init_len = 16;      //存放初始长度
	int   veg_len;            //数据传送状态长度

	ShuCai = keyHuangTotal*10000 + keyXiTotal*100 + keyQieTotal;
	u1_printf("ShuCai:%d\r\n",ShuCai);	
	
	memset(temp,       0, 200);                            //清空缓冲区1
	memset(tempAll,    0, 220);                            //清空缓冲区2
	memset(head1,      0, 3);                              //清空MQTT头
							     
	veg_len = length(ShuCai);	
	u1_printf("ShuCai长度：%d\r\n",veg_len);
	
	sprintf(temp,"{\"ShuCai\":\"%d\"}",ShuCai);//构建报文

	head1[0] = 0x03; //固定报头
	head1[1] = 0x00; //固定报头
	head1[2] = strlen(temp); //剩余长度	
	sprintf(tempAll,"%c%c%c%s", head1[0], head1[1], head1[2],temp);

	Init_len = Init_len + veg_len;
	u1_printf("%s\r\n",tempAll+3);
	u1_printf("总长度：%d\r\n",Init_len);
	MQTT_PublishQs0(Data_TOPIC_NAME,tempAll,Init_len);     //添加数据，发布给服务器		
	
}

