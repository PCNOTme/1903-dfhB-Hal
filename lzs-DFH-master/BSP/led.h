#ifndef __LED_H
#define __LED_H
#include "stm32f4xx_hal.h"
#include "gpio.h"


#define LED_On HAL_GPIO_WritePin(LED_GPIO_Port,LED_Pin,GPIO_PIN_RESET)

#define LED_Off HAL_GPIO_WritePin(LED_GPIO_Port,LED_Pin,GPIO_PIN_SET)

void LED_Flashing(int delay_ms);

void LED_Fast_Flashing(int n);

void LED_Slow_Flashing(int n);


#endif

