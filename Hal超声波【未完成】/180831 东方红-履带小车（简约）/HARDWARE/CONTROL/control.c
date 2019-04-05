#include "sys.h"
#include "delay.h"
#include "delay.h"
#include "key.h"
#include "sys.h"
#include "usart.h"
#include "echo.h"
#include "duoji.h"
#include "motor.h"
#include "control.h"

#define Echo1_Kp  1.1
#define Echo1_Ki  0
#define Echo1_Kd  0.16f           //【0.1】

#define Echo2_Kp  5
#define Echo2_Ki  0
#define Echo2_Kd  0.8      //【0.1】

#define Echo3_Kp  1.1
#define Echo3_Ki  0
#define Echo3_Kd  1.8f           //【0.1】

#define SERVO_INIT1 1720
#define SERVO_INIT2 1690
#define SERVO_INIT3 1710
#define K 10

float Velocity_KP=12,Velocity_KI=12;	       //速度控制PID参数
extern float  length[5]; 
extern int side;
float  Echo_pwm,duoji_pwm,distance;


void Echo_pid1(float dis_front,float dis_after)
{
		static float value;
		static float Integral; //积分常数 Integral Const   
    static float Error; //Error[-1]   
		static float LastError; //Error[-1]   
		static int target;
		static int count1;
		static int count2;
	
		value=dis_front-dis_after;   							////////计算两个超声波通道的差值；
		Error=value-target; 											//////本次偏差值；
//		Integral+=Error;													/////本次及以前的偏差值之和；
//		if(Integral>10000) Integral=10000;             //===积分限幅
//		if(Integral<-10000)	Integral=-10000;              //===积分限幅	
//		Echo_pwm=Echo_Kp*(Error-LastError)+Echo_Kd*Error;  //===计算平衡声波控制的模拟值--【增量式】PD控制，kp是P系数 kd是D系数  
		Echo_pwm=Echo1_Kp*Error+Echo1_Kd*(Error-LastError);   //===计算平衡声波控制的模拟值--【位置式】PD控制，kp是P系数 kd是D系数 
		printf ("Echo_pwm的值: %f\r\n",Echo_pwm);	
	
		duoji_pwm=SERVO_INIT1-Echo_pwm*K;
		xianfu_pwm(duoji_pwm);
		printf ("经过PID1处理的PWM的值: %d\r\n",(int)duoji_pwm);	
		TIM_SetCompare1(TIM5,(int)duoji_pwm);	
	
		if(Echo_pwm<=0.05&&Echo_pwm>=-0.05)count1++; 
		if(duoji_pwm<=1750&&duoji_pwm>=1660)count2++; 
		printf ("成功1次数: %d",count1);	
		printf ("      成功2次数: %d\r\n",count2);	
	
}


void Echo_pid2(float target_distance)
{
		static float value;
		static float Integral; //积分常数 Integral Const   
    static float Error; //Error[-1]   
		static float LastError; //Error[-1]   
		static int target=0;
	  static int count1;
		static int count2;
		printf("                  [3]:%.2f\r\n\r\n",length[2]); 
		if(side==1)	distance=length[2];			 	/////贴边左侧
		if(side==0) distance=length[0];         ////贴边右侧		
		Error=distance-target_distance;   							////////本次偏差值；
//		Integral+=Error;													/////本次及以前的偏差值之和；
//		if(Integral>10000) Integral=10000;             //===积分限幅
//		if(Integral<-10000)	Integral=-10000;              //===积分限幅	
//		Echo_pwm=Echo_Kp*(Error-LastError)+Echo_Kd*Error;  //===计算平衡声波控制的模拟值--【增量式】PD控制，kp是P系数 kd是D系数  
		Echo_pwm=Echo2_Kp*Error+Echo2_Kd*(Error-LastError);   //===计算平衡声波控制的模拟值--【位置式】PD控制，kp是P系数 kd是D系数 
		//printf ("Echo_pwm的值: %f\r\n",Echo_pwm);	
	
		if(side==1)duoji_pwm=SERVO_INIT2-Echo_pwm*K;
		if(side==0)	duoji_pwm=SERVO_INIT2+Echo_pwm*K;
		xianfu_pwm(duoji_pwm);	
		//printf ("经过PID2处理的PWM的值: %d\r\n",(int)duoji_pwm);	
		TIM_SetCompare1(TIM5,(int)duoji_pwm);	
	
	
		if(Echo_pwm<=0.05&&Echo_pwm>=-0.05)count1++; 
		if((length[2]<=target_distance+1)&&(length[2]>=target_distance-1))count2++; 
//		printf ("成功1次数: %d",count1);	
//		printf ("      成功2次数: %d\r\n",count2);
	
}


void Echo_pid3(float dis_left,float dis_right)
{
    static float Error; //Error[-1]   
		static float LastError; //Error[-1]   
	  static int count1;
		static int count2;
	
		Error=dis_left-dis_right;   							////////本次偏差值；
//		Echo_pwm=Echo_Kp*(Error-LastError)+Echo_Kd*Error;  //===计算平衡声波控制的模拟值--【增量式】PD控制，kp是P系数 kd是D系数  
		Echo_pwm=Echo3_Kp*Error+Echo3_Kd*(Error-LastError);   //===计算平衡声波控制的模拟值--【位置式】PD控制，kp是P系数 kd是D系数 
		printf ("Echo_pwm的值: %f\r\n",Echo_pwm);	
	
		duoji_pwm=SERVO_INIT3-Echo_pwm*K;
		xianfu_pwm(duoji_pwm);
		printf ("经过PID3处理的PWM的值: %d\r\n",(int)duoji_pwm);	
		TIM_SetCompare1(TIM5,(int)duoji_pwm);	
	
		if(Echo_pwm<=0.05&&Echo_pwm>=-0.05)count1++; 
		if(duoji_pwm<=1780&&duoji_pwm>=1650)count2++; 
		printf ("成功1次数: %d",count1);	
		printf ("      成功2次数: %d\r\n",count2);
	
}
void xianfu_pwm(int pwm)								 ////函数功能：限幅保护舵机
{
	if(pwm<=1566)duoji_pwm=1566;               
	if(pwm>=1830)duoji_pwm=1830;
}


int PI_A (int Encoder,int Target)
{ 	
	 static int Bias,Pwm,Last_bias;
	 Bias=Target-Encoder;                //计算偏差
	 Pwm+=Velocity_KP*(Bias-Last_bias)+Velocity_KI*Bias;   //增量式PI控制器
	 Last_bias=Bias;	                   //保存上一次偏差 
	 return Pwm;                         //增量输出
}
int PI_B (int Encoder,int Target)
{ 	
	 static int Bias,Pwm,Last_bias;
	 Bias=Target-Encoder;                //计算偏差
	 Pwm+=Velocity_KP*(Bias-Last_bias)+Velocity_KI*Bias;   //增量式PI控制器
	 Last_bias=Bias;	                   //保存上一次偏差 
	 return Pwm;                         //增量输出
}
