/*-------------------------------------------------*/
/*            ��γ����STM32ϵ�п�����              */
/*-------------------------------------------------*/
/*                                                 */
/*              ʵ��LED���ܵ�ͷ�ļ�                */
/*                                                 */
/*-------------------------------------------------*/

#ifndef __LED_H
#define __LED_H

#define LED2_OUT(x)   GPIO_WriteBit(GPIOD, GPIO_Pin_2, (BitAction)x)  //PD2����LED2������PB0�ĵ�ƽ�����Ե���Ϩ��LED2

#define LED2_IN_STA   GPIO_ReadOutputDataBit(GPIOD, GPIO_Pin_2) //PD2����LED2,��ȡ��ƽ״̬�������ж�LED2�ǵ�������Ϩ��
 
#define LED2_ON       GPIO_ResetBits(GPIOD, GPIO_Pin_2)         //PD2����LED2��������������PD2��ƽ������LED2
#define LED2_OFF      GPIO_SetBits(GPIOD, GPIO_Pin_2)           //PD2����LED2��������������PD2��ƽ��Ϩ��LED2

void LED_Init(void);               //��ʼ��	
void LED_AllOn(void);              //��������LED
void LED_AllOff(void);             //Ϩ������LED

#endif
