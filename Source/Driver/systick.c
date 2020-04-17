/*******************************************************************************
** 文件名: 	SysTick.c
** 版本:  		1.0
** 工作环境: 	RealView MDK-ARM 4.20
** 作者: 		linlifeng
** 生成日期: 	2012-12-15
** 功能:		SysTick 系统滴答时钟10us中断函数库,中断时间可自由配置，
**              常用的有 1us 10us 1ms 1s中断。
** 相关文件:	无
** 修改日志：	2012-12-15   创建文档
*******************************************************************************/
#include "systick.h"

u32 TimerSecond = 0;         /* 1s单位步进计数 */
u32 TimingDelay = 0;         /* 10us单位递减计数器 */
u32 timer_counter = 0;       /* systick步进计数器 */
u32 Timer100ms=0;

/*********************************************************
*Function Name     :SysTick_Init
*Copyright         :广州创赢
*CPU               :STM32
*Create Date       :2012/12/15
*Author            :Lin Lifeng
*Abstract          :初始化SysTick-10us中断1次
*Input             :void 
                    void
*Output            :void
*History           :2012/12/15 新规作成
*********************************************************/
void SysTick_Init(void)
{
	/* SystemCoreClock / 10     100ms中断一次
	 * SystemCoreClock / 1000     1ms中断一次
	 * SystemCoreClock / 100000  10us中断一次
	 * SystemCoreClock / 1000000  1us中断一次
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
*Copyright         :广州创赢
*CPU               :STM32
*Create Date       :2012/12/15
*Author            :Lin Lifeng
*Abstract          :1ms延时程序,1ms为一个单位
*                   Delay_1ms(10),则实现的延时为10*1ms=10ms
*Input             :void 
                    void
*Output            :void
*History           :2012/12/15 新规作成
*********************************************************/
void Delay_1ms(__IO u32 nTime)
{ 
	TimingDelay = nTime*100;

	while(TimingDelay != 0);
}

/*********************************************************
*Function Name     :Delay_10us
*Copyright         :广州创赢
*CPU               :STM32
*Create Date       :2012/12/15
*Author            :Lin Lifeng
*Abstract          :10us延时程序,10us为一个单位
*                   Delay_10us(10),则实现的延时为10*10us=100us
*Input             :void 
                    void
*Output            :void
*History           :2012/12/15 新规作成
*********************************************************/
void Delay_10us(__IO u32 nTime)
{ 
	TimingDelay = nTime;

	while(TimingDelay != 0);
}

/*********************************************************
*Function Name     :SysTick_Increment
*Copyright         :广州创赢
*CPU               :STM32
*Create Date       :2012/12/15
*Author            :Lin Lifeng
*Abstract          :获取节拍程序,1s单位步进
*                   在SysTick中断函数 SysTick_Handler()调用
*Input             :void 
                    void
*Output            :void
*History           :2012/12/15 新规作成
*********************************************************/  
void SysTick_Increment(void)
{
	timer_counter++;

	if(timer_counter >= 100000)         /* TimerSecond+1 时间计时1秒 */
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

	if(!(timer_counter % 10000))		/* Timer100ms+1 时间计时0.1秒 */
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
*Copyright         :广州创赢
*CPU               :STM32
*Create Date       :2012/12/15
*Author            :Lin Lifeng
*Abstract          :获取节拍程序，100ms单位递减
*                   在SysTick中断函数 SysTick_Handler()调用
*Input             :void
                    void
*Output            :void
*History           :2012/12/15 新规作成
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
*Copyright         :广州创赢
*CPU               :STM32
*Create Date       :2012/12/15
*Author            :Lin Lifeng
*Abstract          :两个时间点的时间间隔差
*                   应用假设条件：s1>s2
*Input             :void
*Output            :void
*History           :2012/12/15 新规作成
*********************************************************/
unsigned int timerSecondSub(u32 s1, u32 s2)
{
	if(s1 >= s2)
		return (s1-s2);
	else
		return ((unsigned int)((long long)s1 + 0x100000000LL - s2));
}
