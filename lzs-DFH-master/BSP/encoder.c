#include "encoder.h"
#include "tim.h"
#include "path_planning.h"
#include "stm32f4xx_hal.h"


Encoder encoder;

/**
  * @brief  编码器捕获初始化
  * @param  无
  * @retval 无
  */
void Encoder_Init(void)
{
	HAL_TIM_Encoder_Start(&htim2,TIM_CHANNEL_ALL);
	HAL_TIM_Encoder_Start(&htim3,TIM_CHANNEL_ALL);
	HAL_TIM_Encoder_Start(&htim4,TIM_CHANNEL_ALL);
	HAL_TIM_Encoder_Start(&htim5,TIM_CHANNEL_ALL);

}

/**
  * @brief  单位时间读取编码器计数
  * @param  无
  * @retval 定时器脉冲值
  */
int Read_Encoder(int TIMX)
{
   int Encoder_TIM;    
   switch(TIMX)
	 {
	   case 2:  Encoder_TIM= (short)TIM2 -> CNT;   TIM2 -> CNT=0; break;
		 case 3:  Encoder_TIM= (short)TIM3 -> CNT;   TIM3 -> CNT=0; break;
		 case 4:  Encoder_TIM= (short)TIM4 -> CNT;   TIM4 -> CNT=0; break;	
		 case 5:  Encoder_TIM= (short)TIM5 -> CNT;   TIM5 -> CNT=0; break;	
		 default:  Encoder_TIM=0;
	 }
		return Encoder_TIM;
}

/**
  * @brief  从编码器更新电机速度
  * @param  无
  * @retval 无
  */
/*
void Encoder_Update(Encoder * enco)
{    
	//计算原始速度
	enco->encoder_LU = -(float)(Encoder_Get_Speed(Read_Encoder(2)));
	enco->encoder_RU = (float)(Encoder_Get_Speed(Read_Encoder(3)));
	enco->encoder_LD = -(float)(Encoder_Get_Speed(Read_Encoder(4)));
	enco->encoder_RD = (float)(Encoder_Get_Speed(Read_Encoder(5)));
	
}
*/

/**
  * @brief  从编码器更新电机速度
  * @param  无
  * @retval 无
  */
void Encoder_Update(Encoder * enco)
{    
	//计算原始速度
	enco->encoder_LU = -Read_Encoder(2);
	enco->encoder_RU = Read_Encoder(3);
	enco->encoder_LD = -Read_Encoder(4);
	enco->encoder_RD = Read_Encoder(5);
	
}









