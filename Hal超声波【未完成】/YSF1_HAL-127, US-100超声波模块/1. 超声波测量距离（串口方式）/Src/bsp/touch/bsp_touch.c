/**
  ******************************************************************************
  * 文件名程: bsp_touch.c 
  * 作    者: 硬石嵌入式开发团队
  * 版    本: V1.0
  * 编写日期: 2015-10-04
  * 功    能: 电阻触摸底层驱动函数
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
#include "lcd/bsp_lcd.h"
#include "usart/bsp_debug_usart.h"
#include "spiflash/bsp_spiflash.h"
#include "touch/bsp_touch.h"
#include <stdio.h> 
#include <string.h>

/* 私有类型定义 --------------------------------------------------------------*/
/* 私有宏定义 ----------------------------------------------------------------*/

/* 私有变量 ------------------------------------------------------------------*/
XPT2046_Calibration cal_value={0};

/* 扩展变量 ------------------------------------------------------------------*/
/* 私有函数原形 --------------------------------------------------------------*/
/* 函数体 --------------------------------------------------------------------*/
/**
  * 函数功能: 电阻触摸相关GPIO初始化
  * 输入参数: 无
  * 返 回 值: 无
  * 说    明：无
  */
 void Touch_Init_GPIO(void)
{ 
  GPIO_InitTypeDef GPIO_InitStruct;

  /* 开启GPIO时钟 */
  XPT2046_EXTI_GPIO_CLK_ENABLE();
  XPT2046_SPI_GPIO_CLK_ENABLE();

  /* 模拟SPI GPIO初始化 */
  GPIO_InitStruct.Pin = XPT2046_SPI_CLK_PIN|XPT2046_SPI_MOSI_PIN|XPT2046_SPI_CS_PIN;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_MEDIUM;
  HAL_GPIO_Init(XPT2046_SPI_PORT, &GPIO_InitStruct);


  GPIO_InitStruct.Pin  = XPT2046_SPI_MISO_PIN; 
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  HAL_GPIO_Init(XPT2046_SPI_PORT, &GPIO_InitStruct);
  
  GPIO_InitStruct.Pin = XPT2046_EXTI_PIN;
  HAL_GPIO_Init(XPT2046_EXTI_PORT, &GPIO_InitStruct);
   
  /* 拉低片选，选择XPT2046 */
  XPT2046_CS_LOW();
}

/**
  * 函数功能: us 级别延时，不是很精确
  * 输入参数: cnt：延时时间
  * 返 回 值: 无
  * 说    明：无
  */
static void XPT2046_DelayUS ( __IO uint32_t ulCount )
{
	uint32_t i;
	for ( i = 0; i < ulCount; i ++ )
	{
		uint8_t uc = 12;     //设置值为12，大约延1微秒  	      
		while ( uc -- );     //延1微秒	
	}	
}

/**
  * 函数功能: 写命令
  * 输入参数: CHX 	0x90 	//通道Y+的选择控制字 
  *           CHY   0xd0	//通道X+的选择控制字
  * 返 回 值: 无
  * 说    明：无
  */
static void XPT2046_WriteCMD(uint8_t ucCmd)
{
	uint8_t i;
  
	XPT2046_MOSI_LOW();	
	XPT2046_CLK_LOW();

	for ( i = 0; i < 8; i ++ ) 
	{
		( ( ucCmd >> ( 7 - i ) ) & 0x01 ) ? XPT2046_MOSI_HIGH() : XPT2046_MOSI_LOW();		
	  XPT2046_DelayUS ( 5 );		
		XPT2046_CLK_HIGH();
	  XPT2046_DelayUS ( 5 );
		XPT2046_CLK_LOW();
	}	
}

/**
  * 函数功能: 读数据
  * 输入参数: 无
  * 返 回 值: short：读到的数据
  * 说    明：无
  */
static uint16_t XPT2046_ReadCMD ( void ) 
{
	uint8_t i;
	uint16_t usBuf=0, usTemp;

	XPT2046_MOSI_LOW();
	XPT2046_CLK_HIGH();
	for ( i=0;i<12;i++ ) 
	{
		XPT2046_CLK_LOW();	
		usTemp = XPT2046_MISO_READ();		
		usBuf |= usTemp << ( 11 - i );	
		XPT2046_CLK_HIGH();		
	}	
	return usBuf;
}

/**
  * 函数功能: 选择一个模拟通道，启动ADC，并返回ADC采样结果
  * 输入参数: _ucCh = 0x90 表示Y通道；
  *                   0xd0 表示X通道
  * 返 回 值: 无
  * 说    明：无
  */
uint16_t XPT2046_ReadAdc(uint8_t _ucCh)
{
  XPT2046_WriteCMD(_ucCh);
	return 	XPT2046_ReadCMD();
}

/**
  * 函数功能: 校正触摸时画十字专用 
  * 输入参数: x：十字中点x轴
  *           y：十字中点y轴
  * 返 回 值: 无
  * 说    明：无
  */
void LCD_DrawCross(uint16_t x,uint16_t y)
{
  LCD_DrawLine(x-10,y,x+10,y,RED);
  LCD_DrawLine(x,y-10,x,y+10,RED);
}

/**
  * 函数功能: 读取TP x y 的AD值(12bit，最大是4096)
  * 输入参数: x：读到AD值
  *           y：读到AD值
  * 返 回 值: 无
  * 说    明：无
  */
void XPT2046_ReadAdc_XY(int16_t *sX_Ad,int16_t *sY_Ad)  
{ 
	int16_t sX_Ad_Temp, sY_Ad_Temp; 	
	
	sX_Ad_Temp = XPT2046_ReadAdc(XPT2046_CHANNEL_X);
	XPT2046_DelayUS(10); 
	sY_Ad_Temp = XPT2046_ReadAdc(XPT2046_CHANNEL_Y); 		
	* sY_Ad = sX_Ad_Temp; 
	* sX_Ad = sY_Ad_Temp;	
}

/**
  * 函数功能: 得到简单滤波之后的X Y
  * 输入参数: pTouch_AD_x：保存测量X+对应的AD值
  *           pTouch_AD_y：保存测量Y+对应的AD值
  *           wait：松开等待使能
  *            参数 1：使能等待松开
  *                 0：无需等待松开
  * 返 回 值: 无
  * 说    明：画板应用实例专用,不是很精准，但是速度比较快
  */
static uint8_t XPT2046_ReadAdc_Smooth_XY(int32_t *pTouch_AD_x,int32_t *pTouch_AD_y,uint8_t wait)
{
	uint8_t ucCount = 0, i;
	int16_t sAD_X, sAD_Y;
	int16_t sBufferArray[2][10]={{0},{0}};  //坐标X和Y进行多次采样	
	int32_t lX_Min, lX_Max, lY_Min, lY_Max;
  
  while(ucCount<10)
  {
    if(XPT2046_EXTI_Read() == XPT2046_EXTI_ActiveLevel)
    {
      XPT2046_ReadAdc_XY(&sAD_X,&sAD_Y);      
      sBufferArray[0][ucCount]=sAD_X;  
      sBufferArray[1][ucCount]=sAD_Y;
      ucCount ++;     
    }
    else if(wait==0)
      break;
  }
  
  while(wait)
  {
    if(XPT2046_EXTI_Read() != XPT2046_EXTI_ActiveLevel)break;
  }
  
	if(ucCount==10)
  {
    lX_Max=lX_Min=sBufferArray[0][0];
    lY_Max=lY_Min=sBufferArray[1][0];       
    for(i=1;i<10;i++)
    {
      if(sBufferArray[0][i]<lX_Min)
        lX_Min=sBufferArray[0][i];
      else if(sBufferArray[0][i]>lX_Max)
        lX_Max=sBufferArray[0][i];
    }
    for(i=1;i<10;i++)
    {
      if(sBufferArray[1][i]<lY_Min)
        lY_Min=sBufferArray[1][i];			
      else if(sBufferArray[1][i]>lY_Max)
        lY_Max=sBufferArray[1][i];
    }		
    
    /*去除最小值和最大值之后求平均值*/
    *pTouch_AD_x=(sBufferArray[0][0]+sBufferArray[0][1]+sBufferArray[0][2]+sBufferArray[0][3]+sBufferArray[0][4]+
               sBufferArray[0][5]+sBufferArray[0][6]+sBufferArray[0][7]+sBufferArray[0][8]+sBufferArray[0][9]-lX_Min-lX_Max)>>3;
    *pTouch_AD_y=(sBufferArray[1][0]+sBufferArray[1][1]+sBufferArray[1][2]+sBufferArray[1][3]+sBufferArray[1][4]+
               sBufferArray[1][5]+sBufferArray[1][6]+sBufferArray[1][7]+sBufferArray[1][8]+sBufferArray[1][9]-lX_Min-lX_Max)>>3;
    return 0;
  }
  *pTouch_AD_x=-1;
  *pTouch_AD_y=-1;
  return 1;
}

/**
  * 函数功能: 电阻屏校准算法实现
  * 输入参数: XPT2046_Calibration结构体指针
  * 返 回 值: 0：计算成功，1：无法计算
  * 说    明：无
  */
static uint8_t perform_calibration(XPT2046_Calibration *cal) 
{
	int j;
	float n, x, y, x2, y2, xy, z, zx, zy;
	float det, a, b, c, e, f, i;
	float scaling = 65536.0;

// Get sums for matrix
	n = x = y = x2 = y2 = xy = 0;
	for(j=0;j<5;j++) {
		n += 1.0;
		x += (float)cal->touch_x[j];
		y += (float)cal->touch_y[j];
		x2 += (float)(cal->touch_x[j]*cal->touch_x[j]);
		y2 += (float)(cal->touch_y[j]*cal->touch_y[j]);
		xy += (float)(cal->touch_x[j]*cal->touch_y[j]);
	}

// Get determinant of matrix -- check if determinant is too small
	det = n*(x2*y2 - xy*xy) + x*(xy*y - x*y2) + y*(x*xy - y*x2);
	if(det < 0.1 && det > -0.1) {
//		printf("ts_calibrate: determinant is too small -- %f\n\r",det);
		return 1;
	}

// Get elements of inverse matrix
	a = (x2*y2 - xy*xy)/det;
	b = (xy*y - x*y2)/det;
	c = (x*xy - y*x2)/det;
	e = (n*y2 - y*y)/det;
	f = (x*y - n*xy)/det;
	i = (n*x2 - x*x)/det;

// Get sums for x calibration
	z = zx = zy = 0;
	for(j=0;j<5;j++) {
		z += (float)cal->lcd_x[j];
		zx += (float)(cal->lcd_x[j]*cal->touch_x[j]);
		zy += (float)(cal->lcd_x[j]*cal->touch_y[j]);
	}

// Now multiply out to get the calibration for framebuffer x coord
	cal->adjust[0] = (int32_t)((a*z + b*zx + c*zy)*(scaling));
	cal->adjust[1] = (int32_t)((b*z + e*zx + f*zy)*(scaling));
	cal->adjust[2] = (int32_t)((c*z + f*zx + i*zy)*(scaling));

//	printf("%f %f %f\n\r",(a*z + b*zx + c*zy),
//				(b*z + e*zx + f*zy),
//				(c*z + f*zx + i*zy));

// Get sums for y calibration
	z = zx = zy = 0;
	for(j=0;j<5;j++) {
		z += (float)cal->lcd_y[j];
		zx += (float)(cal->lcd_y[j]*cal->touch_x[j]);
		zy += (float)(cal->lcd_y[j]*cal->touch_y[j]);
	}

// Now multiply out to get the calibration for framebuffer y coord
	cal->adjust[3] = (int32_t)((a*z + b*zx + c*zy)*(scaling));
	cal->adjust[4] = (int32_t)((b*z + e*zx + f*zy)*(scaling));
	cal->adjust[5] = (int32_t)((c*z + f*zx + i*zy)*(scaling));

//	printf("%f %f %f\n\r",(a*z + b*zx + c*zy),
//				(b*z + e*zx + f*zy),
//				(c*z + f*zx + i*zy));

// If we got here, we're OK, so assign scaling to a[6] and return
	cal->adjust[6] = (int32_t)scaling;
	return 0;
}
 
/**
  * 函数功能: 触摸屏校正函数
  * 输入参数: 无
  * 返 回 值: 0：校正成功
  *           1：校正失败
  * 说    明：无
  */
uint8_t XPT2046_Touch_Calibrate(void)
{  
  uint8_t i;  

  uint16_t usTest_x=0,usTest_y=0;  
  
  /* 设定“十”字交叉点的坐标 */ 
  cal_value.lcd_x[0]=20;
  cal_value.lcd_y[0]=20;
  
  cal_value.lcd_x[1]=20;
  cal_value.lcd_y[1]=LCD_DEFAULT_HEIGTH-20;
  
  cal_value.lcd_x[2]=LCD_DEFAULT_WIDTH-20;
  cal_value.lcd_y[2]=cal_value.lcd_y[1];
  
  cal_value.lcd_x[3]=cal_value.lcd_x[2];
  cal_value.lcd_y[3]=cal_value.lcd_y[0];	
  
  cal_value.lcd_x[4]=LCD_DEFAULT_WIDTH/2;
  cal_value.lcd_y[4]=LCD_DEFAULT_HEIGTH/2;	
  
  for(i=0; i<5; i++)
  {        
    LCD_Clear(0, 0,LCD_DEFAULT_WIDTH, LCD_DEFAULT_HEIGTH, BACKGROUND);        
    LCD_DispString_EN(50,120,"Touch Calibrate...",BACKGROUND,BLUE,USB_FONT_24);	         
    LCD_DispChar_EN(150, 80, i+'1',BACKGROUND,RED,USB_FONT_24);
 
    /* 适当的延时很有必要 */        
    XPT2046_DelayUS(200000);    
    LCD_DrawCross(cal_value.lcd_x[i],cal_value.lcd_y[i]); //显示校正用的“十”字    
    XPT2046_ReadAdc_Smooth_XY(&cal_value.touch_x[i],&cal_value.touch_y[i],1);
  }
  
  if(perform_calibration(&cal_value)==1) 
  {
//		printf("Calibration failed.\n\r");
    return 1;
	}

  /* 用原始参数计算出 原始参数与坐标的转换系数。 */
  for(i=0; i<2; i++)
  {    
    int xtemp,ytemp,usGap_x,usGap_y;       
    xtemp=cal_value.touch_x[2*i];
    ytemp=cal_value.touch_y[2*i];
//    printf("before Calibration x=(%d)-> %d y=(%d)-> %d\n\r",cal_value.lcd_x[2*i],xtemp,cal_value.lcd_y[2*i],ytemp);
    
		usTest_x=(int)((cal_value.adjust[0]+cal_value.adjust[1]*xtemp+cal_value.adjust[2]*ytemp)/cal_value.adjust[6]);
		usTest_y=(int)((cal_value.adjust[3]+cal_value.adjust[4]*xtemp+cal_value.adjust[5]*ytemp)/cal_value.adjust[6]);
//	  printf("after Calibration x = %d y=%d\n\r",usTest_x,usTest_y);
    
    usGap_x=(usTest_x>cal_value.lcd_x[2*i])?(usTest_x-cal_value.lcd_x[2*i]):(cal_value.lcd_x[2*i]-usTest_x);   //实际X坐标与计算坐标的绝对差
    usGap_y=(usTest_y>cal_value.lcd_y[2*i])?(usTest_y-cal_value.lcd_y[2*i]):(cal_value.lcd_y[2*i]-usTest_y);   //实际Y坐标与计算坐标的绝对差
  
    if((usGap_x>10)||(usGap_y>10))
    {
      LCD_Clear(0, 0,LCD_DEFAULT_WIDTH, LCD_DEFAULT_HEIGTH, BACKGROUND);   
      LCD_DispString_EN(80,100,"Calibrate fail",BACKGROUND,RED,USB_FONT_24);
      LCD_DispString_EN(110,130,"try again",BACKGROUND,RED,USB_FONT_16);
      XPT2046_DelayUS(1000000);   
      return 1;   
    }      
  }
  
  cal_value.cal_flag = 0xAA55;
  SPI_FLASH_SectorErase(XPT2046_SPIFLASH_ADDR);
  SPI_FLASH_BufferWrite((uint8_t *)&cal_value,XPT2046_SPIFLASH_ADDR, sizeof(XPT2046_Calibration));
  
  LCD_Clear(0, 0,LCD_DEFAULT_WIDTH, LCD_DEFAULT_HEIGTH, BACKGROUND); 
  LCD_DispString_EN(50,100,"Calibrate Succed",BACKGROUND,BLUE,USB_FONT_24);
  XPT2046_DelayUS(1000000); 
  LCD_Clear(0, 0,LCD_DEFAULT_WIDTH, LCD_DEFAULT_HEIGTH, BACKGROUND); 
  return 0;    
}

/**
  * 函数功能: 获取 XPT2046 触摸点（校准后）的坐标
  * 输入参数: pLCD_x：校准后x的坐标
  *           pLCD_y：校准后y的坐标
  * 返 回 值: 无
  * 说    明：无
  */
void XPT2046_Get_TouchedPoint(uint16_t *pLCD_x,uint16_t *pLCD_y)
{
  int xtemp,ytemp;
  
  if(XPT2046_ReadAdc_Smooth_XY(&xtemp,&ytemp,0)==0)
  {
    *pLCD_x=(uint16_t)((cal_value.adjust[0]+cal_value.adjust[1]*xtemp+cal_value.adjust[2]*ytemp)/cal_value.adjust[6]);
    *pLCD_y=(uint16_t)((cal_value.adjust[3]+cal_value.adjust[4]*xtemp+cal_value.adjust[5]*ytemp)/cal_value.adjust[6]);
  }
  else
  {
    *pLCD_x=0xFFFF;
    *pLCD_y=0xFFFF;
  }
  
} 

/**
  * 函数功能: 在LCD指定位置画一个大点(包含9个小点)
  * 输入参数: x：x坐标
  *           y：y坐标
  *           usColor：点的颜色
  * 返 回 值: 无
  * 说    明：无
  */
void Palette_draw_point(uint16_t x, uint16_t y,uint16_t usColor)
{  
  LCD_SetPointPixel ( x-1, y-1, usColor );
  LCD_SetPointPixel (   x, y-1, usColor );
  LCD_SetPointPixel ( x+1, y-1, usColor );
  LCD_SetPointPixel ( x-1,   y, usColor );  
  LCD_SetPointPixel (   x,   y, usColor );  
  LCD_SetPointPixel ( x+1,   y, usColor );
  LCD_SetPointPixel ( x-1, y+1, usColor );  
  LCD_SetPointPixel (   x, y+1, usColor );  
  LCD_SetPointPixel ( x+1, y+1, usColor );
}
/******************* (C) COPYRIGHT 2015-2020 硬石嵌入式开发团队 *****END OF FILE****/
