#ifndef __BSP_SDCARD_H__
#define __BSP_SDCARD_H__

/* 包含头文件 ----------------------------------------------------------------*/
#include "stm32f1xx_hal.h"

/* 类型定义 ------------------------------------------------------------------*/
/* 宏定义 --------------------------------------------------------------------*/
/* 扩展变量 ------------------------------------------------------------------*/
extern SD_HandleTypeDef hsdcard;
extern HAL_SD_CardInfoTypedef SDCardInfo;
extern DMA_HandleTypeDef hdma_sdio;

/* 函数声明 ------------------------------------------------------------------*/
void MX_SDIO_SD_Init(void);

#endif  /* __BSP_SDCARD_H__ */

/******************* (C) COPYRIGHT 2015-2020 硬石嵌入式开发团队 *****END OF FILE****/
