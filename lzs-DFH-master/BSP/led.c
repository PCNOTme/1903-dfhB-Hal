#include "led.h"

/**
  * @brief  LED闪烁一次
  * @param  led闪烁间隔时间
  * @retval 无
  */
void LED_Flashing(int delay_ms)
{
 LED_On;
 HAL_Delay(delay_ms);
 LED_Off;
 HAL_Delay(delay_ms);
}

/**
  * @brief  LED快速闪烁
  * @param  led闪烁次数
  * @retval 无
  */
void LED_Fast_Flashing(int n)
{
	for(uint16_t i=1;i<=n;i++)
	{
		LED_On;
		HAL_Delay(100);
		LED_Off;
		HAL_Delay(100);
	}
}

/**
  * @brief  LED慢闪烁
  * @param  led闪烁次数
  * @retval 无
  */
void LED_Slow_Flashing(int n)
{
	for(uint16_t i=1;i<=n;i++)
	{
		LED_On;
		HAL_Delay(500);
		LED_Off;
		HAL_Delay(500);
	}
}



