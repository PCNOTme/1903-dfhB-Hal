#ifndef __TIMER_H
#define __TIMER_H
#include "stm32f10x.h"
#include "sys.h"


void TIM8_Int_Init(u16 arr,u16 psc);      //���
void TIM5_Cap_Init(u16 arr,u16 psc);			//����
void TIM4_Int_Init(u16 arr,u16 psc);			//���


#endif
