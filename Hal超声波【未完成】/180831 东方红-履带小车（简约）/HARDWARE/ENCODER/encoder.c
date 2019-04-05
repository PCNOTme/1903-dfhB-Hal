#include "encoder.h"
#include "stm32f10x_gpio.h"
  /**************************************************************************
作者：平衡小车之家
我的淘宝小店：http://shop114407458.taobao.com/
**************************************************************************/
/**************************************************************************
函数功能：把TIM2初始化为编码器接口模式
入口参数：无
返回  值：无
**************************************************************************/
void Encoder_Init_TIM2(void)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;  
  TIM_ICInitTypeDef TIM_ICInitStructure;  
  GPIO_InitTypeDef GPIO_InitStructure;
	
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);//使能定时器4的时钟
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_GPIOB, ENABLE);//使能PB端口时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
	
  GPIO_PinRemapConfig(GPIO_FullRemap_TIM2, ENABLE);
	GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable , ENABLE);	//禁止JTAG功能，把PB3，PB4作为普通IO口使用
	
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_15;	//端口配置
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING; //浮空输入
  GPIO_Init(GPIOA, &GPIO_InitStructure);					      //根据设定参数初始化GPIOB
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;	//端口配置
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING; //浮空输入
  GPIO_Init(GPIOB, &GPIO_InitStructure);
  
  TIM_TimeBaseStructInit(&TIM_TimeBaseStructure);
  TIM_TimeBaseStructure.TIM_Prescaler = 0x0; // 预分频器 
  TIM_TimeBaseStructure.TIM_Period = ENCODER_TIM_PERIOD; //设定计数器自动重装值
  TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;//选择时钟分频：不分频
  TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;////TIM向上计数  
  TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);
	
  TIM_EncoderInterfaceConfig(TIM2, TIM_EncoderMode_TI12, TIM_ICPolarity_Rising, TIM_ICPolarity_Rising);//使用编码器模式3
  TIM_ICStructInit(&TIM_ICInitStructure);
  TIM_ICInitStructure.TIM_ICFilter = 10;
  TIM_ICInit(TIM2, &TIM_ICInitStructure);
  TIM_ClearFlag(TIM2, TIM_FLAG_Update);//清除TIM的更新标志位
  TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE);
  //Reset counter
  TIM_SetCounter(TIM2,0);
  TIM_Cmd(TIM2, ENABLE); 
}


/**************************************************************************
函数功能：把TIM3初始化为编码器接口模式
入口参数：无
返回  值：无
**************************************************************************/
//void Encoder_Init_TIM3(void)
//{
//	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;  
//  TIM_ICInitTypeDef TIM_ICInitStructure;  
//  GPIO_InitTypeDef GPIO_InitStructure;
//  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);//使能定时器4的时钟
//  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);//使能PB端口时钟
//	
//  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6|GPIO_Pin_7;	//端口配置
//  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING; //浮空输入
//  GPIO_Init(GPIOA, &GPIO_InitStructure);					      //根据设定参数初始化GPIOB
//  
//  TIM_TimeBaseStructInit(&TIM_TimeBaseStructure);
//  TIM_TimeBaseStructure.TIM_Prescaler = 0x0; // 预分频器 
//  TIM_TimeBaseStructure.TIM_Period = ENCODER_TIM_PERIOD; //设定计数器自动重装值
//  TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;//选择时钟分频：不分频
//  TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;////TIM向上计数  
//  TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);
//  TIM_EncoderInterfaceConfig(TIM3, TIM_EncoderMode_TI12, TIM_ICPolarity_Rising, TIM_ICPolarity_Rising);//使用编码器模式3
//  TIM_ICStructInit(&TIM_ICInitStructure);
//  TIM_ICInitStructure.TIM_ICFilter = 10;
//  TIM_ICInit(TIM3, &TIM_ICInitStructure);
//  TIM_ClearFlag(TIM3, TIM_FLAG_Update);//清除TIM的更新标志位
//  TIM_ITConfig(TIM3, TIM_IT_Update, ENABLE);
//  //Reset counter
//  TIM_SetCounter(TIM3,0);
//  TIM_Cmd(TIM3, ENABLE); 
//}

///**************************************************************************
//函数功能：把TIM4初始化为编码器接口模式
//入口参数：无
//返回  值：无  
//**************************************************************************/

////////////【定时器（4）相关初始化】/////////////////
//void TIM4_Int_Init(u16 arr,u16 psc)
//{
//  TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
// // GPIO_PinRemapConfig(GPIO_PartialRemap1_TIM2, ENABLE);
//	GPIO_PinRemapConfig(GPIO_Remap_TIM4, ENABLE);
//	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE); //时钟使能

//	TIM_TimeBaseStructure.TIM_Period = arr; //设置在下一个更新事件装入活动的自动重装载寄存器周期的值	 计数到5000为500ms
//	TIM_TimeBaseStructure.TIM_Prescaler =psc; //设置用来作为TIMx时钟频率除数的预分频值  10Khz的计数频率  
//	TIM_TimeBaseStructure.TIM_ClockDivision = 0; //设置时钟分割:TDTS = Tck_tim
//	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM向上计数模式
//	TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure); //根据TIM_TimeBaseInitStruct中指定的参数初始化TIMx的时间基数单位
// 
//	TIM_ITConfig(TIM4,TIM_IT_Update,ENABLE ); //使能指定的TIM3中断,允许更新中断
//	TIM_Cmd(TIM4, ENABLE);  //使能TIMx外设
//						 
//}

////TIM4 PWM部分初始化 
////PWM输出初始化
////arr：自动重装值
////psc：时钟预分频数
//void TIM4_PWM_Init(u16 arr,u16 psc)
//{  
//	GPIO_InitTypeDef GPIO_InitStructure4;
//	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
//	TIM_OCInitTypeDef  TIM_OCInitStructure;
//	
// 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB , ENABLE);  //使能GPIO外设和AFIO复用功能模块时钟
//	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE); //时钟使能

//	GPIO_InitStructure4.GPIO_Pin = GPIO_Pin_6|GPIO_Pin_7|GPIO_Pin_8|GPIO_Pin_9; 
//	GPIO_InitStructure4.GPIO_Mode = GPIO_Mode_AF_PP;  
//	GPIO_InitStructure4.GPIO_Speed = GPIO_Speed_50MHz;
//	GPIO_Init(GPIOB, &GPIO_InitStructure4);

//	TIM_TimeBaseStructure.TIM_Period = arr; //设置在下一个更新事件装入活动的自动重装载寄存器周期的值
//	TIM_TimeBaseStructure.TIM_Prescaler =psc; //设置用来作为TIMx时钟频率除数的预分频值 
//	TIM_TimeBaseStructure.TIM_ClockDivision = 0; //设置时钟分割:TDTS = Tck_tim
//	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Down;  //TIM向上计数模式
//	TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure); //根据TIM_TimeBaseInitStruct中指定的参数初始化TIMx的时间基数单位

//	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1; //选择定时器模式:TIM脉冲宽度调制模式2
// 	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //比较输出使能
//	TIM_OCInitStructure. TIM_Pulse=0;   
//	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High; //输出极性:TIM输出比较极性高
//	TIM_OC1Init(TIM4, &TIM_OCInitStructure);  //根据T指定的参数初始化外设TIM3 OC2
//	TIM_OC1PreloadConfig(TIM4, TIM_OCPreload_Enable);  //使能TIM3在CCR2上的预装载寄存器

//	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1; //选择定时器模式:TIM脉冲宽度调制模式2
// 	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //比较输出使能
//	TIM_OCInitStructure. TIM_Pulse=0;   
//	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High; //输出极性:TIM输出比较极性高
//	TIM_OC3Init(TIM4, &TIM_OCInitStructure);  //根据T指定的参数初始化外设TIM3 OC2
//	TIM_OC3PreloadConfig(TIM4, TIM_OCPreload_Enable);  //使能TIM3在CCR2上的预装载寄存器

//  TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1; //选择定时器模式:TIM脉冲宽度调制模式2
// 	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //比较输出使能
//	TIM_OCInitStructure. TIM_Pulse=0;   //////////////
//	///////////决定了终点站，         如果没有PWM比较的话最后就是停在了终点站////////////                 //////
//	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High ; //输出极性:TIM输出比较极性高
//	TIM_OC2Init(TIM4, &TIM_OCInitStructure);  //根据T指定的参数初始化外设TIM3 OC2
//	TIM_OC2PreloadConfig(TIM4, TIM_OCPreload_Enable);  //使能TIM3在CCR2上的预装载寄存器
//	
//	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1; //选择定时器模式:TIM脉冲宽度调制模式2
// 	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //比较输出使能
//	TIM_OCInitStructure. TIM_Pulse=0;   
//	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High; //输出极性:TIM输出比较极性高
//	TIM_OC4Init(TIM4, &TIM_OCInitStructure);  //根据T指定的参数初始化外设TIM3 OC2
//	TIM_OC4PreloadConfig(TIM4, TIM_OCPreload_Enable);  //使能TIM3在CCR2上的预装载寄存器
//	
//}


/**************************************************************************
函数功能：单位时间读取编码器计数
入口参数：定时器
返回  值：速度值
**************************************************************************/
int Read_Encoder(u8 TIMX)
{
    int Encoder_TIM;    
   switch(TIMX)
	 {
	   case 2:  Encoder_TIM= (short)TIM2 -> CNT;  TIM2 -> CNT=0;break;
		 case 3:  Encoder_TIM= (short)TIM3 -> CNT;  TIM3 -> CNT=0;break;	
		 //case 4:  Encoder_TIM= (short)TIM4 -> CNT;  TIM4 -> CNT=0;break;	
		 default:  Encoder_TIM=0;
	 }
		return Encoder_TIM;
}
/**************************************************************************
函数功能：TIM4中断服务函数
入口参数：无
返回  值：无
**************************************************************************/
//void TIM4_IRQHandler(void)
//{ 		    		  			    
//	if(TIM4->SR&0X0001)//溢出中断
//	{    				   				     	    	
//	}				   
//	TIM4->SR&=~(1<<0);//清除中断标志位 	    
//}
/**************************************************************************
函数功能：TIM2中断服务函数
入口参数：无
返回  值：无
**************************************************************************/
void TIM2_IRQHandler(void)
{ 		    		  			    
	if(TIM2->SR&0X0001)//溢出中断
	{    				   				     	    	
	}				   
	TIM2->SR&=~(1<<0);//清除中断标志位 	    
}

/**************************************************************************
函数功能：TIM3中断服务函数
入口参数：无
返回  值：无
**************************************************************************/
void TIM3_IRQHandler(void)
{ 		    		  			    
	if(TIM3->SR&0X0001)//溢出中断
	{    				   				     	    	
	}				   
	TIM3->SR&=~(1<<0);//清除中断标志位 	    
}

