#include "agps.h"
#include "Driver/systick.h"
#include "Peripheral/gps.h"

void AGPS_Process(void)
{
		static uint32_t agps_second;
	
		//时间触发，未定位
		if(timerSecondSub(TimerSecond, agps_second) >= 5 && !gps_location.is_location)
		{
				agps_second = TimerSecond;
			
				//发送agps信息
				
		}
}
