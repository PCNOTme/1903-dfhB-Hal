/**
  ******************************************************************************
  * 文件名程: main.c 
  * 作    者: 硬石嵌入式开发团队
  * 版    本: V1.0
  * 编写日期: 2015-10-04
  * 功    能: YSF1_HAL-125. 超声波测量距离和温度（串口方式）
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
#include "stm32f1xx_it.h"
#include "usart/bsp_debug_usart.h"
#include "led/bsp_led.h"
#include "key/bsp_key.h"
#include "usart/bsp_usartx_US100.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
/* 私有类型定义 --------------------------------------------------------------*/
/* 私有宏定义 ----------------------------------------------------------------*/
/* 私有变量 ------------------------------------------------------------------*/
uint8_t aTxBuffer1[]={0x55};
uint8_t aTxBuffer2[]={0x50};
uint8_t aRxBuffer1[2];
uint8_t flag1=0;
uint8_t flag2=0;

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
    /* 板子按键初始化 */
  KEY_GPIO_Init();
  MX_USARTx_Init();
  /* 初始化串口并配置串口中断优先级 */
  MX_DEBUG_USART_Init();
  

  /* 调用格式化输出函数打印输出数据 */
  printf("LCD ID=0x%08X\n",lcdid);

  /* 启用串口接收监听，有数据则进入中断回调 */
  HAL_UART_Receive_IT(&husartx,&aRxBuffer1[0],2);
  /* 无限循环 */
  while (1)
  {  
    if( HAL_UART_Transmit(&husartx,&aTxBuffer1[0],1,0xFFFF)==HAL_OK);
    {
       flag1=1;
        HAL_Delay(1000);
    }     
       
    if( HAL_UART_Transmit(&husartx,&aTxBuffer2[0],1,0xFFFF)==HAL_OK);
    {
       flag2=1;
        HAL_Delay(1000);
    }      
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
 if (flag1==1)
 {
    uint16_t temp; 
    temp=aRxBuffer1[0]*256+aRxBuffer1[1];
    printf("测量的距离为:%dmm\n",temp); 
    flag1=0;
    HAL_UART_Receive_IT(&husartx,&aRxBuffer1[0],1);
  
 }

 if(flag2==1)
 {
   uint8_t wendu;
   wendu=aRxBuffer1[0]-45;
   printf("测量的温度为：%d℃\n",wendu);
   flag2=0; 
   HAL_UART_Receive_IT(&husartx,&aRxBuffer1[0],2);
 }
}

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
