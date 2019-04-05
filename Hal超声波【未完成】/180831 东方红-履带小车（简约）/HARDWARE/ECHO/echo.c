#include "delay.h"
#include "key.h"
#include "sys.h"
#include "usart.h"
#include "echo.h"
#include "turn.h"
#include "motor.h"
#include "control.h"
#include "DataScope_DP.h"


#define delay_echo  10   ////微妙级延时
#define flag1_times1  6
#define flag1_times2  13


extern u8  TIM5CH1_CAPTURE_STA;		//输入捕获状态		    				
extern u16	TIM5CH1_CAPTURE_VAL;	//输入捕获值	
extern u8  TIM5CH2_CAPTURE_STA;		//输入捕获状态		    				
extern u16	TIM5CH2_CAPTURE_VAL;	//输入捕获值	
extern u8  TIM5CH3_CAPTURE_STA;		//输入捕获状态		    				
extern u16	TIM5CH3_CAPTURE_VAL;	//输入捕获值	
extern u8  TIM5CH4_CAPTURE_STA;		//输入捕获状态		    				
extern u16	TIM5CH4_CAPTURE_VAL;	//输入捕获值	

extern int t;
extern float  length[5];           ////length[0]在右侧，[2]在左侧，[1]在前侧,[3]在左后方；
extern int velocity[2];
extern int Encoder_Left,Encoder_Right;             //左右编码器的脉冲计数


int times=1,slow=0,side;
int flag1=0,flag2=0,start=0;
int turn_change=0;

void Echo(void);  

void Trig_Init(void)             /////初始化三个超声波的发射口Trig(前排依次逆时针第一第二第三个)
{
		 GPIO_InitTypeDef  GPIO_InitStructure;
		 RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);	 //使能PB,PE端口时钟
			
		 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_7|GPIO_Pin_4;		
		 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //推挽输出
		 GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 //IO口速度为50MHz
		 GPIO_Init(GPIOA, &GPIO_InitStructure);					 //根据设定参数初始化GPIOB.5
		
}


void Echo_Flag(void)            
{
		
		if(start==0)										///////只执行至第一过弯道
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
							turn_change=1;    ////证明flag2变化的第一次时刻
						}
					}
				}		
			}
			if(flag2==1)start=1;
		}

		
			else if(start==1)										///判断接下来的11个转弯
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
//								if(flag2>12){while(1){LED0=0;delay_ms(100);LED0=1;delay_ms(100);}}     ////【超过flag2计数，停下来，快闪】
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




void Echo(void)                           	 /////////////////4个超声波
{
		
			float  temp1,temp2,temp3,temp4,temp5;        ////////////////【不能够同时拉高，同时拉低，不知道为什么？？？？】
	
			GPIO_SetBits(GPIOA,GPIO_Pin_5); 
			delay_us(20);		
			GPIO_ResetBits(GPIOA,GPIO_Pin_5);     ///////////发射声波
	
			GPIO_SetBits(GPIOA,GPIO_Pin_6); 
			delay_us(20);		
			GPIO_ResetBits(GPIOA,GPIO_Pin_6);     ///////////发射声波
	
			GPIO_SetBits(GPIOA,GPIO_Pin_7); 
			delay_us(20);		
			GPIO_ResetBits(GPIOA,GPIO_Pin_7);     ///////////发射声波
	
			GPIO_SetBits(GPIOA,GPIO_Pin_4); 
			delay_us(20);		
			GPIO_ResetBits(GPIOA,GPIO_Pin_4);     ///////////发射声波
	
				if(TIM5CH1_CAPTURE_STA&0X80)//成功捕获到了一次上升沿
			{
					temp1=TIM5CH1_CAPTURE_STA&0X3F;
					temp1*=65536;//溢出时间总和
					temp1+=TIM5CH1_CAPTURE_VAL;//得到总的高电平时间
					if(temp1<=0)temp1=0;
					TIM5CH1_CAPTURE_STA=0;//开启下一次捕获
					temp1=temp1*17/1000;	
					length[0]=temp1;	
				}

				
				if(TIM5CH2_CAPTURE_STA&0X80)//成功捕获到了一次上升沿
			{
					temp2=TIM5CH2_CAPTURE_STA&0X3F;
					temp2*=65536;//溢出时间总和
					temp2+=TIM5CH2_CAPTURE_VAL;//得到总的高电平时间
					if(temp2<=0)temp2=0;
					TIM5CH2_CAPTURE_STA=0;//开启下一次捕获
					temp2=temp2*17/1000;
				  length[1]=temp2;
//				if(50<length[1]&&length[1]<60)LED1=0;
//				else LED1=1;

				}
			
					if(TIM5CH3_CAPTURE_STA&0X80)//成功捕获到了一次上升沿
			{
					temp3=TIM5CH3_CAPTURE_STA&0X3F;
					temp3*=65536;//溢出时间总和
					temp3+=TIM5CH3_CAPTURE_VAL;//得到总的高电平时间
					if(temp3<=0)temp3=0;
					TIM5CH3_CAPTURE_STA=0;//开启下一次捕获
					temp3=temp3*17/1000;
					length[2]=temp3;

				}
			
				if(TIM5CH4_CAPTURE_STA&0X80)//成功捕获到了一次上升沿
			{
					temp4=TIM5CH4_CAPTURE_STA&0X3F;
					temp4*=65536;//溢出时间总和
					temp4+=TIM5CH4_CAPTURE_VAL;//得到总的高电平时间
					if(temp4<=0)temp4=0;
					TIM5CH4_CAPTURE_STA=0;//开启下一次捕获
					temp4=temp4*17/1000;
					length[3]=temp4;

				}
			
				printf("Echo_value: [1]:%.2f    [2]:%.2f    [3]:%.2f    [4]:%.2f\r\n",length[0],length[1],length[2],length[3]); 
				//delay_ms(50);	
				//printf("HIGH4:%.2f cm\r\n",temp4*17/1000);//打印总的高点平时间
}
