#ifndef JTT808_PARAM_H_
#define JTT808_PARAM_H_

#include "stm32f10x.h"

#define LEN_PARAM_SERVER_APN    32
#define LEN_PARAM_SERVER_USER   32
#define LEN_PARAM_SERVER_PSW    32
#define LEN_PARAM_SERVER_DOMAIN 32
#define LEN_PARAM_SERVER_PORT   8
#define LEN_TEL                 12
#define LEN_PARAM_PROVINCE_ID   8
#define LEN_PARAM_CITY_ID       8
#define LEN_PARAM_LICENSE_PLATE 32






typedef __packed struct
{
    u32 initial;	       //初始化标志

    u32 beat_interval;     //终端心跳发送间隔，单位为秒(s)
    u32 tcp_timeout;       //TCP消息应答超时时间，单位为秒(s)
    u32 tcp_repeat_times;  //TCP消息重传次数
    u32 udp_timeout;       //UDP消息应答超时时间，单位为秒(s)
    u32 udp_repeat_times;  //UDP消息重传次数
    u32 sms_timeout;       //SMS消息应答超时时间，单位为秒(s)
    u32 sms_repeat_times;  //SMS消息重传次数


    u8 main_server_apn[LEN_PARAM_SERVER_APN];          //主服务器APN，无线通信拨号访问点。若网络制式为CDMA，则该处为PPP拨号号码
    u8 main_server_user[LEN_PARAM_SERVER_USER];        //主服务器无线通信拨号用户名
    u8 main_server_psw[LEN_PARAM_SERVER_PSW];          //主服务器无线通信拨号密码
    u8 main_server_domain[LEN_PARAM_SERVER_DOMAIN];    //主服务器地址，IP或域名
    u8 backup_server_apn[LEN_PARAM_SERVER_APN];        //备份服务器APN，无线通信拨号访问点
    u8 backup_server_user[LEN_PARAM_SERVER_USER];      //备份服务器无线通信拨号用户名
    u8 backup_server_psw[LEN_PARAM_SERVER_PSW];        //备份服务器无线通信拨号密码
    u8 backup_server_domain[LEN_PARAM_SERVER_DOMAIN];  //备份服务器地址，IP或域名
    u8 server_tcp_port[LEN_PARAM_SERVER_PORT];	       //服务器TCP端口
    u8 server_udp_port[LEN_PARAM_SERVER_PORT];         //服务器UDP端口
    u8 ic_license_authentication_main_server_domain[LEN_PARAM_SERVER_DOMAIN];  //道路运输证IC卡认证主服务器IP地址或域名
    u8 ic_license_authentication_main_server_tcp_port[LEN_PARAM_SERVER_PORT];  //道路运输证IC卡认证主服务器TCP端口
    u8 ic_license_authentication_main_server_udp_port[LEN_PARAM_SERVER_PORT];  //道路运输证IC卡认证主服务器UDP端口
    u8 ic_license_authentication_backup_server_domain[LEN_PARAM_SERVER_DOMAIN];//道路运输证IC卡认证备份服务器IP地址或域名，端口同主服务器

    u32 location_report_strategy;         //位置汇报策略，0：定时汇报；1：定距汇报；2：定时和定距汇报
    u32 location_report_program;          //位置汇报方案，0：根据ACC状态；1：根据登录状态和ACC状态，先判断登录状态，若登录再根据ACC状态
    u32 report_driver_unlogin_interval;   //驾驶员未登录汇报时间间隔，单位为秒(s),>0

    u32 report_at_sleep_interval;         //休眠时汇报时间间隔，单位为秒(s),>0
    u32 report_at_alarm_interval;         //紧急报警时汇报时间间隔，单位为秒(s),>0
    u32 default_report_interval;          //缺省时间汇报间隔，单位为秒(s),>0

    u32 default_report_dst_interval;      //缺省距离汇报间隔，单位为米(m),>0
    u32 report_driver_unlogin_dst_interval;//驾驶员未登录汇报距离间隔，单位为米(m),>0
    u32 report_at_sleep_dst_interval;     //休眠时汇报距离间隔，单位为米(m),>0
    u32 report_at_alarm_dst_interval;     //紧急报警时汇报距离间隔，单位为米(m),>0
    u32 inflect_point_complement_biography;//拐点补传角度，<180°
    u32 bound_radius;                      //电子围栏半径（非法位移阀值），单位为米

    u8 monitor_platform_tel[LEN_TEL];               //监控平台电话号码
    u8 reset_tel[LEN_TEL];                          //复位电话号码，可采用此电话号码拨打终端电话让终端复位
    u8 restore_factory_tel[LEN_TEL];                //恢复出厂设置电话号码，可采用此电话号码拨打终端电话让终端恢复出厂设置
    u8 monitor_platform_sms_tel[LEN_TEL];           //监控平台SMS电话号码
    u8 sms_alarm_tel[LEN_TEL];                      //接收终端SMS文本报警号码
    u32 phone_answer_strategy;                      //终端电话接听策略，0：自动接听；1：ACC ON时自动接听，OFF时手动接听
    u32 talk_max_time;                              //每次最长通话时间，单位为秒(s),0为不允许通话，0xFFFFFFFF为不限制
    u32 talk_max_time_in_mon;                       //当月最长通话时间，单位为秒(s),0为不允许通话，0xFFFFFFFF为不限制
    u8 monitor_tel[LEN_TEL];                        //监听电话号码
    u8 monitor_platform_privilege_sms_tel[LEN_TEL]; //监管平台特权短信号码

    u32 alarm_mask;          //报警屏蔽字。与位置信息汇报消息中的报警标志相对应，相应位为1则相应报警被屏蔽
    u32 alarm_sms_switch;    //报警发送文本SMS开关，与位置信息汇报消息中的报警标志相对应，相应位为1则相应报警时发送文本SMS
    u32 alarm_shoot_switch;  //报警拍摄开关，与位置信息汇报消息中的报警标志相对应，相应位为1则相应报警时摄像头拍摄
    u32 alarm_store_flag;    //报警拍摄存储标志，与位置信息汇报消息中的报警标志相对应，相应位为1则对相应报警时拍的照片进行存储，否则实时上传
    u32 key_flag;            //关键标志，与位置信息汇报消息中的报警标志相对应，相应位为1则对相应报警为关键报警
    u32 top_speed;           //最高速度，单位为公里每小时(km/h)
    u32 speeding_duration;   //超速持续时间，单位为秒(s)
    u32 continouous_drive_time;//连续驾驶时间门限，单位为秒(s)
    u32 drive_time_in_day;   //当天累计驾驶时间门限，单位为秒(s)
    u32 rest_min_time;       //最小休息时间，单位为秒(s)
    u32 stop_max_time;       //最长停车时间，单位为秒(s)
    u32 overspeed_alarm_diff;//超速报警预警差值，单位为0.1km/h
    u32 fatigue_driving_diff;//疲劳驾驶预警差值，单位为秒(s),>0
    u32 collision_alarm_param;//碰撞报警参数设置：b7-b0:碰撞时间，单位4ms；b15-b8:碰撞加速度，单位0.1g，设置范围在0-79之间，默认为10；
    u32 rollover_alarm_param;//侧翻报警参数设置：侧翻角度，单位为度，默认为30度；

    u32 tim_photo;           //定时拍照控制
    u32 dis_photo;           //定距拍照控制


    u32 multimedia_quality;  //图像(视频)质量，1～10,1最好
    u32 brightness;          //亮度，0～255
    u32 contrast;            //对比度，0～127
    u32 saturation;          //饱和度，0～127
    u32 chroma;              //色度，0～255

    u32 mileage;                            //车辆里程表读数，1/10km
    u8 province_id[LEN_PARAM_PROVINCE_ID];	//车辆所在的省域ID，
    u8 city_id[LEN_PARAM_CITY_ID];	        //车辆所在的市域ID，
    u8 lic_plate[LEN_PARAM_LICENSE_PLATE];  //公安交通管理部门颁发的机动车号牌
    u32 lic_plate_color;                    //车牌颜色，按照JT/T415-2006的5.4.12

    u32 gnss_position_type;   //GNSS定位模式，  bit0，0：禁用GPS，1：启用GPS；      bit1，0：禁用北斗，1：启用北斗；    bit2，0：禁用GLONASS，1：启用GLONASS；      bit3，0：禁用Galileo，1：启用Galileo；
    u32 gnss_baudrate;        //GNSS波特率，    0x00:4800;  0x01:9600;  0x02:19200; 0x03:38400; 0x04:47600; 0x05:115200; 
    u32 gnss_output_rate;     //GNSS模块详细定位数据输出频率， 0x00:500ms;  0x01:1000ms(默认);  0x02:2000ms;    0x03:3000ms;    0x04:4000ms;
    u32 gnss_samp_frequency;  //GNSS模块详细定位数据采集频率，单位为秒，默认1；
    u32 gnss_upload_type;     //GNSS模块详细定位数据上传方式，0x00:本地存储，不上传（默认值）；  0x01：按时间间隔上传；    0x02:按距离间隔上传； 0x0b：按累积时间上传，达到传输时间后自动停止上传；  0x0c：按累积距离上传，达到距离后自动停止上传；  0x0d：按累积条数上传，达到上传条数后自动停止上传；
    u32 gnss_upload_set;      //GNSS模块详细定位数据上传设置，0x01:单位为秒；0x02：单位为米；0x0b:单位为秒；0x0c：单位为米；0x0d：单位为条
    
    u32 can_1_samp_time_interval;   //can总线通道1采集时间间隔（ms），0表示不采集
    u32 can_1_upload_time_interval; //can总线通道1上传时间间隔（s），0表示不上传
    u32 can_2_samp_time_interval;   //can总线通道2采集时间间隔（ms），0表示不采集
    u32 can_2_upload_time_interval; //can总线通道2上传时间间隔（s），0表示不上传
    u8 can_ID_samp_set[CAN_ID_TOTAL][8];          //can总线ID单独采集设置，bit63-bit32：表示此ID采集时间间隔（ms），0表示不采集；bit31：canID通道号，0-can1，1-can2；bit30：帧类型，0-标准帧，1-扩展帧；bit29：数据采集方式，0-原始数据看，1-采集区间的计算值；bit28-bit0：can总线ID；

    /*下面为其他附加参数*/
    u8 terminalID[12];             //终端ID
    u8 local_tel[LEN_TEL];         //终端SIM卡号
    u32 wheel_coefficient;         //车轮系数，用于计算里程
    u32 fuel_tank_capacity;
    u8 authorization_no[32];       //鉴权码

	int lock_car;				   //锁车 

#if	USE_TL
	u32 TL_network_type;
	u8 TL_video_ip[20];
	u8 TL_video_control_port[8];
	u8 TL_video_data_port[8];
#endif
} JTT808t_TerminalParamFlash;


extern JTT808t_TerminalParamFlash JTT808_Terminal_Param;

signed char JTT808_Param_exec(u32 ID, u32* size,u8* buf,u8 prio);

void jtt808_param_init(void);

void JTT808_ParamRestoreFactory(void);

void JTT808_Set_Terminal_Param(u8 *pdata, u32 size);

void JTT808_rereg(void);

void jtt808_read_para(void);

void jtt808_save_para(void);
#endif


