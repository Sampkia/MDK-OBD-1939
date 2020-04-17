#ifndef __GPRS_H__
#define __GPRS_H__

#include "stm32f10x.h"


#define GPRS_RX_buff_SIZE				1500				  //socket���ջ�����
#define GPRS_TX_buff_SIZE				1500				  //socket���ͻ�����

#define GPRS_IO							2					  //GPRSģ����ʹ�õĴ��ں�
#define SOCKET_TOTAL					1					  //socket�������� �������޸�

#define	GPRS_PowerOn()					GPIO_SetBits(GPIOB, GPIO_Pin_6)
#define	GPRS_PowerOff()					GPIO_ResetBits(GPIOB, GPIO_Pin_6)
#define PWR_KEY_ON()						GPIO_SetBits(GPIOB, GPIO_Pin_4)
#define PWR_KEY_OFF()						GPIO_ResetBits(GPIOB, GPIO_Pin_4)




/* ���͸�GPRSģ������������ */
typedef enum
{
	CMD_GPRS_NULL=0,					//��ǰû��ATָ��ִ��

	CMD_GPRS_BASIC_SETTING,				//��������

	//�������
	CMD_GPRS_APN_SET,					//����APN
	CMD_GPRS_PPP_OPEN,					//����·

	//TCP��UDP
	CMD_GPRS_NET_CONNECT,   			//TCP��UDP����
	CMD_GPRS_NET_SEND,					//TCP��UDP���ݷ���

	//״̬��ѯ
	CMD_GPRS_SIM_STATUS,				//SIM��״̬
	CMD_GPRS_REG_STATUS,				//����ע��״̬
	CMD_GPRS_GREG_STATUS,				//GPRSע��״̬
	CMD_GPRS_SIGNAL_STATUS,				//�ź�ǿ��
	CMD_GPRS_LOC_GET,					//��ȡλ����Ϣ
	CMD_GPRS_SEND_STATUS,				//��ѯ������Ϣ
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
	CHECK_GPRS,				 //���GPRS�Ƿ�ע��
	SET_APN,				 //����APN
	OPEN_PPP,				 //����·
}PPP_CONNECT_STEP;			 //PPP���Ӳ���


typedef struct
{
	char signal;	         //�ź�ǿ��
	u32 latitude;            //����	�����֮һ��
	u32 longitude;			 //γ�� �����֮һ��
	u8 BJ_BCD_tim[6];		 //����ʱ��BCD��
}STATUS;


typedef struct
{
    char buf[1024];
    u32  index;
}GPRS_REC;


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
	char connect_socket_id;
	char send_socket_id;
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
	SOCKET_ID socket_id;
	u8 error_count;
}GPRS_DEV;


extern GPRS_DEV gprs_dev;

void GPRS_StartUp(void);
void GPRSProcess(void);
void API_vpdn_set(char* apn,char* user,char* pwd);						   //����apn���û���������
void API_server_ip_port_set(u8 socket_id,u8 *domain,u8 *port,u8 type);	   //����ָ���ķ����� socket_id:0-SOCKET_TOTAL-1 domain:ip������ port:�˿ں�  type:0 TCP  1 UDP
void API_server_data_send(u8 socket_id,u8* src,u16 len);				   //���������ݷ���
u8 API_server_data_recv(u8 socket_id,char* buf);						   //���������ݽ���	socket_id:socket���   buf:������   �����ݷ���1�������ݷ���0   �ɹ�����һ���ֽڵ�����
u8 API_get_socket_status(u8 socket_id);									   //�����·״̬        socket_id:socket���   ����ֵ��0��δ����   1��������
u16 API_get_send_data_len(u8 socket_id);



#endif



