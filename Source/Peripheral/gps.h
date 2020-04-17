#ifndef GPS_H_
#define GPS_H_

#include "stm32f10x.h"

#define GPS_IO  		3			   //GPSʹ�ô���3
#define GPSMsg_BufSize  150			   //һ��GPS���ݳ���
#define GPS_RESET_TIME		600				//GPS��λʱ��


#define	GPS_PowerOn()				{GPIO_SetBits(GPS_PWR_GPIO_GROUP, GPS_PWR_GPIO_PIN);}	   //GPSģ���Դ����
#define	GPS_PowerOff()			{GPIO_ResetBits(GPS_PWR_GPIO_GROUP, GPS_PWR_GPIO_PIN);}   //GPSģ���Դ�ر�

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
}TIM;                       //UTCʱ��

typedef enum
{
	ANTENNA_OK=0,			//��������
	ANTENNA_OPEN,			//���߶Ͽ�
	ANTENNA_SHORT			//���߶�·
}ANTENNA_STA;

typedef struct
{
    u8 is_location;		     		//0: δ��λ, 1: ��λ

    u8 location_type_GPS;    	//0����GPS��λ��1��GPS��λ
    u8 location_type_BD;     	//0���Ǳ�����λ��1��������λ

    u8 GNSS_module_fault;    	//0��GNSSģ��������1��GNSSģ�����
		u8 antenna_sta;			 			//0���������� 1�����߶�·   2�����߶�·


		TIM	utc_tim;			 				//UTCʱ��
		TIM bj_tim;				 				//����ʱ��

		u32 latitude;		     			//�����֮һ��
    u8 latitude_type;		 			//0: N ��γ, 1: S ��γ

		u32 longitude;	         	//�����֮һ��
    u8 longitude_type;	     	//0: E ����, 1: W ����

		int16_t altitude;	     		//���θ߶ȣ���λΪm
		int16_t speed;		     		// ����0.1km/h
		int16_t azimuth;		 			//0~359
    u8 sat_num;              	//��λ���Ǹ���	
}GPS_Location_t;


extern GPS_Location_t gps_location;

void gps_setup(void);
void gps_process(void);
void reset_gps(void);

#endif     

