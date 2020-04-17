#ifndef GPS_H_
#define GPS_H_

#include "stm32f10x.h"

#define GPS_IO  		3			   //GPS使用串口3
#define GPSMsg_BufSize  150			   //一条GPS数据长度
#define GPS_RESET_TIME		600				//GPS复位时间


#define	GPS_PowerOn()				{GPIO_SetBits(GPS_PWR_GPIO_GROUP, GPS_PWR_GPIO_PIN);}	   //GPS模块电源开启
#define	GPS_PowerOff()			{GPIO_ResetBits(GPS_PWR_GPIO_GROUP, GPS_PWR_GPIO_PIN);}   //GPS模块电源关闭

typedef struct
{
	char buf[GPSMsg_BufSize];
	int index;
}GPS_MSG_BUF;

typedef struct
{
	u8  bcd_tim[6];
	u16 year;
	u8  month;
	u8  day;
	u8  hour;
	u8  minute;
	u8  second;
}TIM;                       //UTC时间

typedef enum
{
	ANTENNA_OK=0,			//天线正常
	ANTENNA_OPEN,			//天线断开
	ANTENNA_SHORT			//天线短路
}ANTENNA_STA;

typedef struct
{
    u8 is_location;		     		//0: 未定位, 1: 定位

    u8 location_type_GPS;    	//0：非GPS定位，1：GPS定位
    u8 location_type_BD;     	//0：非北斗定位，1：北斗定位

    u8 GNSS_module_fault;    	//0：GNSS模块正常，1：GNSS模块故障
		u8 antenna_sta;			 			//0：天线正常 1：天线断路   2：天线短路


		TIM	utc_tim;			 				//UTC时间
		TIM bj_tim;				 				//北京时间

		u32 latitude;		     			//百万分之一度
    u8 latitude_type;		 			//0: N 北纬, 1: S 南纬

		u32 longitude;	         	//百万分之一度
    u8 longitude_type;	     	//0: E 东经, 1: W 西经

		int16_t altitude;	     		//海拔高度，单位为m
		int16_t speed;		     		// 精度0.1km/h
		int16_t azimuth;		 			//0~359
    u8 sat_num;              	//定位卫星个数	
}GPS_Location_t;


extern GPS_Location_t gps_location;

void gps_setup(void);
void gps_process(void);
void reset_gps(void);

#endif     

