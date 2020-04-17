#ifndef __GPRS_H__
#define __GPRS_H__

#include "stm32f10x.h"
#include "Driver/usart.h"

#define GPRS_RX_buff_SIZE				1500				  //socket���ջ�����
#define GPRS_TX_buff_SIZE				1500				  //socket���ͻ�����

#define GPRS_IO							2					  //GPRSģ����ʹ�õĴ��ں�
#define SOCKET_TOTAL					1					  //socket�������� �������޸�

#define	GPRS_PowerOn()					GPIO_SetBits(GPIOB, GPIO_Pin_6)
#define	GPRS_PowerOff()					GPIO_ResetBits(GPIOB, GPIO_Pin_6)
#define TTS_ON()						//GPIO_SetBits(GPIOE, GPIO_Pin_7)
#define TTS_OFF()						//GPIO_ResetBits(GPIOE, GPIO_Pin_7)





/* ���͸�GPRSģ������������ */
typedef enum
{
	CMD_GPRS_NULL=0,					//��ǰû��ATָ��ִ��

	CMD_GPRS_BASIC_SETTING,				//��������

	//�������
	CMD_GPRS_APN_SET,					//����APN
	CMD_GPRS_PPP_OPEN,					//����·
	CMD_GPRS_PPP_CLOSE,					//�Ͽ���·
	CMD_GPRS_LOC_IP_GET,				//��ȡ����IP
	CMD_GPRS_DNS_IP_GET,				//��������

	//TCP��UDP
	CMD_GPRS_NET_CONNECT,   			//TCP��UDP����
	CMD_GPRS_NET_SEND,					//TCP��UDP���ݷ���
	CMD_GPRS_NET_CLOSE,					//TCP��UDP�ر�

	//FTP
	CMD_GPRS_FTP_LOGIN,					//FTP��¼
	CMD_GPRS_FTP_TYPE,					//FTP�ļ�����
	CMD_GPRS_FTP_DOWNLOAD,				//FTP�ļ�����
	CMD_GPRS_FTP_QUIT,					//TTP�˳�

	//����
	CMD_GPRS_SMS_SEND,                  //����Ϣ����

	//�绰
	CMD_GPRS_DAIL,                      //����绰

	//״̬��ѯ
	CMD_GPRS_SIM_STATUS,				//SIM��״̬
	CMD_GPRS_REG_STATUS,				//����ע��״̬
	CMD_GPRS_MODULE_STATUS,				//ģ��״̬
	CMD_GPRS_SIGNAL_STATUS,				//�ź�ǿ��
	CMD_GPRS_PPP_STATUS,				//PPP״̬
	CMD_GPRS_NET_STATUS,				//��·״̬
	CMD_GPRS_LOC_GET,					//��ȡλ����Ϣ
} CMD_TO_GPRS;

/* GPRSģ��״̬���� */
typedef enum
{
    GPRS_Null,
    GPRS_Reseting,			  // GPRSģ�鸴λ�׶� 
    GPRS_BasicSetting,		  // GPRSģ���ʼ���׶�
	GPRS_PPP_Connecting,      // PPP���ӽ׶�
    GPRS_NETSERVER_connecting,// ������������ӽ׶�
    GPRS_GprsSend,			  // ģ�鷢������ 
    GPRS_Talk				  // GPRSͨ��״̬��������ͨ���رգ�����ͨ���򿪣���������绰�����ڴ�״̬�£� 
} STATE_GPRS;

typedef enum
{
	CHECK_SIM,				 //���SIM��״̬
	CHECK_REG,				 //���ע��״̬
	CHECK_SIGNAL,			 //����ź�״̬
	SET_APN,				 //����APN
	OPEN_PPP,				 //����·
}PPP_CONNECT_STEP;			 //PPP���Ӳ���


typedef struct
{
	char signal;	         //�ź�ǿ��
	char SMS_Center_no[20];	 //����Ϣ���ĺ���
	char ICCID[32];			 //�ֻ���ICCID��
	u32 latitude;            //����	�����֮һ��
	u32 longitude;			 //γ�� �����֮һ��
}STATUS;


typedef struct
{
    char buf[1024];
    u32  index;
}GPRS_REC;

typedef struct
{
	u32 limit_second; //����ͨ��ʱ������
	u32 start_tim;    //��ʼʱ��
    u8 Listen_Talk;	  //�Լ���ΪĿ��ȥ����绰������Ŀ����Ϊ�˴Ӳ�ͬ�ĵط���ȡ���к��룩	 0����ͨ�绰  1������
    char dial_num[20];
} Talk_Type;


typedef struct
{
	char APN[32];	  //APN
	char USER[32];    //USER
	char PWD[32];	  //PWD
}Network_param;		   //�������

typedef struct
{
	char socket_id;			 //0-3
	char domain[32];		 //IP������
	char port[8];			 //���Ӷ˿ں�
	char type;				 //0:TCP   1:UDP
	char is_connected;       //0:δ����  1:������
	S_CQueue GprsRx;		 //�ѽ��յ�����
	S_CQueue GprsTx;		 //�����͵�����
}Connect_param;		   //���Ӳ���


typedef struct
{
	char mode;				 //����ʱ��0:�޶�����Ҫ����  1:textģʽ����    2:PDU��ʽ����	 ����ʱ��0:û�н��յ����� 1��textģʽ����  2��PDU��ʽ����
	u16 len;				 //�����ͳ���
	char phone_no[20];		 //�������ֻ���
	char buf[160];			 //����������
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
	u8 time_out;                                    //��ʱʱ��
	STATUS status;
	u32 timerSecondSendAT;
	Network_param network_param;					   //apn
	Connect_param connect_param[SOCKET_TOTAL];		   //gprs���Ӳ���
	MSG msg;
	Talk_Type talk;
	SOCKET_ID socket_id;
	u8 error_count;
}GPRS_DEV;



void GPRS_StartUp(void);
void GPRSProcess(void);
void API_vpdn_set(char* apn,char* user,char* pwd);						   //����apn���û���������
void API_server_ip_port_set(u8 socket_id,u8 *domain,u8 *port,u8 type);	   //����ָ���ķ����� socket_id:0-SOCKET_TOTAL-1 domain:ip������ port:�˿ں�  type:0 TCP  1 UDP
void API_server_data_send(u8 socket_id,u8* src,u16 len);				   //���������ݷ���
u8 API_server_data_recv(u8 socket_id,char* buf);						   //���������ݽ���	socket_id:socket���   buf:������   �����ݷ���1�������ݷ���0   �ɹ�����һ���ֽڵ�����
void API_sms_msg_send(char *src,u32 src_size,char *phone_no,u8 type);	   //GPRS����һ������  type 1 textģʽ  2PDUģʽ,���ΪPDU��ʽ���뱣֤����ΪUSC����
void API_make_a_call(char* phone_no,u32 tim,u8 type);					   //GPRS����绰 phone_no���绰����  tim��ʱ�� 0Ϊ��ʱ�� ����Ϊ��ʱ�ޣ����Ϊ0xFFFFFFFF������ʱ�޹Ҷϵ绰  type��ͨ������ 0����ͨ�绰  1������
void API_play_TTS(char* src,u16 src_len);								   //�ݲ�֧��	������TTS����ֻ��ѡ��һ��
u8 API_get_socket_status(u8 socket_id);									   //�����·״̬        socket_id:socket���   ����ֵ��0��δ����   1��������
u16 API_get_send_data_len(u8 socket_id);



#endif



