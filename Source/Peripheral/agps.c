#include "agps.h"
#include "Driver/systick.h"
#include "Peripheral/gps.h"

void AGPS_Process(void)
{
		static uint32_t agps_second;
	
		//ʱ�䴥����δ��λ
		if(timerSecondSub(TimerSecond, agps_second) >= 5 && !gps_location.is_location)
		{
				agps_second = TimerSecond;
			
				//����agps��Ϣ
				
		}
}
