/**
  ******************************************************************************
  * 文件名程: bsp_L298N.c 
  * 作    者: 硬石嵌入式开发团队
  * 版    本: V1.0
  * 编写日期: 2015-10-04
  * 功    能: 25GA370直流减速电机驱动L298N控制底层驱动
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
#include "DCMotor/bsp_L298N.h" 

/* 私有类型定义 --------------------------------------------------------------*/
/* 私有宏定义 ----------------------------------------------------------------*/
/* 私有变量 ------------------------------------------------------------------*/
TIM_HandleTypeDef htimx_L298N;
__IO uint16_t PWM_Duty=0;  // 占空比：PWM_Duty/L298N_TIM_PERIOD*100%
                             // 占空比为：50%时电机不转
                             // 占空比不为：50%时电机旋转，与50%的绝对差越大旋转速度也高
                             // 旋转方向不仅与程序有关，也与电机接线有关，需要具体分析
                             // 简单的方法是：如果控制方向与要求相反，调换两根PWM控制线接法
/* 扩展变量 ------------------------------------------------------------------*/
/* 私有函数原形 --------------------------------------------------------------*/
/* 函数体 --------------------------------------------------------------------*/
/**
  * 函数功能: L298N相关GPIO初始化配置
  * 输入参数: 无
  * 返 回 值: 无
  * 说    明: 无
  */
static void L298N_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct; 
  
  /* 引脚端口时钟使能 */
  L298N_TIM_CH1_GPIO_CLK_ENABLE();
  L298N_TIM_CH1N_GPIO_CLK_ENABLE();

/* 第 1 轴 */
  /* L298N输出脉冲控制引脚IO初始化 */
  GPIO_InitStruct.Pin = L298N_TIM_CH1_PIN;
  GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
  HAL_GPIO_Init(L298N_TIM_CH1_PORT, &GPIO_InitStruct);
  
  GPIO_InitStruct.Pin = L298N_TIM_CH1N_PIN;
  HAL_GPIO_Init(L298N_TIM_CH1N_PORT, &GPIO_InitStruct);
}

/**
  * 函数功能: L298N定时器初始化
  * 输入参数: 无
  * 返 回 值: 无
  * 说    明: 无
  */
void L298N_TIMx_Init(void)
{
  TIM_ClockConfigTypeDef sClockSourceConfig;             // 定时器时钟
  TIM_MasterConfigTypeDef sMasterConfig;                 // 定时器主模式配置
  TIM_BreakDeadTimeConfigTypeDef sBreakDeadTimeConfig;   // 刹车和死区时间配置
  TIM_OC_InitTypeDef sConfigOC;                          // 定时器通道比较输出
  
  /* 定时器基本环境配置 */
  htimx_L298N.Instance = L298N_TIMx;                                 // 定时器编号
  htimx_L298N.Init.Prescaler = L298N_TIM_PRESCALER;                  // 定时器预分频器
  htimx_L298N.Init.CounterMode = TIM_COUNTERMODE_UP;                  // 计数方向：向上计数
  htimx_L298N.Init.Period = L298N_TIM_PERIOD;                        // 定时器周期
  htimx_L298N.Init.ClockDivision=TIM_CLOCKDIVISION_DIV1;              // 时钟分频
  htimx_L298N.Init.RepetitionCounter = L298N_TIM_REPETITIONCOUNTER;  // 重复计数器
  HAL_TIM_Base_Init(&htimx_L298N);

  /* 定时器时钟源配置 */
  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;       // 使用内部时钟源
  HAL_TIM_ConfigClockSource(&htimx_L298N, &sClockSourceConfig);

  /* 初始化定时器比较输出环境 */
  HAL_TIM_PWM_Init(&htimx_L298N);
  
  /* 定时器主输出模式 */
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  HAL_TIMEx_MasterConfigSynchronization(&htimx_L298N, &sMasterConfig);
  
  /* 刹车和死区时间配置 */
  sBreakDeadTimeConfig.OffStateRunMode = TIM_OSSR_DISABLE;
  sBreakDeadTimeConfig.OffStateIDLEMode = TIM_OSSI_DISABLE;
  sBreakDeadTimeConfig.LockLevel = TIM_LOCKLEVEL_OFF;
  sBreakDeadTimeConfig.DeadTime = 0;
  sBreakDeadTimeConfig.BreakState = TIM_BREAK_DISABLE;
  sBreakDeadTimeConfig.BreakPolarity = TIM_BREAKPOLARITY_HIGH;
  sBreakDeadTimeConfig.AutomaticOutput = TIM_AUTOMATICOUTPUT_DISABLE;
  HAL_TIMEx_ConfigBreakDeadTime(&htimx_L298N, &sBreakDeadTimeConfig);

/* 第 1 轴 */
  /* 定时器比较输出配置 */
  sConfigOC.OCMode = TIM_OCMODE_PWM1;                  // 比较输出模式：反转输出
  sConfigOC.Pulse =  PWM_Duty;                         // 占空比
  sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;          // 输出极性
  sConfigOC.OCNPolarity = TIM_OCNPOLARITY_HIGH;        // 互补通道输出极性
  sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;           // 快速模式
  sConfigOC.OCIdleState = TIM_OCIDLESTATE_RESET;       // 空闲电平
  sConfigOC.OCNIdleState = TIM_OCNIDLESTATE_RESET;     // 互补通道空闲电平
  HAL_TIM_PWM_ConfigChannel(&htimx_L298N, &sConfigOC, TIM_CHANNEL_1);
  
  /* L298N相关GPIO初始化配置 */
  L298N_GPIO_Init();
}

/**
  * 函数功能: 基本定时器硬件初始化配置
  * 输入参数: htim_base：基本定时器句柄类型指针
  * 返 回 值: 无
  * 说    明: 该函数被HAL库内部调用
  */
void HAL_TIM_Base_MspInit(TIM_HandleTypeDef* htim_base)
{
  if(htim_base->Instance==L298N_TIMx)
  {
    /* 基本定时器外设时钟使能 */
    L298N_TIM_RCC_CLK_ENABLE();
  }
}

/**
  * 函数功能: 基本定时器硬件反初始化配置
  * 输入参数: htim_base：基本定时器句柄类型指针
  * 返 回 值: 无
  * 说    明: 该函数被HAL库内部调用
  */
void HAL_TIM_Base_MspDeInit(TIM_HandleTypeDef* htim_base)
{

  if(htim_base->Instance==L298N_TIMx)
  {
    /* 基本定时器外设时钟禁用 */
    L298N_TIM_RCC_CLK_DISABLE();
    
    HAL_GPIO_DeInit(L298N_TIM_CH1_PORT,L298N_TIM_CH1_PIN);
    HAL_GPIO_DeInit(L298N_TIM_CH1N_PORT,L298N_TIM_CH1N_PIN);
  }
} 

/**
  * 函数功能: L298N直流电机控制
  * 输入参数: number：电机编号，支持三个电机驱动
  *             参数：1：对应高级定时器通道1和互补通道1
  *                   2：电机停机
  *           direction：电机方向控制
  *             参数：0：目标电机停机
  *                   1：正转
  *                   2：反转
  *           speed：电机速度调节
  *             参数：0 - 900 ：值越大，速度越快
  * 返 回 值: 无
  * 说    明：无
  */
void L298N_DCMOTOR_Contrl(uint8_t number,uint8_t direction,uint16_t speed)
{
  switch (number)
  {
    case 1:
      if(direction==1)
      {
        HAL_TIM_PWM_Start(&htimx_L298N,TIM_CHANNEL_1);
        HAL_TIMEx_PWMN_Stop(&htimx_L298N,TIM_CHANNEL_1);
      }
      else if(direction==2)
      {
        HAL_TIM_PWM_Stop(&htimx_L298N,TIM_CHANNEL_1);
        HAL_TIMEx_PWMN_Start(&htimx_L298N,TIM_CHANNEL_1);
      }
      else
      {
        HAL_TIM_PWM_Stop(&htimx_L298N,TIM_CHANNEL_1);
        HAL_TIMEx_PWMN_Stop(&htimx_L298N,TIM_CHANNEL_1);
      }
      __HAL_TIM_SET_COMPARE(&htimx_L298N,TIM_CHANNEL_1,speed);;
      break;
   case 2:         // 停止输出PWM，停机
     HAL_TIM_PWM_Stop(&htimx_L298N,TIM_CHANNEL_1);
     HAL_TIMEx_PWMN_Stop(&htimx_L298N,TIM_CHANNEL_1); 
     break;
  }
}
/******************* (C) COPYRIGHT 2015-2020 硬石嵌入式开发团队 *****END OF FILE****/
