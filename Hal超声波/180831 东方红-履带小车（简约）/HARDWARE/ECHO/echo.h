#include "sys.h"
#include "delay.h"
#include "stm32f10x.h"
void Trig(void);
void Trig_Init(void);
void Trig_cal(int dis_left,int dis_right,int dis_front);
void Echo_xianfu(void);
void Echo_Flag(void);
void Echo(void);
#define Switch1   		GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_14)

