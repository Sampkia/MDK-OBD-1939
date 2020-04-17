#include "gps.h"
#include "Driver/usart.h"
#include "Public/public.h"
#include "include.h"
#include "debug.h"


GPS_Location_t gps_location={0};

static GPS_MSG_BUF gps_rx={0,0}; 



/*--------------------------------------
��������Get_gps_message
����:rx,	���GPS��Ϣ�Ļ�����
���:��
��������ȡGPS������Ϣ
---------------------------------------*/
static char Get_gps_message(GPS_MSG_BUF* rx)
{
    u16 size;
    u8 val;
    
    if(ioUart.rxBuffOverFlow(GPS_IO))
    {
        ioUart.rxBuffRst(GPS_IO);
        rx->index = 0;
        return 0;
    }

	size=ioUart.rxBuffCount(GPS_IO);

    while(size--)
    {
        val=ioUart.getChar(GPS_IO);
#if DEBUG_GPS
        ioUart.putChar(1, val);
#endif
        switch(val)
        {
            case '$':
                rx->index = 0;
                rx->buf[rx->index++]=val;
								rx->buf[rx->index]=0;
            break;
            case '\n':
                if(rx->buf[rx->index-1]=='\r')
                {
                    rx->buf[rx->index++]=val;
										rx->buf[rx->index]=0;
                    rx->index = 0;
                    return 1;   
                }
                else
                {
                    rx->index = 0;
                    return 0;   
                }
            default:
                if(rx->buf[rx->index-1]=='\r')
                {
                    rx->index = 0;
                    return 0;
                }
                else
                {
                    rx->buf[rx->index++]=val;
										rx->buf[rx->index]=0;

										if(rx->index>=GPSMsg_BufSize)
										{
												rx->index = 0;
												return 0;
										}
                }
            break;
        }
    }
    return 0;
}

#if !USE_RTC
static u8 Is_Leap_Year(u16 year)
{			  
	if(year%4==0) //�����ܱ�4����
	{ 
		if(year%100==0) 
		{ 
			if(year%400==0)return 1;//�����00��β,��Ҫ�ܱ�400���� 	   
			else return 0;   
		}else return 1;   
	}else return 0;	
}
#endif

/*--------------------------------------
��������get_beijing_tim
����:utc_tim,	������־ʱ��
		bj_tim������ʱ��
���:��
��������ȡ����ʱ��
---------------------------------------*/
static void get_beijing_tim(TIM *utc_tim,TIM *bj_tim)
{
	static uint8_t is_agjust = 0;
	static uint32_t rtc_timer = 0;
	u32 seccount=0;
	u16 t;
	u32 daycnt=0;
	u32 temp=0;
	u16 temp1=0;
	const static uint8_t mon_table[12]={31,28,31,30,31,30,31,31,30,31,30,31};

	if(utc_tim->year<1970||utc_tim->year>2099)return;
	for(t=1970;t<utc_tim->year;t++)							//��������ݵ��������
	{
		if(Is_Leap_Year(t))seccount+=31622400;				//�����������
		else seccount+=31536000;			  				//ƽ���������
	}
	for(t=0;t<utc_tim->month-1;t++)	   						//��ǰ���·ݵ����������
	{
		seccount+=(u32)mon_table[t]*86400;					//�·����������
		if(Is_Leap_Year(utc_tim->year)&&t==1)seccount+=86400;//����2�·�����һ���������	   
	}
	seccount+=(u32)(utc_tim->day-1)*86400;					//��ǰ�����ڵ���������� 
	seccount+=(u32)(utc_tim->hour+8)*3600;					//Сʱ������
    seccount+=(u32)utc_tim->minute*60;	 					//����������
	seccount+=utc_tim->second;								//�������Ӽ���ȥ	

	  
 	temp=seccount/86400;   									//�õ�����(��������Ӧ��)
	if(daycnt!=temp)										//����һ����
	{	  
		daycnt=temp;
		temp1=1970;											//��1970�꿪ʼ
		while(temp>=365)
		{				 
			if(Is_Leap_Year(temp1))							//������
			{
				if(temp>=366)temp-=366;						//�����������
				else {temp1++;break;}  
			}
			else temp-=365;	  								//ƽ�� 
			temp1++;
		}
		bj_tim->year=temp1;									//�õ����
		temp1=0;
		while(temp>=28)										//������һ����
		{
			if(Is_Leap_Year(bj_tim->year)&&temp1==1)		//�����ǲ�������/2�·�
			{
				if(temp>=29)temp-=29;						//�����������
				else break; 
			}
			else 
			{
				if(temp>=mon_table[temp1])temp-=mon_table[temp1];//ƽ��
				else break;
			}
			temp1++;  
		}
		bj_tim->month=temp1+1;								//�õ��·�
		bj_tim->day=temp+1;  								//�õ����� 
	}
	temp=seccount%86400;     								//�õ�������   	   
	bj_tim->hour=temp/3600;     							//Сʱ
	bj_tim->minute=(temp%3600)/60; 							//����	
	bj_tim->second=(temp%3600)%60; 							//����

  bj_tim->bcd_tim[0]=((((bj_tim->year-2000)&0xFF)/10)<<4)|(((bj_tim->year-2000)&0xFF)%10);
	bj_tim->bcd_tim[1]=((( bj_tim->month     &0xFF)/10)<<4)|(( bj_tim->month     &0xFF)%10);
	bj_tim->bcd_tim[2]=((( bj_tim->day       &0xFF)/10)<<4)|(( bj_tim->day       &0xFF)%10);
	bj_tim->bcd_tim[3]=((( bj_tim->hour      &0xFF)/10)<<4)|(( bj_tim->hour      &0xFF)%10);
	bj_tim->bcd_tim[4]=((( bj_tim->minute    &0xFF)/10)<<4)|(( bj_tim->minute    &0xFF)%10);
	bj_tim->bcd_tim[5]=((( bj_tim->second    &0xFF)/10)<<4)|(( bj_tim->second    &0xFF)%10);
	
//	if((gps_location.sat_num)&&(!is_agjust))
	if(gps_location.sat_num && (timerSecondSub(TimerSecond, rtc_timer) > 600 || !is_agjust))
	{
			is_agjust=1;
			rtc_timer = TimerSecond;

#if USE_RTC
			RTC_Set(bj_tim->year, bj_tim->month, bj_tim->day, bj_tim->hour, bj_tim->minute, bj_tim->second);
#endif
	}
}

/*--------------------------------------
��������prase_rmc
����:buf, 
���:��
����������GPS��������
---------------------------------------*/
static void prase_rmc(char* buf)
{
	char *pChar1,*pChar2,*pChar3;
	u8 ui1,ui2,ui3;
	u8 len;
	double d1 = 0.0;
	
	pChar1 = strStrfN(buf, ",",1);       //��һ������ ����RMC���ݰ�
	pChar2 = strStrfN(buf, ",",2);
	if((pChar1)&&(pChar2))
	{ 
		pChar1++;
		len=pChar2-pChar1;

		if((len)&&(len<15))
		{
			pChar3 = strstr(pChar1, ".");
			if((pChar3)&&(pChar3<pChar2))
			{  
				len=pChar3-pChar1;
				if(len==6) 
				{                    
						ui1 = (pChar1[0] - '0')*10 + (pChar1[1] - '0');  //ʱ
						ui2 = (pChar1[2] - '0')*10 + (pChar1[3] - '0');  //��
						ui3 = (pChar1[4] - '0')*10 + (pChar1[5] - '0');  //��
						gps_location.utc_tim.hour = ui1;
						gps_location.utc_tim.minute = ui2;
						gps_location.utc_tim.second = ui3;
				}
			}
		}
  }

	pChar1 = strStrfN(buf, ",",2);         //�ڶ������ţ���Ч��λ
	pChar2 = strStrfN(buf, ",",3);
	if ((pChar1)&&(pChar2))
	{
		++pChar1;
		len=pChar2-pChar1;

		if(len==1)
		{
			if(pChar1[0] == 'A')gps_location.is_location = 1;
			else				gps_location.is_location = 0;
		}
	}

	pChar1 = strStrfN(buf, ",",3);   //���������ţ�γ��
	pChar2 = strStrfN(buf, ",",4);
    if((pChar1)&&(pChar2))
	{
		pChar1++;
		len=pChar2-pChar1;

		if((len)&&(len<15))
		{
			pChar3 = strstr(pChar1, ".");
			if((pChar3)&&(pChar3 < pChar2))
			{	
				ui1 = pChar3 - pChar1;
				ui2 = (pChar2 - pChar3) - 1; 
				if((ui1==4)&&(ui2<=10))
				{
					ui1 = (pChar1[0] - '0')*10 + (pChar1[1] - '0');
					pChar1+=2;
					d1 = atof(pChar1);
					gps_location.latitude=(ui1+d1/60)*1000000;
				}
			}
		}
	}
                                 
	pChar1 = strStrfN(buf, ",",4);            //���ĸ����ţ�γ�ȷ���
	pChar2 = strStrfN(buf, ",",5);
	if ((pChar1)&&(pChar2))
	{
		++pChar1;
		len=pChar2-pChar1;

		if(len==1)
		{
			if (pChar1[0] == 'S')gps_location.latitude_type = 1;
			else 				 gps_location.latitude_type = 0;
		}
	}

 	pChar1 = strStrfN(buf, ",",5); //��������ţ�����
	pChar2 = strStrfN(buf, ",",6);
  if((pChar1)&&(pChar2))
	{
		pChar1++;
		len=pChar2-pChar1;

		if((len)&&(len<16))
		{
			pChar3 = strstr(pChar1, ".");
			if((pChar3)&&(pChar3<pChar2))
			{
				ui1 = pChar3 - pChar1;
				ui2 = (pChar2 - pChar3) - 1;
				if((ui1==5)&&(ui2<=10))
				{
				    ui1 = (pChar1[0] - '0')*100 + (pChar1[1] - '0')*10 + (pChar1[2] - '0');
						pChar1+=3;
						d1 = atof(pChar1);		
						gps_location.longitude=(ui1+d1/60)*1000000;	
				}
			}
		}
	}

	pChar1 = strStrfN(buf, ",",6);              //���������ţ����ȷ���
	pChar2 = strStrfN(buf, ",",7);
	if((pChar1)&&(pChar2))
	{
		++pChar1;
		len=pChar2-pChar1;

		if(len==1)
		{
			if(pChar1[0] == 'W')gps_location.longitude_type=1;
			else 				gps_location.longitude_type=0;
		}
	}

	pChar1 = strStrfN(buf, ",",7);  //���߸����ţ��ٶ�
	pChar2 = strStrfN(buf, ",",8);
  if((pChar1)&&(pChar2))
	{
		pChar1++;
		len=pChar2-pChar1;

		if((len)&&(len<8))
		{
			d1 = atof(pChar1);
			gps_location.speed = d1*1.852*10;
		}
	}

	pChar1 = strStrfN(buf, ",",8);          //�ڰ˸����ţ������
	pChar2 = strStrfN(buf, ",",9); 
  if((pChar1)&&(pChar2))
	{
		pChar1++;
		len=pChar2-pChar1;

		if((len)&&(len<8))
		{
			d1 = atof(pChar1);
        	gps_location.azimuth = d1;	
		}
	}

	pChar1 = strStrfN(buf, ",",9);   //�ھŸ����� ������
	pChar2 = strStrfN((char *)buf, ",",10);
   if((pChar1)&&(pChar2))
	{	
		pChar1++;
		len=pChar2-pChar1;

		if(len==6)
		{
			ui1 = (pChar1[0] - '0')*10 + (pChar1[1] - '0');    //��
			ui2 = (pChar1[2] - '0')*10 + (pChar1[3] - '0');    //��
			ui3 = (pChar1[4] - '0')*10 + (pChar1[5] - '0');    //��
			gps_location.utc_tim.day = ui1;
			gps_location.utc_tim.month = ui2;
			gps_location.utc_tim.year = ui3+2000; 

			gps_location.utc_tim.bcd_tim[0]=((((gps_location.utc_tim.year-2000)&0xFF)/10)<<4)|(((gps_location.utc_tim.year-2000)&0xFF)%10);
			gps_location.utc_tim.bcd_tim[1]=((( gps_location.utc_tim.month     &0xFF)/10)<<4)|((gps_location.utc_tim.month      &0xFF)%10);
			gps_location.utc_tim.bcd_tim[2]=((( gps_location.utc_tim.day       &0xFF)/10)<<4)|((gps_location.utc_tim.day        &0xFF)%10);
			gps_location.utc_tim.bcd_tim[3]=((( gps_location.utc_tim.hour      &0xFF)/10)<<4)|((gps_location.utc_tim.hour       &0xFF)%10);
			gps_location.utc_tim.bcd_tim[4]=((( gps_location.utc_tim.minute    &0xFF)/10)<<4)|((gps_location.utc_tim.minute     &0xFF)%10);
			gps_location.utc_tim.bcd_tim[5]=((( gps_location.utc_tim.second    &0xFF)/10)<<4)|((gps_location.utc_tim.second     &0xFF)%10);
            
			get_beijing_tim(&gps_location.utc_tim,&gps_location.bj_tim);
		}
	}
}


/*--------------------------------------
��������prase_gga
����:buf, 
���:��
����������GPS�����������Ǹ��������θ߶�
---------------------------------------*/
static void prase_gga(char* buf)
{
  char *pChar1,*pChar2;
	u8 len;
	double d1 = 0.0;

  pChar1 = strStrfN(buf, ",", 7);
	pChar2 = strStrfN(buf, ",", 8);
	if((pChar1)&&(pChar2))
	{	
		pChar1++;
		len=pChar2-pChar1;
		
			if(len==1)
			{
					gps_location.sat_num = pChar1[0]-'0';
			}
			else if(len==2)
			{
					gps_location.sat_num=(pChar1[0]-'0')*10+pChar1[1]-'0';  //���Ǹ���
			}
	}

	pChar1 = strStrfN(buf, ",", 9);
	pChar2 = strStrfN(buf, ",", 10);
	if((pChar1)&&(pChar2))
	{	
		pChar1++;
		len=pChar2-pChar1;

		if((len)&&(len<8))
		{
			d1 = atof(pChar1);
      gps_location.altitude = d1;  //���θ߶�	
		}
	}
}

/*--------------------------------------
��������parse_acc
����:buf, 
���:��
����������ACC����
---------------------------------------*/
void parse_acc(char* buf)
{
		char *pChar1;
		int i;
	
	
		for(i=0; i<3; i++)
		{
				pChar1 = strStrfN(buf, ",", 1+i);
				if(pChar1)
				{	
						pChar1++;
//						accel[i] = atoi(pChar1);
				}
		}
}


/*--------------------------------------
��������gps_parse
����:buf, 
���:��
����������GPS�����������Ǹ��������θ߶�
---------------------------------------*/
static void gps_parse(GPS_MSG_BUF* rx)
{
    if(strstr((const char*)rx->buf, "GPRMC"))
    {
        gps_location.location_type_GPS=1;
				gps_location.location_type_BD=0;
    }
    if(strstr((const char*)rx->buf, "BDRMC"))
    {
				gps_location.location_type_GPS=0;
        gps_location.location_type_BD=1;
    }
    if(strstr((const char*)rx->buf, "GNRMC"))
    {
        gps_location.location_type_GPS=1;
        gps_location.location_type_BD=1;
    }

		if(strstr((const char*)rx->buf, "ANTENNA OPEN")) gps_location.antenna_sta=ANTENNA_OPEN;
		if(strstr((const char*)rx->buf, "ANTENNA OK"))	gps_location.antenna_sta=ANTENNA_OK;
		if(strstr((const char*)rx->buf, "ANTENNA SHORT"))gps_location.antenna_sta=ANTENNA_SHORT;

    if(strstr((const char*)rx->buf, "RMC"))
    {
        prase_rmc(rx->buf);
    }
    if(strstr((const char*)rx->buf, "GGA"))
    {
        prase_gga(rx->buf);
    }
		
#if USE_INS
		if(strstr((const char*)rx->buf, "ACCEL"))
		{
				parse_acc(rx->buf);
		}
#endif
}

/*--------------------------------------
��������scan_to_reset_gps
����:buf, 
���:��
��������λGPSģ��
---------------------------------------*/
void scan_to_reset_gps(void)
{
    static unsigned int GPS_NO_LOCATION_TIM=0;
//		static uint8_t Reset_Flag = 0;
	
//    if(gps_location.is_location==0 || Reset_Flag)
		if(gps_location.is_location==0)
    {
        if(GPS_NO_LOCATION_TIM==0)
        {
            GPS_NO_LOCATION_TIM=TimerSecond;
        }
        if(timerSecondSub(TimerSecond, GPS_NO_LOCATION_TIM) > GPS_RESET_TIME)
        {
            GPS_PowerOff();
//						Reset_Flag = 1;
						GPS_NO_LOCATION_TIM=TimerSecond;
						IWDGFeed();
            Delay_1ms(2000);
            GPS_PowerOn();
						GPS_NO_LOCATION_TIM=TimerSecond;
        }
//				if(Reset_Flag && timerSecondSub(TimerSecond, GPS_NO_LOCATION_TIM) > 2)
//				{
//					Reset_Flag = 0;
//				  GPS_PowerOn();
//					GPS_NO_LOCATION_TIM=TimerSecond;
//				}
    }
    else
    {
        GPS_NO_LOCATION_TIM=0;
    }
}


void gps_process(void)
{
    static unsigned int GPS_catch_time=0;

    if(Get_gps_message(&gps_rx)==1)
    {
        gps_parse(&gps_rx);
        scan_to_reset_gps();  //��GPS����λ�����涨��ʱ��������GPSģ��

				GPS_catch_time=TimerSecond;
        gps_location.GNSS_module_fault=0;
    }
    else
    {
        if(timerSecondSub(TimerSecond, GPS_catch_time) > 30)	//�������涨ʱ��û�н��յ���λģ�����������Ϊģ�����
        {
            gps_location.GNSS_module_fault=1;
        }   
    }
}


void gps_setup(void)
{
    GPS_PowerOn();
}


void reset_gps(void)
{
	
}
