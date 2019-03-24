#include "path_planning.h"
#include "GY953.h"
#include "pid.h"
#include "hc_sr04.h"
#include "encoder.h"
#include "motor.h"
#include "stm32f4xx_hal.h"

extern gy953 GY953;
extern angle Angle; 
extern Encoder encoder;
extern Hc_Sr04 hc_sr04[7];
extern Path_Plan pathFlag[15];
extern uint16_t lineSpeed;
extern int YAW_Angle_Point;

int timeTick = 0;
int MySecond1 = 0;
int MySecond2 = 0;
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
	if(htim == (&htim6))
	{
		timeTick++;
		if(timeTick%10 == 0)
		{
  		 Path_All_Deal();
   		 Angle_change();	  
		   SpeedPID_Calc();
			
//			Debug_Picking();
//     Debug_Picking();
//			Position_YAW_PID_Calc(&Yaw_PID,YAW_Angle_Point,Angle.car,Yaw_Error_Value,20);
//			Mecanum_Drive(0 , 30 , Yaw_PID.PWMSum);
//			if(HCSR_U<=150)
//			{
//       Motor_Stop();		
//			}
		}
		if(timeTick%50 == 0)
		{
     //printf ("Angle.time=%d  Angle.car=%d\n",Angle.time,Angle.car);
      //printf("HCSR_LD=%d   PWM=%d\n",HCSR_LD,HCSR_PID.PWMSum);			
	   //printf("HCSR_U=%d  HCSR_RD=%d  HCSR_RU=%d  HCSR_LD=%d  HCSR_LU=%d  PWM=%d\n",HCSR_U,HCSR_RD,HCSR_RU,HCSR_LD,HCSR_LU,HCSR_PID.PWMSum);
			//printf("encoder_LU=%d  encoder_RU=%d  encoder_LD=%d  encoder_RD=%d\r\n",encoder.encoder_LU ,encoder.encoder_RU,encoder.encoder_LD,encoder.encoder_RD);
			//printf ("HCSR_LU=%d   HCSR_LD=%d   Angle.car=%d\n",HCSR_LU,HCSR_LD,(HCSR_LU - HCSR_LD));
			//printf("HCSR_U=%d \n ",HCSR_U);
			//printf("flag1=%d  flag2=%d\n ",HAL_GPIO_ReadPin(GPIOA,OpenMV_RX1_Pin),HAL_GPIO_ReadPin(GPIOA,OpenMV_RX2_Pin));
		}
		if(timeTick%1000 == 0)
		{
      MySecond1++;
			MySecond2++;
			timeTick = 0;    //防止整型变量溢出计时不准
		}
	}
}  


