#ifndef __SEVOR_H
#define __SEVOR_H	 
#include "stm32f4xx_hal.h"

#define Sevor_1_PWM TIM11->CCR1   //
#define Sevor_2_PWM TIM13->CCR1		//
//#define Sevor_3_PWM TIM14->CCR1		//

void Sevor_Init(void);

void Picking_Go(void);

void Picking_Up(void);

#endif


