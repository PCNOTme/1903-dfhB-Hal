#include "delay.h"
#include "key.h"
#include "sys.h"
#include "usart.h"
#include "echo.h"
#include "turn.h"
#include "motor.h"
#include "control.h"
#include "DataScope_DP.h"


#define delay_echo  10   ////΢���ʱ
#define flag1_times1  6
#define flag1_times2  13


extern u8  TIM5CH1_CAPTURE_STA;		//���벶��״̬		    				
extern u16	TIM5CH1_CAPTURE_VAL;	//���벶��ֵ	
extern u8  TIM5CH2_CAPTURE_STA;		//���벶��״̬		    				
extern u16	TIM5CH2_CAPTURE_VAL;	//���벶��ֵ	
extern u8  TIM5CH3_CAPTURE_STA;		//���벶��״̬		    				
extern u16	TIM5CH3_CAPTURE_VAL;	//���벶��ֵ	
extern u8  TIM5CH4_CAPTURE_STA;		//���벶��״̬		    				
extern u16	TIM5CH4_CAPTURE_VAL;	//���벶��ֵ	

extern int t;
extern float  length[5];           ////length[0]���Ҳ࣬[2]����࣬[1]��ǰ��,[3]����󷽣�
extern int velocity[2];
extern int Encoder_Left,Encoder_Right;             //���ұ��������������


int times=1,slow=0,side;
int flag1=0,flag2=0,start=0;
int turn_change=0;

void Echo(void);  

void Trig_Init(void)             /////��ʼ�������������ķ����Trig(ǰ��������ʱ���һ�ڶ�������)
{
		 GPIO_InitTypeDef  GPIO_InitStructure;
		 RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);	 //ʹ��PB,PE�˿�ʱ��
			
		 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_7|GPIO_Pin_4;		
		 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //�������
		 GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 //IO���ٶ�Ϊ50MHz
		 GPIO_Init(GPIOA, &GPIO_InitStructure);					 //�����趨������ʼ��GPIOB.5
		
}


void Echo_Flag(void)            
{
		
		if(start==0)										///////ִֻ������һ�����
		{
			if(!Switch1)
			{
					if(length[2]>=30)
				{
					delay_us(delay_echo);
						if(length[2]>=30)
					{
							flag1++;
							slow=1;
						if((turn_change==0)&&(flag1>=flag1_times1))
						{
							flag2++;		
							turn_change=1;    ////֤��flag2�仯�ĵ�һ��ʱ��
						}
					}
				}		
			}
			if(flag2==1)start=1;
		}

		
			else if(start==1)										///�жϽ�������11��ת��
		{
				if((length[1]>=40)||(length[2]>=40))
				{
					delay_us(delay_echo);
						if((length[1]>=40)||(length[2]>=40))
					{
								flag1++;
								slow=1;
							if((turn_change==0)&&(flag1>=flag1_times2))
							{
								flag2++;
								turn_change=1;
//								if(flag2>12){while(1){LED0=0;delay_ms(100);LED0=1;delay_ms(100);}}     ////������flag2������ͣ������������
							}
					}	
				}
		}
		
		
		printf ("  Flag1=%d,Flag2=%d\r\n",flag1,flag2);

		
		if((turn_change==1)&&(flag2>=1))
		{
			turn1();
		}
	
}




void Echo(void)                           	 /////////////////4��������
{
		
			float  temp1,temp2,temp3,temp4,temp5;        ////////////////�����ܹ�ͬʱ���ߣ�ͬʱ���ͣ���֪��Ϊʲô����������
	
			GPIO_SetBits(GPIOA,GPIO_Pin_5); 
			delay_us(20);		
			GPIO_ResetBits(GPIOA,GPIO_Pin_5);     ///////////��������
	
			GPIO_SetBits(GPIOA,GPIO_Pin_6); 
			delay_us(20);		
			GPIO_ResetBits(GPIOA,GPIO_Pin_6);     ///////////��������
	
			GPIO_SetBits(GPIOA,GPIO_Pin_7); 
			delay_us(20);		
			GPIO_ResetBits(GPIOA,GPIO_Pin_7);     ///////////��������
	
			GPIO_SetBits(GPIOA,GPIO_Pin_4); 
			delay_us(20);		
			GPIO_ResetBits(GPIOA,GPIO_Pin_4);     ///////////��������
	
				if(TIM5CH1_CAPTURE_STA&0X80)//�ɹ�������һ��������
			{
					temp1=TIM5CH1_CAPTURE_STA&0X3F;
					temp1*=65536;//���ʱ���ܺ�
					temp1+=TIM5CH1_CAPTURE_VAL;//�õ��ܵĸߵ�ƽʱ��
					if(temp1<=0)temp1=0;
					TIM5CH1_CAPTURE_STA=0;//������һ�β���
					temp1=temp1*17/1000;	
					length[0]=temp1;	
				}

				
				if(TIM5CH2_CAPTURE_STA&0X80)//�ɹ�������һ��������
			{
					temp2=TIM5CH2_CAPTURE_STA&0X3F;
					temp2*=65536;//���ʱ���ܺ�
					temp2+=TIM5CH2_CAPTURE_VAL;//�õ��ܵĸߵ�ƽʱ��
					if(temp2<=0)temp2=0;
					TIM5CH2_CAPTURE_STA=0;//������һ�β���
					temp2=temp2*17/1000;
				  length[1]=temp2;
//				if(50<length[1]&&length[1]<60)LED1=0;
//				else LED1=1;

				}
			
					if(TIM5CH3_CAPTURE_STA&0X80)//�ɹ�������һ��������
			{
					temp3=TIM5CH3_CAPTURE_STA&0X3F;
					temp3*=65536;//���ʱ���ܺ�
					temp3+=TIM5CH3_CAPTURE_VAL;//�õ��ܵĸߵ�ƽʱ��
					if(temp3<=0)temp3=0;
					TIM5CH3_CAPTURE_STA=0;//������һ�β���
					temp3=temp3*17/1000;
					length[2]=temp3;

				}
			
				if(TIM5CH4_CAPTURE_STA&0X80)//�ɹ�������һ��������
			{
					temp4=TIM5CH4_CAPTURE_STA&0X3F;
					temp4*=65536;//���ʱ���ܺ�
					temp4+=TIM5CH4_CAPTURE_VAL;//�õ��ܵĸߵ�ƽʱ��
					if(temp4<=0)temp4=0;
					TIM5CH4_CAPTURE_STA=0;//������һ�β���
					temp4=temp4*17/1000;
					length[3]=temp4;

				}
			
				printf("Echo_value: [1]:%.2f    [2]:%.2f    [3]:%.2f    [4]:%.2f\r\n",length[0],length[1],length[2],length[3]); 
				//delay_ms(50);	
				//printf("HIGH4:%.2f cm\r\n",temp4*17/1000);//��ӡ�ܵĸߵ�ƽʱ��
}
