#include "gps.h"
#include "Driver/usart.h"
#include "Public/public.h"
#include "include.h"
#include "debug.h"


GPS_Location_t gps_location={0};

static GPS_MSG_BUF gps_rx={0,0}; 



/*--------------------------------------
函数名：Get_gps_message
输入:rx,	存放GPS信息的缓冲区
输出:无
描述：获取GPS串口信息
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
	if(year%4==0) //必须能被4整除
	{ 
		if(year%100==0) 
		{ 
			if(year%400==0)return 1;//如果以00结尾,还要能被400整除 	   
			else return 0;   
		}else return 1;   
	}else return 0;	
}
#endif

/*--------------------------------------
函数名：get_beijing_tim
输入:utc_tim,	格林日志时间
		bj_tim，北京时间
输出:无
描述：获取北京时间
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
	for(t=1970;t<utc_tim->year;t++)							//把所有年份的秒钟相加
	{
		if(Is_Leap_Year(t))seccount+=31622400;				//闰年的秒钟数
		else seccount+=31536000;			  				//平年的秒钟数
	}
	for(t=0;t<utc_tim->month-1;t++)	   						//把前面月份的秒钟数相加
	{
		seccount+=(u32)mon_table[t]*86400;					//月份秒钟数相加
		if(Is_Leap_Year(utc_tim->year)&&t==1)seccount+=86400;//闰年2月份增加一天的秒钟数	   
	}
	seccount+=(u32)(utc_tim->day-1)*86400;					//把前面日期的秒钟数相加 
	seccount+=(u32)(utc_tim->hour+8)*3600;					//小时秒钟数
    seccount+=(u32)utc_tim->minute*60;	 					//分钟秒钟数
	seccount+=utc_tim->second;								//最后的秒钟加上去	

	  
 	temp=seccount/86400;   									//得到天数(秒钟数对应的)
	if(daycnt!=temp)										//超过一天了
	{	  
		daycnt=temp;
		temp1=1970;											//从1970年开始
		while(temp>=365)
		{				 
			if(Is_Leap_Year(temp1))							//是闰年
			{
				if(temp>=366)temp-=366;						//闰年的秒钟数
				else {temp1++;break;}  
			}
			else temp-=365;	  								//平年 
			temp1++;
		}
		bj_tim->year=temp1;									//得到年份
		temp1=0;
		while(temp>=28)										//超过了一个月
		{
			if(Is_Leap_Year(bj_tim->year)&&temp1==1)		//当年是不是闰年/2月份
			{
				if(temp>=29)temp-=29;						//闰年的秒钟数
				else break; 
			}
			else 
			{
				if(temp>=mon_table[temp1])temp-=mon_table[temp1];//平年
				else break;
			}
			temp1++;  
		}
		bj_tim->month=temp1+1;								//得到月份
		bj_tim->day=temp+1;  								//得到日期 
	}
	temp=seccount%86400;     								//得到秒钟数   	   
	bj_tim->hour=temp/3600;     							//小时
	bj_tim->minute=(temp%3600)/60; 							//分钟	
	bj_tim->second=(temp%3600)%60; 							//秒钟

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
函数名：prase_rmc
输入:buf, 
输出:无
描述：解析GPS串口数据
---------------------------------------*/
static void prase_rmc(char* buf)
{
	char *pChar1,*pChar2,*pChar3;
	u8 ui1,ui2,ui3;
	u8 len;
	double d1 = 0.0;
	
	pChar1 = strStrfN(buf, ",",1);       //第一个逗号 解析RMC数据包
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
						ui1 = (pChar1[0] - '0')*10 + (pChar1[1] - '0');  //时
						ui2 = (pChar1[2] - '0')*10 + (pChar1[3] - '0');  //分
						ui3 = (pChar1[4] - '0')*10 + (pChar1[5] - '0');  //秒
						gps_location.utc_tim.hour = ui1;
						gps_location.utc_tim.minute = ui2;
						gps_location.utc_tim.second = ui3;
				}
			}
		}
  }

	pChar1 = strStrfN(buf, ",",2);         //第二个逗号，有效定位
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

	pChar1 = strStrfN(buf, ",",3);   //第三个逗号，纬度
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
                                 
	pChar1 = strStrfN(buf, ",",4);            //第四个逗号，纬度方向
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

 	pChar1 = strStrfN(buf, ",",5); //第五个逗号，经度
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

	pChar1 = strStrfN(buf, ",",6);              //第六个逗号，经度方向
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

	pChar1 = strStrfN(buf, ",",7);  //第七个逗号，速度
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

	pChar1 = strStrfN(buf, ",",8);          //第八个逗号，航向角
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

	pChar1 = strStrfN(buf, ",",9);   //第九个逗号 日月年
	pChar2 = strStrfN((char *)buf, ",",10);
   if((pChar1)&&(pChar2))
	{	
		pChar1++;
		len=pChar2-pChar1;

		if(len==6)
		{
			ui1 = (pChar1[0] - '0')*10 + (pChar1[1] - '0');    //日
			ui2 = (pChar1[2] - '0')*10 + (pChar1[3] - '0');    //月
			ui3 = (pChar1[4] - '0')*10 + (pChar1[5] - '0');    //年
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
函数名：prase_gga
输入:buf, 
输出:无
描述：解析GPS串口数据卫星个数，海拔高度
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
					gps_location.sat_num=(pChar1[0]-'0')*10+pChar1[1]-'0';  //卫星个数
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
      gps_location.altitude = d1;  //海拔高度	
		}
	}
}

/*--------------------------------------
函数名：parse_acc
输入:buf, 
输出:无
描述：解析ACC数据
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
函数名：gps_parse
输入:buf, 
输出:无
描述：解析GPS串口数据卫星个数，海拔高度
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
函数名：scan_to_reset_gps
输入:buf, 
输出:无
描述：复位GPS模块
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
        scan_to_reset_gps();  //当GPS不定位超过规定的时间则重启GPS模块

				GPS_catch_time=TimerSecond;
        gps_location.GNSS_module_fault=0;
    }
    else
    {
        if(timerSecondSub(TimerSecond, GPS_catch_time) > 30)	//当超过规定时间没有接收到定位模块的数据则认为模块出错
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
