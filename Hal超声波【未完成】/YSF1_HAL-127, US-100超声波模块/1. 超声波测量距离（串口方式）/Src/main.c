/**
  ******************************************************************************
  * �ļ�����: main.c 
  * ��    ��: ӲʯǶ��ʽ�����Ŷ�
  * ��    ��: V1.0
  * ��д����: 2015-10-04
  * ��    ��: ��LCD��ʾ��������������ֵ�����ڷ�ʽ��
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
#include "stm32f1xx_hal.h"
#include "usart\bsp_debug_usart.h"
#include "lcd\bsp_lcd.h"
#include "spiflash\bsp_spiflash.h"
#include "usart/bsp_usartx.h"
#include "stdlib.h"

/* ˽�����Ͷ��� --------------------------------------------------------------*/
/* ˽�к궨�� ----------------------------------------------------------------*/
/* ˽�б��� ------------------------------------------------------------------*/
uint8_t aTxBuffer[]={0x55};
uint8_t aRxBuffer[2];
char    str1[50];

/* ��չ���� ------------------------------------------------------------------*/
/* ˽�к���ԭ�� --------------------------------------------------------------*/
/* ������ --------------------------------------------------------------------*/
/**
  * ��������: ϵͳʱ������
  * �������: ��
  * �� �� ֵ: ��
  * ˵    ��: ��
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct;
  RCC_ClkInitTypeDef RCC_ClkInitStruct;

  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;  // �ⲿ����8MHz
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.HSEPredivValue = RCC_HSE_PREDIV_DIV1;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL9;  // 9��Ƶ���õ�72MHz��ʱ��
  HAL_RCC_OscConfig(&RCC_OscInitStruct);

  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;       // ϵͳʱ�ӣ�72MHz
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;              // AHBʱ�ӣ�72MHz
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;               // APB1ʱ�ӣ�36MHz
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;               // APB2ʱ�ӣ�72MHz
  HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2);

 	// HAL_RCC_GetHCLKFreq()/1000    1ms�ж�һ��
	// HAL_RCC_GetHCLKFreq()/100000	 10us�ж�һ��
	// HAL_RCC_GetHCLKFreq()/1000000 1us�ж�һ��
  HAL_SYSTICK_Config(HAL_RCC_GetHCLKFreq()/1000);  // ���ò�����ϵͳ�δ�ʱ��
  /* ϵͳ�δ�ʱ��ʱ��Դ */
  HAL_SYSTICK_CLKSourceConfig(SYSTICK_CLKSOURCE_HCLK);
  /* ϵͳ�δ�ʱ���ж����ȼ����� */
  HAL_NVIC_SetPriority(SysTick_IRQn, 0, 0);
}

/**
  * ��������: ������.
  * �������: ��
  * �� �� ֵ: ��
  * ˵    ��: ��
  */
int main(void)
{  
  uint32_t lcdid;
  /* ��λ�������裬��ʼ��Flash�ӿں�ϵͳ�δ�ʱ�� */
  HAL_Init();
  /* ����ϵͳʱ�� */
  SystemClock_Config();
  
  /* ��ʼ��3.5��TFTҺ��ģ�飬һ�������ڵ��Դ��ڳ�ʼ�� */
  lcdid=BSP_LCD_Init(); 
  
  MX_USARTx_Init();
  /* ��ʼ�����ڲ����ô����ж����ȼ� */
  MX_DEBUG_USART_Init();
  
  MX_SPIFlash_Init();

  /* ���ø�ʽ�����������ӡ������� */
  printf("LCD ID=0x%08X\n",lcdid);
    
  LCD_Clear(0,0,LCD_DEFAULT_WIDTH,LCD_DEFAULT_HEIGTH,BLACK);
  /* ������ */
  LCD_BK_ON();
  LCD_DispString_CH(40,100,(uint8_t *)"�������������",BLACK,MAGENTA,USB_FONT_24);
  /* ���ô��ڽ��ռ�����������������жϻص� */
  HAL_UART_Receive_IT(&husartx,&aRxBuffer[0],2);
 
  /* ����ѭ�� */
  while (1)
  {  
    LCD_Clear(200,200,120,24,BLACK);
    HAL_UART_Transmit(&husartx,&aTxBuffer[0],1,1);
    HAL_Delay(1000);
  }
}

/**
  * ��������: �����жϻص�����
  * �������: ��
  * �� �� ֵ: ��
  * ˵    ��: ��
  */
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *UartHandle)
{
    uint16_t temp;
    temp=aRxBuffer[0]*256+aRxBuffer[1];
    printf("�����ľ���Ϊ:%dmm\n",temp); 
  
    sprintf(str1,"�����ľ���Ϊ:%dmm",temp);
    LCD_DispString_EN_CH(30,200,(uint8_t *)str1,BLACK,DARKRED,USB_FONT_24);
  
    HAL_UART_Receive_IT(&husartx,&aRxBuffer[0],2);
  
}
/******************* (C) COPYRIGHT 2015-2020 ӲʯǶ��ʽ�����Ŷ� *****END OF FILE****/
