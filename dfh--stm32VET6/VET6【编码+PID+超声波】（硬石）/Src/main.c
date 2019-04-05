
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

#define  P_DATA      4                                 //P参数
#define  I_DATA      0                                //I参数
#define  D_DATA     -1                              //D参数

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

typedef struct              //测量高电平脉宽
{   
	uint8_t   ucFinishFlag;
	uint8_t   ucStartFlag;
	uint16_t  usCtr;
	uint16_t  usPeriod;
}STRUCT_CAPTURE;

STRUCT_CAPTURE strCapture = { 0, 0, 0 };

typedef struct 
{
   __IO int      SetPoint;                                 //设定目标 Desired Value
   __IO double   Proportion;                               //比例常数 Proportional Const
   __IO double   Integral;                                 //积分常数 Integral Const
   __IO double   Derivative;                               //微分常数 Derivative Const
   __IO int      LastError;                                //Error[-1]
   __IO int      PrevError;                                //Error[-2]
}PID;

__IO uint16_t time_count=0;        // 时间计数，每1ms增加一(与滴定时器频率有关)
__IO uint32_t CaptureNumber=0;     // 输入捕获数
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
	/* 启动定时器 */
	HAL_TIM_Base_Start(&htim1);
//	HAL_TIM_Base_Start(&htim3);
	HAL_TIM_Base_Start(&htim4);
	  /* 启动定时器通道和互补通道PWM输出 */
  HAL_TIM_PWM_Start(&htim1,TIM_CHANNEL_1);
  HAL_TIMEx_PWMN_Start(&htim1,TIM_CHANNEL_1);	

//  HAL_TIM_IC_Start_IT(&htim3,TIM_CHANNEL_1);
	HAL_TIM_IC_Start_IT(&htim4,TIM_CHANNEL_1);
	
	start_flag=1; 
	printf("增量式PID算法控制电机旋转\r\n");
	printf("超声波输入捕获测试\r\n");
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
	sptr->SetPoint=10000;           //设定目标Desired Value
	  /* 获取定时器时钟周期 */	
	ulTmrClk = HAL_RCC_GetHCLKFreq()/16;   
  while (1)
  {
	
  /* USER CODE END WHILE */

  /* USER CODE BEGIN 3 */
	  HAL_GPIO_WritePin(Echo1_Trig_GPIO,Echo1_Trig_GPIO_PIN,GPIO_PIN_SET);
		HAL_Delay(1);  
    HAL_GPIO_WritePin(Echo1_Trig_GPIO,Echo1_Trig_GPIO_PIN,GPIO_PIN_RESET);
//		printf("Triging……\r\n");
//		HAL_Delay(2000);   
  
    /* 完成测量高电平脉宽 */
    if(strCapture.ucFinishFlag == 1 )
		{
      /* 计算高电平计数值 */
			ulTime = strCapture .usPeriod * 0xFFFF + strCapture .usCtr;
      temp=((ulTime % ulTmrClk)*340)/(1000*2);
      printf ( ">>测得距离为：%dmm\r\n",temp); 
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
    if(time_count==200)
    {
      __IO uint32_t count;
      __IO int para;
      __IO double cal;
      
      /* 得到编码器计数值，数值越大说明速度越大 */
      count=CaptureNumber; 
      CaptureNumber=0;    // 清零，从零开始计数
      
      /* 计数得到增量式PID的增量数值 */
      para=IncPIDCalc(count);
      
      /* 根据增量数值调整当前电机速度 */
      if((para<-3)||(para>3)) // 不做 PID 调整，避免误差较小时频繁调节引起震荡。
      {
        PWM_Duty +=para/100;  
      }        
      if(PWM_Duty>899)PWM_Duty=899;  
      
      
      // 11：编码器线数(转速一圈输出脉冲数)
      // 34：电机减数比，内部电机转动圈数与电机输出轴转动圈数比，即减速齿轮比      
      cal=sptr->SetPoint;
			printf("\n差值电机速度->	%d			目标速度->	%d			电机速度->	%d			新的占空比->	%d		     \r\n\r\n",count-sptr->SetPoint,sptr->SetPoint,count,PWM_Duty);     
//      printf("\n设定目标速度 ->%d个脉冲     ",sptr->SetPoint);        
//      printf("当前电机速度-> %d个脉冲\n     ",count);
//			printf("差值电机速度-> %d个脉冲\n     ",count-sptr->SetPoint);     
//      printf("增量式PID算法计数结果值：%d   设置新的占空比为：%d\r\n",para,PWM_Duty);
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
//			printf("Triging……\r\n");
		  TIM_IC_InitTypeDef sConfigIC;
		
			if ( strCapture .ucStartFlag == 0 )
			{
				htim4.Instance->CNT=0; // 清零定时器计数
				strCapture .usPeriod = 0;			
				strCapture .usCtr = 0;
				
				// 配置输入捕获参数，主要是修改触发电平
				sConfigIC.ICPolarity = TIM_INPUTCHANNELPOLARITY_FALLING;
				sConfigIC.ICSelection = TIM_ICSELECTION_DIRECTTI;
				sConfigIC.ICPrescaler = TIM_ICPSC_DIV1;
				sConfigIC.ICFilter = 0;
				HAL_TIM_IC_ConfigChannel(&htim4, &sConfigIC, TIM_CHANNEL_1);
				// 清除中断标志位
				__HAL_TIM_CLEAR_IT(htim, TIM_IT_CC1);
				// 启动输入捕获并开启中断
				HAL_TIM_IC_Start_IT(&htim4,TIM_CHANNEL_1);    
				strCapture .ucStartFlag = 1;			
			}		
			
			else
			{  
				// 获取定时器计数值
				strCapture .usCtr = HAL_TIM_ReadCapturedValue(&htim4,TIM_CHANNEL_1);
				// 配置输入捕获参数，主要是修改触发电平
				sConfigIC.ICPolarity = TIM_INPUTCHANNELPOLARITY_RISING;
				sConfigIC.ICSelection = TIM_ICSELECTION_DIRECTTI;
				sConfigIC.ICPrescaler = TIM_ICPSC_DIV1;
				sConfigIC.ICFilter = 0;
				HAL_TIM_IC_ConfigChannel(&htim4, &sConfigIC, TIM_CHANNEL_1);
				
				// 清除中断标志位
				__HAL_TIM_CLEAR_IT(htim, TIM_IT_CC1); 
				// 启动输入捕获并开启中断
				HAL_TIM_IC_Start_IT(&htim4,TIM_CHANNEL_1);    
				strCapture .ucStartFlag = 0;			
				strCapture .ucFinishFlag = 1;    
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
	if(htim == &(htim4))
	{
		strCapture .usPeriod ++;
	} 
}


void HcSr04_Delay(__IO uint32_t i){
	
	while(i--);
}


/**************PID参数初始化********************************/
void IncPIDInit(void) 
{
    sptr->LastError=0;            //Error[-1]
    sptr->PrevError=0;            //Error[-2]
    sptr->Proportion=P_DATA;      //比例常数 Proportional Const
    sptr->Integral=I_DATA;        //积分常数  Integral Const
    sptr->Derivative=D_DATA;      //微分常数 Derivative Const
    sptr->SetPoint=0;           //设定目标Desired Value
}
/********************增量式PID控制设计************************************/
int IncPIDCalc(int NextPoint) 
{
  int iError,iIncpid;                                 //当前误差
  iError=sptr->SetPoint-NextPoint;                    //增量计算
  iIncpid=(sptr->Proportion * iError)                 //E[k]项
              -(sptr->Integral * sptr->LastError)     //E[k-1]项
              +(sptr->Derivative * sptr->PrevError);  //E[k-2]项
              
  sptr->PrevError=sptr->LastError;                    //存储误差，用于下次计算
  sptr->LastError=iError;
  return(iIncpid);                                    //返回增量值
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
