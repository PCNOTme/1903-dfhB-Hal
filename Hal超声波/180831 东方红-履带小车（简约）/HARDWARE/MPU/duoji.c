#include "led.h"
#include "delay.h"
#include "sys.h"
#include "usart.h"
#include "timer.h"
#include "echo.h"
#include "duoji.h"
#include "motor.h"
#include "DATA.h"
#include "anbt_dmp_fun.h"
#include "anbt_i2c.h"
#include "anbt_dmp_mpu6050.h"
#include "anbt_dmp_driver.h"


#define left  0
#define right  1
#define left_pwm  1300
#define right_pwm  2300
#define left_small  100
#define right_small 100
#define vel  35
#define L  1


extern  int flag1;
extern float length[5]; 
extern int velocity[2];
extern int pwm_first;
float  pwm_last=1650;
int pwm_first=1700;
extern float roll,yaw,pitch;	
extern int flag2;
extern int straight_times;
extern unsigned char  TIM3CH1_CAPTURE_STA, TIM3CH3_CAPTURE_STA, TIM3CH4_CAPTURE_STA;	//输入捕获状态
extern unsigned short 	TIM3CH1_CAPTURE_VAL,TIM3CH2_CAPTURE_VAL,TIM3CH3_CAPTURE_VAL,TIM3CH4_CAPTURE_VAL;	//输入捕获值

float value_abs(float temp)               ////函数功能：取绝对值
{
	if(temp>=0)return temp;
	if(temp<0)return -temp;
}


void turn(int dir,int pwm_change)          		 ///////////////前舵机转弯，方向+角度（相对初始位置）
{
		extern int pwm_first;
	  if(dir==0)pwm_first-=pwm_change;
		if(dir==1) pwm_first+=pwm_change;    							//////////////以1500为中位置
	  //printf ("PWM变化量: %d\r\n",pwm_change);
	  if(pwm_first<=1200)pwm_first=1200;                 /////////////保护舵机
	  if(pwm_first>=2400)pwm_first=2400;
    TIM_SetCompare1(TIM5,pwm_first);	
		printf ("PWM总: %d\r\n",pwm_first);
	  delay_ms(100);
	
}


void turn_temp1(void)								/////////////转弯90度
{
	float  t;
	int angle;
	DATA_Processing();
	mpu_dmp_get_data(&pitch,&roll,&yaw);
	t=yaw;
	printf("t= %f\r\n",t);
	
		if(flag2==1)
		{
				angle=60;
				while(value_abs(yaw-t)<=angle) 
			{
				TIM_SetCompare1(TIM5,2300);
				motor1(50,20);
				DATA_Processing();
				mpu_dmp_get_data(&pitch,&roll,&yaw);
			}				
		}	
		if(flag2==7)
		{
				angle=60;
				while(value_abs(yaw-t)<=angle) 
			{
				TIM_SetCompare1(TIM5,1380);
				motor1(20,50);
				DATA_Processing();
				mpu_dmp_get_data(&pitch,&roll,&yaw);
			}				
		}	
//	if(flag2==7)angle=60;
//	if(flag2==1)angle=60;
//		while(value_abs(yaw-t)<=angle) 
//	{
//		if(flag2==7)
//		{
//			TIM_SetCompare1(TIM5,1380);
//			motor1(20,50);	
//		}
//		if(flag2==1)
//		{
//			TIM_SetCompare1(TIM5,2300);
//			motor1(50,20);						
//		}	
//		
//		DATA_Processing();
//		mpu_dmp_get_data(&pitch,&roll,&yaw);
//	}		
	//TIM_SetCompare1(TIM5,1700);
		flag1=0;
		LED0=1;
		straight_times=0;	
}

void turn_temp2(void)								//////////转弯180度
{
	float  t;
	int motor_pwm;
	DATA_Processing();
	mpu_dmp_get_data(&pitch,&roll,&yaw);
	t=yaw;
	printf("t= %f\r\n",t);
	
//	while(value_abs(yaw-t)<=190) 
//	{
//			if(flag2==3|flag2==5)
//			{
//				motor1(20,50);		
//				TIM_SetCompare1(TIM5,1380);
//			}
//			if(flag2==2|flag2==4)
//			{
//				motor1(60,20);		
//				TIM_SetCompare1(TIM5,2200);
//			}
//			if(flag2==6)
//			{
//				motor1(60,20);	
//				TIM_SetCompare1(TIM5,2200);		
//			}
//			
//		DATA_Processing();
//		mpu_dmp_get_data(&pitch,&roll,&yaw);

//	}
			while(1)
		{
				if(flag2==3|flag2==5)
				{
					motor1(20,50);		
					TIM_SetCompare1(TIM5,1380);
				}
				if(flag2==2|flag2==4)
				{
					motor1(60,20);		
					TIM_SetCompare1(TIM5,2200);
				}
				if(flag2==6)
				{
					motor1(60,20);	
					TIM_SetCompare1(TIM5,2200);		
				}
			DATA_Processing();
			mpu_dmp_get_data(&pitch,&roll,&yaw);
			if((value_abs(yaw-t)<=190)&&(value_abs(yaw-t)>=180))break;
		}

}

