#ifndef __ENCODER_TIM_H__
#define __ENCODER_TIM_H__

/* ����ͷ�ļ� ----------------------------------------------------------------*/
#include "stm32f1xx_hal.h"

/* ���Ͷ��� ------------------------------------------------------------------*/
/* �궨�� --------------------------------------------------------------------*/
#define ENCODER_TIMx                        TIM2
#define ENCODER_TIM_RCC_CLK_ENABLE()        __HAL_RCC_TIM2_CLK_ENABLE()
#define ENCODER_TIM_RCC_CLK_DISABLE()       __HAL_RCC_TIM2_CLK_DISABLE()

#define ENCODER_TIM_GPIO_CLK_ENABLE()       __HAL_RCC_GPIOA_CLK_ENABLE()
#define ENCODER_TIM_CH1_PIN                 GPIO_PIN_0
#define ENCODER_TIM_CH1_GPIO                GPIOA
#define ENCODER_TIM_CH2_PIN                 GPIO_PIN_1
#define ENCODER_TIM_CH2_GPIO                GPIOA

#define TIM_ENCODERMODE_TIx                 TIM_ENCODERMODE_TI2


// ���嶨ʱ��Ԥ��Ƶ����ʱ��ʵ��ʱ��Ƶ��Ϊ��72MHz/��ENCODER_TIMx_PRESCALER+1��
#define ENCODER_TIM_PRESCALER               0  // ʵ��ʱ��Ƶ��Ϊ��36kHz

// ���嶨ʱ�����ڣ�����ʱ����ʼ������ENCODER_TIMx_PERIODֵ�Ǹ��¶�ʱ�������ɶ�Ӧ�¼����ж�
#define ENCODER_TIM_PERIOD                  0xFFFF

/* ��չ���� ------------------------------------------------------------------*/
extern TIM_HandleTypeDef htimx_Encoder;

/* �������� ------------------------------------------------------------------*/
void ENCODER_TIMx_Init(void);

#endif	/* __ENCODER_TIM_H__ */
/******************* (C) COPYRIGHT 2015-2020 ӲʯǶ��ʽ�����Ŷ� *****END OF FILE****/
