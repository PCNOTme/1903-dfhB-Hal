#ifndef __ENCODER_TIM_H__
#define __ENCODER_TIM_H__

/* 包含头文件 ----------------------------------------------------------------*/
#include "stm32f1xx_hal.h"

/* 类型定义 ------------------------------------------------------------------*/
/* 宏定义 --------------------------------------------------------------------*/

#define ENCODER_TIMx                         TIM3
#define ENCODER_TIM_RCC_CLK_ENABLE()         __HAL_RCC_TIM3_CLK_ENABLE()
#define ENCODER_TIM_RCC_CLK_DISABLE()        __HAL_RCC_TIM3_CLK_DISABLE()

#define ENCODER_TIM_CHANNELx                 TIM_CHANNEL_3                    // 编码器输入捕获通道

#define ENCODER_TIM_GPIO_CLK_ENABLE()        __HAL_RCC_GPIOB_CLK_ENABLE()     // 输入捕获通道引脚
#define ENCODER_TIM_PORT                     GPIOB 
#define ENCODER_TIM_PIN                      GPIO_PIN_0


// 定义定时器预分频，定时器实际时钟频率为：72MHz/（ENCODER_TIMx_PRESCALER+1）
#define ENCODER_TIM_PRESCALER                71    // 实际时钟频率为：1MHz

// 定义定时器周期，输入捕获功能设置为：0xFFFF
#define ENCODER_TIM_PERIOD                   0xFFFF

#define ENCODER_TIMx_IRQn                    TIM3_IRQn
#define ENCODER_TIMx_IRQHandler              TIM3_IRQHandler
/* 扩展变量 ------------------------------------------------------------------*/
extern TIM_HandleTypeDef htimx_ENCODER;

/* 函数声明 ------------------------------------------------------------------*/

void ENCODER_TIMx_Init(void);

#endif	/* __ENCODER_TIM_H__ */
/******************* (C) COPYRIGHT 2015-2020 硬石嵌入式开发团队 *****END OF FILE****/
