/**
  ******************************************************************************
  * �ļ�����: main.c 
  * ��    ��: ӲʯǶ��ʽ�����Ŷ�
  * ��    ��: V1.0
  * ��д����: 2015-10-04
  * ��    ��: �������������루��ƽ��ʽ��
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
#include "GeneralTIM/bsp_GeneralTIM.h"
#include "led/bsp_led.h"
#include "usart/bsp_debug_usart.h"

/* ˽�����Ͷ��� --------------------------------------------------------------*/
/* ˽�к궨�� ----------------------------------------------------------------*/
/* ˽�б��� ------------------------------------------------------------------*/
STRUCT_CAPTURE strCapture = { 0, 0, 0 };

/* ��չ���� ------------------------------------------------------------------*/
/* ˽�к���ԭ�� --------------------------------------------------------------*/
//static void US100_DelayUS ( __IO uint32_t ulCount );
void HcSr04_Delay(__IO uint32_t i);

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
  uint32_t temp;
  uint32_t ulTmrClk, ulTime;
  
  /* ��λ�������裬��ʼ��Flash�ӿں�ϵͳ�δ�ʱ�� */
  HAL_Init();
  /* ����ϵͳʱ�� */
  SystemClock_Config();

  /* LED��ʼ�� */
//  LED_GPIO_Init();
  
  /* ��ʼ�����ڲ����ô����ж����ȼ� */
  MX_DEBUG_USART_Init();
  
  /* ͨ�ö�ʱ����ʼ�����������벶���� */
  GENERAL_TIMx_Init();
  TROPIN_GPIO_Init();
  

  /* ��ȡ��ʱ��ʱ������ */	
	ulTmrClk = HAL_RCC_GetHCLKFreq()/GENERAL_TIM_PRESCALER;    
  /* ������ʱ�� */
  HAL_TIM_Base_Start_IT(&htimx);  
  /* ������ʱ��ͨ�����벶�񲢿����ж� */
  HAL_TIM_IC_Start_IT(&htimx,TIM_CHANNEL_1);
   printf ( "STM32 ���벶��ʵ��\n" );
	printf ( "����KEY1������KEY1���µ�ʱ��\n" );
  /* ����ѭ�� */
  while (1)
  {
//		printf ( "ing\r\n" );
    HAL_GPIO_WritePin(GPIOC,GPIO_PIN_13,GPIO_PIN_SET);
//    US100_DelayUS(20);
		HcSr04_Delay(500);
    HAL_GPIO_WritePin(GPIOC,GPIO_PIN_13,GPIO_PIN_RESET);
  
    /* ��ɲ����ߵ�ƽ���� */
    if(strCapture.ucFinishFlag == 1 )
		{
      /* ����ߵ�ƽ����ֵ */
			ulTime = strCapture .usPeriod * GENERAL_TIM_PERIOD + strCapture .usCtr;
      temp=((ulTime % ulTmrClk)*340)/(1000*2);
      printf ( ">>��þ���Ϊ��%dmm\n",temp); 
      strCapture .ucFinishFlag = 0;		
      HAL_Delay(1000);       
		}
  }
}

/**
  * ��������: ������ģʽ�¶�ʱ���Ļص�����
  * �������: htim����ʱ�����
  * �� �� ֵ: ��
  * ˵    ��: ��
  */
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
  strCapture .usPeriod ++;
}

/**
  * ��������: ��ʱ�����벶���жϻص�����
  * �������: htim����ʱ�����
  * �� �� ֵ: ��
  * ˵    ��: ��
  */
void HAL_TIM_IC_CaptureCallback(TIM_HandleTypeDef *htim)
{
  TIM_IC_InitTypeDef sConfigIC;
  
  if ( strCapture .ucStartFlag == 0 )
  {
//    LED1_TOGGLE;       
    
    htimx.Instance->CNT=0; // ���㶨ʱ������
    strCapture .usPeriod = 0;			
    strCapture .usCtr = 0;
    
    // �������벶���������Ҫ���޸Ĵ�����ƽ
    sConfigIC.ICPolarity = GENERAL_TIM_END_ICPolarity;
    sConfigIC.ICSelection = TIM_ICSELECTION_DIRECTTI;
    sConfigIC.ICPrescaler = TIM_ICPSC_DIV1;
    sConfigIC.ICFilter = 0;
    HAL_TIM_IC_ConfigChannel(&htimx, &sConfigIC, TIM_CHANNEL_1);
    // ����жϱ�־λ
    __HAL_TIM_CLEAR_IT(htim, TIM_IT_CC1);
    // �������벶�񲢿����ж�
    HAL_TIM_IC_Start_IT(&htimx,TIM_CHANNEL_1);    
    strCapture .ucStartFlag = 1;			
  }		
  
  else
  {
//    LED1_TOGGLE;
    
    // ��ȡ��ʱ������ֵ
    strCapture .usCtr = HAL_TIM_ReadCapturedValue(&htimx,TIM_CHANNEL_1);
    // �������벶���������Ҫ���޸Ĵ�����ƽ
    sConfigIC.ICPolarity = GENERAL_TIM_STRAT_ICPolarity;
    sConfigIC.ICSelection = TIM_ICSELECTION_DIRECTTI;
    sConfigIC.ICPrescaler = TIM_ICPSC_DIV1;
    sConfigIC.ICFilter = 0;
    HAL_TIM_IC_ConfigChannel(&htimx, &sConfigIC, TIM_CHANNEL_1);
    
    // ����жϱ�־λ
    __HAL_TIM_CLEAR_IT(htim, TIM_IT_CC1); 
    // �������벶�񲢿����ж�
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
//		uint8_t uc = 12;     //����ֵΪ12����Լ��1΢��  	      
//		while ( uc -- );     //��1΢��	
//	}	
//}
void HcSr04_Delay(__IO uint32_t i){
	
	while(i--);
}
/******************* (C) COPYRIGHT 2015-2020 ӲʯǶ��ʽ�����Ŷ� *****END OF FILE****/
