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
#define Echo1_Kd  0.16f           //��0.1��

#define Echo2_Kp  5
#define Echo2_Ki  0
#define Echo2_Kd  0.8      //��0.1��

#define Echo3_Kp  1.1
#define Echo3_Ki  0
#define Echo3_Kd  1.8f           //��0.1��

#define SERVO_INIT1 1720
#define SERVO_INIT2 1690
#define SERVO_INIT3 1710
#define K 10

float Velocity_KP=12,Velocity_KI=12;	       //�ٶȿ���PID����
extern float  length[5]; 
extern int side;
float  Echo_pwm,duoji_pwm,distance;


void Echo_pid1(float dis_front,float dis_after)
{
		static float value;
		static float Integral; //���ֳ��� Integral Const   
    static float Error; //Error[-1]   
		static float LastError; //Error[-1]   
		static int target;
		static int count1;
		static int count2;
	
		value=dis_front-dis_after;   							////////��������������ͨ���Ĳ�ֵ��
		Error=value-target; 											//////����ƫ��ֵ��
//		Integral+=Error;													/////���μ���ǰ��ƫ��ֵ֮�ͣ�
//		if(Integral>10000) Integral=10000;             //===�����޷�
//		if(Integral<-10000)	Integral=-10000;              //===�����޷�	
//		Echo_pwm=Echo_Kp*(Error-LastError)+Echo_Kd*Error;  //===����ƽ���������Ƶ�ģ��ֵ--������ʽ��PD���ƣ�kp��Pϵ�� kd��Dϵ��  
		Echo_pwm=Echo1_Kp*Error+Echo1_Kd*(Error-LastError);   //===����ƽ���������Ƶ�ģ��ֵ--��λ��ʽ��PD���ƣ�kp��Pϵ�� kd��Dϵ�� 
		printf ("Echo_pwm��ֵ: %f\r\n",Echo_pwm);	
	
		duoji_pwm=SERVO_INIT1-Echo_pwm*K;
		xianfu_pwm(duoji_pwm);
		printf ("����PID1�����PWM��ֵ: %d\r\n",(int)duoji_pwm);	
		TIM_SetCompare1(TIM5,(int)duoji_pwm);	
	
		if(Echo_pwm<=0.05&&Echo_pwm>=-0.05)count1++; 
		if(duoji_pwm<=1750&&duoji_pwm>=1660)count2++; 
		printf ("�ɹ�1����: %d",count1);	
		printf ("      �ɹ�2����: %d\r\n",count2);	
	
}


void Echo_pid2(float target_distance)
{
		static float value;
		static float Integral; //���ֳ��� Integral Const   
    static float Error; //Error[-1]   
		static float LastError; //Error[-1]   
		static int target=0;
	  static int count1;
		static int count2;
		printf("                  [3]:%.2f\r\n\r\n",length[2]); 
		if(side==1)	distance=length[2];			 	/////�������
		if(side==0) distance=length[0];         ////�����Ҳ�		
		Error=distance-target_distance;   							////////����ƫ��ֵ��
//		Integral+=Error;													/////���μ���ǰ��ƫ��ֵ֮�ͣ�
//		if(Integral>10000) Integral=10000;             //===�����޷�
//		if(Integral<-10000)	Integral=-10000;              //===�����޷�	
//		Echo_pwm=Echo_Kp*(Error-LastError)+Echo_Kd*Error;  //===����ƽ���������Ƶ�ģ��ֵ--������ʽ��PD���ƣ�kp��Pϵ�� kd��Dϵ��  
		Echo_pwm=Echo2_Kp*Error+Echo2_Kd*(Error-LastError);   //===����ƽ���������Ƶ�ģ��ֵ--��λ��ʽ��PD���ƣ�kp��Pϵ�� kd��Dϵ�� 
		//printf ("Echo_pwm��ֵ: %f\r\n",Echo_pwm);	
	
		if(side==1)duoji_pwm=SERVO_INIT2-Echo_pwm*K;
		if(side==0)	duoji_pwm=SERVO_INIT2+Echo_pwm*K;
		xianfu_pwm(duoji_pwm);	
		//printf ("����PID2�����PWM��ֵ: %d\r\n",(int)duoji_pwm);	
		TIM_SetCompare1(TIM5,(int)duoji_pwm);	
	
	
		if(Echo_pwm<=0.05&&Echo_pwm>=-0.05)count1++; 
		if((length[2]<=target_distance+1)&&(length[2]>=target_distance-1))count2++; 
//		printf ("�ɹ�1����: %d",count1);	
//		printf ("      �ɹ�2����: %d\r\n",count2);
	
}


void Echo_pid3(float dis_left,float dis_right)
{
    static float Error; //Error[-1]   
		static float LastError; //Error[-1]   
	  static int count1;
		static int count2;
	
		Error=dis_left-dis_right;   							////////����ƫ��ֵ��
//		Echo_pwm=Echo_Kp*(Error-LastError)+Echo_Kd*Error;  //===����ƽ���������Ƶ�ģ��ֵ--������ʽ��PD���ƣ�kp��Pϵ�� kd��Dϵ��  
		Echo_pwm=Echo3_Kp*Error+Echo3_Kd*(Error-LastError);   //===����ƽ���������Ƶ�ģ��ֵ--��λ��ʽ��PD���ƣ�kp��Pϵ�� kd��Dϵ�� 
		printf ("Echo_pwm��ֵ: %f\r\n",Echo_pwm);	
	
		duoji_pwm=SERVO_INIT3-Echo_pwm*K;
		xianfu_pwm(duoji_pwm);
		printf ("����PID3�����PWM��ֵ: %d\r\n",(int)duoji_pwm);	
		TIM_SetCompare1(TIM5,(int)duoji_pwm);	
	
		if(Echo_pwm<=0.05&&Echo_pwm>=-0.05)count1++; 
		if(duoji_pwm<=1780&&duoji_pwm>=1650)count2++; 
		printf ("�ɹ�1����: %d",count1);	
		printf ("      �ɹ�2����: %d\r\n",count2);
	
}
void xianfu_pwm(int pwm)								 ////�������ܣ��޷��������
{
	if(pwm<=1566)duoji_pwm=1566;               
	if(pwm>=1830)duoji_pwm=1830;
}


int PI_A (int Encoder,int Target)
{ 	
	 static int Bias,Pwm,Last_bias;
	 Bias=Target-Encoder;                //����ƫ��
	 Pwm+=Velocity_KP*(Bias-Last_bias)+Velocity_KI*Bias;   //����ʽPI������
	 Last_bias=Bias;	                   //������һ��ƫ�� 
	 return Pwm;                         //�������
}
int PI_B (int Encoder,int Target)
{ 	
	 static int Bias,Pwm,Last_bias;
	 Bias=Target-Encoder;                //����ƫ��
	 Pwm+=Velocity_KP*(Bias-Last_bias)+Velocity_KI*Bias;   //����ʽPI������
	 Last_bias=Bias;	                   //������һ��ƫ�� 
	 return Pwm;                         //�������
}
