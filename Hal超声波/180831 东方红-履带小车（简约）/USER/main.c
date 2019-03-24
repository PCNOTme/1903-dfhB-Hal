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
������޸����ڣ�201808025��


Ӳ��: 1.�򿪶�ʱ����1/5�����ڳ�����4·���񡣣�PA 11; PA 0/1/2/3) +��PA 4/5/6/7; PA 12������������
			2.�����˶�ʱ����8�����������������PC 7/8��
			3.���4���ź����ţ�PG 11/12/13/14����
			4.���붨ʱ����3������4���������ҵ���ı�������[������ӳ��]-- PB 4/5; PB 6/7����
			5.����ʹ�ô��ڡ�3����PB 10/11����
			6.�������������ţ�PB 12/13����
			7.��翪�أ�PB 14����
			
˵����1.��ʱͨ�������ϵ�·��
      2.����������ֱ��
			3.��õ�
			
	
	
�������֤��


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
int Encoder_Left,Encoder_Right;             //���ұ��������������


 int main(void)
 {		 
	LED_Init();			     //LED�˿ڳ�ʼ��
	delay_init();	    	 //��ʱ������ʼ��	 
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1); 	 //����NVIC�жϷ���2:2λ��ռ���ȼ���2λ��Ӧ���ȼ�
 	uart_init(115200);	 
	TIM8_Int_Init(100-1,7200-1);		 ///////////���ڿ��Ƶ��
	Motor_trig_Init();
	TIM5_Cap_Init(0Xffff,72-1);		 ///////////���ڳ������Ĳ���
	Trig_Init();
	TIM4_Int_Init(200-1,7200-1); 	 ////////////���ڿ��Ƶ����arr��100��200����Ч��
	//Encoder_Init_TIM2();            //=====�������ӿ�
	//Encoder_Init_TIM3();            //=====�������ӿ�
	ANBT_I2C_Configuration();		//IIC��ʼ�� 
	AnBT_DMP_MPU6050_Init();//6050DMP��ʼ��
	 
//	 
//	turn(left,turn_left);    ///������ڵ��ԣ�����2�ɸ�������ֵ��
//	turn(right,turn_right);    ///������ڵ��ԣ�����2�ɸ�������ֵ��
//	 	 DATA_Processing();
//		if(mpu_dmp_get_data(&pitch,&roll,&yaw)==0)
//		{
//		  printf("yaw %f\r\n",yaw);
//			//delay_ms(20);
//		}
	//printf ("��ʼ��������������ʼ����\r\n");
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
	






