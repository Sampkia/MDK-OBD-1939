#ifndef __GPRS_H__
#define __GPRS_H__

#include "stm32f10x.h"
#include "Driver/usart.h"

#define GPRS_RX_buff_SIZE				1500				  //socket接收缓冲区
#define GPRS_TX_buff_SIZE				1500				  //socket发送缓冲区

#define GPRS_IO							2					  //GPRS模块所使用的串口号
#define SOCKET_TOTAL					1					  //socket连接总数 按需求修改

#define	GPRS_PowerOn()					GPIO_SetBits(GPIOB, GPIO_Pin_6)
#define	GPRS_PowerOff()					GPIO_ResetBits(GPIOB, GPIO_Pin_6)
#define TTS_ON()						//GPIO_SetBits(GPIOE, GPIO_Pin_7)
#define TTS_OFF()						//GPIO_ResetBits(GPIOE, GPIO_Pin_7)





/* 发送给GPRS模块的命令的类型 */
typedef enum
{
	CMD_GPRS_NULL=0,					//当前没有AT指令执行

	CMD_GPRS_BASIC_SETTING,				//基础设置

	//网络基础
	CMD_GPRS_APN_SET,					//设置APN
	CMD_GPRS_PPP_OPEN,					//打开链路
	CMD_GPRS_PPP_CLOSE,					//断开链路
	CMD_GPRS_LOC_IP_GET,				//获取本地IP
	CMD_GPRS_DNS_IP_GET,				//域名解析

	//TCP、UDP
	CMD_GPRS_NET_CONNECT,   			//TCP、UDP连接
	CMD_GPRS_NET_SEND,					//TCP、UDP数据发送
	CMD_GPRS_NET_CLOSE,					//TCP、UDP关闭

	//FTP
	CMD_GPRS_FTP_LOGIN,					//FTP登录
	CMD_GPRS_FTP_TYPE,					//FTP文件类型
	CMD_GPRS_FTP_DOWNLOAD,				//FTP文件下载
	CMD_GPRS_FTP_QUIT,					//TTP退出

	//短信
	CMD_GPRS_SMS_SEND,                  //短消息发送

	//电话
	CMD_GPRS_DAIL,                      //拨打电话

	//状态查询
	CMD_GPRS_SIM_STATUS,				//SIM卡状态
	CMD_GPRS_REG_STATUS,				//网络注册状态
	CMD_GPRS_MODULE_STATUS,				//模块状态
	CMD_GPRS_SIGNAL_STATUS,				//信号强度
	CMD_GPRS_PPP_STATUS,				//PPP状态
	CMD_GPRS_NET_STATUS,				//链路状态
	CMD_GPRS_LOC_GET,					//获取位置信息
} CMD_TO_GPRS;

/* GPRS模块状态类型 */
typedef enum
{
    GPRS_Null,
    GPRS_Reseting,			  // GPRS模块复位阶段 
    GPRS_BasicSetting,		  // GPRS模块初始化阶段
	GPRS_PPP_Connecting,      // PPP连接阶段
    GPRS_NETSERVER_connecting,// 网络服务器连接阶段
    GPRS_GprsSend,			  // 模块发送数据 
    GPRS_Talk				  // GPRS通话状态（即数据通道关闭，语音通道打开；监听、打电话皆属于此状态下） 
} STATE_GPRS;

typedef enum
{
	CHECK_SIM,				 //检查SIM卡状态
	CHECK_REG,				 //检查注册状态
	CHECK_SIGNAL,			 //检查信号状态
	SET_APN,				 //设置APN
	OPEN_PPP,				 //打开链路
}PPP_CONNECT_STEP;			 //PPP连接步骤


typedef struct
{
	char signal;	         //信号强度
	char SMS_Center_no[20];	 //短信息中心号码
	char ICCID[32];			 //手机卡ICCID号
	u32 latitude;            //经度	百万分之一度
	u32 longitude;			 //纬度 百万分之一度
}STATUS;


typedef struct
{
    char buf[1024];
    u32  index;
}GPRS_REC;

typedef struct
{
	u32 limit_second; //监听通话时间限制
	u32 start_tim;    //起始时间
    u8 Listen_Talk;	  //以监听为目的去拨打电话（区分目的是为了从不同的地方获取呼叫号码）	 0：普通电话  1：监听
    char dial_num[20];
} Talk_Type;


typedef struct
{
	char APN[32];	  //APN
	char USER[32];    //USER
	char PWD[32];	  //PWD
}Network_param;		   //网络参数

typedef struct
{
	char socket_id;			 //0-3
	char domain[32];		 //IP或域名
	char port[8];			 //连接端口号
	char type;				 //0:TCP   1:UDP
	char is_connected;       //0:未连接  1:已连接
	S_CQueue GprsRx;		 //已接收的数据
	S_CQueue GprsTx;		 //待发送的数据
}Connect_param;		   //连接参数


typedef struct
{
	char mode;				 //发送时：0:无短信需要发送  1:text模式短信    2:PDU格式短信	 接收时：0:没有接收到短信 1：text模式短信  2：PDU格式短信
	u16 len;				 //待发送长度
	char phone_no[20];		 //待发送手机号
	char buf[160];			 //待发送内容
}MSG_TYPE;

typedef struct
{
	MSG_TYPE msg_recv;
	MSG_TYPE msg_send;
}MSG;

typedef struct
{
	char dns_socket_id;
	char connect_socket_id;
	char send_socket_id;
	char net_status_socket_id;	
}SOCKET_ID;



typedef struct
{
	STATE_GPRS  currentGPRSState;
	CMD_TO_GPRS currentCmdToGPRS;
	PPP_CONNECT_STEP ppp_step;
	char ack[15];
	u8 time_out;                                    //超时时间
	STATUS status;
	u32 timerSecondSendAT;
	Network_param network_param;					   //apn
	Connect_param connect_param[SOCKET_TOTAL];		   //gprs连接参数
	MSG msg;
	Talk_Type talk;
	SOCKET_ID socket_id;
	u8 error_count;
}GPRS_DEV;



void GPRS_StartUp(void);
void GPRSProcess(void);
void API_vpdn_set(char* apn,char* user,char* pwd);						   //设置apn、用户名、密码
void API_server_ip_port_set(u8 socket_id,u8 *domain,u8 *port,u8 type);	   //连接指定的服务器 socket_id:0-SOCKET_TOTAL-1 domain:ip或域名 port:端口号  type:0 TCP  1 UDP
void API_server_data_send(u8 socket_id,u8* src,u16 len);				   //服务器数据发送
u8 API_server_data_recv(u8 socket_id,char* buf);						   //服务器数据接收	socket_id:socket编号   buf:缓冲区   有数据返回1，无数据返回0   成功返回一个字节的数据
void API_sms_msg_send(char *src,u32 src_size,char *phone_no,u8 type);	   //GPRS发送一条短信  type 1 text模式  2PDU模式,如果为PDU格式，请保证内容为USC编码
void API_make_a_call(char* phone_no,u32 tim,u8 type);					   //GPRS拨打电话 phone_no：电话号码  tim：时限 0为无时限 非零为有时限（最大为0xFFFFFFFF）超过时限挂断电话  type：通话类型 0：普通电话  1：监听
void API_play_TTS(char* src,u16 src_len);								   //暂不支持	蓝牙和TTS功能只能选择一个
u8 API_get_socket_status(u8 socket_id);									   //检查链路状态        socket_id:socket编号   返回值：0：未连接   1：已连接
u16 API_get_send_data_len(u8 socket_id);



#endif



