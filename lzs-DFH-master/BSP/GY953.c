#include "GY953.h"
#include "usart.h"
#include "led.h"
#include <math.h>

/*由于GY953陀螺仪的问题，Angle.time会自增，导致Angle.car自减，
  可以设置Yaw轴目标值自减以抵消自增误差*/
	
/*车头右摆Angle.time减少，Angle.car增大
  车头左摆Angle.time增大，Angle.car减少*/

gy953 GY953;
angle Angle; 

uint8_t  data_buf[13]; 
uint8_t sent_buf1[3]={0xa5,0xaf,0x54}; // 波特率设置为115200
uint8_t sent_buf2[3]={0xa5,0xa4,0x49}; // 数据输出速率50hz
uint8_t sent_buf3[3]={0xa5,0x45,0xEA}; // 欧拉角（默认50HZ）(自动输出指令)
uint8_t sent_buf4[3]={0xa5,0x57,0xFC}; // 加计陀螺校准
uint8_t sent_buf5[3]={0xa5,0x65,0x0A}; // 四元数
uint8_t sent_buf6[3]={0xa5,0xa6,0x4B}; // 数据输出速率200hz
uint8_t sent_buf7[3]={0xa5,0X59,0xFE}; // 恢复出厂设置
 
/**
  * @brief  陀螺仪初始化函数
  * @param  无
  * @retval 无
  */
void GY953_Init(void)
{
	  HAL_UART_Receive_DMA(&huart3,data_buf,13);// 启动DMA接收
		/*HAL_UART_Transmit_DMA(&huart3,sent_buf7,3);
		HAL_Delay(1000);
	  */
		HAL_UART_Transmit_DMA(&huart3,sent_buf1,3);
		HAL_Delay(1000);
	  HAL_UART_Transmit_DMA(&huart3,sent_buf4,3);
		HAL_Delay(1000);
		HAL_UART_Transmit_DMA(&huart3,sent_buf6,3);
		HAL_Delay(1000);
	  HAL_UART_Transmit_DMA(&huart3,sent_buf5,3);
		HAL_Delay(1000);   
	
    LED_Slow_Flashing(5);
} 

/**
  * @brief  陀螺仪值函获取函数
  * @param  无
  * @retval 陀螺仪数值
  */
int16_t GY953_GetDate(void)
{
	int16_t q0=0,q1=0,q2=0,q3=0;
	int16_t YAW=66;
	if(data_buf[2]==0x65)
	{
		q0=((data_buf[4]<<8)|data_buf[5]);
		q1=((data_buf[6]<<8)|data_buf[7]);
		q2=((data_buf[8]<<8)|data_buf[9]);
		q3=((data_buf[10]<<8)|data_buf[11]);
	
		YAW = atan2( 2 * (q0 * q3 +q2 * q1), q0*q0 + q1*q1 - q2*q2 - q3*q3 )*57.3 ;
		return YAW;
	}
	else
	{		
		LED_Flashing(70);
		return 0;
	}
}  


/**
  * @brief  方向定位函数
  * @param  无
  * @retval 无
  */
void Angle_Init(void)
{
	GY953_Init();
	Angle.front=GY953_GetDate();
	Angle.flag=0;
	if(Angle.front>=90)
	{
		Angle.right=Angle.front-90;
		Angle.left=Angle.front-270;
		Angle.back=Angle.front-180;
	}
	else if(Angle.front<90&&Angle.front>=0)
	{
		Angle.right=Angle.front-90;
		Angle.left=Angle.front+90;
		Angle.back=Angle.front-180;
	}
	else if(Angle.front<0&&Angle.front>=-90)
	{
		Angle.right=Angle.front-90;
		Angle.left=Angle.front+90;
		Angle.back=Angle.front+180;
	}
	else if(Angle.front<-90)
	{
		Angle.right=Angle.front+270;
		Angle.left=Angle.front+90;
		Angle.back=Angle.front+180;
	}
}

/**
  * @brief  方向定位辅助函数
  * @param  无
  * @retval 无
  */
void Angle_change(void)
{
	if(Angle.flag==Go_straight)
	{
		Angle.car=Angle.front-Angle.time;
		if(Angle.car>180)
		{
			Angle.car-=360;
		}
		else if(Angle.car<-180)
		{
			Angle.car+=360;
		}
	}
	else if(Angle.flag==Turn_right_90)
	{
	Angle.car=Angle.right-Angle.time;
		if(Angle.car>180)
		{
			Angle.car-=360;
		}
		else if(Angle.car<-180)
		{
			Angle.car+=360;
		}
	}
		else if(Angle.flag==Turn_left_90)
	{
	Angle.car=Angle.left-Angle.time;
		if(Angle.car>180)
		{
			Angle.car-=360;
		}
		else if(Angle.car<-180)
		{
			Angle.car+=360;
		}
	}
		else if(Angle.flag==Back)
	{
	Angle.car=Angle.back-Angle.time;
		if(Angle.car>180)
		{
			Angle.car-=360;
		}
		else if(Angle.car<-180)
		{
			Angle.car+=360;
		}
	}
}
