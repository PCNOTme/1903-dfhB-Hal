
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  ** This notice applies to any and all portions of this file
  * that are not between comment pairs USER CODE BEGIN and
  * USER CODE END. Other portions of this file, whether 
  * inserted by the user or by software development tools
  * are owned by their respective copyright owners.
  *
  * COPYRIGHT(c) 2019 STMicroelectronics
  *
  * Redistribution and use in source and binary forms, with or without modification,
  * are permitted provided that the following conditions are met:
  *   1. Redistributions of source code must retain the above copyright notice,
  *      this list of conditions and the following disclaimer.
  *   2. Redistributions in binary form must reproduce the above copyright notice,
  *      this list of conditions and the following disclaimer in the documentation
  *      and/or other materials provided with the distribution.
  *   3. Neither the name of STMicroelectronics nor the names of its contributors
  *      may be used to endorse or promote products derived from this software
  *      without specific prior written permission.
  *
  * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
  * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
  * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
  * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
  * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
  * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
  * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
  * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
  * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
  * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
  *
  ******************************************************************************
  */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "stm32f4xx_hal.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"

/* USER CODE BEGIN Includes */
#define Echo1_Trig_GPIO_PIN                       GPIO_PIN_11
#define Echo1_Trig_GPIO                           GPIOD

#define  P_DATA      4                                 //P����
#define  I_DATA      0                                //I����
#define  D_DATA     -1                              //D����

#include "stdio.h"
#ifdef __GNUC__
#define PUTCHAR_PROTOTYPE int __io_putchar(int ch)
#else
#define PUTCHAR_PROTOTYPE int fputc(int ch, FILE *f)
#endif /* __GNUC__ */
PUTCHAR_PROTOTYPE
{
	HAL_UART_Transmit(&huart1 , (uint8_t *)&ch, 1, 0xFFFF);
	return ch;
}
/* USER CODE END Includes */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */
/* Private variables ---------------------------------------------------------*/

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);

/* USER CODE BEGIN PFP */
/* Private function prototypes -----------------------------------------------*/

typedef struct              //�����ߵ�ƽ����
{   
	uint8_t   ucFinishFlag;
	uint8_t   ucStartFlag;
	uint16_t  usCtr;
	uint16_t  usPeriod;
}STRUCT_CAPTURE;

STRUCT_CAPTURE strCapture = { 0, 0, 0 };

typedef struct 
{
   __IO int      SetPoint;                                 //�趨Ŀ�� Desired Value
   __IO double   Proportion;                               //�������� Proportional Const
   __IO double   Integral;                                 //���ֳ��� Integral Const
   __IO double   Derivative;                               //΢�ֳ��� Derivative Const
   __IO int      LastError;                                //Error[-1]
   __IO int      PrevError;                                //Error[-2]
}PID;

__IO uint16_t time_count=0;        // ʱ�������ÿ1ms����һ(��ζ�ʱ��Ƶ���й�)
__IO uint32_t CaptureNumber=0;     // ���벶����
__IO uint8_t  start_flag=0;
__IO double encoder_speed=0;
__IO uint16_t PWM_Duty;
;
static PID sPID;
static PID *sptr = &sPID;
/* USER CODE END PFP */

/* USER CODE BEGIN 0 */
void IncPIDInit(void);
int IncPIDCalc(int NextPoint);
void HcSr04_Delay(__IO uint32_t i);
/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  *
  * @retval None
  */
int main(void)
{
  /* USER CODE BEGIN 1 */
	uint32_t temp;
  uint32_t ulTmrClk, ulTime;
  /* USER CODE END 1 */

  /* MCU Configuration----------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_TIM1_Init();
  MX_TIM3_Init();
  MX_USART1_UART_Init();
  MX_TIM4_Init();
  /* USER CODE BEGIN 2 */

	IncPIDInit();
	/* ������ʱ�� */
	HAL_TIM_Base_Start(&htim1);
//	HAL_TIM_Base_Start(&htim3);
	HAL_TIM_Base_Start(&htim4);
	  /* ������ʱ��ͨ���ͻ���ͨ��PWM��� */
  HAL_TIM_PWM_Start(&htim1,TIM_CHANNEL_1);
  HAL_TIMEx_PWMN_Start(&htim1,TIM_CHANNEL_1);	

//  HAL_TIM_IC_Start_IT(&htim3,TIM_CHANNEL_1);
	HAL_TIM_IC_Start_IT(&htim4,TIM_CHANNEL_1);
	
	start_flag=1; 
	printf("����ʽPID�㷨���Ƶ����ת\r\n");
	printf("���������벶�����\r\n");
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
	sptr->SetPoint=10000;           //�趨Ŀ��Desired Value
	  /* ��ȡ��ʱ��ʱ������ */	
	ulTmrClk = HAL_RCC_GetHCLKFreq()/16;   
  while (1)
  {
	
  /* USER CODE END WHILE */

  /* USER CODE BEGIN 3 */
	  HAL_GPIO_WritePin(Echo1_Trig_GPIO,Echo1_Trig_GPIO_PIN,GPIO_PIN_SET);
		HAL_Delay(1);  
    HAL_GPIO_WritePin(Echo1_Trig_GPIO,Echo1_Trig_GPIO_PIN,GPIO_PIN_RESET);
//		printf("Triging����\r\n");
//		HAL_Delay(2000);   
  
    /* ��ɲ����ߵ�ƽ���� */
    if(strCapture.ucFinishFlag == 1 )
		{
      /* ����ߵ�ƽ����ֵ */
			ulTime = strCapture .usPeriod * 0xFFFF + strCapture .usCtr;
      temp=((ulTime % ulTmrClk)*340)/(1000*2);
      printf ( ">>��þ���Ϊ��%dmm\r\n",temp); 
      strCapture .ucFinishFlag = 0;		
      HAL_Delay(200);       
		}
  }
  /* USER CODE END 3 */

}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{

  RCC_OscInitTypeDef RCC_OscInitStruct;
  RCC_ClkInitTypeDef RCC_ClkInitStruct;

    /**Configure the main internal regulator output voltage 
    */
  __HAL_RCC_PWR_CLK_ENABLE();

  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

    /**Initializes the CPU, AHB and APB busses clocks 
    */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 4;
  RCC_OscInitStruct.PLL.PLLN = 168;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 4;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

    /**Initializes the CPU, AHB and APB busses clocks 
    */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_5) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

    /**Configure the Systick interrupt time 
    */
  HAL_SYSTICK_Config(HAL_RCC_GetHCLKFreq()/1000);

    /**Configure the Systick 
    */
  HAL_SYSTICK_CLKSourceConfig(SYSTICK_CLKSOURCE_HCLK);

  /* SysTick_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(SysTick_IRQn, 0, 0);
}

/* USER CODE BEGIN 4 */

/**
  * ��������: ϵͳ�δ�ʱ���жϻص�����
  * �������: ��
  * �� �� ֵ: ��
  * ˵    ��: ÿ����һ�εδ�ʱ���жϽ���ûص�����һ��
  */
void HAL_SYSTICK_Callback(void)
{
  if(start_flag) // �ȴ����������ſ�ʼ��ʱ
  {
    time_count++;         // ÿ1ms�Զ���һ
    if(time_count==200)
    {
      __IO uint32_t count;
      __IO int para;
      __IO double cal;
      
      /* �õ�����������ֵ����ֵԽ��˵���ٶ�Խ�� */
      count=CaptureNumber; 
      CaptureNumber=0;    // ���㣬���㿪ʼ����
      
      /* �����õ�����ʽPID��������ֵ */
      para=IncPIDCalc(count);
      
      /* ����������ֵ������ǰ����ٶ� */
      if((para<-3)||(para>3)) // ���� PID ��������������СʱƵ�����������𵴡�
      {
        PWM_Duty +=para/100;  
      }        
      if(PWM_Duty>899)PWM_Duty=899;  
      
      
      // 11������������(ת��һȦ���������)
      // 34����������ȣ��ڲ����ת��Ȧ�����������ת��Ȧ���ȣ������ٳ��ֱ�      
      cal=sptr->SetPoint;
			printf("\n��ֵ����ٶ�->	%d			Ŀ���ٶ�->	%d			����ٶ�->	%d			�µ�ռ�ձ�->	%d		     \r\n\r\n",count-sptr->SetPoint,sptr->SetPoint,count,PWM_Duty);     
//      printf("\n�趨Ŀ���ٶ� ->%d������     ",sptr->SetPoint);        
//      printf("��ǰ����ٶ�-> %d������\n     ",count);
//			printf("��ֵ����ٶ�-> %d������\n     ",count-sptr->SetPoint);     
//      printf("����ʽPID�㷨�������ֵ��%d   �����µ�ռ�ձ�Ϊ��%d\r\n",para,PWM_Duty);
//      
			__HAL_TIM_SET_COMPARE(&htim1,TIM_CHANNEL_1,PWM_Duty);
      time_count=0;      
    }
  }
}

void HAL_TIM_IC_CaptureCallback(TIM_HandleTypeDef *htim)
{
	if(htim == &(htim3))
	{
		CaptureNumber++;
	}
	
  if(htim == &(htim4))
	{
//			printf("Triging����\r\n");
		  TIM_IC_InitTypeDef sConfigIC;
		
			if ( strCapture .ucStartFlag == 0 )
			{
				htim4.Instance->CNT=0; // ���㶨ʱ������
				strCapture .usPeriod = 0;			
				strCapture .usCtr = 0;
				
				// �������벶���������Ҫ���޸Ĵ�����ƽ
				sConfigIC.ICPolarity = TIM_INPUTCHANNELPOLARITY_FALLING;
				sConfigIC.ICSelection = TIM_ICSELECTION_DIRECTTI;
				sConfigIC.ICPrescaler = TIM_ICPSC_DIV1;
				sConfigIC.ICFilter = 0;
				HAL_TIM_IC_ConfigChannel(&htim4, &sConfigIC, TIM_CHANNEL_1);
				// ����жϱ�־λ
				__HAL_TIM_CLEAR_IT(htim, TIM_IT_CC1);
				// �������벶�񲢿����ж�
				HAL_TIM_IC_Start_IT(&htim4,TIM_CHANNEL_1);    
				strCapture .ucStartFlag = 1;			
			}		
			
			else
			{  
				// ��ȡ��ʱ������ֵ
				strCapture .usCtr = HAL_TIM_ReadCapturedValue(&htim4,TIM_CHANNEL_1);
				// �������벶���������Ҫ���޸Ĵ�����ƽ
				sConfigIC.ICPolarity = TIM_INPUTCHANNELPOLARITY_RISING;
				sConfigIC.ICSelection = TIM_ICSELECTION_DIRECTTI;
				sConfigIC.ICPrescaler = TIM_ICPSC_DIV1;
				sConfigIC.ICFilter = 0;
				HAL_TIM_IC_ConfigChannel(&htim4, &sConfigIC, TIM_CHANNEL_1);
				
				// ����жϱ�־λ
				__HAL_TIM_CLEAR_IT(htim, TIM_IT_CC1); 
				// �������벶�񲢿����ж�
				HAL_TIM_IC_Start_IT(&htim4,TIM_CHANNEL_1);    
				strCapture .ucStartFlag = 0;			
				strCapture .ucFinishFlag = 1;    
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
	if(htim == &(htim4))
	{
		strCapture .usPeriod ++;
	} 
}


void HcSr04_Delay(__IO uint32_t i){
	
	while(i--);
}


/**************PID������ʼ��********************************/
void IncPIDInit(void) 
{
    sptr->LastError=0;            //Error[-1]
    sptr->PrevError=0;            //Error[-2]
    sptr->Proportion=P_DATA;      //�������� Proportional Const
    sptr->Integral=I_DATA;        //���ֳ���  Integral Const
    sptr->Derivative=D_DATA;      //΢�ֳ��� Derivative Const
    sptr->SetPoint=0;           //�趨Ŀ��Desired Value
}
/********************����ʽPID�������************************************/
int IncPIDCalc(int NextPoint) 
{
  int iError,iIncpid;                                 //��ǰ���
  iError=sptr->SetPoint-NextPoint;                    //��������
  iIncpid=(sptr->Proportion * iError)                 //E[k]��
              -(sptr->Integral * sptr->LastError)     //E[k-1]��
              +(sptr->Derivative * sptr->PrevError);  //E[k-2]��
              
  sptr->PrevError=sptr->LastError;                    //�洢�������´μ���
  sptr->LastError=iError;
  return(iIncpid);                                    //��������ֵ
}


/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @param  file: The file name as string.
  * @param  line: The line in file as a number.
  * @retval None
  */
void _Error_Handler(char *file, int line)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  while(1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t* file, uint32_t line)
{ 
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     tex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

/**
  * @}
  */

/**
  * @}
  */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
