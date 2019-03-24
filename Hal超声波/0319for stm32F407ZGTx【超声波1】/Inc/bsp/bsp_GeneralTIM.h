#ifndef __GENERAL_TIM_H__
#define __GENERAL_TIM_H__

/* 包含头文件 ----------------------------------------------------------------*/
#include "stm32f4xx_hal.h"

/* 类型定义 ------------------------------------------------------------------*/
/************************** 输入捕获外部变量 ********************************/
typedef struct              //测量高电平脉宽
{   
	uint8_t   ucFinishFlag;
	uint8_t   ucStartFlag;
	uint16_t  usCtr;
	uint16_t  usPeriod;
}STRUCT_CAPTURE;

/* 宏定义 --------------------------------------------------------------------*/
#define GENERAL_TIMx                        TIM5
#define GENERAL_TIM_RCC_CLK_ENABLE()        __HAL_RCC_TIM5_CLK_ENABLE()
#define GENERAL_TIM_RCC_CLK_DISABLE()       __HAL_RCC_TIM5_CLK_DISABLE()
#define GENERAL_TIM_GPIO_RCC_CLK_ENABLE()   __HAL_RCC_GPIOA_CLK_ENABLE();


#define GENERAL_TIM_IRQn                    TIM5_IRQn
#define GENERAL_TIM_IRQHANDLER              TIM5_IRQHandler

#define KEY1_RCC_CLK_ENABLE()               __HAL_RCC_GPIOA_CLK_ENABLE()
#define KEY1_GPIO_PIN                       GPIO_PIN_0
#define KEY1_GPIO                           GPIOA

// 定义定时器预分频，定时器实际时钟频率为：72MHz/（GENERAL_TIMx_PRESCALER+1）
#define GENERAL_TIM_PRESCALER               167  // 实际时钟频率为：36kHz

// 定义定时器周期，当定时器开始计数到GENERAL_TIMx_PERIOD值是更新定时器并生成对应事件和中断
#define GENERAL_TIM_PERIOD                  0xFFFF

#define GENERAL_TIM_STRAT_ICPolarity        TIM_INPUTCHANNELPOLARITY_RISING          //测量的起始边沿
#define GENERAL_TIM_END_ICPolarity          TIM_INPUTCHANNELPOLARITY_FALLING         //测量的结束边沿

/* 扩展变量 ------------------------------------------------------------------*/
extern TIM_HandleTypeDef htimx;
extern uint8_t indexWave[];

/* 函数声明 ------------------------------------------------------------------*/
void GENERAL_TIMx_Init(void);
void TROPIN_GPIO_Init(void);


#endif	/* __GENERAL_TIM_H__ */
/******************* (C) COPYRIGHT 2015-2020 硬石嵌入式开发团队 *****END OF FILE****/
