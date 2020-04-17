#include "include.h"


static unsigned char _Rxbuff[SOCKET_TOTAL][GPRS_RX_buff_SIZE]={0};		/* 接收队列数组 */
static unsigned char _Txbuff[SOCKET_TOTAL][GPRS_TX_buff_SIZE]={0};		/* 发送队列数组 */
static unsigned int _rxrIndex[SOCKET_TOTAL]={0};			  			/* 读取数据的位置索引（接收队列） */
static unsigned int _txrIndex[SOCKET_TOTAL]={0};						/* 读取数据的位置索引（发送队列） */
static unsigned int _rxwIndex[SOCKET_TOTAL]={0};						/* 写入数据的位置索引（接收队列） */
static unsigned int _txwIndex[SOCKET_TOTAL]={0};						/* 写入数据的位置索引（发送队列） */
static unsigned int _rxlock[SOCKET_TOTAL]={0};							/* 队列锁（接收队列） */
static unsigned int _txlock[SOCKET_TOTAL]={0};							/* 队列锁（发送队列） */
static unsigned int _rxtimerSPushToBuff[SOCKET_TOTAL]={0};				/* 存入数组的时间标志（接收队列） */
static unsigned int _txtimerSPushToBuff[SOCKET_TOTAL]={0};				/* 存入数组的时间标志（发送队列） */
static unsigned int _rxoverFlow[SOCKET_TOTAL]={0};						/* 溢出标志（接收队列） */
static unsigned int _txoverFlow[SOCKET_TOTAL]={0};             			/* 溢出标志（发送队列） */


GPRS_DEV gprs_dev;


void GPRS_StartUp(void)
{
	int i=0;


	for(i=0;i<SOCKET_TOTAL;i++)
	{
		gprs_dev.connect_param[i].GprsRx.buffP             = (unsigned char*)&_Rxbuff[i];
		gprs_dev.connect_param[i].GprsRx.buffSize          = GPRS_RX_buff_SIZE;
		gprs_dev.connect_param[i].GprsRx.rIndexP           = &_rxrIndex[i];
		gprs_dev.connect_param[i].GprsRx.wIndexP           = &_rxwIndex[i];
		gprs_dev.connect_param[i].GprsRx.lockP             = &_rxlock[i];
		gprs_dev.connect_param[i].GprsRx.timerSPushToBuffP = &_rxtimerSPushToBuff[i];
		gprs_dev.connect_param[i].GprsRx.overFlowP         = &_rxoverFlow[i];

		gprs_dev.connect_param[i].GprsTx.buffP             = (unsigned char*)&_Txbuff[i];
		gprs_dev.connect_param[i].GprsTx.buffSize          = GPRS_TX_buff_SIZE;
		gprs_dev.connect_param[i].GprsTx.rIndexP           = &_txrIndex[i];
		gprs_dev.connect_param[i].GprsTx.wIndexP           = &_txwIndex[i];
		gprs_dev.connect_param[i].GprsTx.lockP             = &_txlock[i];
		gprs_dev.connect_param[i].GprsTx.timerSPushToBuffP = &_txtimerSPushToBuff[i];
		gprs_dev.connect_param[i].GprsTx.overFlowP         = &_txoverFlow[i];	
	}
	for(i=0;i<SOCKET_TOTAL;i++)
	{
		gprs_dev.connect_param[i].is_connected=0;
	}
	GPRS_PowerOn();
	Delay_1ms(100);
	PWR_KEY_OFF();
	Delay_1ms(2000);
	PWR_KEY_ON();
	gprs_dev.currentGPRSState=GPRS_BasicSetting;
	gprs_dev.currentCmdToGPRS=CMD_GPRS_NULL;
	gprs_dev.ppp_step=CHECK_SIM;
	gprs_dev.error_count=0;
	gprs_dev.timerSecondSendAT=TimerSecond;
}


/*
	GPRS发送一条AT指令,如果是字符串发送src_size可为0 ,size的目的是为了MG2639数据发送中可能出现0x00
*/
static void GPRS_send_cmd(char *src,u32 src_size,char *ack,CMD_TO_GPRS cmd,u8 time_out)
{
	if(src_size)ioUart.putChars(GPRS_IO, (unsigned char*)src,src_size);
	else 		ioUart.puts(GPRS_IO,(unsigned char*)src);
	
	if(ack)strcpy(gprs_dev.ack,ack);
	else   gprs_dev.ack[0]=0x00;

	gprs_dev.currentCmdToGPRS=cmd;

	gprs_dev.time_out=time_out;

	gprs_dev.timerSecondSendAT=TimerSecond;

#if DEBUG_GPRS
	if(!strstr(ack,"SEND")) //当不为数据发送时
	{
		ioUart.puts(1,"\r\n");
		ioUart.puts(1,(unsigned char*)src);
	}
	else
	{
		u16 i=0;

		for(i=0;i<src_size-1;i++)
		{
			printf("%02X",src[i]);
		}
	}
	ioUart.puts(1,"\r\n");
#endif
}



static void callback_cmd_gprs_basic_setting(const char* rx)
{
	gprs_dev.currentGPRSState=GPRS_PPP_Connecting;
	gprs_dev.ppp_step=CHECK_SIM;
}

static void callback_cmd_gprs_sim_status(const char* rx)
{
	gprs_dev.ppp_step=CHECK_REG;
}

static void callback_cmd_gprs_reg_status(const char* rx)
{
	char* pchar1;
	u8 reg_status;
	
	pchar1=strstr(rx,"+CREG:");
	pchar1=strstr(pchar1,",")+1;
	reg_status=atoi(pchar1);
	
	switch(reg_status)
	{
		case 1:
		case 5:	   //已注册
			gprs_dev.ppp_step=CHECK_SIGNAL;
		break;
		
		case 0:
		case 3:	   //欠费停机
			GPRS_PowerOff();
			gprs_dev.currentGPRSState=GPRS_Null;
		break;	
		
		default:
		break;
	}
}

static void callback_cmd_gprs_signal_status(const char* rx)
{
	char* pchar1;
	
	pchar1=strstr(rx,"+CSQ:");
	pchar1=strstr(pchar1,":")+1;
	
	gprs_dev.status.signal=atoi(pchar1);

	if(gprs_dev.currentGPRSState==GPRS_PPP_Connecting&&gprs_dev.status.signal>=10&&gprs_dev.status.signal<=31)gprs_dev.ppp_step=CHECK_GPRS;
}

static void callback_cmd_gprs_cgreg_status(const char* rx)
{
	char* pchar1;
	u8 reg_status;
	
	pchar1=strstr(rx,"+CGREG:");
	pchar1=strstr(pchar1,",")+1;
	reg_status=atoi(pchar1);
	
	switch(reg_status)
	{
		case 1:
		case 5:	   //已注册
			gprs_dev.ppp_step=SET_APN;
		break;
		
		case 0:
		case 3:	   
			gprs_dev.currentGPRSState=GPRS_Reseting;
		break;	
		
		default:
		break;
	}
}

static void callback_cmd_gprs_apn_set(const char* rx)
{
	gprs_dev.ppp_step=OPEN_PPP;	
}

static void callback_cmd_gprs_ppp_open(const char* rx)
{
	GPRS_send_cmd("AT+QILOCIP\r",0,0,CMD_GPRS_NULL,0);
	gprs_dev.currentGPRSState=GPRS_NETSERVER_connecting;
}

static void callback_cmd_gprs_net_connect(const char* rx)
{
	if(strstr(rx,"ALREAY CONNECT")||strstr(rx,"CONNECT OK"))
	{
		gprs_dev.currentGPRSState=GPRS_GprsSend;
		gprs_dev.connect_param[gprs_dev.socket_id.connect_socket_id].is_connected=1;	
	}
	else
	{
		gprs_dev.connect_param[gprs_dev.socket_id.connect_socket_id].is_connected=0;
		gprs_dev.error_count++;	
	}
}

static void callback_cmd_gprs_net_send(const char* rx)
{
	if(strstr(rx,"SEND FAIL"))gprs_dev.error_count++;
}


static void callback_cmd_gprs_loc_get(const char* rx)
{
	char* pchar1;
	double tmp;

	pchar1=strstr(rx,"+QCELLLOC:");
	pchar1=strstr(rx,":")+1;
	tmp=atof(pchar1);
	gprs_dev.status.longitude=tmp*1000000;
	pchar1=strstr(rx,",")+1;
	tmp=atof(pchar1);
	gprs_dev.status.latitude=tmp*1000000;
}

static void callback_cmd_gprs_send_status(const char* rx)
{
	static u32 last_confirmed_len=0;						//上次确认发送的数据长度
	static u32 confirmed_is_same_times=0;					//确认数据发送长度一致的次数
	u32 is_confirmed_len,not_confirmed_len;					//已确认发送的数据，未确认发送的数据
	char* pchar1;

	pchar1=strstr(rx,"+QISACK:");
	pchar1=strstr(pchar1,":")+1;		 //第一个逗号
	pchar1=strstr(pchar1,",")+1;		 //第二个逗号
	is_confirmed_len=atoi(pchar1);
	pchar1=strstr(pchar1,",")+1;		 //第三个逗号
	not_confirmed_len=atoi(pchar1);

	if(not_confirmed_len&&(is_confirmed_len==last_confirmed_len))confirmed_is_same_times++;
	else 														 confirmed_is_same_times=0;

	last_confirmed_len=is_confirmed_len;
	if(not_confirmed_len&&confirmed_is_same_times>=15)
	{
		confirmed_is_same_times=0;
		last_confirmed_len=0;
		gprs_dev.currentGPRSState=GPRS_Reseting;
	}
}

/*
	检查指令返回
*/
static void GPRS_check_cmd(const char* rx)
{
	if(!gprs_dev.ack[0])return;
	if(!strstr(rx,gprs_dev.ack))return;
	
	switch(gprs_dev.currentCmdToGPRS)
	{
		case CMD_GPRS_BASIC_SETTING:				   //初始化设置
			callback_cmd_gprs_basic_setting(rx);	
		break;

		case CMD_GPRS_SIM_STATUS:					   //SIM卡状态
			callback_cmd_gprs_sim_status(rx);
		break;

		case CMD_GPRS_REG_STATUS:		 			   //注册状态
			callback_cmd_gprs_reg_status(rx);
		break;

		case CMD_GPRS_SIGNAL_STATUS:				   //信号强度
			callback_cmd_gprs_signal_status(rx);
		break;

		case CMD_GPRS_GREG_STATUS:					   //GPRS注册状态
			callback_cmd_gprs_cgreg_status(rx);  
		break;

		case CMD_GPRS_APN_SET:						   //设置APN
			callback_cmd_gprs_apn_set(rx);
		break;

		case CMD_GPRS_PPP_OPEN:						   //PPP连接
			callback_cmd_gprs_ppp_open(rx);
		break;

		case CMD_GPRS_NET_CONNECT:					   //连接网络服务器
			callback_cmd_gprs_net_connect(rx);
		break;

		case CMD_GPRS_NET_SEND:						   //数据发送
			callback_cmd_gprs_net_send(rx);
		break;

		case CMD_GPRS_LOC_GET:						   //获取经纬度
			callback_cmd_gprs_loc_get(rx);
		break;

		case CMD_GPRS_SEND_STATUS:					   //获取发送数据长度信息
			callback_cmd_gprs_send_status(rx);
		break;
	}
	gprs_dev.currentCmdToGPRS=CMD_GPRS_NULL;
	gprs_dev.ack[0]=0x00;
}

static u8 is_received_server_data=0;
static void GPRS_recv_server_data(const char* rx)
{
	static u8 socket_id=0;
	static u16 len=0;
	static u32 rec_tim=0;

	if(strstr(rx,"+QIRDI:"))
	{
		char* pchar1;
		char buf[30]={0};
		
		pchar1=strstr(rx,"+QIRDI:");
		pchar1=strStrfN(rx,",",2)+1;
		socket_id=atoi(pchar1);
		sprintf(buf,"AT+QIRD=0,1,%d,1500\r",socket_id);
		GPRS_send_cmd(buf,0,0,CMD_GPRS_NULL,0);		
	}
	else if(strstr(rx,"+QIRD:"))
	{
		char* pchar1;

		pchar1=strstr(rx,"+QIRD:");
		pchar1=strStrfN(pchar1,",",2)+1;
		len=atoi(pchar1);
		is_received_server_data=1;
		rec_tim=TimerSecond;	
	}
	else if(is_received_server_data)
	{
		is_received_server_data=0;
		QueuePutchars(&gprs_dev.connect_param[socket_id].GprsRx,(char*)rx, len, 0);
	}
	if(gprs_dev.currentGPRSState==GPRS_GprsSend&&TimerSecond-rec_tim>=180)
	{
		char buf[30]={0};
		rec_tim=TimerSecond;	
		if(++socket_id>=SOCKET_TOTAL)socket_id=0;

		sprintf(buf,"AT+QIRD=0,1,%d,1500\r",socket_id);
		GPRS_send_cmd(buf,0,0,CMD_GPRS_NULL,0);
	}
	if(strstr(rx,"+CCLK:"))
	{
		char* pchar1;
		u8 year,month,day,hour,minute,second;
		pchar1=strstr(rx,"+CCLK:");
		pchar1=strstr(pchar1,"\"")+1;
		year=atoi(pchar1);
		pchar1=strstr(pchar1,"/")+1;
		month=atoi(pchar1);
		pchar1=strstr(pchar1,"/")+1;
		day=atoi(pchar1);
		pchar1=strstr(pchar1,",")+1;
		hour=atoi(pchar1);
		pchar1=strstr(pchar1,":")+1;
		minute=atoi(pchar1);
		pchar1=strstr(pchar1,":")+1;
		second=atoi(pchar1);

		gprs_dev.status.BJ_BCD_tim[0]=((year  /10)<<4)|(year  %10);
		gprs_dev.status.BJ_BCD_tim[1]=((month /10)<<4)|(month %10);
		gprs_dev.status.BJ_BCD_tim[2]=((day   /10)<<4)|(day   %10);
		gprs_dev.status.BJ_BCD_tim[3]=((hour  /10)<<4)|(hour  %10);
		gprs_dev.status.BJ_BCD_tim[4]=((minute/10)<<4)|(minute%10);
		gprs_dev.status.BJ_BCD_tim[5]=((second/10)<<4)|(second%10);
	} 
}


static void GPRS_check_AbnormalData(const char* rx)
{
	if(strstr(rx,"NO CARRIER")||strstr(rx,"BUSY")||strstr(rx,"NO ANSWER")) //电话无人应答
	{
		gprs_dev.currentGPRSState=GPRS_Reseting;
	}
	if(strstr(rx,"ERROR"))
	{
		gprs_dev.error_count++;
	}

	if(strstr(rx,"+CME ERROR: 10"))			   //无SIM卡
	{
		GPRS_PowerOff();
		gprs_dev.currentGPRSState=GPRS_Null;
	}
	if(strstr(rx,"+PDP DEACT"))
	{
		gprs_dev.currentGPRSState=GPRS_Reseting;
	}
	if(strstr(rx,", CLOSED"))
	{
		gprs_dev.connect_param[atoi(rx)].is_connected=0;
		gprs_dev.currentCmdToGPRS=CMD_GPRS_NULL;
	}
	if(strstr(rx,"+CME ERROR: 3813"))
	{
		gprs_dev.currentCmdToGPRS=CMD_GPRS_NULL;	
	}
}

/*
	设置apn、用户名、密码
*/
void API_vpdn_set(char* apn,char* user,char* pwd)
{
	strcpy(gprs_dev.network_param.APN,apn);
	strcpy(gprs_dev.network_param.USER,user);
	strcpy(gprs_dev.network_param.PWD,pwd);
	gprs_dev.currentGPRSState=GPRS_BasicSetting;
	gprs_dev.currentCmdToGPRS=CMD_GPRS_NULL;
} 

/*
	连接指定的服务器 socket_id:0-SOCKET_TOTAL-1 domain:ip或域名 port:端口号  type:0 TCP  1 UDP
*/
void API_server_ip_port_set(u8 socket_id,u8 *domain,u8 *port,u8 type)
{
	gprs_dev.connect_param[socket_id].socket_id=socket_id;
	strcpy((char*)gprs_dev.connect_param[socket_id].domain,(char*)domain);
	strcpy((char*)gprs_dev.connect_param[socket_id].port,(char*)port);
	gprs_dev.connect_param[socket_id].type=type;
	gprs_dev.connect_param[socket_id].is_connected=0;
}

/*
	服务器数据发送
*/
void API_server_data_send(u8 socket_id,u8* src,u16 len)
{
	QueuePutchars(&gprs_dev.connect_param[socket_id].GprsTx, (char *)src, len, 0);
}


/*
	服务器数据接收	socket_id:socket编号   buf:缓冲区   有数据返回1，无数据返回0   成功返回一个字节的数据
*/
u8 API_server_data_recv(u8 socket_id,char* buf)
{
	if(QueueGetDataLen(&gprs_dev.connect_param[socket_id].GprsRx))
	{
		QueueGetchar(&gprs_dev.connect_param[socket_id].GprsRx,buf);
		return 1;	
	}
	return 0;
}


/*
	检查链路状态        socket_id:socket编号   返回值：0：未连接   1：已连接
*/
u8 API_get_socket_status(u8 socket_id)
{
	return gprs_dev.connect_param[socket_id].is_connected;
}

u16 API_get_send_data_len(u8 socket_id)
{
	return QueueGetDataLen(&gprs_dev.connect_param[socket_id].GprsTx);
}

/*
	GPRS数据接收  成功返回1  失败返回0
*/
static u8 GPRSGetMsg(GPRS_REC *rx)
{
	static u32 rece_time=0;
	static u8 head_flag =0;
	char val;

	u32 size=ioUart.rxBuffCount(GPRS_IO);

	while(size--)
	{
		val = ioUart.getChar(GPRS_IO);
		rece_time=Timer100ms;

		if(!head_flag)							  //如果没有包头，当接收到数据时则认为有包头
		{
			head_flag=0x01;
			rx->index=0x00;
		}
		
		switch(val)
		{
			case '\n':
				if(rx->buf[rx->index-1]=='\r')	  //如果出现0x0D 0x0A的情况	  判断是否为包头或包尾或服务器数据接收的中间值
				{
					if(rx->index==1)              //如果缓冲区的第一第二个字节为0x0D 0x0A 则去掉
					{
						head_flag=0x00;
						rece_time=0x00;
					}
					else						  //否则则认为在数据包尾或服务器数据包接收中，如果当前不是接收服务器数据就是包尾，如果当前为接收服务器数据则需要判断接收的数据长度来确定是否为包尾
					{
						if(!is_received_server_data)	  //当前不是接收服务器数据
						{
							head_flag=0x00;
							rece_time=0x00;
							rx->buf[--rx->index]=0x00;
#if DEBUG_GPRS
							ioUart.puts(1,"\r\n");
							ioUart.puts(1,"<");
							ioUart.puts(1,(unsigned char*)rx->buf);
							ioUart.puts(1,">");
							ioUart.puts(1,"\r\n");
#endif
							return 1;
						}
						else					  //当前接收到服务器数据
						{
							head_flag=0x00;
							rece_time=0x00;
							rx->buf[--rx->index]=0x00;
#if DEBUG_GPRS
							{
								u16 i=0;

								ioUart.puts(1,"\r\n");
								ioUart.puts(1,"<");
								for(i=0;i<rx->index;i++)
								{
									printf("%02X",rx->buf[i]);
								}
								ioUart.puts(1,">");
								ioUart.puts(1,"\r\n");
							}								
#endif
							return 1;
						}		
					}
				}
				else							  //0x0A前面不是0x0D  继续赋值
				{
					rx->buf[rx->index++]=val;
					rx->buf[rx->index]=0x00;
				}
			break;

			default:
				rx->buf[rx->index++]=val;
				rx->buf[rx->index]=0x00;	
			break;
		}
	}

	if(head_flag&&rece_time&&Timer100ms-rece_time>=5)	   //0.5秒未接受到后续数据
	{
		head_flag=0x00;
		rece_time=0x00;
	}   
	return 0;
}

static void gprs_prase(GPRS_REC* rx)
{
	GPRS_check_cmd(rx->buf);		 //指令处理
	GPRS_recv_server_data(rx->buf);	 //服务器数据接收处理
	GPRS_check_AbnormalData(rx->buf);//异常数据处理
}

/*
	GPRS初始化设置
*/
static void GPRS_basic_set(void)
{
	if(gprs_dev.currentGPRSState!=GPRS_BasicSetting)return;				   //不是基础设置  退出
	if(gprs_dev.currentCmdToGPRS!=CMD_GPRS_NULL)	return;				   //有其他指令    退出
	if(TimerSecond-gprs_dev.timerSecondSendAT<15)	return;				   //没有等待15秒  退出

	GPRS_send_cmd("ATE0\r",0,0,CMD_GPRS_NULL,0);
	Delay_1ms(50);
	GPRS_send_cmd("AT+CMGF=1\r",0,0,CMD_GPRS_NULL,0);
	Delay_1ms(50);
	GPRS_send_cmd("AT+CLIP=1\r",0,0,CMD_GPRS_NULL,0);
	Delay_1ms(50);
	GPRS_send_cmd("AT+CNMI=3,1,0,0,0\r",0,0,CMD_GPRS_NULL,0);
	Delay_1ms(50);
	GPRS_send_cmd("AT+QIMODE=0\r",0,0,CMD_GPRS_NULL,0);
	Delay_1ms(50);
	GPRS_send_cmd("AT+QIMUX=1\r",0,0,CMD_GPRS_NULL,0);
	Delay_1ms(50);
	GPRS_send_cmd("AT+QIPROMPT=0\r",0,0,CMD_GPRS_NULL,0);
	Delay_1ms(50);
	GPRS_send_cmd("AT+CTZU=3\r",0,0,CMD_GPRS_NULL,0);
	Delay_1ms(50);
	GPRS_send_cmd("AT+QNITZ=1\r",0,0,CMD_GPRS_NULL,0);
	Delay_1ms(50);
	GPRS_send_cmd("AT+QINDI=1\r",0,"OK",CMD_GPRS_BASIC_SETTING,2);
}


/*
	PPP连接处理
*/
static void GPRS_PPP_connect(void)
{
	if(gprs_dev.currentGPRSState!=GPRS_PPP_Connecting)return;			   //不是PPP连接阶段  退出
	if(gprs_dev.currentCmdToGPRS!=CMD_GPRS_NULL)	  return;			   //有其他指令       退出
	if(TimerSecond==gprs_dev.timerSecondSendAT)return;

	switch(gprs_dev.ppp_step)
	{
		case CHECK_SIM:			//检查SIM卡状态	
			GPRS_send_cmd("AT+CPIN?\r",0,"+CPIN: READY",CMD_GPRS_SIM_STATUS,2);
		break;

		case CHECK_REG:		   //检查注册状态
			GPRS_send_cmd("AT+CREG?\r",0,"+CREG",CMD_GPRS_REG_STATUS,2);
		break;

		case CHECK_SIGNAL:	   //检查信号强度
			GPRS_send_cmd("AT+CSQ\r",0,"+CSQ:",CMD_GPRS_SIGNAL_STATUS,2);
		break;

		case CHECK_GPRS:
			GPRS_send_cmd("AT+CGREG?\r",0,"+CGREG",CMD_GPRS_GREG_STATUS,2);
		break;

		case SET_APN:		   //设置APN
			{
				char buf[100]={0};

				GPRS_send_cmd("AT+QIFGCNT=0\r",0,0,CMD_GPRS_NULL,0);
				Delay_1ms(100);
				GPRS_send_cmd("AT+QICSGP=1,\"CMNET\"\r",0,0,CMD_GPRS_NULL,0);
				Delay_1ms(100);
				GPRS_send_cmd("AT+QIDEACT\r",0,0,CMD_GPRS_NULL,0);
				Delay_1ms(100);
				sprintf(buf,"AT+QIREGAPP=\"%s\",\"%s\",\"%s\"\r",gprs_dev.network_param.APN,gprs_dev.network_param.USER,gprs_dev.network_param.PWD);				
				GPRS_send_cmd(buf,0,"OK",CMD_GPRS_APN_SET,2);
			}
		break;

		case OPEN_PPP:		   //打开链路
			GPRS_send_cmd("AT+QIACT\r",0,"OK",CMD_GPRS_PPP_OPEN,60);
		break;

		default:
			gprs_dev.ppp_step=CHECK_SIM;
		break;
	}		
}


/*
	用于维护GPRS链路通信处理
*/
static void GPRS_netserver_connect(void)
{
	static u8 current_socket_id=0;

	if(gprs_dev.currentGPRSState!=GPRS_NETSERVER_connecting&&gprs_dev.currentGPRSState!=GPRS_GprsSend)return;
	if(gprs_dev.currentCmdToGPRS!=CMD_GPRS_NULL)return;
	if(TimerSecond==gprs_dev.timerSecondSendAT)return;

	if(!gprs_dev.connect_param[current_socket_id].is_connected&&gprs_dev.connect_param[current_socket_id].domain[0]&&gprs_dev.connect_param[current_socket_id].port[0])//当前IP、port有效，但未连接
	{
		u8 i=0;
		char buf[100]={0};

		for(i=0;i<strlen(gprs_dev.connect_param[current_socket_id].domain);i++)
		{
			if((gprs_dev.connect_param[current_socket_id].domain[i]>'9'||gprs_dev.connect_param[current_socket_id].domain[i]<'0')&&gprs_dev.connect_param[current_socket_id].domain[i]!='.')//如果出现比'9'大，比'0'小并且不为'.'时退出
			break;
		}	
		if(i<strlen(gprs_dev.connect_param[current_socket_id].domain))	//当前为域名
		{
			GPRS_send_cmd("AT+QIDNSIP=1\r",0,0,CMD_GPRS_NULL,0);	 //配置为域名
		}
		else                                                            //当前为IP
		{
			GPRS_send_cmd("AT+QIDNSIP=0\r",0,0,CMD_GPRS_NULL,0);	 //配置为IP
		}
		Delay_1ms(100);
		if(!gprs_dev.connect_param[current_socket_id].type)sprintf(buf,"AT+QIOPEN=%d,\"TCP\",\"%s\",\"%s\"\r",current_socket_id,gprs_dev.connect_param[current_socket_id].domain,gprs_dev.connect_param[current_socket_id].port);	//当前为TCP连接
		else 											   sprintf(buf,"AT+QIOPEN=%d,\"UDP\",\"%s\",\"%s\"\r",current_socket_id,gprs_dev.connect_param[current_socket_id].domain,gprs_dev.connect_param[current_socket_id].port);	//当前为UDP连接

		GPRS_send_cmd(buf,0,"CONNECT",CMD_GPRS_NET_CONNECT,60);		//服务器连接
		gprs_dev.socket_id.connect_socket_id=current_socket_id;
	}
	current_socket_id++;
	if(current_socket_id>=SOCKET_TOTAL)current_socket_id=0;
}


/*
	数据发送处理
*/
static void GPRS_netserver_send(void)
{
	u32 datalen=0;
	static u8 current_socket_id=0;
	
	if(gprs_dev.currentGPRSState!=GPRS_GprsSend)return;
	if(gprs_dev.currentCmdToGPRS!=CMD_GPRS_NULL)return;

	datalen=QueueGetDataLen(&gprs_dev.connect_param[current_socket_id].GprsTx);

	if(datalen&&gprs_dev.connect_param[current_socket_id].is_connected)	 //如果有数据待发送并且当前模块空闲并且当前socket已连接则发送
	{
		char buf[GPRS_TX_buff_SIZE]={0};

		sprintf(buf,"AT+QISEND=%d,%d\r", current_socket_id,datalen);        
		GPRS_send_cmd(buf,0,0,CMD_GPRS_NULL,0);
		Delay_1ms(50);
		QueueGetchars(&gprs_dev.connect_param[current_socket_id].GprsTx,buf,datalen);
		buf[datalen]='\r';
		GPRS_send_cmd(buf,datalen+1,"SEND",CMD_GPRS_NET_SEND,30);	
		gprs_dev.socket_id.send_socket_id=current_socket_id;
	}
	current_socket_id++;
	if(current_socket_id>=SOCKET_TOTAL)current_socket_id=0;	
}



/*
	扫描状态
*/
static void GPRS_status_check(void)
{
	static u8 status_scan_count=0;
	static u32 tim=0;
	
	if(gprs_dev.currentGPRSState!=GPRS_GprsSend)return; 
	if(gprs_dev.currentCmdToGPRS!=CMD_GPRS_NULL)return;
	if(TimerSecond==tim)return;
	tim=TimerSecond;

	switch(status_scan_count++)
	{
		case 0:		   //CMD_GPRS_SIGNAL_STATUS,			//信号强度查询
			GPRS_send_cmd("AT+CSQ\r",0,"+CSQ:",CMD_GPRS_SIGNAL_STATUS,2);
		break;

		case 1:												//查询位置信息
			GPRS_send_cmd("AT+QCELLLOC=1\r",0,"+QCELLLOC:",CMD_GPRS_LOC_GET,60);	   				
		break;

		case 2:												//查询发送数据长度信息
			GPRS_send_cmd("AT+QISACK=0\r",0,"+QISACK:",CMD_GPRS_SEND_STATUS,2);
		break;

		case 3:
			GPRS_send_cmd("AT+CCLK?\r",0,0,CMD_GPRS_NULL,0);
		break;

		default:
			status_scan_count=0;
		break;
	}
} 


static void GPRS_timeout_check(void)
{
	if(gprs_dev.currentGPRSState==GPRS_Null)
	{
		if(TimerSecond-gprs_dev.timerSecondSendAT>=3600)gprs_dev.currentGPRSState=GPRS_Reseting;
	}

	if(gprs_dev.currentCmdToGPRS!=CMD_GPRS_NULL&&gprs_dev.time_out&&TimerSecond-gprs_dev.timerSecondSendAT>gprs_dev.time_out)
	{
		gprs_dev.error_count++;
		gprs_dev.currentCmdToGPRS=CMD_GPRS_NULL;		
	}
	if(gprs_dev.error_count>=15)
	{
		gprs_dev.error_count=0;
		gprs_dev.currentGPRSState=GPRS_Reseting;
	}
}

void GPRS_reset_check(void)
{
	static u8 tim=0;
	if(gprs_dev.currentGPRSState==GPRS_Reseting)
	{
		if(!tim)
		{
			tim=TimerSecond;
			PWR_KEY_OFF();			
		}
		else if(TimerSecond-tim>=20)
		{
			tim=0;
			GPRS_StartUp();
		}
		else if(TimerSecond-tim>=15)
		{
			GPRS_PowerOff();	
		}
		else if(TimerSecond-tim>=1)
		{
		   PWR_KEY_ON();
		}	
	}
}


void GPRSProcess(void)
{
	static	GPRS_REC gprs_rec={0};

	if(GPRSGetMsg(&gprs_rec))
	{
		gprs_prase(&gprs_rec);
	}
	GPRS_basic_set();		   //基础设置
	GPRS_PPP_connect();		   //PPP连接处理
	GPRS_netserver_connect();  //服务器连接处理
	GPRS_netserver_send();	   //数据发送处理
	GPRS_status_check();	   	//状态扫描
	GPRS_timeout_check();      //超时处理
	GPRS_reset_check();		   //重启处理
}












