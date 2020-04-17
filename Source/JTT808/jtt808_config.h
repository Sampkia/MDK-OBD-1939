#ifndef JTT808_CONFIG_H_
#define JTT808_CONFIG_H_

#include "stm32f10x.h"


/*******************APP程序区***************************/
#define NEW_APP_START_ADDR          (0*4096)             //新APP起始地址
#define NEW_APP_END_ADDR            (100*4096-1)         //新APP结束地址
#define OLD_APP_START_ADDR          (100*4096)           //旧APP起始地址
#define OLD_APP_END_ADDR            (200*4096-1)         //旧APP结束地址


/*******************在线升级信息区**********************/
#define IAP_INFO_ADDR    			(201*4096)           //在线升级 

/*******************电子围栏区**************************/
#define CYCLE_AREAR_ADDR            (202*4096)           //圆形围栏
#define RECTANGULAR_AREAR_ADDR      (203*4096)           //矩形围栏
#define POLYGON_AREAR_ADDR          (204*4096)           //多边形围栏
#define ROUTE_AREAR_ADDR            (205*4096)           //路线

/**********************杂项区****************************/
#define EVENT_ADDR                  (206*4096)           //事件存储区
#define MSGBROADCASTMENU_ADDR       (207*4096)           //信息菜单存储区
#define TELDICT_ADDR                (208*4096)           //电话本存储区

/*******************断点续传区**************************/
#define LOACTION_START_ADDR         (210*4096)           //断点续传起始地址
#define LOACTION_END_ADDR           (310*4096-1)         //断点续传结束地址

/*******************消息发送区**************************/
#define JTT808_MSG_START_ADDR       (310*4096)           //消息包保存起始地址
#define JTT808_MSG_END_ADDR         (510*4096-1)         //消息包保存结束地址



/**************************终端属性*****************************/
#define PRODUCTOR_ID        "70407"                                     //生产商ID
#define MODULE_TYPE         "TR2200"                                    //终端型号
#define HARDWARE_VERSION    "HV01"                                      //硬件版本号
#define SOFTWARE_VERSION    "V150516"                                   //固件版本号、软件版本号
#define GPS_MODULE_TYPE     0x03                                        //支持GPS定位，支持北斗定位，不支持Galileo定位，  不支持GLONASS定位
#define GPRS_MODULE_TYPE    (USE_CDMA?0x02:0x01)                        //GPRS：移动联通2G，CDMA：电信2G，TD-SCDMA：移动3G，WCDMA：联通3G，CDMA2000：电信3G，TD-LTE：移动4G
#define TERMINAL_TYPE       0x0F                                        //适用：客车、危险车、货车出租车，不支持硬盘录像，一体机


#define JTT808_MSG_MAX_BODY_SIZE (1023+100)                               //最大消息体长度
#define JTT808_MSG_MAX_SIZE      (16 + JTT808_MSG_MAX_BODY_SIZE + 3+6)//一条消息最大长度=标识位（2）+校验位（1）+消息头（16）+消息体（1024）
#define	JTT808_RECE_TIMEOUT	5											//终端接收一条数据超时时间  100ms单位

#define CAN_ID_TOTAL			  30                                    //支持的最大CAN_ID个数




extern u8 SIM_ICCID[21];                                                //SIM卡CCID号




int API_get_gprs_state(void);
void API_GPRS_Reset(void);
int API_get_send_buf_len(void);
void JTT808_TXD(u8 *src, u32 size);
void JTT808_TXD_BAK(u8 *src, u32 size);
void Terminal_PowerDown(void);
void JTT808_Gprs_Disconnect(void);
void JTT808_Disconnect_All(void);
void API_make_phone_call(void);
void LockCar(void);
void UnLockCar(void);
int API_Get_singal(void);
void JTT808_Forbidden_opendoor(void);
void JTT808_GNSS_Sample(void);
void API_Play_TTS(char*);

u32 two_point_distance(u32,u8,u32,u8,u32,u8,u32,u8);
int API_get_Can_ID_data(void);

#endif
