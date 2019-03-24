#ifndef __MOTOR_H
#define __MOTOR_H
#include "stm32f4xx_hal.h"

#define PWM_LU1 TIM1->CCR1		//左前电机PWM通道2
#define PWM_LU2 TIM1->CCR2   //左前电机PWM通道1

#define PWM_RU2 TIM1->CCR3		//右前电机PWM通道1
#define PWM_RU1 TIM1->CCR4		//右前电机PWM通道2

#define PWM_LD1 TIM8->CCR1		//左后电机PWM通道1
#define PWM_LD2 TIM8->CCR2		//左后电机PWM通道2

#define PWM_RD2 TIM8->CCR3		//右后电机PWM通道1
#define PWM_RD1 TIM8->CCR4		//右后电机PWM通道2


void Motor_Init(void);
void Set_Pwm(int motor_a,int motor_b,int motor_c,int motor_d);

#endif 
