/*******************************************************************************
** �ļ���: 	SysTick.c
** �汾:  		1.0
** ��������: 	RealView MDK-ARM 4.20
** ����: 		linlifeng
** ��������: 	2012-12-15
** ����:		SysTick ϵͳ�δ�ʱ��10us�жϺ�����,�ж�ʱ����������ã�
**              ���õ��� 1us 10us 1ms 1s�жϡ�
** ����ļ�:	��
** �޸���־��	2012-12-15   �����ĵ�
*******************************************************************************/
#include "systick.h"

u32 TimerSecond = 0;         /* 1s��λ�������� */
u32 TimingDelay = 0;         /* 10us��λ�ݼ������� */
u32 timer_counter = 0;       /* systick���������� */
u32 Timer100ms=0;

/*********************************************************
*Function Name     :SysTick_Init
*Copyright         :���ݴ�Ӯ
*CPU               :STM32
*Create Date       :2012/12/15
*Author            :Lin Lifeng
*Abstract          :��ʼ��SysTick-10us�ж�1��
*Input             :void 
                    void
*Output            :void
*History           :2012/12/15 �¹�����
*********************************************************/
void SysTick_Init(void)
{
	/* SystemCoreClock / 10     100ms�ж�һ��
	 * SystemCoreClock / 1000     1ms�ж�һ��
	 * SystemCoreClock / 100000  10us�ж�һ��
	 * SystemCoreClock / 1000000  1us�ж�һ��
	 */
	if(SysTick_Config(SystemCoreClock/100000))
	{ 
		/* Capture error */ 
		while (1);
	}
}


void Delay_us(uint32_t time)
{
		__asm("nop");
		__asm("nop");
		__asm("nop");
}
/*********************************************************
*Function Name     :Delay_1ms
*Copyright         :���ݴ�Ӯ
*CPU               :STM32
*Create Date       :2012/12/15
*Author            :Lin Lifeng
*Abstract          :1ms��ʱ����,1msΪһ����λ
*                   Delay_1ms(10),��ʵ�ֵ���ʱΪ10*1ms=10ms
*Input             :void 
                    void
*Output            :void
*History           :2012/12/15 �¹�����
*********************************************************/
void Delay_1ms(__IO u32 nTime)
{ 
	TimingDelay = nTime*100;

	while(TimingDelay != 0);
}

/*********************************************************
*Function Name     :Delay_10us
*Copyright         :���ݴ�Ӯ
*CPU               :STM32
*Create Date       :2012/12/15
*Author            :Lin Lifeng
*Abstract          :10us��ʱ����,10usΪһ����λ
*                   Delay_10us(10),��ʵ�ֵ���ʱΪ10*10us=100us
*Input             :void 
                    void
*Output            :void
*History           :2012/12/15 �¹�����
*********************************************************/
void Delay_10us(__IO u32 nTime)
{ 
	TimingDelay = nTime;

	while(TimingDelay != 0);
}

/*********************************************************
*Function Name     :SysTick_Increment
*Copyright         :���ݴ�Ӯ
*CPU               :STM32
*Create Date       :2012/12/15
*Author            :Lin Lifeng
*Abstract          :��ȡ���ĳ���,1s��λ����
*                   ��SysTick�жϺ��� SysTick_Handler()����
*Input             :void 
                    void
*Output            :void
*History           :2012/12/15 �¹�����
*********************************************************/  
void SysTick_Increment(void)
{
	timer_counter++;

	if(timer_counter >= 100000)         /* TimerSecond+1 ʱ���ʱ1�� */
	{
		timer_counter = 0;

		if(TimerSecond == 0xFFFFFFFFLL)
		{
			TimerSecond = 0;
		}
		else
		{
			TimerSecond++;
		}
	}

	if(!(timer_counter % 10000))		/* Timer100ms+1 ʱ���ʱ0.1�� */
	{
		if(Timer100ms == 0xFFFFFFFFLL)
		{
			Timer100ms = 0;
		}
		else
		{
			Timer100ms++;
		}
	}
}

/*********************************************************
*Function Name     :SysTick_Decrement
*Copyright         :���ݴ�Ӯ
*CPU               :STM32
*Create Date       :2012/12/15
*Author            :Lin Lifeng
*Abstract          :��ȡ���ĳ���100ms��λ�ݼ�
*                   ��SysTick�жϺ��� SysTick_Handler()����
*Input             :void
                    void
*Output            :void
*History           :2012/12/15 �¹�����
*********************************************************/
void SysTick_Decrement(void)
{
  if(TimingDelay != 0x00)
  {
    TimingDelay--;
  }
}

/*********************************************************
*Function Name     :timerSecondSub
*Copyright         :���ݴ�Ӯ
*CPU               :STM32
*Create Date       :2012/12/15
*Author            :Lin Lifeng
*Abstract          :����ʱ����ʱ������
*                   Ӧ�ü���������s1>s2
*Input             :void
*Output            :void
*History           :2012/12/15 �¹�����
*********************************************************/
unsigned int timerSecondSub(u32 s1, u32 s2)
{
	if(s1 >= s2)
		return (s1-s2);
	else
		return ((unsigned int)((long long)s1 + 0x100000000LL - s2));
}
