#ifndef JTT808_CONFIG_H_
#define JTT808_CONFIG_H_

#include "stm32f10x.h"


/*******************APP������***************************/
#define NEW_APP_START_ADDR          (0*4096)             //��APP��ʼ��ַ
#define NEW_APP_END_ADDR            (100*4096-1)         //��APP������ַ
#define OLD_APP_START_ADDR          (100*4096)           //��APP��ʼ��ַ
#define OLD_APP_END_ADDR            (200*4096-1)         //��APP������ַ


/*******************����������Ϣ��**********************/
#define IAP_INFO_ADDR    			(201*4096)           //�������� 

/*******************����Χ����**************************/
#define CYCLE_AREAR_ADDR            (202*4096)           //Բ��Χ��
#define RECTANGULAR_AREAR_ADDR      (203*4096)           //����Χ��
#define POLYGON_AREAR_ADDR          (204*4096)           //�����Χ��
#define ROUTE_AREAR_ADDR            (205*4096)           //·��

/**********************������****************************/
#define EVENT_ADDR                  (206*4096)           //�¼��洢��
#define MSGBROADCASTMENU_ADDR       (207*4096)           //��Ϣ�˵��洢��
#define TELDICT_ADDR                (208*4096)           //�绰���洢��

/*******************�ϵ�������**************************/
#define LOACTION_START_ADDR         (210*4096)           //�ϵ�������ʼ��ַ
#define LOACTION_END_ADDR           (310*4096-1)         //�ϵ�����������ַ

/*******************��Ϣ������**************************/
#define JTT808_MSG_START_ADDR       (310*4096)           //��Ϣ��������ʼ��ַ
#define JTT808_MSG_END_ADDR         (510*4096-1)         //��Ϣ�����������ַ



/**************************�ն�����*****************************/
#define PRODUCTOR_ID        "70407"                                     //������ID
#define MODULE_TYPE         "TR2200"                                    //�ն��ͺ�
#define HARDWARE_VERSION    "HV01"                                      //Ӳ���汾��
#define SOFTWARE_VERSION    "V150516"                                   //�̼��汾�š�����汾��
#define GPS_MODULE_TYPE     0x03                                        //֧��GPS��λ��֧�ֱ�����λ����֧��Galileo��λ��  ��֧��GLONASS��λ
#define GPRS_MODULE_TYPE    (USE_CDMA?0x02:0x01)                        //GPRS���ƶ���ͨ2G��CDMA������2G��TD-SCDMA���ƶ�3G��WCDMA����ͨ3G��CDMA2000������3G��TD-LTE���ƶ�4G
#define TERMINAL_TYPE       0x0F                                        //���ã��ͳ���Σ�ճ����������⳵����֧��Ӳ��¼��һ���


#define JTT808_MSG_MAX_BODY_SIZE (1023+100)                               //�����Ϣ�峤��
#define JTT808_MSG_MAX_SIZE      (16 + JTT808_MSG_MAX_BODY_SIZE + 3+6)//һ����Ϣ��󳤶�=��ʶλ��2��+У��λ��1��+��Ϣͷ��16��+��Ϣ�壨1024��
#define	JTT808_RECE_TIMEOUT	5											//�ն˽���һ�����ݳ�ʱʱ��  100ms��λ

#define CAN_ID_TOTAL			  30                                    //֧�ֵ����CAN_ID����




extern u8 SIM_ICCID[21];                                                //SIM��CCID��




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
