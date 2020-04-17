/*-----------------INCLUDE------------------*/
#include "timer.h"

uint32_t __TIM_COUNT;

/*---------------------------------------
函数名：Timer_RCC_Configuration
输  入：无
输  出：无
描  述：定时器的时钟使能
---------------------------------------*/
static void Timer_RCC_Configuration(void)
{
	/*----------------使能TIM1时钟----------------*/
	/*----------------使能TIM2时钟----------------*/
	/*----------------使能TIM3时钟----------------*/
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
}

/*---------------------------------------
函数名：NVIC_Configuration
输  入：无
输  出：无
描  述：中断配置
---------------------------------------*/
static void Timer_NVIC_Configuration(void)
{
	NVIC_InitTypeDef NVIC_InitStructure;
		/* ------------------定时器中断配置------------------- */
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);
	/*-------TIM1中断配置--------*/
	/*-------TIM2中断配置--------*/
	/*-------TIM3中断配置--------*/
  NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;               //TIM3 中断
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;     //先占优先级 1 级
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;            //从优先级 3 级
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;               //IRQ 通道被使能
  NVIC_Init(&NVIC_InitStructure);
}

/*-----------------------------------------------
函数名：Timer3_init
输  入：TIMx, 定时器x
				Period，重载时间值。典型值，1000
				Prescaler，预分频。典型值，72
输  出：无
描  述：初始化定时器，使用典型值配置，将1ms产生一次中断
				该定时器配置不能配置TIM6，TIM7
------------------------------------------------*/
void Timer_Configuration(TIM_TypeDef *TIMx, uint32_t Period, uint8_t Prescaler)
{
  TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
	
  TIM_TimeBaseStructure.TIM_Period = Period;
  TIM_TimeBaseStructure.TIM_Prescaler = Prescaler;
  TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;       //时钟分割
  TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;   //计时模式
  TIM_TimeBaseInit(TIMx, &TIM_TimeBaseStructure);       				//初始化
  TIM_ITConfig(TIMx, TIM_IT_Update, ENABLE );            			  //时能中断
  
  TIM_Cmd(TIMx, ENABLE);
}

/*---------------------------------------
函数名：Timer_Init
输  入：无
输  出：无
描  述：timer初始化，主函数调用
---------------------------------------*/
void Timer_Initialization(void)
{
	Timer_RCC_Configuration();
	Timer_NVIC_Configuration();
	Timer_Configuration(TIM3, 1000, 72);
}

/*---------------------------------------
函数名：NVIC_Configuration
输  入：无
输  出：无
描  述：TIM3中断函数
---------------------------------------*/
/*
void TIM3_IRQHandler(void)
{
  if (TIM_GetITStatus(TIM3, TIM_IT_Update) != RESET)
  {
//		timer_Interrupt();
		TIM_ClearITPendingBit(TIM3, TIM_IT_Update);         //Clear Interrupt
	}
}
*/
