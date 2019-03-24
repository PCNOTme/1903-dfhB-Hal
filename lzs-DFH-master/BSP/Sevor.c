#include "Sevor.h"
#include "stm32f4xx_hal.h"
#include "tim.h"

/**
  * @brief  舵机初始化
  * @param  无
  * @retval 无
  */
void Sevor_Init(void)
{
	HAL_TIM_PWM_Start(&htim11,TIM_CHANNEL_1);
	HAL_TIM_PWM_Start(&htim13,TIM_CHANNEL_1);
//HAL_TIM_PWM_Start(&htim14,TIM_CHANNEL_1);
	
  Sevor_1_PWM=100; //右边     减少--->抬升   [100,200]   垂直：100
	Sevor_2_PWM=200; //左边     增加--->抬升   [200,100]   垂直：200 
}


void Picking_Go(void)
{
  Sevor_1_PWM=200; 
	Sevor_2_PWM=100;  
}

void Picking_Up(void)
{
  Sevor_1_PWM=100; 
	Sevor_2_PWM=200; 
}

