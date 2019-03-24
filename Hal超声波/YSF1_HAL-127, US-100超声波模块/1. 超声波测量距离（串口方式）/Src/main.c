/**
  ******************************************************************************
  * 文件名程: main.c 
  * 作    者: 硬石嵌入式开发团队
  * 版    本: V1.0
  * 编写日期: 2015-10-04
  * 功    能: 在LCD显示超声波测量距离值（串口方式）
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
#include "usart\bsp_debug_usart.h"
#include "lcd\bsp_lcd.h"
#include "spiflash\bsp_spiflash.h"
#include "usart/bsp_usartx.h"
#include "stdlib.h"

/* 私有类型定义 --------------------------------------------------------------*/
/* 私有宏定义 ----------------------------------------------------------------*/
/* 私有变量 ------------------------------------------------------------------*/
uint8_t aTxBuffer[]={0x55};
uint8_t aRxBuffer[2];
char    str1[50];

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
  uint32_t lcdid;
  /* 复位所有外设，初始化Flash接口和系统滴答定时器 */
  HAL_Init();
  /* 配置系统时钟 */
  SystemClock_Config();
  
  /* 初始化3.5寸TFT液晶模组，一般优先于调试串口初始化 */
  lcdid=BSP_LCD_Init(); 
  
  MX_USARTx_Init();
  /* 初始化串口并配置串口中断优先级 */
  MX_DEBUG_USART_Init();
  
  MX_SPIFlash_Init();

  /* 调用格式化输出函数打印输出数据 */
  printf("LCD ID=0x%08X\n",lcdid);
    
  LCD_Clear(0,0,LCD_DEFAULT_WIDTH,LCD_DEFAULT_HEIGTH,BLACK);
  /* 开背光 */
  LCD_BK_ON();
  LCD_DispString_CH(40,100,(uint8_t *)"超声波距离测量",BLACK,MAGENTA,USB_FONT_24);
  /* 启用串口接收监听，有数据则进入中断回调 */
  HAL_UART_Receive_IT(&husartx,&aRxBuffer[0],2);
 
  /* 无限循环 */
  while (1)
  {  
    LCD_Clear(200,200,120,24,BLACK);
    HAL_UART_Transmit(&husartx,&aTxBuffer[0],1,1);
    HAL_Delay(1000);
  }
}

/**
  * 函数功能: 接收中断回调函数
  * 输入参数: 无
  * 返 回 值: 无
  * 说    明: 无
  */
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *UartHandle)
{
    uint16_t temp;
    temp=aRxBuffer[0]*256+aRxBuffer[1];
    printf("测量的距离为:%dmm\n",temp); 
  
    sprintf(str1,"测量的距离为:%dmm",temp);
    LCD_DispString_EN_CH(30,200,(uint8_t *)str1,BLACK,DARKRED,USB_FONT_24);
  
    HAL_UART_Receive_IT(&husartx,&aRxBuffer[0],2);
  
}
/******************* (C) COPYRIGHT 2015-2020 硬石嵌入式开发团队 *****END OF FILE****/
