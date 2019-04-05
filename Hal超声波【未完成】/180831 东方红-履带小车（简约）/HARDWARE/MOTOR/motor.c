#include "led.h"
#include "delay.h"
#include "motor.h"
#include "sys.h"
#include "usart.h"
#include "timer.h"
#include "echo.h"
#include "control.h"
#include "turn.h"

#define MotorA_1 		PGout(11)
#define MotorA_2 		PGout(12)
#define MotorB_1 		PGout(13)
#define MotorB_2   	PGout(14)

#define Keep_distance  11  //ԭ��13
#define Motion_distance 0
#define Velocity_KP 10
#define Velocity_KI 0
#define Ks1 2.5     //ԭ��2.1
#define Ks2 1.1      //ԭ��1.1
#define straight_times 20  //��Σ��ʱ��ά�ֶ�á�
#define slower_pwm  45   //����ǰ�ٶȼ��ٶ��١�25
#define faster_pwm  12
#define targetA  33     //ԭ��36
#define targetB  41   //ԭ��38
#define High_valueA 75   //ԭ��50
#define Low_valueA  12   //ԭ��12
#define High_valueB 75
#define Low_valueB  12

extern  int flag1,flag2,slow;
extern int Target_A,Target_B;
extern float  length[5]; 
extern int Encoder_Left,Encoder_Right;             //���ұ��������������
int straight_time=0;
int Motor_A,Motor_B;
int danger_times=0,danger_times2=0,add;
extern int turn_change;
extern int turn_times;

//void motor1(int left_pwm,int right_pwm)   ////���1���ٶȣ�����ǰ�ڲ���ʹ�ã�
//{
//		TIM_SetCompare3(TIM8,right_pwm);
//		TIM_SetCompare2(TIM8,left_pwm);	
//	printf("                                                                                                MotorA=%d   MotorB=%d\r\n\r\n",Motor_A,Motor_B);
//	  //delay_ms(100);
//	
//		//printf("                                                                                                                                          ����ٶȣ�  %d  �Ҳ��ٶȣ�  %d\r\n",left_pwm,right_pwm);
//}

void Motor_trig_Init(void)
{
		GPIO_InitTypeDef  GPIO_InitStructure;
		 RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOG, ENABLE);	 //ʹ��PB,PE�˿�ʱ��
			
		 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11|GPIO_Pin_12|GPIO_Pin_13|GPIO_Pin_14;		
		 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //�������
		 GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 //IO���ٶ�Ϊ50MHz
		 GPIO_Init(GPIOG, &GPIO_InitStructure);					 //�����趨������ʼ��GPIOB.5	
}


void Chasu_Analysis1(void)
{
	float Echo_chazhi;
	
		////����������Σ��ʱ��ĸ��š�
			if((danger_times==1)&&(flag2>=1))
		{
//				MotorA_1=1;
//				MotorA_2=0;
//				MotorB_1=1;
//				MotorB_2=0;
			Set_Pwm(26,30);  	
			while(1)
			{
				Echo();
				if((length[2]<=30)&&(length[1]<=30)) {straight_time++;  }   //&&(length[2]<=30)&&(length[0]<=30
				printf("Number��%d\r\n",straight_time);
				if(straight_time>=straight_times)break;
				if(flag2==12){delay_ms(1000);delay_ms(500);Set_Pwm(0,0);}
			}
			straight_time=0;
			danger_times=0;
//			if(flag2==4){ while(1){printf("�������\r\n");Set_Pwm(0,0);} }
		}
		
		if(danger_times2==1)
		{
			LED1=0;
			Set_Pwm(48,48); 
			delay_ms(900);
			LED1=1;
			danger_times2=0;
		}
		
		////������ѡ�����ıߵĳ�����Ϊֱ�߱�׼��
			if((turn_change==0)&&(flag2==0||flag2==1||flag2==2||flag2==6||flag2==10||flag2==11||flag2==3||flag2==4||flag2==7))   
		{
				Echo_chazhi=length[0]-Keep_distance;
				Motor_A=targetA-(int)(Echo_chazhi*Ks1);
				Motor_B=targetB+(int)(Echo_chazhi*Ks1);		
		}
		
		
		////������ѡ�����ıߵĳ�����Ϊֱ�߱�׼��
			if((turn_change==0)&&(flag2==8||flag2==12))
		{
				Echo_chazhi=length[3]-Keep_distance;
				Motor_A=targetA+(int)(Echo_chazhi*Ks1);
				Motor_B=targetB-(int)(Echo_chazhi*Ks1);	
		}
		
		if((turn_change==0)&&(flag2==5||flag2==9))
		{
				Echo_chazhi=length[0]-Keep_distance;
				Motor_A=targetA-(int)(Echo_chazhi*Ks2)+faster_pwm;
				Motor_B=targetB+(int)(Echo_chazhi*Ks2)+faster_pwm;		
		}
}

void Chasu_Analysis2()
{
		LED1=~LED1;
		Motor_A=targetA-slower_pwm;
		Motor_B=targetB-slower_pwm;	
}


void Motor_pwm(void)
{
		MotorA_1=1;
		MotorA_2=0;
		MotorB_1=1;
		MotorB_2=0;
	
//		if(!Switch1)slow=1;
		if(slow==1)Chasu_Analysis2();
		else Chasu_Analysis1();
//	PID_Analysis(Velocity,Echo_value);
		Xianfu_Pwm();                                                      //===PWM�޷�
		Set_Pwm(Motor_A,Motor_B);                                 	 //===��ֵ��PWM�Ĵ���  
	
}


void Xianfu_Pwm(void)
{	
		if(Motor_A>=High_valueA)  Motor_A=High_valueA;
		if(Motor_A<=Low_valueA)  Motor_A=Low_valueA;	
		if(Motor_B>=High_valueB)  Motor_B=High_valueB;	
		if(Motor_B<=Low_valueB)  Motor_B=Low_valueB;			
}


void Set_Pwm(int motor_a,int motor_b)
{
	TIM_SetCompare2(TIM8,motor_a);
	TIM_SetCompare3(TIM8,motor_b);
	printf("MotorA=%d,MotorB=%d\r\n\r\n",motor_a,motor_b);
}
