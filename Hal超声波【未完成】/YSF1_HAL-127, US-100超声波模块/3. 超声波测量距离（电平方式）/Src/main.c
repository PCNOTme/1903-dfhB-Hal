/**
  ******************************************************************************
  * 文件名程: main.c 
  * 作    者: 硬石嵌入式开发团队
  * 版    本: V1.0
  * 编写日期: 2015-10-04
  * 功    能: 超声波测量距离（电平方式）
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
#include "GeneralTIM/bsp_GeneralTIM.h"
#include "led/bsp_led.h"
#include "usart/bsp_debug_usart.h"

/* 私有类型定义 --------------------------------------------------------------*/
/* 私有宏定义 ----------------------------------------------------------------*/
/* 私有变量 ------------------------------------------------------------------*/
STRUCT_CAPTURE strCapture = { 0, 0, 0 };

/* 扩展变量 ------------------------------------------------------------------*/
/* 私有函数原形 --------------------------------------------------------------*/
//static void US100_DelayUS ( __IO uint32_t ulCount );
void HcSr04_Delay(__IO uint32_t i);

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
  uint32_t temp;
  uint32_t ulTmrClk, ulTime;
  
  /* 复位所有外设，初始化Flash接口和系统滴答定时器 */
  HAL_Init();
  /* 配置系统时钟 */
  SystemClock_Config();

  /* LED初始化 */
//  LED_GPIO_Init();
  
  /* 初始化串口并配置串口中断优先级 */
  MX_DEBUG_USART_Init();
  
  /* 通用定时器初始化并配置输入捕获功能 */
  GENERAL_TIMx_Init();
  TROPIN_GPIO_Init();
  

  /* 获取定时器时钟周期 */	
	ulTmrClk = HAL_RCC_GetHCLKFreq()/GENERAL_TIM_PRESCALER;    
  /* 启动定时器 */
  HAL_TIM_Base_Start_IT(&htimx);  
  /* 启动定时器通道输入捕获并开启中断 */
  HAL_TIM_IC_Start_IT(&htimx,TIM_CHANNEL_1);
   printf ( "STM32 输入捕获实验\n" );
	printf ( "按下KEY1，测试KEY1按下的时间\n" );
  /* 无限循环 */
  while (1)
  {
//		printf ( "ing\r\n" );
    HAL_GPIO_WritePin(GPIOC,GPIO_PIN_13,GPIO_PIN_SET);
//    US100_DelayUS(20);
		HcSr04_Delay(500);
    HAL_GPIO_WritePin(GPIOC,GPIO_PIN_13,GPIO_PIN_RESET);
  
    /* 完成测量高电平脉宽 */
    if(strCapture.ucFinishFlag == 1 )
		{
      /* 计算高电平计数值 */
			ulTime = strCapture .usPeriod * GENERAL_TIM_PERIOD + strCapture .usCtr;
      temp=((ulTime % ulTmrClk)*340)/(1000*2);
      printf ( ">>测得距离为：%dmm\n",temp); 
      strCapture .ucFinishFlag = 0;		
      HAL_Delay(1000);       
		}
  }
}

/**
  * 函数功能: 非阻塞模式下定时器的回调函数
  * 输入参数: htim：定时器句柄
  * 返 回 值: 无
  * 说    明: 无
  */
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
  strCapture .usPeriod ++;
}

/**
  * 函数功能: 定时器输入捕获中断回调函数
  * 输入参数: htim：定时器句柄
  * 返 回 值: 无
  * 说    明: 无
  */
void HAL_TIM_IC_CaptureCallback(TIM_HandleTypeDef *htim)
{
  TIM_IC_InitTypeDef sConfigIC;
  
  if ( strCapture .ucStartFlag == 0 )
  {
//    LED1_TOGGLE;       
    
    htimx.Instance->CNT=0; // 清零定时器计数
    strCapture .usPeriod = 0;			
    strCapture .usCtr = 0;
    
    // 配置输入捕获参数，主要是修改触发电平
    sConfigIC.ICPolarity = GENERAL_TIM_END_ICPolarity;
    sConfigIC.ICSelection = TIM_ICSELECTION_DIRECTTI;
    sConfigIC.ICPrescaler = TIM_ICPSC_DIV1;
    sConfigIC.ICFilter = 0;
    HAL_TIM_IC_ConfigChannel(&htimx, &sConfigIC, TIM_CHANNEL_1);
    // 清除中断标志位
    __HAL_TIM_CLEAR_IT(htim, TIM_IT_CC1);
    // 启动输入捕获并开启中断
    HAL_TIM_IC_Start_IT(&htimx,TIM_CHANNEL_1);    
    strCapture .ucStartFlag = 1;			
  }		
  
  else
  {
//    LED1_TOGGLE;
    
    // 获取定时器计数值
    strCapture .usCtr = HAL_TIM_ReadCapturedValue(&htimx,TIM_CHANNEL_1);
    // 配置输入捕获参数，主要是修改触发电平
    sConfigIC.ICPolarity = GENERAL_TIM_STRAT_ICPolarity;
    sConfigIC.ICSelection = TIM_ICSELECTION_DIRECTTI;
    sConfigIC.ICPrescaler = TIM_ICPSC_DIV1;
    sConfigIC.ICFilter = 0;
    HAL_TIM_IC_ConfigChannel(&htimx, &sConfigIC, TIM_CHANNEL_1);
    
    // 清除中断标志位
    __HAL_TIM_CLEAR_IT(htim, TIM_IT_CC1); 
    // 启动输入捕获并开启中断
    HAL_TIM_IC_Start_IT(&htimx,TIM_CHANNEL_1);    
    strCapture .ucStartFlag = 0;			
    strCapture .ucFinishFlag = 1;    
  }
 // HAL_Delay(1000);
}

//static void US100_DelayUS ( __IO uint32_t ulCount )
//{
//	uint32_t i;
//	for ( i = 0; i < ulCount; i ++ )
//	{
//		uint8_t uc = 12;     //设置值为12，大约延1微秒  	      
//		while ( uc -- );     //延1微秒	
//	}	
//}
void HcSr04_Delay(__IO uint32_t i){
	
	while(i--);
}
/******************* (C) COPYRIGHT 2015-2020 硬石嵌入式开发团队 *****END OF FILE****/
