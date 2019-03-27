/**
  ******************************************************************************
  * �ļ�����: bsp_EncoderTIM.c 
  * ��    ��: ӲʯǶ��ʽ�����Ŷ�
  * ��    ��: V1.0
  * ��д����: 2015-10-04
  * ��    ��: ������
  ******************************************************************************
  * ˵����
  * ����������Ӳʯstm32������YS-F1Proʹ�á�
  * 
  * �Ա���
  * ��̳��http://www.ing10bbs.com
  * ��Ȩ��ӲʯǶ��ʽ�����Ŷ����У��������á�
  ******************************************************************************
  */
/* ����ͷ�ļ� ----------------------------------------------------------------*/
#include "DCMotor/bsp_encoder.h"

/* ˽�����Ͷ��� --------------------------------------------------------------*/
/* ˽�к궨�� ----------------------------------------------------------------*/
/* ˽�б��� ------------------------------------------------------------------*/
/* Timer handler declaration */
TIM_HandleTypeDef    htimx_Encoder;

/* Timer Encoder Configuration Structure declaration */
TIM_Encoder_InitTypeDef sEncoderConfig;

/* ��չ���� ------------------------------------------------------------------*/
/* ˽�к���ԭ�� --------------------------------------------------------------*/
/* ������ --------------------------------------------------------------------*/
/**
  * ��������: ͨ�ö�ʱ����ʼ��������ͨ��PWM���
  * �������: ��
  * �� �� ֵ: ��
  * ˵    ��: ��
  */
void ENCODER_TIMx_Init(void)
{    
  htimx_Encoder.Instance = ENCODER_TIMx;
  htimx_Encoder.Init.Prescaler = ENCODER_TIM_PRESCALER;
  htimx_Encoder.Init.CounterMode = TIM_COUNTERMODE_UP;
  htimx_Encoder.Init.Period = ENCODER_TIM_PERIOD;
  htimx_Encoder.Init.ClockDivision=TIM_CLOCKDIVISION_DIV1;
  //HAL_TIM_Base_Init(&htimx_Encoder);
  
  sEncoderConfig.EncoderMode        = TIM_ENCODERMODE_TIx;  
  
  sEncoderConfig.IC1Polarity        = TIM_ICPOLARITY_RISING;   
  sEncoderConfig.IC1Selection       = TIM_ICSELECTION_DIRECTTI;  
  sEncoderConfig.IC1Prescaler       = TIM_ICPSC_DIV1; 
  sEncoderConfig.IC1Filter          = 0;
  
  sEncoderConfig.IC2Polarity        = TIM_ICPOLARITY_RISING;   
  sEncoderConfig.IC2Selection       = TIM_ICSELECTION_DIRECTTI;  
  sEncoderConfig.IC2Prescaler       = TIM_ICPSC_DIV1; 
  sEncoderConfig.IC2Filter          = 0;
  HAL_TIM_Encoder_Init(&htimx_Encoder, &sEncoderConfig);
}

/**
  * ��������: ������ʱ��Ӳ����ʼ������
  * �������: htim_base��������ʱ���������ָ��
  * �� �� ֵ: ��
  * ˵    ��: �ú�����HAL���ڲ�����
  */
void HAL_TIM_Encoder_MspInit(TIM_HandleTypeDef* htim_base)
{
  GPIO_InitTypeDef GPIO_InitStruct;
  if(htim_base->Instance==ENCODER_TIMx)
  {
    /* ������ʱ������ʱ��ʹ�� */
    ENCODER_TIM_RCC_CLK_ENABLE();
    ENCODER_TIM_GPIO_CLK_ENABLE();
    
    /* ��ʱ��ͨ��1��������IO��ʼ�� */
    GPIO_InitStruct.Pin = ENCODER_TIM_CH1_PIN;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull=GPIO_PULLUP;
    HAL_GPIO_Init(ENCODER_TIM_CH1_GPIO, &GPIO_InitStruct);
    
    /* ��ʱ��ͨ��1��������IO��ʼ�� */
    GPIO_InitStruct.Pin = ENCODER_TIM_CH2_PIN;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull=GPIO_PULLUP;
    HAL_GPIO_Init(ENCODER_TIM_CH2_GPIO, &GPIO_InitStruct);
  }
}

/**
  * ��������: ������ʱ��Ӳ������ʼ������
  * �������: htim_base��������ʱ���������ָ��
  * �� �� ֵ: ��
  * ˵    ��: �ú�����HAL���ڲ�����
  */
void HAL_TIM_Encoder_MspDeInit(TIM_HandleTypeDef* htim_base)
{
  if(htim_base->Instance==ENCODER_TIMx)
  {
    /* ������ʱ������ʱ�ӽ��� */
    ENCODER_TIM_RCC_CLK_DISABLE();
    
    HAL_GPIO_DeInit(ENCODER_TIM_CH1_GPIO, ENCODER_TIM_CH1_PIN);
    HAL_GPIO_DeInit(ENCODER_TIM_CH2_GPIO, ENCODER_TIM_CH2_PIN);
  }
} 
/******************* (C) COPYRIGHT 2015-2020 ӲʯǶ��ʽ�����Ŷ� *****END OF FILE****/
