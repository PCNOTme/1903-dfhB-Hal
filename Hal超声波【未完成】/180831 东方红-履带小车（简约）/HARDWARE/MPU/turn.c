#include "led.h"
#include "delay.h"
#include "sys.h"
#include "usart.h"
#include "timer.h"
#include "echo.h"
#include "turn.h"
#include "motor.h"
#include "DATA.h"
#include "anbt_dmp_fun.h"
#include "anbt_i2c.h"
#include "anbt_dmp_mpu6050.h"
#include "anbt_dmp_driver.h"


#define MotorA_1 		PGout(11)
#define MotorA_2 		PGout(12)
#define MotorB_1 		PGout(13)
#define MotorB_2   	PGout(14)

#define left  0
#define right  1
#define left_pwm  1300
#define right_pwm  2300
#define left_small  100
#define right_small 100
#define vel  35
#define L  1


extern  int flag1,flag2,slow;
extern int turn_change;
extern int danger_times;
extern int danger_times2;
extern int straight_time;
extern float length[5]; 
extern int velocity[2];
extern float roll,yaw,pitch;	

extern unsigned char  TIM3CH1_CAPTURE_STA, TIM3CH3_CAPTURE_STA, TIM3CH4_CAPTURE_STA;	//输入捕获状态
extern unsigned short 	TIM3CH1_CAPTURE_VAL,TIM3CH2_CAPTURE_VAL,TIM3CH3_CAPTURE_VAL,TIM3CH4_CAPTURE_VAL;	//输入捕获值

int turn_finish=0;


float value_abs(float temp)               ////函数功能：取绝对值
{
	if(temp>=0)return temp;
	if(temp<0)return -temp;
}


//void turn(int dir,int pwm_change)          		 ///////////////前舵机转弯，方向+角度（相对初始位置）
//{
//		extern int pwm_first;
//	  if(dir==0)pwm_first-=pwm_change;
//		if(dir==1) pwm_first+=pwm_change;    							//////////////以1500为中位置
//	  //printf ("PWM变化量: %d\r\n",pwm_change);
//	  if(pwm_first<=1200)pwm_first=1200;                 /////////////保护舵机
//	  if(pwm_first>=2400)pwm_first=2400;
//    TIM_SetCompare1(TIM5,pwm_first);	
//		printf ("PWM总: %d\r\n",pwm_first);
//	  delay_ms(100);
//	
//}
void turn1()
{
	int  now;
	int target1,target2;
	DATA_Processing();
	mpu_dmp_get_data(&pitch,&roll,&yaw);
	now=(int)yaw;
	printf("t= %d\r\n",now);
	Set_Pwm(0,0); 
	delay_ms(100);
//	if(now>=95)now=94;
//	if(now<=-95)now=-94;
//	
		if(flag2==4||flag2==5||flag2==8||flag2==9||flag2==12)//逆时针转
	{
//		if(now>100&&now<=180)target=-280+now;
//		else target=now+79;
		MotorA_1=0;
		MotorA_2=1;
		MotorB_1=1;
		MotorB_2=0;
	}
		else if(flag2==1||flag2==2||flag2==3||flag2==6||flag2==7||flag2==10||flag2==11)//顺时针转
	{
//		if(now<-100&&now>=-180)target=280+now;
//		else target=now-79;
		MotorA_1=1;
		MotorA_2=0;
		MotorB_1=0;
		MotorB_2=1;
	}
	
	if(flag2==1){target1=-87;target2=-78;}       //原先 -85，-80
	if(flag2==3){target1=88;target2=97;}         //原先85，90
	if(flag2==5){target1=-97;target2=-88;}
	if(flag2==6){target1=-180;target2=-175;}
	if(flag2==7){target1=88;target2=97;}
	if(flag2==8){target1=177;target2=180;}
	if(flag2==9){target1=-97;target2=-88;}
	if(flag2==10){target1=-180;target2=-175;}
	if(flag2==11){target1=88;target2=97;}
	if(flag2==12){target1=177;target2=180;}
	
	if((flag2==5)||(flag2==6)||(flag2==9)||(flag2==10))
	{
		if(flag2==5||flag2==9)Set_Pwm(55,65);   //【45，47】
		else Set_Pwm(65,55); 	
		while((now<=target1)||(now>=target2))
		{
			DATA_Processing();
			mpu_dmp_get_data(&pitch,&roll,&yaw);
			now=(int)yaw;
			printf("t= %d\r\n",now);
		}
	}
	
	if((flag2==4)||(flag2==12)||(flag2==8))
	{
		Set_Pwm(38,46); 
		while(!((now>=177)&&(now<=180))||((now<=-173)&&(now>=-180)))
		{
			DATA_Processing();
			mpu_dmp_get_data(&pitch,&roll,&yaw);
			now=(int)yaw;
			printf("t= %d\r\n",now);
		}
	}
	
	if((flag2==2)||(flag2==7))
	{
		Set_Pwm(46,38); 
		while(!((now>=-180)&&(now<=-175))||((now>=175)&&(now<=180)))
		{
			DATA_Processing();
			mpu_dmp_get_data(&pitch,&roll,&yaw);
			now=(int)yaw;
			printf("t= %d\r\n",now);
		}
	}
	
	if((flag2==1)||(flag2==3)||(flag2==11))
	{
		Set_Pwm(46,38);   //【36，38】
			while((now<=target1)||(now>=target2))
		{
			DATA_Processing();
			mpu_dmp_get_data(&pitch,&roll,&yaw);
			now=(int)yaw;
			printf("t= %d\r\n",now);		
		}
	}	
	
	turn_change=0;			////【所有标志位相关都在转弯后清零】
	danger_times=1;     ////【转弯完成为危险时间】
	if((flag2==4)||(flag2==8)){danger_times=0;danger_times2=1;}
	slow=0;
	flag1=0;
	straight_time=0;
//	while(1){printf("调试完成\r\n");motor1(0,0);}
}

