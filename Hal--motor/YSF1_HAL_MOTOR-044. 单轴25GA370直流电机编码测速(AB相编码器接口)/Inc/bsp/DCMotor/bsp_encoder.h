#ifndef __ENCODER_TIM_H__
#define __ENCODER_TIM_H__

/* 包含头文件 ----------------------------------------------------------------*/
#include "stm32f1xx_hal.h"

/* 类型定义 ------------------------------------------------------------------*/
/* 宏定义 --------------------------------------------------------------------*/
#define ENCODER_TIMx                        TIM2
#define ENCODER_TIM_RCC_CLK_ENABLE()        __HAL_RCC_TIM2_CLK_ENABLE()
#define ENCODER_TIM_RCC_CLK_DISABLE()       __HAL_RCC_TIM2_CLK_DISABLE()

#define ENCODER_TIM_GPIO_CLK_ENABLE()       __HAL_RCC_GPIOA_CLK_ENABLE()
#define ENCODER_TIM_CH1_PIN                 GPIO_PIN_0
#define ENCODER_TIM_CH1_GPIO                GPIOA
#define ENCODER_TIM_CH2_PIN                 GPIO_PIN_1
#define ENCODER_TIM_CH2_GPIO                GPIOA

#define TIM_ENCODERMODE_TIx                 TIM_ENCODERMODE_TI2


// 定义定时器预分频，定时器实际时钟频率为：72MHz/（ENCODER_TIMx_PRESCALER+1）
#define ENCODER_TIM_PRESCALER               0  // 实际时钟频率为：36kHz

// 定义定时器周期，当定时器开始计数到ENCODER_TIMx_PERIOD值是更新定时器并生成对应事件和中断
#define ENCODER_TIM_PERIOD                  0xFFFF

/* 扩展变量 ------------------------------------------------------------------*/
extern TIM_HandleTypeDef htimx_Encoder;

/* 函数声明 ------------------------------------------------------------------*/
void ENCODER_TIMx_Init(void);

#endif	/* __ENCODER_TIM_H__ */
/******************* (C) COPYRIGHT 2015-2020 硬石嵌入式开发团队 *****END OF FILE****/
