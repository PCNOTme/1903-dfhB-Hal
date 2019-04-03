#ifndef __L298N_TIM_H__
#define __L298N_TIM_H__

/* 包含头文件 ----------------------------------------------------------------*/
#include "stm32f1xx_hal.h"

/* 类型定义 ------------------------------------------------------------------*/
/* 宏定义 --------------------------------------------------------------------*/

#define L298N_TIMx                         TIM1
#define L298N_TIM_RCC_CLK_ENABLE()         __HAL_RCC_TIM1_CLK_ENABLE()
#define L298N_TIM_RCC_CLK_DISABLE()        __HAL_RCC_TIM1_CLK_DISABLE()

/* 第 1 轴 */
#define L298N_TIM_CH1_GPIO_CLK_ENABLE()    __HAL_RCC_GPIOA_CLK_ENABLE()     // 输出PWM脉冲给电机控制器的的EN引脚
#define L298N_TIM_CH1_PORT                 GPIOA                            // CH1和CH1N两个引脚配套使用
#define L298N_TIM_CH1_PIN                  GPIO_PIN_8                       // 如果电机接在驱动器的OUT1和OUT2端子上
#define L298N_TIM_CH1N_GPIO_CLK_ENABLE()   __HAL_RCC_GPIOB_CLK_ENABLE()     // CH1和CH1N对应接在EN1和EN2
#define L298N_TIM_CH1N_PORT                GPIOB                            // 如果电机接在驱动器的OUT3和OUT4端子上
#define L298N_TIM_CH1N_PIN                 GPIO_PIN_13                      // CH1和CH1N对应接在EN3和EN4




// 定义定时器预分频，定时器实际时钟频率为：72MHz/（L298N_TIMx_PRESCALER+1）
#define L298N_TIM_PRESCALER                1    // 实际时钟频率为：36MHz

// 定义定时器周期，PWM频率为：72MHz/（L298N_TIMx_PRESCALER+1）/（L298N_TIM_PERIOD+1）
#define L298N_TIM_PERIOD                   900  // PWM频率为36MHz/(900+1)=40KHz

// 定义高级定时器重复计数寄存器值
//实际PWM频率为：72MHz/（L298N_TIMx_PRESCALER+1）/（L298N_TIM_PERIOD+1）/（L298N_TIM_REPETITIONCOUNTER+1）
#define L298N_TIM_REPETITIONCOUNTER        0

/* 扩展变量 ------------------------------------------------------------------*/
extern TIM_HandleTypeDef htimx_L298N;
extern __IO uint16_t PWM_Duty;
/* 函数声明 ------------------------------------------------------------------*/

void L298N_TIMx_Init(void);
void L298N_DCMOTOR_Contrl(uint8_t number,uint8_t direction,uint16_t speed);

#endif	/* __L298N_TIM_H__ */
/******************* (C) COPYRIGHT 2015-2020 硬石嵌入式开发团队 *****END OF FILE****/
