/*-------------------------------------------------*/
/*            超纬电子STM32系列开发板              */
/*-------------------------------------------------*/
/*                                                 */
/*              实现LED功能的头文件                */
/*                                                 */
/*-------------------------------------------------*/

#ifndef __LED_H
#define __LED_H

#define LED2_OUT(x)   GPIO_WriteBit(GPIOD, GPIO_Pin_2, (BitAction)x)  //PD2控制LED2，设置PB0的电平，可以点亮熄灭LED2

#define LED2_IN_STA   GPIO_ReadOutputDataBit(GPIOD, GPIO_Pin_2) //PD2控制LED2,读取电平状态，可以判断LED2是点亮还是熄灭
 
#define LED2_ON       GPIO_ResetBits(GPIOD, GPIO_Pin_2)         //PD2控制LED2，共阳极，拉低PD2电平，点亮LED2
#define LED2_OFF      GPIO_SetBits(GPIOD, GPIO_Pin_2)           //PD2控制LED2，共阳极，拉高PD2电平，熄灭LED2

void LED_Init(void);               //初始化	
void LED_AllOn(void);              //点亮所有LED
void LED_AllOff(void);             //熄灭所有LED

#endif
