#include "include.h"


static unsigned char _Rxbuff[SOCKET_TOTAL][GPRS_RX_buff_SIZE]={0};		/* ���ն������� */
static unsigned char _Txbuff[SOCKET_TOTAL][GPRS_TX_buff_SIZE]={0};		/* ���Ͷ������� */
static unsigned int _rxrIndex[SOCKET_TOTAL]={0};			  			/* ��ȡ���ݵ�λ�����������ն��У� */
static unsigned int _txrIndex[SOCKET_TOTAL]={0};						/* ��ȡ���ݵ�λ�����������Ͷ��У� */
static unsigned int _rxwIndex[SOCKET_TOTAL]={0};						/* д�����ݵ�λ�����������ն��У� */
static unsigned int _txwIndex[SOCKET_TOTAL]={0};						/* д�����ݵ�λ�����������Ͷ��У� */
static unsigned int _rxlock[SOCKET_TOTAL]={0};							/* �����������ն��У� */
static unsigned int _txlock[SOCKET_TOTAL]={0};							/* �����������Ͷ��У� */
static unsigned int _rxtimerSPushToBuff[SOCKET_TOTAL]={0};				/* ���������ʱ���־�����ն��У� */
static unsigned int _txtimerSPushToBuff[SOCKET_TOTAL]={0};				/* ���������ʱ���־�����Ͷ��У� */
static unsigned int _rxoverFlow[SOCKET_TOTAL]={0};						/* �����־�����ն��У� */
static unsigned int _txoverFlow[SOCKET_TOTAL]={0};             			/* �����־�����Ͷ��У� */


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
	GPRS����һ��ATָ��,������ַ�������src_size��Ϊ0 ,size��Ŀ����Ϊ��MG2639���ݷ����п��ܳ���0x00
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
	if(!strstr(ack,"SEND")) //����Ϊ���ݷ���ʱ
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
		case 5:	   //��ע��
			gprs_dev.ppp_step=CHECK_SIGNAL;
		break;
		
		case 0:
		case 3:	   //Ƿ��ͣ��
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
		case 5:	   //��ע��
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
	static u32 last_confirmed_len=0;						//�ϴ�ȷ�Ϸ��͵����ݳ���
	static u32 confirmed_is_same_times=0;					//ȷ�����ݷ��ͳ���һ�µĴ���
	u32 is_confirmed_len,not_confirmed_len;					//��ȷ�Ϸ��͵����ݣ�δȷ�Ϸ��͵�����
	char* pchar1;

	pchar1=strstr(rx,"+QISACK:");
	pchar1=strstr(pchar1,":")+1;		 //��һ������
	pchar1=strstr(pchar1,",")+1;		 //�ڶ�������
	is_confirmed_len=atoi(pchar1);
	pchar1=strstr(pchar1,",")+1;		 //����������
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
	���ָ���
*/
static void GPRS_check_cmd(const char* rx)
{
	if(!gprs_dev.ack[0])return;
	if(!strstr(rx,gprs_dev.ack))return;
	
	switch(gprs_dev.currentCmdToGPRS)
	{
		case CMD_GPRS_BASIC_SETTING:				   //��ʼ������
			callback_cmd_gprs_basic_setting(rx);	
		break;

		case CMD_GPRS_SIM_STATUS:					   //SIM��״̬
			callback_cmd_gprs_sim_status(rx);
		break;

		case CMD_GPRS_REG_STATUS:		 			   //ע��״̬
			callback_cmd_gprs_reg_status(rx);
		break;

		case CMD_GPRS_SIGNAL_STATUS:				   //�ź�ǿ��
			callback_cmd_gprs_signal_status(rx);
		break;

		case CMD_GPRS_GREG_STATUS:					   //GPRSע��״̬
			callback_cmd_gprs_cgreg_status(rx);  
		break;

		case CMD_GPRS_APN_SET:						   //����APN
			callback_cmd_gprs_apn_set(rx);
		break;

		case CMD_GPRS_PPP_OPEN:						   //PPP����
			callback_cmd_gprs_ppp_open(rx);
		break;

		case CMD_GPRS_NET_CONNECT:					   //�������������
			callback_cmd_gprs_net_connect(rx);
		break;

		case CMD_GPRS_NET_SEND:						   //���ݷ���
			callback_cmd_gprs_net_send(rx);
		break;

		case CMD_GPRS_LOC_GET:						   //��ȡ��γ��
			callback_cmd_gprs_loc_get(rx);
		break;

		case CMD_GPRS_SEND_STATUS:					   //��ȡ�������ݳ�����Ϣ
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
	if(strstr(rx,"NO CARRIER")||strstr(rx,"BUSY")||strstr(rx,"NO ANSWER")) //�绰����Ӧ��
	{
		gprs_dev.currentGPRSState=GPRS_Reseting;
	}
	if(strstr(rx,"ERROR"))
	{
		gprs_dev.error_count++;
	}

	if(strstr(rx,"+CME ERROR: 10"))			   //��SIM��
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
	����apn���û���������
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
	����ָ���ķ����� socket_id:0-SOCKET_TOTAL-1 domain:ip������ port:�˿ں�  type:0 TCP  1 UDP
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
	���������ݷ���
*/
void API_server_data_send(u8 socket_id,u8* src,u16 len)
{
	QueuePutchars(&gprs_dev.connect_param[socket_id].GprsTx, (char *)src, len, 0);
}


/*
	���������ݽ���	socket_id:socket���   buf:������   �����ݷ���1�������ݷ���0   �ɹ�����һ���ֽڵ�����
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
	�����·״̬        socket_id:socket���   ����ֵ��0��δ����   1��������
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
	GPRS���ݽ���  �ɹ�����1  ʧ�ܷ���0
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

		if(!head_flag)							  //���û�а�ͷ�������յ�����ʱ����Ϊ�а�ͷ
		{
			head_flag=0x01;
			rx->index=0x00;
		}
		
		switch(val)
		{
			case '\n':
				if(rx->buf[rx->index-1]=='\r')	  //�������0x0D 0x0A�����	  �ж��Ƿ�Ϊ��ͷ���β����������ݽ��յ��м�ֵ
				{
					if(rx->index==1)              //����������ĵ�һ�ڶ����ֽ�Ϊ0x0D 0x0A ��ȥ��
					{
						head_flag=0x00;
						rece_time=0x00;
					}
					else						  //��������Ϊ�����ݰ�β����������ݰ������У������ǰ���ǽ��շ��������ݾ��ǰ�β�������ǰΪ���շ�������������Ҫ�жϽ��յ����ݳ�����ȷ���Ƿ�Ϊ��β
					{
						if(!is_received_server_data)	  //��ǰ���ǽ��շ���������
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
						else					  //��ǰ���յ�����������
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
				else							  //0x0Aǰ�治��0x0D  ������ֵ
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

	if(head_flag&&rece_time&&Timer100ms-rece_time>=5)	   //0.5��δ���ܵ���������
	{
		head_flag=0x00;
		rece_time=0x00;
	}   
	return 0;
}

static void gprs_prase(GPRS_REC* rx)
{
	GPRS_check_cmd(rx->buf);		 //ָ���
	GPRS_recv_server_data(rx->buf);	 //���������ݽ��մ���
	GPRS_check_AbnormalData(rx->buf);//�쳣���ݴ���
}

/*
	GPRS��ʼ������
*/
static void GPRS_basic_set(void)
{
	if(gprs_dev.currentGPRSState!=GPRS_BasicSetting)return;				   //���ǻ�������  �˳�
	if(gprs_dev.currentCmdToGPRS!=CMD_GPRS_NULL)	return;				   //������ָ��    �˳�
	if(TimerSecond-gprs_dev.timerSecondSendAT<15)	return;				   //û�еȴ�15��  �˳�

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
	PPP���Ӵ���
*/
static void GPRS_PPP_connect(void)
{
	if(gprs_dev.currentGPRSState!=GPRS_PPP_Connecting)return;			   //����PPP���ӽ׶�  �˳�
	if(gprs_dev.currentCmdToGPRS!=CMD_GPRS_NULL)	  return;			   //������ָ��       �˳�
	if(TimerSecond==gprs_dev.timerSecondSendAT)return;

	switch(gprs_dev.ppp_step)
	{
		case CHECK_SIM:			//���SIM��״̬	
			GPRS_send_cmd("AT+CPIN?\r",0,"+CPIN: READY",CMD_GPRS_SIM_STATUS,2);
		break;

		case CHECK_REG:		   //���ע��״̬
			GPRS_send_cmd("AT+CREG?\r",0,"+CREG",CMD_GPRS_REG_STATUS,2);
		break;

		case CHECK_SIGNAL:	   //����ź�ǿ��
			GPRS_send_cmd("AT+CSQ\r",0,"+CSQ:",CMD_GPRS_SIGNAL_STATUS,2);
		break;

		case CHECK_GPRS:
			GPRS_send_cmd("AT+CGREG?\r",0,"+CGREG",CMD_GPRS_GREG_STATUS,2);
		break;

		case SET_APN:		   //����APN
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

		case OPEN_PPP:		   //����·
			GPRS_send_cmd("AT+QIACT\r",0,"OK",CMD_GPRS_PPP_OPEN,60);
		break;

		default:
			gprs_dev.ppp_step=CHECK_SIM;
		break;
	}		
}


/*
	����ά��GPRS��·ͨ�Ŵ���
*/
static void GPRS_netserver_connect(void)
{
	static u8 current_socket_id=0;

	if(gprs_dev.currentGPRSState!=GPRS_NETSERVER_connecting&&gprs_dev.currentGPRSState!=GPRS_GprsSend)return;
	if(gprs_dev.currentCmdToGPRS!=CMD_GPRS_NULL)return;
	if(TimerSecond==gprs_dev.timerSecondSendAT)return;

	if(!gprs_dev.connect_param[current_socket_id].is_connected&&gprs_dev.connect_param[current_socket_id].domain[0]&&gprs_dev.connect_param[current_socket_id].port[0])//��ǰIP��port��Ч����δ����
	{
		u8 i=0;
		char buf[100]={0};

		for(i=0;i<strlen(gprs_dev.connect_param[current_socket_id].domain);i++)
		{
			if((gprs_dev.connect_param[current_socket_id].domain[i]>'9'||gprs_dev.connect_param[current_socket_id].domain[i]<'0')&&gprs_dev.connect_param[current_socket_id].domain[i]!='.')//������ֱ�'9'�󣬱�'0'С���Ҳ�Ϊ'.'ʱ�˳�
			break;
		}	
		if(i<strlen(gprs_dev.connect_param[current_socket_id].domain))	//��ǰΪ����
		{
			GPRS_send_cmd("AT+QIDNSIP=1\r",0,0,CMD_GPRS_NULL,0);	 //����Ϊ����
		}
		else                                                            //��ǰΪIP
		{
			GPRS_send_cmd("AT+QIDNSIP=0\r",0,0,CMD_GPRS_NULL,0);	 //����ΪIP
		}
		Delay_1ms(100);
		if(!gprs_dev.connect_param[current_socket_id].type)sprintf(buf,"AT+QIOPEN=%d,\"TCP\",\"%s\",\"%s\"\r",current_socket_id,gprs_dev.connect_param[current_socket_id].domain,gprs_dev.connect_param[current_socket_id].port);	//��ǰΪTCP����
		else 											   sprintf(buf,"AT+QIOPEN=%d,\"UDP\",\"%s\",\"%s\"\r",current_socket_id,gprs_dev.connect_param[current_socket_id].domain,gprs_dev.connect_param[current_socket_id].port);	//��ǰΪUDP����

		GPRS_send_cmd(buf,0,"CONNECT",CMD_GPRS_NET_CONNECT,60);		//����������
		gprs_dev.socket_id.connect_socket_id=current_socket_id;
	}
	current_socket_id++;
	if(current_socket_id>=SOCKET_TOTAL)current_socket_id=0;
}


/*
	���ݷ��ʹ���
*/
static void GPRS_netserver_send(void)
{
	u32 datalen=0;
	static u8 current_socket_id=0;
	
	if(gprs_dev.currentGPRSState!=GPRS_GprsSend)return;
	if(gprs_dev.currentCmdToGPRS!=CMD_GPRS_NULL)return;

	datalen=QueueGetDataLen(&gprs_dev.connect_param[current_socket_id].GprsTx);

	if(datalen&&gprs_dev.connect_param[current_socket_id].is_connected)	 //��������ݴ����Ͳ��ҵ�ǰģ����в��ҵ�ǰsocket����������
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
	ɨ��״̬
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
		case 0:		   //CMD_GPRS_SIGNAL_STATUS,			//�ź�ǿ�Ȳ�ѯ
			GPRS_send_cmd("AT+CSQ\r",0,"+CSQ:",CMD_GPRS_SIGNAL_STATUS,2);
		break;

		case 1:												//��ѯλ����Ϣ
			GPRS_send_cmd("AT+QCELLLOC=1\r",0,"+QCELLLOC:",CMD_GPRS_LOC_GET,60);	   				
		break;

		case 2:												//��ѯ�������ݳ�����Ϣ
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
	GPRS_basic_set();		   //��������
	GPRS_PPP_connect();		   //PPP���Ӵ���
	GPRS_netserver_connect();  //���������Ӵ���
	GPRS_netserver_send();	   //���ݷ��ʹ���
	GPRS_status_check();	   	//״̬ɨ��
	GPRS_timeout_check();      //��ʱ����
	GPRS_reset_check();		   //��������
}












