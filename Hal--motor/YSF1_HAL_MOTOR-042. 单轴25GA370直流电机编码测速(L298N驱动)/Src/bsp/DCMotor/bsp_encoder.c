/**
  ******************************************************************************
  * 文件名程: bsp_encoder.c 
  * 作    者: 硬石嵌入式开发团队
  * 版    本: V1.0
  * 编写日期: 2015-10-04
  * 功    能: 25GA370直流减速电机编码器底层驱动
  ******************************************************************************
  * 说明：
  * 本例程配套硬石stm32开发板YS-F1Pro使用。
  * 
  * 淘宝：
  * 论坛：http://www.ing10bbs.com
  * 版权归硬石嵌入式开发团队所有，请勿商用。
  ******************************************************************************
  */
/* 包含头文件 ----------------------------------------------------------------*/
#include "DCMotor/bsp_encoder.h" 

/* 私有类型定义 --------------------------------------------------------------*/
/* 私有宏定义 ----------------------------------------------------------------*/
/* 私有变量 ------------------------------------------------------------------*/
TIM_HandleTypeDef htimx_ENCODER;
/* 扩展变量 ------------------------------------------------------------------*/
/* 私有函数原形 --------------------------------------------------------------*/
/* 函数体 --------------------------------------------------------------------*/
/**
  * 函数功能: 编码器引脚初始化
  * 输入参数: 无
  * 返 回 值: 无
  * 说    明: 无
  */
static void ENCODER_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct; 
  
  /* 引脚端口时钟使能 */
  ENCODER_TIM_GPIO_CLK_ENABLE();

  /* 编码器输入捕获引脚IO初始化 */
  GPIO_InitStruct.Pin = ENCODER_TIM_PIN;
  GPIO_InitStruct.Mode = GPIO_MODE_AF_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(ENCODER_TIM_PORT, &GPIO_InitStruct);  
}

/**
  * 函数功能: 编码器引脚初始化
  * 输入参数: 无
  * 返 回 值: 无
  * 说    明: 无
  */
static void ENCODER_NVIC_Conf(void)
{
  /* 编码器定时器中断配置 */
  HAL_NVIC_SetPriority(ENCODER_TIMx_IRQn, 1, 0);
  HAL_NVIC_EnableIRQ(ENCODER_TIMx_IRQn);
}
/**
  * 函数功能: 编码器定时器初始化
  * 输入参数: 无
  * 返 回 值: 无
  * 说    明: 无
  */
void ENCODER_TIMx_Init(void)
{
  TIM_ClockConfigTypeDef sClockSourceConfig;  // 定时器时钟
  TIM_MasterConfigTypeDef sMasterConfig;      // 定时器主模式配置
  TIM_IC_InitTypeDef sConfigIC;               // 定时器输入捕获
  
  
  /* 使能定时器时钟 */
  ENCODER_TIM_RCC_CLK_ENABLE();

  /* 编码器引脚初始化 */
  ENCODER_GPIO_Init();
  
  /* 中断优先级配置 */
  ENCODER_NVIC_Conf();
  
  /* 定时器基本环境配置 */
  htimx_ENCODER.Instance = ENCODER_TIMx;                       // 定时器编号
  htimx_ENCODER.Init.Prescaler = ENCODER_TIM_PRESCALER;        // 定时器预分频器
  htimx_ENCODER.Init.CounterMode = TIM_COUNTERMODE_UP;         // 计数方向：向上计数
  htimx_ENCODER.Init.Period = ENCODER_TIM_PERIOD;              // 定时器周期
  htimx_ENCODER.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;   // 时钟分频
  HAL_TIM_Base_Init(&htimx_ENCODER);

  /* 定时器时钟源配置 */
  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;  // 使用内部时钟源
  HAL_TIM_ConfigClockSource(&htimx_ENCODER, &sClockSourceConfig);
  
  /* 初始化定时器输入捕获环境 */
  //HAL_TIM_IC_Init(&htimx_ENCODER);

  /* 定时器主输出模式 */
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  HAL_TIMEx_MasterConfigSynchronization(&htimx_ENCODER, &sMasterConfig);

  /* 定时器输入捕获配置 */
  sConfigIC.ICPolarity = TIM_INPUTCHANNELPOLARITY_RISING;   // 上升沿触发
  sConfigIC.ICSelection = TIM_ICSELECTION_DIRECTTI;         // 通道 
  sConfigIC.ICPrescaler = TIM_ICPSC_DIV1;                   // 分频
  sConfigIC.ICFilter = 0;                                   // 滤波器 
  HAL_TIM_IC_ConfigChannel(&htimx_ENCODER, &sConfigIC, ENCODER_TIM_CHANNELx);
}

/******************* (C) COPYRIGHT 2015-2020 硬石嵌入式开发团队 *****END OF FILE****/
