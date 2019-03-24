#include "path_planning.h"
#include "GY953.h"
#include "pid.h"
#include "hc_sr04.h"
#include "encoder.h"
#include "motor.h"
#include "led.h"
#include "OpenMV_RX.h"

uint8_t endflag = 0;
uint16_t lineSpeed=50;

uint16_t LINE_High_SPEED=70;
uint16_t LINE_Middle_SPEED=50 ;//45
uint16_t LINE_Low_SPEED=29;//25

uint16_t ROTATE_LOW_SPEED=30;
extern int MySecond1,MySecond2;
int YAW_Angle_Point=0; //YAW轴目标角度
int pwm1,pwm2,pwm3,pwm4;

Path_Plan pathFlag[15];
int finish_flag=0;
extern angle Angle; 
extern Encoder encoder;
extern Hc_Sr04 hc_sr04[7];

extern PIDStruct Motor1_SpeedPID;
extern PIDStruct Motor2_SpeedPID;  
extern PIDStruct Motor3_SpeedPID;
extern PIDStruct Motor4_SpeedPID;

extern PIDStruct Yaw_PID;
extern PIDStruct HCSR_PID;


/*-------------------------------------- 路径规划部分函数 --------------------------------------*/
 void Path_Init(void)
{
	for(int i=0; i<=15; i++)
	{
		pathFlag[i].FlagGet = 0;
		pathFlag[i].FlagFinish = 0;
	}
}

void Path_All_Deal(void)
{
	if(Path_Zero_Deal())
	{
		if(Path_One_Deal())
		{
			if(Path_Two_Deal())
			{
				if(Path_Three_Deal())
				{
					if(Path_Four_Deal())
					{
						if(Path_Five_Deal())
						{
							if(Path_Six_Deal())
							{
								if(Path_Seven_Deal())
								{
									if(Path_Eight_Deal())
									{
										if(Path_Nine_Deal())
										{
											if(Path_Ten_Deal())
											{
												if(Path_Eleven_Deal())
												{
													if(Path_Twelve_Deal())
													{
														Path_Plan_Finish();
													}
												}
											}
										}
									}
								}
							}
						}
					}
				}
			}
		}
  }
}

uint8_t Path_Zero_Deal(void)
{
	if(!pathFlag[0].FlagFinish)
	{
		if(HCSR_LU < 300 && HCSR_RU < 300 )
		{
			pathFlag[0].FlagGet = 1;
		}
		if(!pathFlag[0].FlagGet)
		{
			lineSpeed=LINE_High_SPEED;
			YawPID_Calc();
		}
		if(pathFlag[0].FlagGet==1 && HCSR_LU < 300 )
		{		
		  finish_flag=0;
			pathFlag[0].FlagFinish = 1;
			return 1;
		}
		return 0;
	}
	return 1;
}


uint8_t Path_One_Deal(void)
{
	if(!pathFlag[1].FlagFinish)
	{
		if(HCSR_U < 300 && HCSR_RU > 300 &&finish_flag )//290:70
		{
			if(!pathFlag[1].FlagGet)
			{
				Angle.flag=Turn_right_90;
				Angle_change();
				Motor_Stop();
				Picking_Up();
				LED_On;
				HAL_Delay(100);
			}
			Rotate_Right();
			pathFlag[1].FlagGet = 1;
		}
		if(!pathFlag[1].FlagGet)
		{
			if(HCSR_U < 750)
			{
			 lineSpeed=LINE_Middle_SPEED;
			}
      else			
			 lineSpeed=LINE_High_SPEED;
			 HCSRPID_Calc();
			if(HCSR_LD < 300 && HCSR_LU < 300) 
			{
				finish_flag = 1;  
			}
//			if(HCSR_U < 800)
//			{
//			 lineSpeed=LINE_Middle_SPEED;
//			}
		}
		if(pathFlag[1].FlagGet==1 && Angle.car>=-Angel_Point && Angle.car<=Angel_Point )
		{		
		  finish_flag=0;
			pathFlag[1].FlagFinish = 1;
		 	LED_Off;
			printf("car.Angle=%d",Angle.car);
			return 1;
		}
		return 0;
	}
	return 1;
}



uint8_t Path_Two_Deal(void)
{
	if(!pathFlag[2].FlagFinish)
	{
		if(HCSR_U < AF_Distance_Point  && HCSR_RU > 400 && finish_flag )
		{
			if(!pathFlag[2].FlagGet)
			{
			  Angle.flag=Back;
				Angle_change();
				Picking_Up();
		 	  LED_On;
			}
			Rotate_Right();
			pathFlag[2].FlagGet = 1;
		}
		if(!pathFlag[2].FlagGet)
		{
			Go_Stright_line();
      if(HCSR_RD > 300 )
			{
				OpenMV_control_R();
			} 							
			if(HCSR_RD < 300 && HCSR_RU < 300 && HCSR_LD < 300 && HCSR_LU < 300) 
			{
			 finish_flag = 1;
			 OpenMV_control_R(); 
			}
		}
		if(pathFlag[2].FlagGet==1 &&  Angle.car <=Angel_Point && Angle.car >=-Angel_Point )
		{
			finish_flag=0;
			pathFlag[2].FlagFinish = 1;
      LED_Off;		
			printf("car.Angle=%d",Angle.car);
			lineSpeed=LINE_Low_SPEED;
			return 1;
		}
		return 0;
	}
	return 1;
}


uint8_t Path_Three_Deal(void)
{
	if (!pathFlag[3].FlagFinish)
	{
		if(HCSR_RU > 400 && HCSR_RD > 400 && finish_flag==1)
		{
			if(!pathFlag[3].FlagGet)
			{
				Angle.flag=Turn_left_90;
				Angle_change();
				Picking_Up();
				LED_On;
			}
			Rotate_Right();
			pathFlag[3].FlagGet = 1;
		}
		if(!pathFlag[3].FlagGet)
		{
			Go_Stright_Low_line();
			if(HCSR_RD < 300 && HCSR_RU < 300 && HCSR_LD < 300 && HCSR_LU < 300)
			{
        finish_flag = 1;
			}
		}
		if(pathFlag[3].FlagGet==1 && Angle.car >=-Angel_Point && Angle.car <=Angel_Point )
		{
      finish_flag=0;
			pathFlag[3].FlagFinish = 1;
		 	LED_Off;
			printf("car.Angle=%d",Angle.car);
			//Picking_Go();
			lineSpeed=LINE_Middle_SPEED;
			return 1;
		}
		return 0;
	}
	return 1;
}


uint8_t Path_Four_Deal(void)
{
	if (!pathFlag[4].FlagFinish )
	{			
		if(HCSR_U < AF_Distance_Point  && HCSR_RU > 400 && finish_flag==1)
		{		
			if(!pathFlag[4].FlagGet)
			{
				Angle.flag=Back;
				Angle_change();
				Picking_Up();
		 	  LED_On;
			}
			Rotate_Left();
			pathFlag[4].FlagGet = 1;		
		}
		if(!pathFlag[4].FlagGet)
		{
		  Go_Stright_line();
			if(HCSR_RD > 300 &&  HCSR_LD > 300 )
			{
				OpenMV_control();
			}
			if(HCSR_RD < 300 && HCSR_RU < 300 && HCSR_LD < 300 && HCSR_LU < 300)
			{		
        finish_flag = 1;
				OpenMV_control();
			} 
		}
		if(pathFlag[4].FlagGet==1 && Angle.car >=-Angel_Point && Angle.car <=Angel_Point )
		{
			finish_flag=0;
			pathFlag[4].FlagFinish = 1;
		 	LED_Off;
			printf("car.Angle=%d",Angle.car);
			lineSpeed=LINE_Low_SPEED;
		  return 1;
		}
		return 0;
	}
	return 1;
}


uint8_t Path_Five_Deal(void)
{
	if(!pathFlag[5].FlagFinish)
	{
		if(HCSR_LU > 400 && HCSR_LD > 400 && finish_flag==1)
		{
			if(!pathFlag[5].FlagGet)
			{
				Angle.flag=Turn_right_90;
				Angle_change();
				Picking_Up();
		 	  LED_On;
			}
			Rotate_Left();
			pathFlag[5].FlagGet = 1;
		}
		if(!pathFlag[5].FlagGet)
		{
			Go_Stright_line();
			if(HCSR_RD < 300 && HCSR_RU < 300 && HCSR_LD < 300 && HCSR_LU < 300)
			{
        finish_flag = 1;
			} 
		}
		if(pathFlag[5].FlagGet && Angle.car >=-Angel_Point && Angle.car <=Angel_Point )
		{
			finish_flag=0;
			pathFlag[5].FlagFinish = 1;
		 	LED_Off;
			printf("car.Angle=%d",Angle.car);
			//Picking_Go();
			lineSpeed=LINE_Middle_SPEED;
			YAW_Angle_Point=-1; 
			return 1;
		}
		return 0;
	}
	return 1;
}


uint8_t Path_Six_Deal(void)
{
	if(!pathFlag[6].FlagFinish)
	{
		if(HCSR_U < AF_Distance_Point && HCSR_RU > 400 && finish_flag==1)
		{
			if(!pathFlag[6].FlagGet)
			{
			 Angle.flag=Back;
			 Angle_change();
			 Picking_Up();
		 	 LED_On;
			}
			Rotate_Right();
			pathFlag[6].FlagGet = 1;
		}
		if(!pathFlag[6].FlagGet)
		{
		  Go_Stright_line();
			if(HCSR_RD > 300 &&  HCSR_LD > 300 )
			{
				OpenMV_control();
			}
			if(HCSR_RD < 300 && HCSR_RU < 300 && HCSR_LD < 300 && HCSR_LU < 300)
			{
				finish_flag = 1;
				OpenMV_control(); 
			} 
		}
		if(pathFlag[6].FlagGet  && Angle.car >=-Angel_Point && Angle.car <=Angel_Point )
		{
			finish_flag=0;
			pathFlag[6].FlagFinish = 1;
		 	LED_Off;
			printf("car.Angle=%d",Angle.car);
			lineSpeed=LINE_Low_SPEED;
		  return 1;
		}
		return 0;
	}
	return 1;
}


uint8_t Path_Seven_Deal(void)
{
	if (!pathFlag[7].FlagFinish)
	{
		if(HCSR_RU > 400 && HCSR_RD > 400 && finish_flag==1)
		{			
			if(!pathFlag[7].FlagGet)
			{
			 Angle.flag=Turn_left_90 ;
			 Angle_change();
			 Picking_Up();
		 	 LED_On;
			}
			Rotate_Right();
			pathFlag[7].FlagGet = 1;
		}
		if(!pathFlag[7].FlagGet)
		{
			 Go_Stright_Low_line();
			if(HCSR_RD < 300 && HCSR_RU < 300 && HCSR_LD < 300 && HCSR_LU < 300)
			{
        finish_flag = 1;
			} 
		}
		if(pathFlag[7].FlagGet && Angle.car >=-Angel_Point && Angle.car <=Angel_Point )
		{
			finish_flag=0;
			pathFlag[7].FlagFinish = 1;
		 	LED_Off;
			printf("car.Angle=%d",Angle.car);
			//Picking_Go();
			lineSpeed=LINE_Middle_SPEED;
			YAW_Angle_Point=-2;  
			return 1;
		}
		return 0;
	}
	return 1;
}



uint8_t Path_Eight_Deal(void)
{
	if (!pathFlag[8].FlagFinish )
	{			
		if(HCSR_U < AF_Distance_Point  && HCSR_RU > 400 && finish_flag)
		{
			if(!pathFlag[8].FlagGet)
			{
			 Angle.flag=Back  ;
			 Angle_change();
			 Picking_Up();
			 LED_On;
			}
			Rotate_Left();
			pathFlag[8].FlagGet = 1;		
		}
		if(!pathFlag[8].FlagGet)
		{
			Go_Stright_line();
			if(HCSR_RD > 300 &&  HCSR_LD > 300 )
			{
				OpenMV_control();
			}
			if(HCSR_RD < 300 && HCSR_RU < 300 && HCSR_LD < 300 && HCSR_LU < 300)
			{
				OpenMV_control();
        finish_flag = 1;
			} 
		}
		if(pathFlag[8].FlagGet && Angle.car >=-Angel_Point && Angle.car <=Angel_Point )
		{
			finish_flag=0;
			pathFlag[8].FlagFinish = 1;
		 	LED_Off;
			printf("car.Angle=%d",Angle.car);
			lineSpeed=LINE_Low_SPEED;
			return 1;
		}
		return 0;
	}
	return 1;
}



uint8_t Path_Nine_Deal(void)
{
	if(!pathFlag[9].FlagFinish)
	{
		if(HCSR_LU > 400 && HCSR_LD > 400 && finish_flag)
		{
			if(!pathFlag[9].FlagGet)
			{
			 Angle.flag=Turn_right_90   ;
			 Angle_change();
			 Picking_Up();
			 LED_On;
			}
			Rotate_Left();
			pathFlag[9].FlagGet = 1;
		}
		if(!pathFlag[9].FlagGet)
		{
			Go_Stright_line();
			if(HCSR_RD < 300 && HCSR_RU < 300 && HCSR_LD < 300 && HCSR_LU < 300)
			{
        finish_flag = 1;
			} 
		}
		if(pathFlag[9].FlagGet && Angle.car >=-Angel_Point && Angle.car <=Angel_Point)
		{
			finish_flag=0;
			pathFlag[9].FlagFinish = 1;
		 	LED_Off;
			printf("car.Angle=%d",Angle.car);
			//Picking_Go();
			lineSpeed=LINE_Middle_SPEED;
			return 1;
		}
		return 0;
	}
	return 1;
}



uint8_t Path_Ten_Deal(void)
{
	if(!pathFlag[10].FlagFinish)
	{
		if(HCSR_U < AF_Distance_Point && HCSR_RU > 400 &&  finish_flag)
		{
			if(!pathFlag[10].FlagGet)
			{
			 Angle.flag=Back;
			 Angle_change();
			 Picking_Up();
			 LED_On;
			}
			Rotate_Right();
			pathFlag[10].FlagGet = 1;
		}
		if(!pathFlag[10].FlagGet)
		{
			Go_Stright_line();
			if(HCSR_RD > 300 &&  HCSR_LD > 300 )
			{
				OpenMV_control();
			}
			if(HCSR_RD < 300 && HCSR_RU < 300 && HCSR_LD < 300 && HCSR_LU < 300)
			{
				OpenMV_control();
        finish_flag = 1;
			} 	
		}
		if(pathFlag[10].FlagGet  && Angle.car >=-Angel_Point && Angle.car <=Angel_Point)
		{
      finish_flag=0;
			pathFlag[10].FlagFinish = 1;
		 	LED_Off;
			printf("car.Angle=%d",Angle.car);
			lineSpeed=LINE_Low_SPEED;
			YAW_Angle_Point=-3;	
			return 1;
		}
		return 0;
	}
	return 1;
}


uint8_t Path_Eleven_Deal(void)
{
	if (!pathFlag[11].FlagFinish)
	{
		if(HCSR_U < AF_Distance_Point && HCSR_RD > 400 && finish_flag)
		{
			if(!pathFlag[11].FlagGet)
			{
			 Angle.flag=Turn_left_90;
			 Angle_change();
			 Picking_Up();
			 LED_On;
			}
			Rotate_Right();
			pathFlag[11].FlagGet = 1;
		}
		if(!pathFlag[11].FlagGet)
		{
			Go_Stright_Low_line();
			if(HCSR_RD < 300 && HCSR_RU < 300 && HCSR_LD < 300 && HCSR_LU < 300)
			{
        finish_flag = 1;
			} 	
		}
		if(pathFlag[11].FlagGet && Angle.car >=-Angel_Point && Angle.car <=Angel_Point)
		{
		  finish_flag=0;
			pathFlag[11].FlagFinish = 1;
		 	LED_Off;
			printf("car.Angle=%d",Angle.car);
			//Picking_Go();
      lineSpeed=LINE_Middle_SPEED;			
			return 1;
		}
		return 0;
	}
	return 1;
}


uint8_t Path_Twelve_Deal(void)
{
	if (!pathFlag[12].FlagFinish )
	{			
		if(HCSR_U < AF_Distance_Point  && HCSR_LU > 400 && finish_flag)
		{	
			if(!pathFlag[12].FlagGet)
			{
			 Angle.flag=Back ;
			 Angle_change();
			 Picking_Up();
			 LED_On;	
			}
			Rotate_Left();
			pathFlag[12].FlagGet = 1;		
		}
		if(!pathFlag[12].FlagGet)
		{
			Go_Stright_line();
			if(HCSR_RD > 300 &&  HCSR_LD > 300 )
			{
				OpenMV_control_R();
			}
			if(HCSR_RD < 300 && HCSR_RU < 300 && HCSR_LD < 300 && HCSR_LU < 300)
			{
				OpenMV_control_R();
        finish_flag = 1;
			} 				
		}
		if(pathFlag[12].FlagGet && Angle.car >=-Angel_Point && Angle.car <=Angel_Point )
		{
			finish_flag=0;
			pathFlag[12].FlagFinish = 1;
		 	LED_Off;
			printf("car.Angle=%d",Angle.car);
			return 1;
		}
		return 0;
	}
	return 1;
}


void Path_Plan_Finish(void)
{
	if(HCSR_LD > 400 && HCSR_LU > 400 && HCSR_RU > 400 && HCSR_RD > 400)
	{	
	  LED_On;
		HAL_TIM_Base_Stop_IT(&htim6);
		endflag = 1;
		while(endflag)
		{
      Set_Pwm(0,0,0,0);
      LED_Slow_Flashing(1);			
		}			
 	}
	else 
	  Go_Stright_line();  
}


/*-------------------------------------- 控制部分 --------------------------------------*/
/**
  * @brief  左转函数
  * @param  无
  * @retval 无
  */
void Rotate_Left(void)
{
	Motor1_SpeedPID.SetPoint = -ROTATE_LOW_SPEED;
	Motor2_SpeedPID.SetPoint = ROTATE_LOW_SPEED;
	Motor3_SpeedPID.SetPoint = -ROTATE_LOW_SPEED;  
	Motor4_SpeedPID.SetPoint = ROTATE_LOW_SPEED;
}
/**
  * @brief  右转函数
  * @param  无
  * @retval 无
  */
void Rotate_Right(void)
{
	Motor1_SpeedPID.SetPoint = ROTATE_LOW_SPEED;
	Motor2_SpeedPID.SetPoint = -ROTATE_LOW_SPEED;
	Motor3_SpeedPID.SetPoint = ROTATE_LOW_SPEED;
	Motor4_SpeedPID.SetPoint = -ROTATE_LOW_SPEED;
}

/**
  * @brief  电机锁死函数
  * @param  无
  * @retval 无
  */
void Motor_Stop(void)
{
	Motor1_SpeedPID.SetPoint = 0;
	Motor2_SpeedPID.SetPoint = 0;
	Motor3_SpeedPID.SetPoint = 0;
	Motor4_SpeedPID.SetPoint = 0;
}

/**
  * @brief  小段直线PID计算
  * @param  lineSpeed 直线速度
  * @retval 无
  */
void Go_Stright_Low_line(void)
{
	LED_Off;
	//位置环闭环	
	Position_HCSR_PID_Calc(&HCSR_PID,HCSR_Distance_Point,HCSR_LU,HCSR_Error_Value,50);
	//YAW轴外环PID闭环
	Position_YAW_PID_Calc(&Yaw_PID,YAW_Angle_Point,Angle.car,Yaw_Error_Value,20);
	Mecanum_Drive(HCSR_PID.PWMSum , lineSpeed , Yaw_PID.PWMSum);						
  	
}

/**
  * @brief  直线PID计算
  * @param  lineSpeed 直线速度
  * @retval 无
  */
void Go_Stright_line(void)
{
	  if( HCSR_U <= 400)
		{
		  lineSpeed=40;
		}
  	if(HCSR_LD < 300 && HCSR_LU < 300 && HCSR_RD < 300 && HCSR_RU < 300)
	  {	
			LED_Off;
  		//位置环闭环	
			Position_HCSR_PID_Calc(&HCSR_PID,HCSR_Distance_Point,HCSR_LU,HCSR_Error_Value,50);
			//YAW轴外环PID闭环
			Position_YAW_PID_Calc(&Yaw_PID,YAW_Angle_Point,Angle.car,Yaw_Error_Value,20);
		  Mecanum_Drive(HCSR_PID.PWMSum , lineSpeed , Yaw_PID.PWMSum);						
  	}
	else 
		{	
		  //YAW轴外环PID闭环
			Position_YAW_PID_Calc(&Yaw_PID,YAW_Angle_Point,Angle.car,Yaw_Error_Value,20);
			Mecanum_Drive(0 , lineSpeed , Yaw_PID.PWMSum);
			LED_On;
	  }		
}

/**
  * @brief  Mecanum各轴速度合成
  * @param  x 平移轴速度
	* @param  y 直行轴速度
	* @param  z 旋转轴速度
  * @retval 无
  */
void Mecanum_Drive(int x, int y, int z)
{
	Motor1_SpeedPID.SetPoint = y+(0.3*x)+(0.5*z);
	Motor2_SpeedPID.SetPoint = y-(0.3*x)-(0.5*z);
	Motor3_SpeedPID.SetPoint = y-(0.3*x)+(0.5*z);
	Motor4_SpeedPID.SetPoint = y+(0.3*x)-(0.5*z);
}

/**
  * @brief  速度闭环PID(内环)
  * @param  无
  * @retval 无
  */
void SpeedPID_Calc(void)
{
	Encoder_Update(&encoder);
	//PID速度内环计算
	Inc_PID_Calc(&Motor1_SpeedPID,Motor1_SpeedPID.SetPoint,encoder.encoder_LU,Speed_Error_Value,PWM_max);
	Inc_PID_Calc(&Motor2_SpeedPID,Motor2_SpeedPID.SetPoint,encoder.encoder_RU,Speed_Error_Value,PWM_max);
	Inc_PID_Calc(&Motor3_SpeedPID,Motor3_SpeedPID.SetPoint,encoder.encoder_LD,Speed_Error_Value,PWM_max);
	Inc_PID_Calc(&Motor4_SpeedPID,Motor4_SpeedPID.SetPoint,encoder.encoder_RD,Speed_Error_Value,PWM_max);
	//更新电机PWM
	pwm1 = Motor1_SpeedPID.PWMSum;
	pwm2 = Motor2_SpeedPID.PWMSum;
	pwm3 = Motor3_SpeedPID.PWMSum;
	pwm4 = Motor4_SpeedPID.PWMSum;
	
	Set_Pwm(pwm1,pwm2,pwm3,pwm4);
}


/*-------------------------------------- 调试所用函数 --------------------------------------*/
/**
  * @brief  HCSR位置环PID计算（调试）【直线】
  * @param  无
  * @retval 无
  */
void HCSRPID_Calc(void)
{
	Position_HCSR_PID_Calc(&HCSR_PID,HCSR_Distance_Point,HCSR_LD,HCSR_Error_Value,50);
	Position_YAW_PID_Calc(&Yaw_PID,YAW_Angle_Point,Angle.car,Yaw_Error_Value,20);
	Mecanum_Drive(HCSR_PID.PWMSum , lineSpeed , Yaw_PID.PWMSum);
}

/**
  * @brief  YAW轴PID计算（调试）【直线】
  * @param  无
  * @retval 无
  */
void YawPID_Calc(void)
{
	Position_YAW_PID_Calc(&Yaw_PID,YAW_Angle_Point,Angle.car,Yaw_Error_Value,20);
	Mecanum_Drive(0, lineSpeed ,Yaw_PID.PWMSum);
}

/**
  * @brief  PID调试函数（内容不定）
  * @param  无
  * @retval 无
  */
void PidDebugFunc(void)
{
	Position_HCSR_PID_Calc(&HCSR_PID,HCSR_Distance_Point,HCSR_LD,HCSR_Error_Value,50);
	Position_YAW_PID_Calc(&Yaw_PID,YAW_Angle_Point,Angle.car,Yaw_Error_Value,20);
	Mecanum_Drive(HCSR_PID.PWMSum,lineSpeed,Yaw_PID.PWMSum);
}


/*-------------------------------------- debug --------------------------------------*/

void Debug_Picking(void)
{		
   Go_Stright_line();
//	 if(HCSR_U < 300)
//		{
//		   lineSpeed=0;
//		}
  if(HAL_GPIO_ReadPin(GPIOA,OpenMV_RX2_Pin)==1) //串口2RX口  
		{ 
			MySecond1 = 0;
			Sevor_1_PWM=202;                  //右边
			LED_On ;
			lineSpeed=LINE_Low_SPEED;
		}

	if(HAL_GPIO_ReadPin(GPIOA,OpenMV_RX1_Pin)==1) //串口2TX口
		{		
		 MySecond2 = 0;
		 Sevor_2_PWM=98;                    //左边
		 LED_On ;
		 lineSpeed=LINE_Low_SPEED;
		 }
  if(MySecond1 == 3)
		{
			Sevor_1_PWM=100; 
		  LED_Off ;
		  lineSpeed=0;
		}
	if(MySecond2 == 3 )
		{
		 Sevor_2_PWM=200;
		 LED_Off ;
		 lineSpeed=0;
		}
}
