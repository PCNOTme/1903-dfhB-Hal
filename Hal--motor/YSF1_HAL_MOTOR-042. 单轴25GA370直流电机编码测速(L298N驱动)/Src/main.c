/**
  ******************************************************************************
  * 文件名程: main.c 
  * 作    者: 硬石嵌入式开发团队
  * 版    本: V1.0
  * 编写日期: 2015-10-04
  * 功    能: 25GA370直流电机编码测速(L298N驱动)实现
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
#include "stm32f1xx_hal.h"
#include "DCMotor/bsp_L298N.h"
#include "key/bsp_key.h"
#include "usart/bsp_debug_usart.h"
#include "DCMotor/bsp_encoder.h"

/* 私有类型定义 --------------------------------------------------------------*/
/* 私有宏定义 ----------------------------------------------------------------*/
/* 私有变量 ------------------------------------------------------------------*/
__IO uint16_t time_count=0;        // 时间计数，每1ms增加一(与滴定时器频率有关)
__IO uint32_t CaptureNumber=0;     // 输入捕获数
__IO uint8_t  start_flag=0;
/* 扩展变量 ------------------------------------------------------------------*/
/* 私有函数原形 --------------------------------------------------------------*/
/* 函数体 --------------------------------------------------------------------*/
/**
  * 函数功能: 系统时钟配置
  * 输入参数: 无
  * 返 回 值: 无
  * 说    明: 无
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct;
  RCC_ClkInitTypeDef RCC_ClkInitStruct;

  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;  // 外部晶振，8MHz
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.HSEPredivValue = RCC_HSE_PREDIV_DIV1;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL9;  // 9倍频，得到72MHz主时钟
  HAL_RCC_OscConfig(&RCC_OscInitStruct);

  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;       // 系统时钟：72MHz
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;              // AHB时钟：72MHz
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;               // APB1时钟：36MHz
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;               // APB2时钟：72MHz
  HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2);

 	// HAL_RCC_GetHCLKFreq()/1000    1ms中断一次
	// HAL_RCC_GetHCLKFreq()/100000	 10us中断一次
	// HAL_RCC_GetHCLKFreq()/1000000 1us中断一次
  HAL_SYSTICK_Config(HAL_RCC_GetHCLKFreq()/1000);  // 配置并启动系统滴答定时器
  /* 系统滴答定时器时钟源 */
  HAL_SYSTICK_CLKSourceConfig(SYSTICK_CLKSOURCE_HCLK);
  /* 系统滴答定时器中断优先级配置 */
  HAL_NVIC_SetPriority(SysTick_IRQn, 0, 0);
}

/**
  * 函数功能: 主函数.
  * 输入参数: 无
  * 返 回 值: 无
  * 说    明: 无
  */
int main(void)
{ 
  /* 复位所有外设，初始化Flash接口和系统滴答定时器 */
  HAL_Init();
  /* 配置系统时钟 */
  SystemClock_Config();

  KEY_GPIO_Init();
  MX_DEBUG_USART_Init();
  
  ENCODER_TIMx_Init(); 
  HAL_TIM_Base_Start(&htimx_ENCODER);  
  
  /* 高级控制定时器初始化并配置PWM输出功能 */
  L298N_TIMx_Init();
  /* 启动定时器 */
  HAL_TIM_Base_Start(&htimx_L298N);
  
  HAL_TIM_IC_Start_IT(&htimx_ENCODER,ENCODER_TIM_CHANNELx);
  
  /* 启动定时器通道和互补通道PWM输出 */
  L298N_DCMOTOR_Contrl(1,1,PWM_Duty);
  start_flag=1; 
  
  /* 无限循环 */
  while (1)
  {
    if(KEY1_StateRead()==KEY_DOWN)  // 增速
    {
      PWM_Duty+=50;
      if(PWM_Duty>900)    // PWM_Duty=900已经对应100%占空比
        PWM_Duty=900;
      L298N_DCMOTOR_Contrl(1,1,PWM_Duty);      
    }
    if(KEY2_StateRead()==KEY_DOWN)  // 减速
    {
      PWM_Duty-=50;
      if(PWM_Duty<200)   // 最低速度保证，防止电机阻塞烧毁
        PWM_Duty=200;
      L298N_DCMOTOR_Contrl(1,1,PWM_Duty);
    }    
  }
}

/**
  * 函数功能: 系统滴答定时器中断回调函数
  * 输入参数: 无
  * 返 回 值: 无
  * 说    明: 每发生一次滴答定时器中断进入该回调函数一次
  */
void HAL_SYSTICK_Callback(void)
{
  if(start_flag) // 等待脉冲输出后才开始计时
  {
    time_count++;         // 每1ms自动增一
    if(time_count==1000)  // 1s
    {
      printf("输入捕获值：%d\n",CaptureNumber);
      // 11：编码器线数(转速一圈输出脉冲数)
      // 34：电机减数比，内部电机转动圈数与电机输出轴转动圈数比，即减速齿轮比
      printf("电机实际转动速度：%0.2f(圈/s)\n",(float)CaptureNumber/11/34);
      CaptureNumber=0;    // 重新开始计数
      time_count=0;
    }
  }
}

/**
  * 函数功能: 定时器输入捕获中断回调函数
  * 输入参数: htim：定时器句柄
  * 返 回 值: 无
  * 说    明: 无
  */
void HAL_TIM_IC_CaptureCallback(TIM_HandleTypeDef *htim)
{
  CaptureNumber++;
}
/******************* (C) COPYRIGHT 2015-2020 硬石嵌入式开发团队 *****END OF FILE****/
