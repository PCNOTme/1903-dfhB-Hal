/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  ** This notice applies to any and all portions of this file
  * that are not between comment pairs USER CODE BEGIN and
  * USER CODE END. Other portions of this file, whether 
  * inserted by the user or by software development tools
  * are owned by their respective copyright owners.
  *
  * COPYRIGHT(c) 2018 STMicroelectronics
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

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H__
#define __MAIN_H__

/* Includes ------------------------------------------------------------------*/

/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Private define ------------------------------------------------------------*/

#define Echo_LU_Pin GPIO_PIN_5
#define Echo_LU_GPIO_Port GPIOE
#define Echo_RU_Pin GPIO_PIN_6
#define Echo_RU_GPIO_Port GPIOE
#define LED_Pin GPIO_PIN_13
#define LED_GPIO_Port GPIOC
#define Encoder4_A_Pin GPIO_PIN_0
#define Encoder4_A_GPIO_Port GPIOA
#define Encoder4_B_Pin GPIO_PIN_1
#define Encoder4_B_GPIO_Port GPIOA
#define OpenMV_RX1_Pin GPIO_PIN_2
#define OpenMV_RX1_GPIO_Port GPIOA
#define OpenMV_RX2_Pin GPIO_PIN_3
#define OpenMV_RX2_GPIO_Port GPIOA
#define Encoder1_A_Pin GPIO_PIN_5
#define Encoder1_A_GPIO_Port GPIOA
#define Sevor_2_Pin GPIO_PIN_6
#define Sevor_2_GPIO_Port GPIOA
#define Motor1_1_Pin GPIO_PIN_9
#define Motor1_1_GPIO_Port GPIOE
#define Motor1_2_Pin GPIO_PIN_11
#define Motor1_2_GPIO_Port GPIOE
#define Motor2_1_Pin GPIO_PIN_13
#define Motor2_1_GPIO_Port GPIOE
#define Motor2_2_Pin GPIO_PIN_14
#define Motor2_2_GPIO_Port GPIOE
#define Echo_LD_Pin GPIO_PIN_14
#define Echo_LD_GPIO_Port GPIOB
#define Echo_RD_Pin GPIO_PIN_15
#define Echo_RD_GPIO_Port GPIOB
#define Encoder3_A_Pin GPIO_PIN_12
#define Encoder3_A_GPIO_Port GPIOD
#define Encoder3_B_Pin GPIO_PIN_13
#define Encoder3_B_GPIO_Port GPIOD
#define Motor3_1_Pin GPIO_PIN_6
#define Motor3_1_GPIO_Port GPIOC
#define Motor3_2_Pin GPIO_PIN_7
#define Motor3_2_GPIO_Port GPIOC
#define Motor4_1_Pin GPIO_PIN_8
#define Motor4_1_GPIO_Port GPIOC
#define Motor4_2_Pin GPIO_PIN_9
#define Motor4_2_GPIO_Port GPIOC
#define Trig_U_Pin GPIO_PIN_0
#define Trig_U_GPIO_Port GPIOD
#define Trig_LU_Pin GPIO_PIN_2
#define Trig_LU_GPIO_Port GPIOD
#define Trig_RU_Pin GPIO_PIN_3
#define Trig_RU_GPIO_Port GPIOD
#define Trig_LD_Pin GPIO_PIN_4
#define Trig_LD_GPIO_Port GPIOD
#define Trig_RD_Pin GPIO_PIN_5
#define Trig_RD_GPIO_Port GPIOD
#define Encoder1_B_Pin GPIO_PIN_3
#define Encoder1_B_GPIO_Port GPIOB
#define Encoder2_A_Pin GPIO_PIN_4
#define Encoder2_A_GPIO_Port GPIOB
#define Encoder2_B_Pin GPIO_PIN_5
#define Encoder2_B_GPIO_Port GPIOB
#define Echo_U_Pin GPIO_PIN_8
#define Echo_U_GPIO_Port GPIOB
#define Sevor_1_Pin GPIO_PIN_9
#define Sevor_1_GPIO_Port GPIOB

/* ########################## Assert Selection ############################## */
/**
  * @brief Uncomment the line below to expanse the "assert_param" macro in the 
  *        HAL drivers code
  */
/* #define USE_FULL_ASSERT    1U */

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
 extern "C" {
#endif
void _Error_Handler(char *, int);

#define Error_Handler() _Error_Handler(__FILE__, __LINE__)
#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H__ */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
