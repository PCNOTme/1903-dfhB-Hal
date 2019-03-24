#ifndef __BSP_USARTX_H__
#define __BSP_USARTX_H__

/* ����ͷ�ļ� ----------------------------------------------------------------*/
#include "stm32f1xx_hal.h"

/* ���Ͷ��� ------------------------------------------------------------------*/
/* �궨�� --------------------------------------------------------------------*/
#define USARTx                                 USART3
#define USARTx_BAUDRATE                        9600
#define USART_RCC_CLK_ENABLE()                 __HAL_RCC_USART3_CLK_ENABLE()
#define USART_RCC_CLK_DISABLE()                __HAL_RCC_USART3_CLK_DISABLE()

#define USARTx_GPIO_ClK_ENABLE()               __HAL_RCC_GPIOB_CLK_ENABLE()
#define USARTx_Tx_GPIO_PIN                     GPIO_PIN_10
#define USARTx_Tx_GPIO                         GPIOB
#define USARTx_Rx_GPIO_PIN                     GPIO_PIN_11
#define USARTx_Rx_GPIO                         GPIOB

#define USARTx_IRQHANDLER                      USART3_IRQHandler
#define USARTx_IRQn                            USART3_IRQn

/* ��չ���� ------------------------------------------------------------------*/
extern UART_HandleTypeDef husartx;

/* �������� ------------------------------------------------------------------*/
void MX_USARTx_Init(void);

#endif  /* __BSP_USARTX_H__ */

/******************* (C) COPYRIGHT 2015-2020 ӲʯǶ��ʽ�����Ŷ� *****END OF FILE****/
