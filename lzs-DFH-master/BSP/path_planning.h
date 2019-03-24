#ifndef __PATH_PLANNING_H
#define __PATH_PLANNING_H
#include "stm32f4xx_hal.h"


#define Yaw_Error_Value (1.0f) //角度容差
#define HCSR_Error_Value (5.0f) //距离容差mm
#define Speed_Error_Value (0)//速度容差
#define HCSR_Distance_Point (120)//目标距离mm

#define AF_Distance_Point (225)//预刹车距离mm
#define Angel_Point (32)//目标角度


#define PWM_max 8000

typedef volatile struct __Path_Plan
{
	uint8_t FlagGet;
	uint8_t FlagFinish;
}Path_Plan;

extern int pwm1,pwm2,pwm3,pwm4;
extern float lineYaw;
extern uint8_t endflag;


/*-------------------------------------- 路径规划相关函数 --------------------------------------*/
void Path_Init(void);
void Path_All_Deal(void);
/*-------------------------------------- 相关的PID计算及底层控制函数 --------------------------------------*/

void Rotate_Left(void);
void Rotate_Right(void);
void Motor_Stop(void);
void Go_Stright_line(void);
void Mecanum_Drive(int x, int y, int z);
void SpeedPID_Calc(void);
void Go_Stright_Low_line(void);
	
/*-------------------------------------- 调试所用函数 --------------------------------------*/
void YawPID_Calc(void);
void HCSRPID_Calc(void);
void PidDebugFunc(void);
void Debug_Picking(void);
/*-------------------------------------- 路径规划函数 --------------------------------------*/
void Path_All_Deal(void);
uint8_t Path_Zero_Deal(void);
uint8_t Path_One_Deal(void);
uint8_t Path_Two_Deal(void);
uint8_t Path_Three_Deal(void);
uint8_t Path_Four_Deal(void);
uint8_t Path_Five_Deal(void);
uint8_t Path_Six_Deal(void);
uint8_t Path_Seven_Deal(void);
uint8_t Path_Eight_Deal(void);
uint8_t Path_Nine_Deal(void);
uint8_t Path_Ten_Deal(void);
uint8_t Path_Eleven_Deal(void);
uint8_t Path_Twelve_Deal(void);
void Path_Plan_Finish(void);
void PathPlanStruct_Init(void);
#endif
