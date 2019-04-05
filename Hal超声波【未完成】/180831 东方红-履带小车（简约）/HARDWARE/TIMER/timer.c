#include "timer.h"
#include "led.h"
#include "usart.h"
#include "delay.h"
#include "stm32f10x.h"
extern int pwm_first;

void TIM5_Cap_Init(u16 arr,u16 psc)
{	 
	TIM_ICInitTypeDef  TIM5_ICInitStructure;
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	GPIO_InitTypeDef GPIO_InitStructure;
  NVIC_InitTypeDef NVIC_InitStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM5, ENABLE);	//使能TIM5时钟
 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);  //使能GPIOA时钟
	
	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3;  //PA0 清除之前设置  
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD; //PA0 输入  
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	GPIO_ResetBits(GPIOA,GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3);		 //PA0 下拉

	//初始化定时器5 TIM5	 
	TIM_TimeBaseStructure.TIM_Period = arr; //设定计数器自动重装值 
	TIM_TimeBaseStructure.TIM_Prescaler =psc; 	//预分频器   
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; //设置时钟分割:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM向上计数模式
	TIM_TimeBaseStructure.TIM_RepetitionCounter = 0;//重复计数设置                    【网上的新增】
	TIM_TimeBaseInit(TIM5, &TIM_TimeBaseStructure); //根据TIM_TimeBaseInitStruct中指定的参数初始化TIMx的时间基数单位
	TIM_ClearFlag(TIM5, TIM_IT_CC1|TIM_IT_CC2|TIM_IT_CC3|TIM_IT_CC4);//清中断标志位
	TIM_ITConfig(TIM5,TIM_IT_CC1|TIM_IT_CC2|TIM_IT_CC3|TIM_IT_CC4,ENABLE);//允许更新中断 ,允许CC1IE捕获中断	
  
	//初始化TIM5输入捕获参数
		TIM5_ICInitStructure.TIM_Channel = TIM_Channel_1; //CC1S=01 	选择输入端 IC1映射到TI1上
  	TIM5_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Rising;	//上升沿捕获
  	TIM5_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI; //映射到TI1上
  	TIM5_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;	 //配置输入分频,不分频 
  	TIM5_ICInitStructure.TIM_ICFilter = 0x00;//IC1F=0000 配置输入滤波器 不滤波
  	TIM_ICInit(TIM5, &TIM5_ICInitStructure);
		////////////////////////////
		TIM5_ICInitStructure.TIM_Channel = TIM_Channel_2; //CC1S=01 	选择输入端 IC1映射到TI1上
  	TIM5_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Rising;	//上升沿捕获
  	TIM5_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI; //映射到TI1上
  	TIM5_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;	 //配置输入分频,不分频 
  	TIM5_ICInitStructure.TIM_ICFilter = 0x00;//IC1F=0000 配置输入滤波器 不滤波
  	TIM_ICInit(TIM5, &TIM5_ICInitStructure);
		///////////////////////////
		TIM5_ICInitStructure.TIM_Channel = TIM_Channel_3; //CC1S=01 	选择输入端 IC1映射到TI1上
  	TIM5_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Rising;	//上升沿捕获
  	TIM5_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI; //映射到TI1上
  	TIM5_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;	 //配置输入分频,不分频 
  	TIM5_ICInitStructure.TIM_ICFilter = 0x00;//IC1F=0000 配置输入滤波器 不滤波
  	TIM_ICInit(TIM5, &TIM5_ICInitStructure);
		//////////////////////////////
		TIM5_ICInitStructure.TIM_Channel = TIM_Channel_4; //CC1S=01 	选择输入端 IC1映射到TI1上
  	TIM5_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Rising;	//上升沿捕获
  	TIM5_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI; //映射到TI1上
  	TIM5_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;	 //配置输入分频,不分频 
  	TIM5_ICInitStructure.TIM_ICFilter = 0x00;//IC1F=0000 配置输入滤波器 不滤波
  	TIM_ICInit(TIM5, &TIM5_ICInitStructure);
		//////////////////////////////
	
		//中断分组初始化
		NVIC_InitStructure.NVIC_IRQChannel =  TIM5_IRQn;    
		NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
		NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;       
		NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;  
		NVIC_Init(&NVIC_InitStructure); 
	
		TIM_Cmd(TIM5,ENABLE ); 	//使能定时器2
}

unsigned char   TIM5CH1_CAPTURE_STA=0, TIM5CH3_CAPTURE_STA=0, TIM5CH4_CAPTURE_STA=0, TIM5CH2_CAPTURE_STA=0;	//输入捕获状态 
unsigned short 	TIM5CH1_CAPTURE_VAL,TIM5CH2_CAPTURE_VAL,TIM5CH3_CAPTURE_VAL,TIM5CH4_CAPTURE_VAL;	//输入捕获值
 
//定时器2中断服务程序	 
void TIM5_IRQHandler(void)
{ 

 	if((TIM5CH1_CAPTURE_STA&0X80)==0)//还未成功捕获	
	{	  
		if  (TIM_GetITStatus(TIM5, TIM_IT_Update) != RESET)	 
		{	    
			if(TIM5CH1_CAPTURE_STA&0X40)//已经捕获到高电平了
			{
				if((TIM5CH1_CAPTURE_STA&0X3F)==0X3F)//高电平太长了
				{
					TIM5CH1_CAPTURE_STA|=0X80;//标记成功捕获了一次
					TIM5CH1_CAPTURE_VAL=0XFFFF;
				}else TIM5CH1_CAPTURE_STA++;
			}	 
		}
	if (TIM_GetITStatus(TIM5, TIM_IT_CC1) != RESET)//捕获1发生捕获事件
		{	
			if(TIM5CH1_CAPTURE_STA&0X40)		//捕获到一个下降沿 		
			{	  			
				TIM5CH1_CAPTURE_STA|=0X80;		//标记成功捕获到一次上升沿
				TIM5CH1_CAPTURE_VAL=TIM_GetCapture1(TIM5);
		   		TIM_OC1PolarityConfig(TIM5,TIM_ICPolarity_Rising); //CC1P=0 设置为上升沿捕获
			}else  								//还未开始,第一次捕获上升沿
			{
				TIM5CH1_CAPTURE_STA=0;			//清空
				TIM5CH1_CAPTURE_VAL=0;
	 			TIM_SetCounter(TIM5,0);
				TIM5CH1_CAPTURE_STA|=0X40;		//标记捕获到了上升沿
		   	TIM_OC1PolarityConfig(TIM5,TIM_ICPolarity_Falling);		//CC1P=1 设置为下降沿捕获
			}		    
		}			     	    					   
 	}
	
										
											if((TIM5CH2_CAPTURE_STA&0X80)==0)//还未成功捕获	
										{	  
											if (TIM_GetITStatus(TIM5, TIM_IT_Update) != RESET)
											 
											{	    
												if(TIM5CH2_CAPTURE_STA&0X40)//已经捕获到高电平了
												{
													if((TIM5CH2_CAPTURE_STA&0X3F)==0X3F)//高电平太长了
													{
														TIM5CH2_CAPTURE_STA|=0X80;//标记成功捕获了一次
														TIM5CH2_CAPTURE_VAL=0XFFFF;
													}else TIM5CH2_CAPTURE_STA++;
												}	 
											}
										if (TIM_GetITStatus(TIM5, TIM_IT_CC2) != RESET)//捕获1发生捕获事件
											{	
												if(TIM5CH2_CAPTURE_STA&0X40)		//捕获到一个下降沿 		
												{	  			
													TIM5CH2_CAPTURE_STA|=0X80;		//标记成功捕获到一次上升沿
													TIM5CH2_CAPTURE_VAL=TIM_GetCapture2(TIM5);
														TIM_OC2PolarityConfig(TIM5,TIM_ICPolarity_Rising); //CC1P=0 设置为上升沿捕获
												}else  								//还未开始,第一次捕获上升沿
												{
													TIM5CH2_CAPTURE_STA=0;			//清空
													TIM5CH2_CAPTURE_VAL=0;
													TIM_SetCounter(TIM5,0);
													TIM5CH2_CAPTURE_STA|=0X40;		//标记捕获到了上升沿
													TIM_OC2PolarityConfig(TIM5,TIM_ICPolarity_Falling);		//CC1P=1 设置为下降沿捕获
												}		    
											}			     	    					   
										}
										
							
																if((TIM5CH3_CAPTURE_STA&0X80)==0)//还未成功捕获	
																{	  
																	if (TIM_GetITStatus(TIM5, TIM_IT_Update) != RESET)
																	 
																	{	    
																		if(TIM5CH3_CAPTURE_STA&0X40)//已经捕获到高电平了
																		{
																			if((TIM5CH3_CAPTURE_STA&0X3F)==0X3F)//高电平太长了
																			{
																				TIM5CH3_CAPTURE_STA|=0X80;//标记成功捕获了一次
																				TIM5CH3_CAPTURE_VAL=0XFFFF;
																			}else TIM5CH3_CAPTURE_STA++;
																		}	 
																	}
																if (TIM_GetITStatus(TIM5, TIM_IT_CC3) != RESET)//捕获1发生捕获事件
																	{	
																		if(TIM5CH3_CAPTURE_STA&0X40)		//捕获到一个下降沿 		
																		{	  			
																			TIM5CH3_CAPTURE_STA|=0X80;		//标记成功捕获到一次上升沿
																			TIM5CH3_CAPTURE_VAL=TIM_GetCapture3(TIM5);
																				TIM_OC3PolarityConfig(TIM5,TIM_ICPolarity_Rising); //CC1P=0 设置为上升沿捕获
																		}else  								//还未开始,第一次捕获上升沿
																		{
																			TIM5CH3_CAPTURE_STA=0;			//清空
																			TIM5CH3_CAPTURE_VAL=0;
																			TIM_SetCounter(TIM5,0);
																			TIM5CH3_CAPTURE_STA|=0X40;		//标记捕获到了上升沿
																			TIM_OC3PolarityConfig(TIM5,TIM_ICPolarity_Falling);		//CC1P=1 设置为下降沿捕获
																		}		    
																	}			     	    					   
																}
																			
																									if((TIM5CH4_CAPTURE_STA&0X80)==0)//还未成功捕获	
																										{	  
																											if (TIM_GetITStatus(TIM5, TIM_IT_Update) != RESET)
																											 
																											{	    
																												if(TIM5CH4_CAPTURE_STA&0X40)//已经捕获到高电平了
																												{
																													if((TIM5CH4_CAPTURE_STA&0X3F)==0X3F)//高电平太长了
																													{
																														TIM5CH4_CAPTURE_STA|=0X80;//标记成功捕获了一次
																														TIM5CH4_CAPTURE_VAL=0XFFFF;
																													}else TIM5CH4_CAPTURE_STA++;
																												}	 
																											}
																										if (TIM_GetITStatus(TIM5, TIM_IT_CC4) != RESET)//捕获1发生捕获事件
																											{	
																												if(TIM5CH4_CAPTURE_STA&0X40)		//捕获到一个下降沿 		
																												{	  			
																													TIM5CH4_CAPTURE_STA|=0X80;		//标记成功捕获到一次上升沿
																													TIM5CH4_CAPTURE_VAL=TIM_GetCapture4(TIM5);
																														TIM_OC4PolarityConfig(TIM5,TIM_ICPolarity_Rising); //CC1P=0 设置为上升沿捕获
																												}else  								//还未开始,第一次捕获上升沿
																												{
																													TIM5CH4_CAPTURE_STA=0;			//清空
																													TIM5CH4_CAPTURE_VAL=0;
																													TIM_SetCounter(TIM5,0);
																													TIM5CH4_CAPTURE_STA|=0X40;		//标记捕获到了上升沿
																													TIM_OC4PolarityConfig(TIM5,TIM_ICPolarity_Falling);		//CC1P=1 设置为下降沿捕获
																												}		    
																											}			     	    					   
																										}
																										
    TIM_ClearITPendingBit(TIM5, TIM_IT_CC1|TIM_IT_CC2|TIM_IT_CC3|TIM_IT_CC4); //清除中断标志位

}



////////////【定时器（3）相关初始化】/////////////////
//void TIM5_Int_Init(u16 arr,u16 psc)
//{
//  TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
// // GPIO_PinRemapConfig(GPIO_PartialRemap1_TIM5, ENABLE);
//	//GPIO_PinRemapConfig(GPIO_Remap_TIM5, ENABLE);
//	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM5, ENABLE); //时钟使能

//	TIM_TimeBaseStructure.TIM_Period = arr; //设置在下一个更新事件装入活动的自动重装载寄存器周期的值	 计数到5000为500ms
//	TIM_TimeBaseStructure.TIM_Prescaler =psc; //设置用来作为TIMx时钟频率除数的预分频值  10Khz的计数频率  
//	TIM_TimeBaseStructure.TIM_ClockDivision = 0; //设置时钟分割:TDTS = Tck_tim
//	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM向上计数模式
//	TIM_TimeBaseInit(TIM5, &TIM_TimeBaseStructure); //根据TIM_TimeBaseInitStruct中指定的参数初始化TIMx的时间基数单位
// 
//	TIM_ITConfig(TIM5,TIM_IT_Update,ENABLE ); //使能指定的TIM5中断,允许更新中断
//	TIM_Cmd(TIM5, ENABLE);  //使能TIMx外设

//							 
//}
////TIM5 PWM部分初始化 
////PWM输出初始化
////arr：自动重装值
////psc：时钟预分频数
//void TIM5_PWM_Init(u16 arr,u16 psc)
//{  
//	GPIO_InitTypeDef GPIO_InitStructure3;
//	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
//	TIM_OCInitTypeDef  TIM_OCInitStructure;
//	TIM_OCInitTypeDef  TIM_OCInitStructure1;
//	TIM_OCInitTypeDef  TIM_OCInitStructure2;
//	
//	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM5, ENABLE);	//使能定时器3时钟
// 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB , ENABLE);  //使能GPIO外设和AFIO复用功能模块时钟
//	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA , ENABLE);  //使能GPIO外设和AFIO复用功能模块时钟
//	

//	GPIO_InitStructure3.GPIO_Pin = GPIO_Pin_6|GPIO_Pin_7; 
//	GPIO_InitStructure3.GPIO_Mode = GPIO_Mode_AF_PP;  
//	GPIO_InitStructure3.GPIO_Speed = GPIO_Speed_50MHz;
//	GPIO_Init(GPIOA, &GPIO_InitStructure3);
//		
//	TIM_TimeBaseStructure.TIM_Period = arr; //设置在下一个更新事件装入活动的自动重装载寄存器周期的值
//	TIM_TimeBaseStructure.TIM_Prescaler =psc; //设置用来作为TIMx时钟频率除数的预分频值 
//	TIM_TimeBaseStructure.TIM_ClockDivision = 0; //设置时钟分割:TDTS = Tck_tim
//	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Down;  //TIM向上计数模式
//	TIM_TimeBaseInit(TIM5, &TIM_TimeBaseStructure); //根据TIM_TimeBaseInitStruct中指定的参数初始化TIMx的时间基数单位

//	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1; //选择定时器模式:TIM脉冲宽度调制模式2
// 	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //比较输出使能
//	TIM_OCInitStructure. TIM_Pulse=pwm_first;   
//	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High; //输出极性:TIM输出比较极性高
//	TIM_OC1Init(TIM5, &TIM_OCInitStructure);  //根据T指定的参数初始化外设TIM5 OC2
//	TIM_OC1PreloadConfig(TIM5, TIM_OCPreload_Enable);  //使能TIM5在CCR2上的预装载寄存器

//  TIM_OCInitStructure1.TIM_OCMode = TIM_OCMode_PWM1; //选择定时器模式:TIM脉冲宽度调制模式2
// 	TIM_OCInitStructure1.TIM_OutputState = TIM_OutputState_Enable; //比较输出使能
//	TIM_OCInitStructure1. TIM_Pulse=1000;   //////////////
//	///////////决定了终点站，         如果没有PWM比较的话最后就是停在了终点站////////////                 //////
//	TIM_OCInitStructure1.TIM_OCPolarity = TIM_OCPolarity_High ; //输出极性:TIM输出比较极性高
//	TIM_OC2Init(TIM5, &TIM_OCInitStructure1);  //根据T指定的参数初始化外设TIM5 OC2
//	TIM_OC2PreloadConfig(TIM5, TIM_OCPreload_Enable);  //使能TIM5在CCR2上的预装载寄存器
//	
//	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1; //选择定时器模式:TIM脉冲宽度调制模式2
// 	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //比较输出使能
//	TIM_OCInitStructure. TIM_Pulse=1000;   
//	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High; //输出极性:TIM输出比较极性高
//	TIM_OC3Init(TIM5, &TIM_OCInitStructure);  //根据T指定的参数初始化外设TIM5 OC2
//	TIM_OC3PreloadConfig(TIM5, TIM_OCPreload_Enable);  //使能TIM5在CCR2上的预装载寄存器
//	
//	TIM_OCInitStructure2.TIM_OCMode = TIM_OCMode_PWM1; //选择定时器模式:TIM脉冲宽度调制模式2
// 	TIM_OCInitStructure2.TIM_OutputState = TIM_OutputState_Enable; //比较输出使能
//	TIM_OCInitStructure2. TIM_Pulse=1000;   
//	TIM_OCInitStructure2.TIM_OCPolarity = TIM_OCPolarity_High; //输出极性:TIM输出比较极性高
//	TIM_OC4Init(TIM5, &TIM_OCInitStructure2);  //根据T指定的参数初始化外设TIM5 OC2
//	TIM_OC4PreloadConfig(TIM5, TIM_OCPreload_Enable);  //使能TIM5在CCR2上的预装载寄存器
//	
//}


//////////【定时器（4）相关初始化】/////////////////

void TIM4_Int_Init(u16 arr,u16 psc)
{
  GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_OCInitTypeDef  TIM_OCInitStructure;
	
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB , ENABLE); 

	GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_6|GPIO_Pin_7;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;		    // 复用推挽输出
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	TIM_TimeBaseStructure.TIM_Period = arr; //设置在下一个更新事件装入活动的自动重装载寄存器周期的值
	TIM_TimeBaseStructure.TIM_Prescaler =psc; //设置用来作为TIMx时钟频率除数的预分频值 
	TIM_TimeBaseStructure.TIM_ClockDivision = 0; //设置时钟分割:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Down;  //TIM向上计数模式
	TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure); //根据TIM_TimeBaseInitStruct中指定的参数初始化TIMx的时间基数单位

	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1; //选择定时器模式:TIM脉冲宽度调制模式2
 	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //比较输出使能
	TIM_OCInitStructure. TIM_Pulse=0;   
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High; //输出极性:TIM输出比较极性高
	TIM_OC3Init(TIM4, &TIM_OCInitStructure);  //根据T指定的参数初始化外设TIM8 OC2
	TIM_OC3PreloadConfig(TIM4, TIM_OCPreload_Enable);  //使能TIM8在CCR2上的预装载寄存器
	
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1; //选择定时器模式:TIM脉冲宽度调制模式2
 	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //比较输出使能
	TIM_OCInitStructure. TIM_Pulse=0;   
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High; //输出极性:TIM输出比较极性高
	TIM_OC1Init(TIM4, &TIM_OCInitStructure);  //根据T指定的参数初始化外设TIM8 OC2
	TIM_OC1PreloadConfig(TIM4, TIM_OCPreload_Enable);  //使能TIM8在CCR2上的预装载寄存器
	
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1; //选择定时器模式:TIM脉冲宽度调制模式2
 	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //比较输出使能
	TIM_OCInitStructure. TIM_Pulse=0;   
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High; //输出极性:TIM输出比较极性高
	TIM_OC4Init(TIM4, &TIM_OCInitStructure);  //根据T指定的参数初始化外设TIM8 OC2
	TIM_OC4PreloadConfig(TIM4, TIM_OCPreload_Enable);  //使能TIM8在CCR2上的预装载寄存器

  TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1; //选择定时器模式:TIM脉冲宽度调制模式2
 	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //比较输出使能
	TIM_OCInitStructure. TIM_Pulse=0;   //////////////
	///////////决定了终点站，         如果没有PWM比较的话最后就是停在了终点站////////////                 //////
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High ; //输出极性:TIM输出比较极性高
	TIM_OC2Init(TIM4, &TIM_OCInitStructure);  //根据T指定的参数初始化外设TIM8 OC2
	TIM_OC2PreloadConfig(TIM4, TIM_OCPreload_Enable);  //使能TIM8在CCR2上的预装载寄存器

	TIM_ITConfig(TIM4,TIM_IT_Update,ENABLE ); //使能指定的TIM8中断,允许更新中断
	TIM_Cmd(TIM4, ENABLE);  //使能TIMx外设
						 					 
}



//////////【定时器（5）相关初始化】/////////////////

void TIM5_Int_Init(u16 arr,u16 psc)
{
  GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_OCInitTypeDef  TIM_OCInitStructure;
	
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM5, ENABLE);
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA , ENABLE); 
	
	GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;		    // 复用推挽输出
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(GPIOA, &GPIO_InitStructure);

	TIM_TimeBaseStructure.TIM_Period = arr; //设置在下一个更新事件装入活动的自动重装载寄存器周期的值
	TIM_TimeBaseStructure.TIM_Prescaler =psc; //设置用来作为TIMx时钟频率除数的预分频值 
	TIM_TimeBaseStructure.TIM_ClockDivision = 0; //设置时钟分割:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Down;  //TIM向上计数模式
	TIM_TimeBaseInit(TIM5, &TIM_TimeBaseStructure); //根据TIM_TimeBaseInitStruct中指定的参数初始化TIMx的时间基数单位

	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1; //选择定时器模式:TIM脉冲宽度调制模式2
 	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //比较输出使能
	TIM_OCInitStructure. TIM_Pulse=1500;   
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High; //输出极性:TIM输出比较极性高
	TIM_OC1Init(TIM5, &TIM_OCInitStructure);  //根据T指定的参数初始化外设TIM8 OC2
	TIM_OC1PreloadConfig(TIM5, TIM_OCPreload_Enable);  //使能TIM8在CCR2上的预装载寄存器

  TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1; //选择定时器模式:TIM脉冲宽度调制模式2
 	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //比较输出使能
	TIM_OCInitStructure. TIM_Pulse=1000;   //////////////
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High ; //输出极性:TIM输出比较极性高
	TIM_OC2Init(TIM5, &TIM_OCInitStructure);  //根据T指定的参数初始化外设TIM8 OC2
	TIM_OC2PreloadConfig(TIM5, TIM_OCPreload_Enable);  //使能TIM8在CCR2上的预装载寄存器
	
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1; //选择定时器模式:TIM脉冲宽度调制模式2
 	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //比较输出使能
	TIM_OCInitStructure. TIM_Pulse=1000;   //////////////
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High ; //输出极性:TIM输出比较极性高
	TIM_OC3Init(TIM5, &TIM_OCInitStructure);  //根据T指定的参数初始化外设TIM8 OC2
	TIM_OC3PreloadConfig(TIM5, TIM_OCPreload_Enable);  //使能TIM8在CCR2上的预装载寄存器
	
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1; //选择定时器模式:TIM脉冲宽度调制模式2
 	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //比较输出使能
	TIM_OCInitStructure. TIM_Pulse=1000;   //////////////
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High ; //输出极性:TIM输出比较极性高
	TIM_OC4Init(TIM5, &TIM_OCInitStructure);  //根据T指定的参数初始化外设TIM8 OC2
	TIM_OC4PreloadConfig(TIM5, TIM_OCPreload_Enable);  //使能TIM8在CCR2上的预装载寄存器

	TIM_ITConfig(TIM5,TIM_IT_Update,ENABLE ); //使能指定的TIM8中断,允许更新中断
	TIM_Cmd(TIM5, ENABLE);  //使能TIMx外设
							 
}

void TIM8_Int_Init(u16 arr,u16 psc)
{		 					 
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_OCInitTypeDef  TIM_OCInitStructure;
	

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM8, ENABLE); //使能TIMx外设
 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);  //使能GPIOA外设时钟使能
	
 
  //设置该引脚为复用输出功能,输出TIM8 CH1的PWM脉冲波形
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7|GPIO_Pin_8; //TIM8_CH1
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;  //复用功能输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOC, &GPIO_InitStructure); //初始化GPIO
 
	TIM_TimeBaseStructure.TIM_Period = arr; //设置自动重装载周期值
	TIM_TimeBaseStructure.TIM_Prescaler =psc; //设置预分频值 不分频
	TIM_TimeBaseStructure.TIM_ClockDivision = 0; //设置时钟分割:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM向上计数模式
	TIM_TimeBaseStructure.TIM_RepetitionCounter=0; 
	TIM_TimeBaseInit(TIM8, &TIM_TimeBaseStructure); //根据指定的参数初始化TIMx
	
	 
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM2; //CH1 PWM2模式	
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //比较输出使能
	TIM_OCInitStructure.TIM_Pulse = 0; //设置待装入捕获比较寄存器的脉冲值
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_Low; //OC1 低电平有效 
	TIM_OCInitStructure.TIM_OutputNState = TIM_OutputNState_Enable;
  TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;  
	TIM_OCInitStructure.TIM_OCNPolarity = TIM_OCNPolarity_High;   
	TIM_OCInitStructure.TIM_OCIdleState = TIM_OCIdleState_Set;
	TIM_OCInitStructure.TIM_OCNIdleState = TIM_OCIdleState_Reset;
	TIM_OC2Init(TIM8, &TIM_OCInitStructure);  //根据指定的参数初始化外设TIMx
	TIM_OC2PreloadConfig(TIM8, TIM_OCPreload_Enable);  //CH1 预装载使能
	
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM2; //CH1 PWM2模式	
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //比较输出使能
	TIM_OCInitStructure.TIM_Pulse = 0; //设置待装入捕获比较寄存器的脉冲值
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_Low; //OC1 低电平有效 
	TIM_OCInitStructure.TIM_OutputNState = TIM_OutputNState_Enable;
  TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;  
	TIM_OCInitStructure.TIM_OCNPolarity = TIM_OCNPolarity_High;   
	TIM_OCInitStructure.TIM_OCIdleState = TIM_OCIdleState_Set;
	TIM_OCInitStructure.TIM_OCNIdleState = TIM_OCIdleState_Reset;
	TIM_OC3Init(TIM8, &TIM_OCInitStructure);  //根据指定的参数初始化外设TIMx
	TIM_OC3PreloadConfig(TIM8, TIM_OCPreload_Enable);  //CH1 预装载使能
	
	TIM_ARRPreloadConfig(TIM8, ENABLE); //使能TIMx在ARR上的预装载寄存器
	TIM_CtrlPWMOutputs(TIM8,ENABLE);	//MOE 主输出使能,高级定时器必须开启这个 
	TIM_Cmd(TIM8, ENABLE);  //使能TIMx										  
} 				 

