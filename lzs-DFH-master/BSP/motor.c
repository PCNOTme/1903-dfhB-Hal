#include "motor.h"
#include "stm32f4xx_hal.h"
#include "tim.h"
#include "encoder.h"
#include "hc_sr04.h"

/**
  * @brief  设置四个电机PWM
  * @param  四个电机PWM
  * @retval 无
  */
void Set_Pwm(int motor_1,int motor_2,int motor_3,int motor_4)
{
	  	if(motor_1>0)			PWM_LU1=8400,PWM_LU2=8400-motor_1;
			else 	            PWM_LU2=8400,PWM_LU1=8400+motor_1;		
		  if(motor_2>0)			PWM_RU1=8400,PWM_RU2=8400-motor_2;
			else 	            PWM_RU2=8400,PWM_RU1=8400+motor_2;
	    if(motor_3>0)			PWM_LD1=8400,PWM_LD2=8400-motor_3;
			else 	            PWM_LD2=8400,PWM_LD1=8400+motor_3;	
	    if(motor_4>0)			PWM_RD1=8400,PWM_RD2=8400-motor_4;
			else 	            PWM_RD2=8400,PWM_RD1=8400+motor_4;
}


/**
  * @brief  电机驱动初始化
  * @param  无
  * @retval 无
  */
void Motor_Init(void)
{
	HAL_TIM_PWM_Start(&htim1,TIM_CHANNEL_1);
	HAL_TIM_PWM_Start(&htim1,TIM_CHANNEL_2);
	HAL_TIM_PWM_Start(&htim1,TIM_CHANNEL_3);
	HAL_TIM_PWM_Start(&htim1,TIM_CHANNEL_4);
	HAL_TIM_PWM_Start(&htim8,TIM_CHANNEL_1);
	HAL_TIM_PWM_Start(&htim8,TIM_CHANNEL_2);
	HAL_TIM_PWM_Start(&htim8,TIM_CHANNEL_3);
	HAL_TIM_PWM_Start(&htim8,TIM_CHANNEL_4);
}

