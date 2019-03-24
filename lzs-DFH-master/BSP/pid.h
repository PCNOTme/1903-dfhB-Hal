#ifndef __PID_H
#define __PID_H
#include "stm32f4xx_hal.h"
#include "usart.h"

//速度环
#define PID_Motor1_KP 3.5       //1.5
#define PID_Motor1_KI 5.4  //0.7:2.884  0.6:2.472//3.8
#define PID_Motor1_KD 0.02

#define PID_Motor2_KP 3.5   //1.5
#define PID_Motor2_KI 5.4   //0.7:2.884  0.6:2.472//3.8
#define PID_Motor2_KD 0.01

#define PID_Motor3_KP 3.5   //1.5
#define PID_Motor3_KI 5.4   //0.7:2.884  0.6:2.472//3.8
#define PID_Motor3_KD 0.01

#define PID_Motor4_KP 3.5   //1.5
#define PID_Motor4_KI 5.4   //0.7:2.884  0.6:2.472//3.8
#define PID_Motor4_KD 0.01


//陀螺仪闭环的YAW环
#define PID_YAW_KP 1.299  //0.7:1.05  0.6:0.90
#define PID_YAW_KI 0    //0.3
#define PID_YAW_KD 0.18

//HCSR位置环
#define PID_HCSR_KP 0.5212    //0.3142    
#define PID_HCSR_KI 0         //0.00898  
#define PID_HCSR_KD 0.235     //0.72    


typedef volatile struct __PIDStruct
{
	int PWMSum;
	int SetPoint;
	float Proportion;
	float Integral;
	float Derivative;
	float LastError;
	float PrevError;
}PIDStruct;  

extern PIDStruct Motor1_SpeedPID;
extern PIDStruct Motor2_SpeedPID;
extern PIDStruct Motor3_SpeedPID;
extern PIDStruct Motor4_SpeedPID;

extern PIDStruct Yaw_PID;
extern PIDStruct HCSR_PID;
	

void PID_SetPoint(PIDStruct * pidStuc, int SetPoint);
		
void Inc_PID_Calc(PIDStruct * pidStuc,int SetPoint,float currentPoint,float acceptError,int limitValue);

void Position_Motor_PID_Calc(PIDStruct * pidStuc,int SetPoint,float currentPoint,float acceptError, int limitValue);

void Position_YAW_PID_Calc(PIDStruct * pidStuc,int SetPoint,float currentPoint,float acceptError, int limitValue);

void Position_HCSR_PID_Calc(PIDStruct * pidStuc,int SetPoint,float currentPoint,float acceptError, int limitValue);

void PID_Init(PIDStruct * pidStuc, float kp, float ki, float kd);

#endif
