#include "stm32f10x.h"
#include "led.h"
#include "delay.h"
//#include "key.h"
#include "sys.h"
#include "usart.h"
#include "timer.h"
#include "echo.h"
#include "motor.h"
#include "turn.h"
#include "control.h"
#include "encoder.h"
#include "DATA.h"
#include "anbt_dmp_fun.h"
#include "anbt_i2c.h"
#include "anbt_dmp_mpu6050.h"
#include "anbt_dmp_driver.h"

/*/////////////////////////////////////////////////////////////////////////
【最后修改日期：201808025】


硬设: 1.打开定时器【1/5】用于超声波4路捕获。（PA 11; PA 0/1/2/3) +（PA 4/5/6/7; PA 12）触发上升沿
			2.加入了定时器【8】驱动两个电机。（PC 7/8）
			3.电机4个信号引脚（PG 11/12/13/14）。
			4.加入定时器【3】、【4】用于左右电机的编码器（[部分重映射]-- PB 4/5; PB 6/7）。
			5.更改使用串口【3】（PB 10/11）。
			6.陀螺仪两个引脚（PB 12/13）。
			7.光电开关（PB 14）。
			
说明：1.延时通过出故障的路段
      2.调超声波走直线
			3.充好电
			
	
	
需测量验证：


*//////////////////////////////////////////////////////////////////////

#define MotorA_1 		PGout(11)
#define MotorA_2 		PGout(12)
#define MotorB_1 		PGout(13)
#define MotorB_2   	PGout(14)
#define Switch   	GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_14)

float length[5]={0};
float  temp1,temp2,temp3,temp4;     
float roll,yaw,pitch;	
float  t;
int Encoder_Left,Encoder_Right;             //左右编码器的脉冲计数


 int main(void)
 {		 
	LED_Init();			     //LED端口初始化
	delay_init();	    	 //延时函数初始化	 
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1); 	 //设置NVIC中断分组2:2位抢占优先级，2位响应优先级
 	uart_init(115200);	 
	TIM8_Int_Init(100-1,7200-1);		 ///////////用于控制电机
	Motor_trig_Init();
	TIM5_Cap_Init(0Xffff,72-1);		 ///////////用于超声波的捕获
	Trig_Init();
	TIM4_Int_Init(200-1,7200-1); 	 ////////////用于控制电机，arr在100至200间有效？
	//Encoder_Init_TIM2();            //=====编码器接口
	//Encoder_Init_TIM3();            //=====编码器接口
	ANBT_I2C_Configuration();		//IIC初始化 
	AnBT_DMP_MPU6050_Init();//6050DMP初始化
	 
//	 
//	turn(left,turn_left);    ///方便后期调试，方案2可给予正负值；
//	turn(right,turn_right);    ///方便后期调试，方案2可给予正负值；
//	 	 DATA_Processing();
//		if(mpu_dmp_get_data(&pitch,&roll,&yaw)==0)
//		{
//		  printf("yaw %f\r\n",yaw);
//			//delay_ms(20);
//		}
	//printf ("初始化就绪，即将开始表演\r\n");
	//delay_ms(300);
		LED1=1;
	 
	 	while(1)
	{  
//			motor1(20,60);	
//			TIM_SetCompare1(TIM1,1680);
//		delay_ms(1000);
//			TIM_SetCompare1(TIM1,1480);
//		delay_ms(1000);
//			if(side==2){while(1){;}}
//			if(flag1==0)Echo();
			Echo();
			Echo_Flag();
			Motor_pwm();
//			delay_ms(100);
		} 
}	 
	






