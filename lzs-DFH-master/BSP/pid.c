#include "pid.h"

//速度闭环PID结构体
PIDStruct Motor1_SpeedPID;
PIDStruct Motor2_SpeedPID;
PIDStruct Motor3_SpeedPID;
PIDStruct Motor4_SpeedPID;

//直线闭环结构体
PIDStruct Yaw_PID;
//超声波位置闭环结构体
PIDStruct HCSR_PID;


/**
  * @brief  增量式PID计算函数
  * @param  pidStuc pid结构体指针
  * @param  currentPoint 传感器采集值
  * @param  limieValue 输出限幅
  * @param  acceptError 误差
  * @retval 无
  */
void Inc_PID_Calc(PIDStruct * pidStuc,int SetPoint,float currentPoint,float acceptError, int limitValue)
{
	float nowError,outPut;
	pidStuc->SetPoint = SetPoint;
	nowError = pidStuc->SetPoint - currentPoint;	
	//死区控制
	if(-acceptError <= nowError  && nowError <= acceptError)
	{
		outPut = 0;	
	}
	else 
	{
		outPut = (pidStuc->Proportion * (nowError - pidStuc->LastError))+ \
					   (pidStuc->Integral * nowError)+ \
	           (pidStuc->Derivative * (nowError - 2*pidStuc->LastError + pidStuc->PrevError));
		pidStuc->PrevError = pidStuc->LastError;
		pidStuc->LastError = nowError;
	}
	pidStuc->PWMSum +=(int)outPut;
	
	//输出限幅
	if(pidStuc->PWMSum >= limitValue)
	{
		pidStuc->PWMSum = limitValue;
	}else if (pidStuc->PWMSum <=   (-limitValue))
	{
		pidStuc->PWMSum = -limitValue;
	}
}

/**
  * @brief  电机速度位置式PID计算函数
  * @param  pidStuc pid结构体指针
  * @param  currentPoint 传感器采集值
  * @param  limieValue 输出限幅
  * @param  acceptError 误差
  * @retval 无
  */
void Position_Motor_PID_Calc(PIDStruct * pidStuc,int SetPoint,float currentPoint,float acceptError, int limitValue)
{
	static float Bias,Last_bias,Integral_bias,outPut;
	pidStuc->SetPoint = SetPoint;
	Bias = pidStuc->SetPoint - currentPoint;//求偏差
  Integral_bias += Bias;	//对偏差积分
	//死区控制
	if(-acceptError <= Bias  && Bias <= acceptError)
	{
		outPut = 0;	
	}
	else 
	{
		outPut = (pidStuc->Proportion * Bias)+ \
					   (pidStuc->Integral * Integral_bias)+ \
	           (pidStuc->Derivative * (Bias - Last_bias));
		Last_bias = Bias;
	}
	pidStuc->PWMSum =(int)outPut;
	
	//输出限幅
	if(pidStuc->PWMSum >= limitValue)
	{
		pidStuc->PWMSum = limitValue;
	}else if (pidStuc->PWMSum <=   (-limitValue))
	{
		pidStuc->PWMSum = -limitValue;
	}
}

/**
  * @brief  YAW轴位置式PID计算函数
  * @param  pidStuc pid结构体指针
  * @param  currentPoint 传感器采集值
  * @param  limieValue 输出限幅
  * @param  acceptError 误差
  * @retval 无
  */
void Position_YAW_PID_Calc(PIDStruct * pidStuc,int SetPoint,float currentPoint,float acceptError, int limitValue)
{
	static float Bias,Last_bias,Integral_bias,outPut;
	pidStuc->SetPoint = SetPoint;
	Bias = pidStuc->SetPoint - currentPoint;//求偏差
  Integral_bias += Bias;	//对偏差积分
	//死区控制
	if(-acceptError <= Bias  && Bias <= acceptError)
	{
		outPut = 0;	
	}
	else 
	{
		outPut = (pidStuc->Proportion * Bias)+ \
					   (pidStuc->Integral * Integral_bias)+ \
	           (pidStuc->Derivative * (Bias - Last_bias));
		Last_bias = Bias;
	}
	pidStuc->PWMSum =(int)outPut;
	
	//输出限幅
	if(pidStuc->PWMSum >= limitValue)
	{
		pidStuc->PWMSum = limitValue;
	}else if (pidStuc->PWMSum <=   (-limitValue))
	{
		pidStuc->PWMSum = -limitValue;
	}
}

/**
  * @brief  HCSR轴位置式PID计算函数
  * @param  pidStuc pid结构体指针  
  * @param  currentPoint 传感器采集值
  * @param  limieValue 输出限幅
  * @param  acceptError 误差
  * @retval 无
  */
void Position_HCSR_PID_Calc(PIDStruct * pidStuc,int SetPoint,float currentPoint,float acceptError, int limitValue)
{
	static float Bias,Last_bias,Integral_bias,outPut;
	pidStuc->SetPoint = SetPoint;
	Bias = pidStuc->SetPoint - currentPoint;//求偏差
  Integral_bias += Bias;	//对偏差积分
	//死区控制
	if(-acceptError <= Bias  && Bias <= acceptError)
	{
		outPut = 0;	
	}
	else 
	{
		outPut = (pidStuc->Proportion * Bias)+ \
					   (pidStuc->Integral * Integral_bias)+ \
	           (pidStuc->Derivative * (Bias - Last_bias));
		Last_bias = Bias;
	}
	pidStuc->PWMSum =(int)outPut;
	
	//输出限幅
	if(pidStuc->PWMSum >= limitValue)
	{
		pidStuc->PWMSum = limitValue;
	}else if (pidStuc->PWMSum <=   (-limitValue))
	{
		pidStuc->PWMSum = -limitValue;
	}
}
/**
  * @brief  PID结构体初始化函数
  * @param  pidStuc PID结构体指针
  * @param  SetPoint 目标值
  * @retval 无
  */
void PID_Init(PIDStruct * pidStuc, float kp, float ki, float kd)
{
	pidStuc->PWMSum = 0;
	pidStuc->Proportion = kp;
	pidStuc->Integral = ki;
	pidStuc->Derivative = kd;
	pidStuc->LastError = 0;
	pidStuc->PrevError = 0;
}


/**
  * @brief  PID设定目标值
  * @param  pidStuc PID结构体指针
  * @param  setPoint 目标值
  * @retval 无
  */

void PID_SetPoint(PIDStruct * pidStuc, int SetPoint)
{
	pidStuc->SetPoint = SetPoint;
}
