/*-----------------INCLUDE------------------*/
#include "timer.h"

uint32_t __TIM_COUNT;

/*---------------------------------------
��������Timer_RCC_Configuration
��  �룺��
��  ������
��  ������ʱ����ʱ��ʹ��
---------------------------------------*/
static void Timer_RCC_Configuration(void)
{
	/*----------------ʹ��TIM1ʱ��----------------*/
	/*----------------ʹ��TIM2ʱ��----------------*/
	/*----------------ʹ��TIM3ʱ��----------------*/
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
}

/*---------------------------------------
��������NVIC_Configuration
��  �룺��
��  ������
��  �����ж�����
---------------------------------------*/
static void Timer_NVIC_Configuration(void)
{
	NVIC_InitTypeDef NVIC_InitStructure;
		/* ------------------��ʱ���ж�����------------------- */
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);
	/*-------TIM1�ж�����--------*/
	/*-------TIM2�ж�����--------*/
	/*-------TIM3�ж�����--------*/
  NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;               //TIM3 �ж�
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;     //��ռ���ȼ� 1 ��
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;            //�����ȼ� 3 ��
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;               //IRQ ͨ����ʹ��
  NVIC_Init(&NVIC_InitStructure);
}

/*-----------------------------------------------
��������Timer3_init
��  �룺TIMx, ��ʱ��x
				Period������ʱ��ֵ������ֵ��1000
				Prescaler��Ԥ��Ƶ������ֵ��72
��  ������
��  ������ʼ����ʱ����ʹ�õ���ֵ���ã���1ms����һ���ж�
				�ö�ʱ�����ò�������TIM6��TIM7
------------------------------------------------*/
void Timer_Configuration(TIM_TypeDef *TIMx, uint32_t Period, uint8_t Prescaler)
{
  TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
	
  TIM_TimeBaseStructure.TIM_Period = Period;
  TIM_TimeBaseStructure.TIM_Prescaler = Prescaler;
  TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;       //ʱ�ӷָ�
  TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;   //��ʱģʽ
  TIM_TimeBaseInit(TIMx, &TIM_TimeBaseStructure);       				//��ʼ��
  TIM_ITConfig(TIMx, TIM_IT_Update, ENABLE );            			  //ʱ���ж�
  
  TIM_Cmd(TIMx, ENABLE);
}

/*---------------------------------------
��������Timer_Init
��  �룺��
��  ������
��  ����timer��ʼ��������������
---------------------------------------*/
void Timer_Initialization(void)
{
	Timer_RCC_Configuration();
	Timer_NVIC_Configuration();
	Timer_Configuration(TIM3, 1000, 72);
}

/*---------------------------------------
��������NVIC_Configuration
��  �룺��
��  ������
��  ����TIM3�жϺ���
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
