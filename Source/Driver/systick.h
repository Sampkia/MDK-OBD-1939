#ifndef __SYSTICK_H
#define __SYSTICK_H

#include "stm32f10x.h"

extern u32 TimerSecond;
extern u32 Timer100ms;

void SysTick_Init(void);
void Delay_1ms(__IO u32 nTime);
void Delay_10us(__IO u32 nTime);
void Delay_us(uint32_t time);
void SysTick_Increment(void);
void SysTick_Decrement(void);
unsigned int timerSecondSub(u32 s1, u32 s2);

#endif
