#include "wdg.h"

/*---------------------------------------------------------
函数名：IWDGInit
输  入：prer，预分频值。典型数值 7，低3位有效
				Reload_Value,重加在值。典型数值 2344，低11位有效
输  出：无
描  述：初始化看门狗
时间计算(大概):Tout=((4*2^prer)*rlr)/40 (ms).
Tout = 256*2344/40 = 15001.6 ms = 15秒
----------------------------------------------------------*/
void IWDGInit(uint8_t Prescaler,uint16_t Reload_Value) 
{
	//IWDG_WriteAccess_Enable:使能对寄存器IWDG_PR和IWDG_RLR的写操作
	IWDG_WriteAccessCmd(IWDG_WriteAccess_Enable);
	
	//设置IWDG预分频值:设置IWDG预分频值为256
	IWDG_SetPrescaler(Prescaler);  
		
	//设置IWDG重装载值 Set counter reload value to 2344 
	IWDG_SetReload(Reload_Value);
	
	// Reload IWDG counter
	IWDG_ReloadCounter(); 		//按照IWDG重装载寄存器的值重装载IWDG计数器

	IWDG_Enable();						//enable wdg
}

/*---------------------------------------
函数名：IWDGFeed
输  入：无
输  出：无
描  述：喂狗
---------------------------------------*/
void IWDGFeed(void)
{	
	IWDG_ReloadCounter();	 //按照IWDG重装载寄存器的值重装载IWDG									   
}
