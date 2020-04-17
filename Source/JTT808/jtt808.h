#ifndef __JTT808_H
#define __JTT808_H

#include "include.h"
#include "JTT808/jtt808_config.h"

#define JTT808_MSG_TAG          0x7E
#define JTT808_MSG_ESCAPE_TAG   0x7D



typedef enum
{
    JTT808_OK,			/**< There is no error       */
    JTT808_ERR,				/**< A generic error happens */
    JTT808_TIMEOUT		/**< Timed out               */
} JTT808t_Err;



/******************ƽ̨��ϢID********************************************************/
#define JTT808_CMD_PLATFORM_ACT					0x8001	//ƽ̨ͨ��Ӧ��
#define JTT808_CMD_RUERY_PACKAGE                0x8003  //�����ְ�����
#define JTT808_CMD_TERMINAL_REGACT				0x8100	//�ն�ע��Ӧ��
#define JTT808_CMD_SET_TERMINAL_PARAM			0x8103	//�����ն˲���
#define JTT808_CMD_QUERY_TERMINAL_PARAM			0x8104	//��ѯ�ն˲���
#define JTT808_CMD_QUERY_APPOINT_TERMINAL_PARAM 0x8106  //��ѯָ���ն˲���
#define JTT808_CMD_TERMINAL_CTRL				0x8105	//�ն˿���
#define JTT808_CMD_QUERY_TERMINAL_PROPERTY      0x8107  //��ѯ�ն�����
#define JTT808_CMD_UPDATE_PKG_DOWN				0x8108	//�·��ն�������
#define JTT808_CMD_LOCATION_QUERY				0x8201	//λ����Ϣ��ѯ
#define JTT808_CMD_LOCATION_TRACK_CTRL			0x8202	//��ʱλ�ø��ٿ���
#define JTT808_CMD_MANUAL_CONFIRM_ALARM         0x8203  //�˹�ȷ�ϱ���
#define JTT808_CMD_TXTMSG_DOWN					0x8300	//�ı���Ϣ�·�
#define JTT808_CMD_EVENT_SET					0x8301	//�¼�����
#define JTT808_CMD_QUESTION_DOWN				0x8302	//�����·�
#define JTT808_CMD_SET_MSG_BROADCAST_MENU		0x8303	//��Ϣ�㲥�˵�����
#define JTT808_CMD_MSG_SERVICE					0x8304	//��Ϣ����
#define JTT808_CMD_DIAL_BACK					0x8400	//�绰�ز�
#define JTT808_CMD_SET_TEL_DICT					0x8401	//���õ绰��
#define JTT808_CMD_CAR_CTRL						0x8500	//��������
#define JTT808_CMD_AREA_CYCLE_SET				0x8600	//����Բ������
#define JTT808_CMD_AREA_CYCLE_DEL				0x8601	//ɾ��Բ������
#define JTT808_CMD_AREA_RECTANGLE_SET			0x8602	//���þ�������
#define JTT808_CMD_AREA_RECTANGLE_DEL			0x8603	//ɾ����������
#define JTT808_CMD_AREA_POLYGON_SET				0x8604	//���ö��������
#define JTT808_CMD_AREA_POLYGON_DEL				0x8605	//ɾ�����������
#define JTT808_CMD_ROUTE_SET					0x8606	//����·��
#define JTT808_CMD_ROUTE_DEL					0x8607	//ɾ��·��
#define JTT808_CMD_DRIVE_RECORD_COLLECT			0x8700	//��ʻ��¼�����ݲɼ�����
#define JTT808_CMD_DRIVE_RECORD_PARAM_DOWN		0x8701	//��ʻ��¼�ǲ����´�����
#define JTT808_CMD_DRIVER_ID_DOWN				0x8702	//��ʻԱ�����Ϣ�ɼ�����
#define JTT808_CMD_MULTIMEDIA_UP_ACT			0x8800	//��ý�������ϴ�Ӧ��
#define JTT808_CMD_CAMERA_SHOOT					0x8801	//����ͷ������������
#define JTT808_CMD_STORE_MULTIMEDIA_INDEX		0x8802	//�洢��ý�����ݼ���
#define JTT808_CMD_STORE_MULTIMEDIA_UP			0x8803	//�洢��ý�������ϴ�
#define JTT808_CMD_AUDIO_RECORD					0x8804	//¼����ʼ����
#define JTT808_CMD_SINGAL_STORE_MULTIMEDIA_UP	0x8805	//�����洢��ý�����ݼ����ϴ�
#define JTT808_CMD_DOWNLINK_DATA_TRANSPARENT	0x8900	//��������͸��
#define JTT808_CMD_PLATFORM_RSA_PUBKEY			0x8A00	//ƽ̨RSA��Կ



/*****************�ն���ϢID*********************************************************************************/
#define JTT808_CMD_TERMINAL_ACT					0x0001	//�ն�ͨ��Ӧ��
#define JTT808_CMD_TERMINAL_BEAT				0x0002	//�ն�����
#define JTT808_CMD_TERMINAL_REG					0x0100	//�ն�ע��
#define JTT808_CMD_TERMINAL_DEREG				0x0003	//�ն�ע��
#define JTT808_CMD_TERMINAL_AUTHENTICATION		0x0102	//�ն˼�Ȩ
#define JTT808_CMD_QUERY_TERMINAL_PARAM_ACT		0x0104	//��ѯ�ն˲���Ӧ��
#define JTT808_CMD_QUERY_TERMINAL_PROPERTY_ACT  0x0107  //��ѯ�ն�����Ӧ��
#define JTT808_CMD_UPDATE_PKG_DOWN_ACT			0x0108	//�ն��������Ӧ��
#define JTT808_CMD_LOCATION_REPORT				0x0200	//λ����Ϣ�㱨
#define JTT808_CMD_LOCATION_QUERY_ACT			0x0201	//λ����Ϣ��ѯӦ��
#define JTT808_CMD_EVENT_REPORT					0x0301	//�¼�����
#define JTT808_CMD_QUESTION_ACT					0x0302	//����Ӧ��
#define JTT808_CMD_MSG_BROADCAST_FLAG			0x0303	//��Ϣ�㲥/ȡ��
#define JTT808_CMD_CAR_CTRL_ACT					0x0500	//��������Ӧ��
#define JTT808_CMD_DRIVE_RECORD_UP				0x0700	//��ʻ��¼�������ϴ�
#define JTT808_CMD_WAYBILL_UP					0x0701	//�����˵��ϱ�
#define JTT808_CMD_DRIVER_ID_UP					0x0702	//��ʻԱ�����Ϣ�ɼ��ϱ�
#define JTT808_CMD_LOCATION_BULK_UPLOAD         0x0704  //��λ���������ϴ�
#define JTT808_CMD_CAN_UPLOAD                   0x0705  //CAN���������ϴ�
#define JTT808_CMD_MULTIMEDIA_EVENT_UP			0x0800	//��ý���¼���Ϣ�ϴ�
#define JTT808_CMD_MULTIMEDIA_DATA_UP			0x0801	//��ý�������ϴ�
#define JTT808_CMD_CAMERA_SHOOT_ACT				0x0805	//����ͷ������������Ӧ��
#define JTT808_CMD_STORE_MULTIMEDIA_INDEX_ACT	0x0802	//�洢��ý�����ݼ���Ӧ��
#define JTT808_CMD_UPSTREAM_DATA_TRANSPARENT	0x0900	//��������͸��
#define JTT808_CMD_ZIP_DATA_UP					0x0901	//����ѹ���ϱ�
#define JTT808_CMD_TERMINAL_RSA_PUBKEY			0x0A00	//�ն�RSA��Կ
#define JTT808_PLACE_UP									0x0B00 




typedef __packed struct
{
    u16 cnt;
    u16 num;
} JTT808t_MsgPackage;


typedef __packed struct
{
    u16 size:10;
    u16 encrypt:3;
    u16 packet:1;
    u16 reserve:2;
} JTT808t_MsgProperty;

typedef __packed struct
{
    u16 id;
    JTT808t_MsgProperty property;
    u8 mobile_no[6];
    u16 serial_no;
    JTT808t_MsgPackage package;
} JTT808t_MsgHead;


typedef __packed struct
{
    u8 head_tag;
    JTT808t_MsgHead head;
    void * body;
    u8 checksum;
    u8 end_tag;
} JTT808t_Msg;



typedef __packed struct
{
	u16 serial_no;
	u16 ACT_id;
	u8 ret;
} JTT808t_ACT;            /*ͨ��Ӧ��, ������ 2013-07-01*/

typedef enum
{
    JTT808_ACT_RET_OK,				//�ɹ�/ȷ��
    JTT808_ACT_RET_FAIL,			//ʧ��
    JTT808_ACT_RET_ERR,				//��Ϣ����
    JTT808_ACT_RET_INVALID,			//��֧��
    JTT808_ACT_RET_ALARM_COMFIRM	//��������ȷ��
} JTT808t_ACTRet;


typedef __packed struct
{
    int16_t ID;
}JTT808_PACKET_Sequence_no;


typedef __packed struct
{
    int16_t first_package_seral_no;
    u8 total;
    JTT808_PACKET_Sequence_no* ID_list;
}JTT808t_Ruery_package;

typedef __packed struct
{
	u16 province_id;
	u16 city_id;
	u8 producer_id[5];
	u8 model_type[20];
	u8 terminal_id[7];
	u8 lic_plate_color;
	u8 lic_plate[20];
} JTT808t_TerminalReg;    /*�ն�ע��, ������ 2013-07-01*/


typedef __packed struct
{
	u16 serial_no;
	u8 ret;
	void * AKey;
} JTT808t_TerminalRegACT;  /*�ն�ע��Ӧ��, ������ 2013-07-01*/


typedef __packed struct
{
	u32 can_id:29;				 //bit28-bit0 ��ʾ CAN ���� ID
	u32 sample_type:1;           //bit29 ��ʾ���ݲɼ���ʽ��0��ԭʼ���ݣ�1���ɼ�����ļ���ֵ��
	u32 frame_type:1;            //bit30 ��ʾ֡���ͣ�0����׼֡��1����չ֡��
	u32 can_channel:1;           //bit31 ��ʾ CAN ͨ���ţ�0��CAN1��1��CAN2��
	u32 sample_time_interval:32; //bit63-bit32 ��ʾ�� ID �ɼ�ʱ����(ms)��0 ��ʾ���ɼ���
}JTT808t_CanID_Sample_Set;

typedef __packed struct
{
	u32 can_id:29;				 //bit28-bit0 ��ʾ CAN ���� ID
	u32 sample_type:1;			 //bit29 ��ʾ���ݲɼ���ʽ��0��ԭʼ���ݣ�1���ɼ�����ļ���ֵ��
	u32 frame_type:1;			 //bit30 ��ʾ֡���ͣ�0����׼֡��1����չ֡��
	u32 can_channel:1;			 //bit31 ��ʾ CAN ͨ���ţ�0��CAN1��1��CAN2��
	char can_data[8];			 //CAN ����
}JTT808t_CanID_Upload;



typedef enum
{
    JTT808_TERMINAL_PARAM_BEAT = 									0x0001,//�ն��������
    JTT808_TERMINAL_PARAM_TCP_TIMEOUT,								// 0x0002//TCPӦ��ʱʱ��
    JTT808_TERMINAL_PARAM_TCP_REPEAT_TIMES,						    // 0x0003//TCP�ش�����
    JTT808_TERMINAL_PARAM_UDP_TIMEOUT,								// 0x0004//UDPӦ��ʱʱ��
    JTT808_TERMINAL_PARAM_UDP_REPEAT_TIMES,						    // 0x0005//UDP�ش�����
    JTT808_TERMINAL_PARAM_SMS_TIMEOUT,								// 0x0006//SMSӦ��ʱʱ��
    JTT808_TERMINAL_PARAM_SMS_REPEAT_TIMES,						    // 0x0007//SMS�ش�����


    JTT808_TERMINAL_PARAM_MAIN_SERVER_APN = 						0x0010,//��������APN
    JTT808_TERMINAL_PARAM_MAIN_SERVER_USER,						    // 0x0011//�������������û���
    JTT808_TERMINAL_PARAM_MAIN_SERVER_PSW,						    // 0x0012//����������������
    JTT808_TERMINAL_PARAM_MAIN_SERVER_DOMAIN,						// 0x0013//��������������IP
    JTT808_TERMINAL_PARAM_BACKUP_SERVER_APN,						// 0x0014//���ݷ�����APN
    JTT808_TERMINAL_PARAM_BACKUP_SERVER_USER,						// 0x0015//���ݷ����������û���
    JTT808_TERMINAL_PARAM_BACKUP_SERVER_PSW,						// 0x0016//���ݷ�������������
    JTT808_TERMINAL_PARAM_BACKUP_SERVER_DOMAIN,					    // 0x0017//���ݷ�����������IP
    JTT808_TERMINAL_PARAM_SERVER_TCP_PORT,							// 0x0018//TCP�˿�
    JTT808_TERMINAL_PARAM_SERVER_UDP_PORT,							// 0x0019//UDP�˿�
    JTT808_TERMINAL_PARAM_IC_AUTHENTICATION_MAIN_SERVER_DOMAIN,     // 0x001A//��·����֤IC����֤��������APN
    JTT808_TERMINAL_PARAM_IC_AUTHENTICATION_TCP_PORT,               // 0x001B//��·����֤IC����֤��������TCP�˿�
    JTT808_TERMINAL_PARAM_IC_AUTHENTICATION_UDP_PORT,               // 0x001C//��·����֤IC����֤��������UDP�˿�
    JTT808_TERMINAL_PARAM_IC_AUTHENTICATION_BACKUP_SERVER_DOMAIN,   // 0x001D//��·����֤IC����֤���ݷ�����APN



    JTT808_TERMINAL_PARAM_LOCATION_REPORT_STRATEGY = 			    0x0020,//λ�û㱨����
    JTT808_TERMINAL_PARAM_LOCATION_REPORT_PROGRAM,				    // 0x0021//λ�û㱨����
    JTT808_TERMINAL_PARAM_REPORT_DRIVER_UNLOGIN_INTERVAL,		    // 0x0022//��ʻԱδ��¼�㱨ʱ����

    JTT808_TERMINAL_PARAM_REPORT_AT_SLEEP_INTERVAL = 				0x0027,//���߻㱨ʱ����
    JTT808_TERMINAL_PARAM_REPORT_AT_ALARM_INTERVAL,				    // 0x0028//���������㱨ʱ����
    JTT808_TERMINAL_PARAM_DEFAULT_REPORT_INTERVAL,				    // 0x0029//ȱʡʱ��㱨���

    JTT808_TERMINAL_PARAM_DEFAULT_REPORT_DST_INTERVAL = 			0x002C,//ȱʡ����㱨���
    JTT808_TERMINAL_PARAM_REPORT_DRIVER_UNLOGIN_DST_INTERVAL,	    // 0x002D//��ʻԱδ��¼�㱨������
    JTT808_TERMINAL_PARAM_REPORT_AT_SLEEP_DST_INTERVAL,			    // 0x002E//���߻㱨������
    JTT808_TERMINAL_PARAM_REPORT_AT_ALARM_DST_INTERVAL,			    // 0x002F//���������㱨������
    JTT808_TERMINAL_PARAM_INFLECT_POINT_COMPLEMENT_BIOGRAPHY,	    // 0x0030//�յ㲹���Ƕ�
    JTT808_TERMINAL_PARAM_BOUND_RADIUS,							    //0x0031//����Χ���뾶

    JTT808_TERMINAL_PARAM_MONITOR_PLATFORM_TEL = 					0x0040,//���ƽ̨�绰����
    JTT808_TERMINAL_PARAM_RESET_TEL,								// 0x0041//��λ�绰����
    JTT808_TERMINAL_PARAM_RESTORE_FACTORY_TEL,					    // 0x0042//�ָ��������õ绰����
    JTT808_TERMINAL_PARAM_MONITOR_PLATFORM_SMS_TEL,				    // 0x0043//���ƽ̨SMS�绰����
    JTT808_TERMINAL_PARAM_SMS_ALARM_TEL,							// 0x0044//�����ն�SMS�ı���������
    JTT808_TERMINAL_PARAM_PHONE_ANSWER_STRATEGY,					// 0x0045//�ն˵绰��������
    JTT808_TERMINAL_PARAM_TALK_MAX_TIME,							// 0x0046//ÿ���ͨ��ʱ��
    JTT808_TERMINAL_PARAM_TALK_MAX_TIME_IN_MON,					    // 0x0047//ÿ���ͨ��ʱ��
    JTT808_TERMINAL_PARAM_MONITOR_TEL,								// 0x0048//�����绰����
    JTT808_TERMINAL_PARAM_MONITOR_PLATFORM_PRIVILEGE_SMS_TEL,	    // 0x0049//���ƽ̨��Ȩ���ź���


    JTT808_TERMINAL_PARAM_ALARM_MASK = 						    	0x0050,//����������
    JTT808_TERMINAL_PARAM_ALARM_SMS_SWITCH,					    	// 0x0051//���������ı�SMS����
    JTT808_TERMINAL_PARAM_ALARM_SHOOT_SWITCH,				    	// 0x0052//�������㿪��
    JTT808_TERMINAL_PARAM_ALARM_STORE_FLAG,					    	// 0x0053//�������㴢���־
    JTT808_TERMINAL_PARAM_KEY_FLAG,									// 0x0054//�ؼ���־
    JTT808_TERMINAL_PARAM_TOP_SPEED,								// 0x0055//����ٶ�
    JTT808_TERMINAL_PARAM_SPEEDING_DURATION,						// 0x0056//���ٳ���ʱ��
    JTT808_TERMINAL_PARAM_CONTINOUOUS_DRIVE_TIME,					// 0x0057//������ʻʱ������
    JTT808_TERMINAL_PARAM_DRIVE_TIME_IN_DAY,						// 0x0058//�����ۼƼ�ʻʱ������
    JTT808_TERMINAL_PARAM_REST_MIN_TIME,							// 0x0059//��С��Ϣʱ��
    JTT808_TERMINAL_PARAM_STOP_MAX_TIME,							// 0x005A//�ͣ��ʱ��
    JTT808_TERMINAL_PARAM_OVERSPEED_ALARM_DIFF,                     // 0x005B//���ٱ���Ԥ����ֵ
    JTT808_TERMINAL_PARAM_FATIGUE_ALARM_DIFF,                       // 0x005C//ƣ�ͼ�ʻԤ����ֵ
    JTT808_TERMINAL_PARAM_COLLISION_ALARM_SET,                      // 0x005D//��ײ������������
    JTT808_TERMINAL_PARAM_ROLLOVER_ALARM_SET,                       // 0x005E//�෭������������


    JTT808_TERMINAL_PARAM_TIMING_PHOTOGRAPHY=                       0x0064,//��ʱ���տ���
    JTT808_TERMINAL_PARAM_DISTANCE_PHOTOGRAPHY,                     // 0x0065//�������տ���


    JTT808_TERMINAL_PARAM_MULTIMEDIA_QUALITY = 					    0x0070,//ͼ������1~10
    JTT808_TERMINAL_PARAM_BRIGHTNESS, 								// 0x0071//�߶�0~255
    JTT808_TERMINAL_PARAM_CONTRAST,									// 0x0072//�Աȶ�0~127
    JTT808_TERMINAL_PARAM_SATURATION,								// 0x0073//���Ͷ�0~127
    JTT808_TERMINAL_PARAM_CHROMA,									// 0x0074//ɫ��0~255

    JTT808_TERMINAL_PARAM_MILEAGE = 								0x0080,//������̱����
    JTT808_TERMINAL_PARAM_PROVINCE_ID,								// 0x0081//ʡID
    JTT808_TERMINAL_PARAM_CITY_ID,									// 0x0082//��ID
    JTT808_TERMINAL_PARAM_LICENSE_PLATE,							// 0x0083//����
    JTT808_TERMINAL_PARAM_LICENSE_PLATE_COLOR,						// 0x0084//������ɫ


    JTT808_TERMINAL_PARAM_GNSS_POSITION_TYPE=                       0x0090,//GNSSģ�鶨λģʽ
    JTT808_TERMINAL_PARAM_GNSS_BAUDRATE,                            // 0x0091//GNSS������
    JTT808_TERMINAL_PARAM_GNSS_OUTPUT_RATE,                         // 0x0092//GNSSģ����ϸ��λ�������Ƶ��
    JTT808_TERMINAL_PARAM_GNSS_SAMP_FREQUENCY,                      // 0x0093//GNSSģ����ϸ��λ���ݲɼ�Ƶ��
    JTT808_TERMINAL_PARAM_GNSS_UPLOAD_TYPE,                         // 0x0094//GNSSģ����ϸ��λ�����ϴ���ʽ
    JTT808_TERMINAL_PARAM_GNSS_UPLOAD_SET,                          // 0x0095//GNSSģ����ϸ��λ�����ϴ�����


    JTT808_TERMINAL_PARAM_CAN1_SAMP_INTERVAL=                       0x0100,//CAN����ͨ��1�ɼ�ʱ����
    JTT808_TERMINAL_PARAM_CAN1_UPLOAD_INTERVAL,                     // 0x0101//CAN����ͨ��1�ϴ�ʱ����
    JTT808_TERMINAL_PARAM_CAN2_SAMP_INTERVAL,                       // 0x0102//CAN����ͨ��2�ɼ�ʱ����
    JTT808_TERMINAL_PARAM_CAN2_UPLOAD_INTERVAL,                     // 0x0103//CAN����ͨ��2�ϴ�ʱ����
//    JTT808_TERMINAL_PARAM_CAN_ID_SAMP_SET,                          // 0x0110//CAN����ID�����ɼ�����

} JTT808t_TerminalParamId;       /*�ն˲�������, ������ 2013-07-01*/


typedef __packed struct
{
	JTT808t_TerminalParamId id;
	u8 size;
	void * val;
} JTT808t_TerminalParamItem;


typedef __packed struct
{
	u8 cnt;
	JTT808t_TerminalParamItem *param_list;
} JTT808t_TerminalParam;


typedef __packed struct
{
	u16 serial_no;
	u8 cnt;
	JTT808t_TerminalParamItem *param_list;
} JTT808t_TerminalParamACT;



typedef __packed struct
{
	u32 acc:1;                      //0��ACC��      1��ACC��                            0
	u32 is_location:1;              //0��GNSSδ��λ 1��GNSS��λ                         1
	u32 latitude_type:1;            //0����γ       1����γ                             2
	u32 longitude_type:1;           //0������       1������                             3
	u32 operation:1;                //0����Ӫ״̬   1��ͣ��״̬                         4
	u32 coordinate_is_encrypt:1;	//0: ��γ��δ����  1����γ���Ѽ���                  5
	u32 reserve1:2;                 //����                                             6-7

	u32 delivery_status1:1;         //                                                  8
	u32 delivery_status2:1;         //00:�ճ�  01������   10������   11������           9

	u32 fuel_way:1;                 //0��������·����     1��������·�Ͽ�               10
	u32 circuit:1;                  //0��������·����     1��������·�Ͽ�               11
	u32 door_lock:1;                //0�����Ž���         1�����ż���                   12

	u32 door1:1;                    //0����1��        1����1����ǰ�ţ�                  13
	u32 door2:1;                    //0����2��        1����2�������ţ�                  14
	u32 door3:1;                    //0����3��        1����3�������ţ�                  15
	u32 door4:1;                    //0����4��        1����4������ʻϯ�ţ�              16
	u32 door5:1;                    //0����5��        1����5�����Զ��壩                17

	u32 location_type_GPS:1;        //0��δʹ�� GPS ���ǽ��ж�λ��1��ʹ�� GPS ���ǽ��ж�λ               18
	u32 location_type_BD:1;         //0��δʹ�� ���� ���ǽ��ж�λ��1��ʹ�� ���� ���ǽ��ж�λ             19
    u32 location_type_GLONASS:1;    //0��δʹ�� GLONASS ���ǽ��ж�λ��1��ʹ�� GLONASS ���ǽ��ж�λ       20
    u32 location_type_GALILEO:1;    //0��δʹ�� Galileo ���ǽ��ж�λ��1��ʹ�� Galileo ���ǽ��ж�λ       21

	u32 reserve2:10;                //����
} JTT808t_LocationState;


typedef __packed struct
{
	u32 emergency:1;                 //������Ƴ�������������غ󴥷���    �յ�Ӧ�������                  0
	u32 speeding:1;                  //���ٱ���                          ��־ά���������������          1
	u32 fatigue_drive:1;             //ƣ�ͼ�ʻ                          ��־ά���������������          2
	u32 warning:1;                   //Σ��Ԥ��                          �յ�Ӧ�������                  3
	u32 gnss_err:1;                  //GNSSģ�鷢������                  ��־ά���������������          4
	u32 gnss_disconnect:1;           //GNSS����δ�ӻ򱻼���              ��־ά���������������          5
	u32 gnss_ant_shortcircuit:1;     //GNSS���߶�·                      ��־ά���������������          6
	u32 power_down:1;                //�ն�����ԴǷѹ                    ��־ά���������������          7

	u32 power_off:1;                 //�ն�����Դ����                    ��־ά���������������          8
	u32 display_err:1;               //�ն�LCD����ʾ������               ��־ά���������������          9
	u32 tts_err:1;                   //TTSģ�����                       ��־ά���������������          10
	u32 camera_err:1;                //����ͷ����                        ��־ά���������������          11
    u32 IC_err:1;                    //��·����֤IC��ģ�����            ��־ά���������������          12
    u32 speeding_warning:1;          //����Ԥ��                          ��־ά���������������          13
    u32 fatigue_drive_warning:1;     //ƣ�ͼ�ʻԤ��                      ��־ά���������������          14
	u32 reserve1:3;                  //����                                                             15-17

	u32 drive_timeout_in_day:1;      //�����ۼƼ�ʻ��ʱ                  ��־ά���������������          18
	u32 stop_timeout:1;              //��ʱͣ��                          ��־ά���������������          19
	u32 across_area:1;               //��������                          �յ�Ӧ�������                  20
	u32 across_route:1;              //����·��                          �յ�Ӧ�������                  21
	u32 section_drive_time_err:1;    //·����ʻʱ�䲻��/����             �յ�Ӧ�������                  22
	u32 route_out:1;                 //·��ƫ�뱨��                      ��־ά���������������          23

	u32 vss_err:1;                   //����VSS����                       ��־ά���������������          24
	u32 abnormal_fuel:1;             //���������쳣                      ��־ά���������������          25
	u32 stolen:1;                    //��������(ͨ������������)          ��־ά���������������          26
	u32 illegal_ignition:1;          //�����Ƿ����                      �յ�Ӧ�������                  27
	u32 illegal_move:1;              //�����Ƿ�λ��                      �յ�Ӧ�������                  28
	u32 rollover_crash:1;            //��ײԤ��                          ��־ά���������������          29
	u32 side_turn:1;                 //�෭Ԥ��                          ��־ά���������������          30
	u32 illegal_dooropen:1;          //�Ƿ������ű������ն�δ����        �յ�Ӧ�������                  31
} JTT808t_LocationAlarm;             //����ʱ�����жϷǷ����ţ�


typedef __packed struct
{
	JTT808t_LocationAlarm alarm_flag;
	JTT808t_LocationState state;
	u32 latitude;
	u32 longitude;
	s16 altitude;
	u16 speed;
	u16 azimuth;
	u8 time[6];
} JTT808t_LocationBasic;


typedef __packed struct
{
	u16 interval;
	u32 valid;
    u32 start_sec;
} JTT808t_LocationTrackCtrl;


typedef __packed struct
{
	u8 emergency:1;
	u8 reserve:1;
	u8 display:1;
	u8 tts_broadcast:1;
	u8 ad_display:1;
    u8 navi_info_can_fault_code:1;
	u8 rev:2;
} JTT808t_TxtMsgFlag;

typedef struct
{
	JTT808t_TxtMsgFlag flag;
	u8 *msg;
} JTT808t_TxtMsgDown;


typedef struct
{
	u8 id;
	u8 size;
	void *event;
} JTT808t_Event;

typedef struct
{
	u8 type;
	u8 cnt;
	JTT808t_Event * event_list;
} JTT808t_EventSet;


typedef __packed struct
{
	u8 emergency:1;
	u8 reserve:2;
	u8 tts_broadcast:1;
	u8 ad_display:1;
	u8 reserve2:3;
} JTT808t_QuestionDownFlag;

typedef __packed struct
{
	u8 id;
	u16 size;
	void * content;
} JTT808t_QuestionDownAnswer;

typedef __packed struct
{
	JTT808t_QuestionDownFlag flag;
	u8 size;
	void * question;
	JTT808t_QuestionDownAnswer * answer_list;
} JTT808t_QuestionDown;


typedef struct
{
	u16 serial_no;
	u8 id;
} JTT808t_QuestionACT;


typedef __packed struct
{
	u8 type;
	u16 size;
	void * name;
} JTT808t_BroadcastMsg;

typedef struct
{
	u8 type;
	u8 cnt;
	JTT808t_BroadcastMsg * broadcast_msg_list;
} dataSetMsgBroadcastMenu;


typedef struct
{
	u8 type;
	u8 flag;	// 0:ȡ����1���㲥
} JTT808t_MsgBroadcastFlag;


typedef __packed struct
{
	u8 type;
	u16 size;
	void *content;
} JTT808t_MsgService;


typedef __packed struct
{
	u8 flag;	// 0:��ͨͨ����1������
	u8 tel[20];
} JTT808t_DialBack;


typedef __packed struct
{
	u8 flag;	// 1:���룻2��������3������/����
	u8 tel_len;
	u8 tel[22];
	u8 contact_len;
	u8 contact[60];
} JTT808t_TelDict;

typedef struct
{
	u8 type;	// 0: del all; 1: remove & update; 2: add; 3: modify
	u8 cnt;
	JTT808t_TelDict * contact_list;
} JTT808t_SetTelDict;


typedef struct
{
	u8 lock:1;	// 0: unlock door; 1: lock door
	u32 reserve1:7;
} JTT808t_CarCtrlFlag;


typedef struct
{
	JTT808t_CarCtrlFlag flag;
} JTT808t_CarCtrl;


typedef struct
{
	u16 serial_no;
	JTT808t_LocationBasic locat_report;
} JTT808t_CarCtrlACT;


typedef __packed struct
{
	u16 time:1;
	u16 limit_speed:1;
	u16 in2driver:1;
	u16 in2platform:1;
	u16 out2driver:1;
	u16 out2platform:1;
	u16 latitude_type:1;
	u16 longitude_type:1;
	u16 doorcontrol:1;
	u16 reserve:5;
	u16 in2community:1;
	u16 in2GNSSdate:1;
} JTT808t_AreaProperty;



typedef __packed struct
{
	u32 id;
	JTT808t_AreaProperty area_property;
	u32 latitude;
	u32 longitude;
	u32 radius;
	u8 start_time[6];
	u8 end_time[6];
	u16 top_speed;
	u8 speeding_time;
} JTT808t_AreaCycle;



typedef __packed struct
{
	u32 id;
	JTT808t_AreaProperty area_property;
	u32 upleft_latitude;
	u32 upleft_longitude;
	u32 lowright_latitude;
	u32 lowright_longitude;
	u8 start_time[6];
	u8 end_time[6];
	u16 top_speed;
	u8 speeding_time;
} JTT808t_AreaRectangle;


typedef __packed struct
{
	u32 latitude;
	u32 longitude;
} JTT808t_AreaPolygonItem;

typedef __packed struct
{
	u32 id;
	JTT808t_AreaProperty area_property;
	u8 start_time[6];
	u8 end_time[6];
	u16 top_speed;
	u8 speeding_time;
	u16 peak_cnt;
} JTT808t_AreaPolygonSet;


typedef struct
{
	u8 drive_time:1;
	u8 limit_speed:1;
	u8 latitude_type:1;
	u8 longitude_type:1;
	u8 reserve1:4;
} JTT808t_RouteSectionProperty;

typedef struct
{
	u32 turn_point_id;
	u32 section_id;
	u32 turn_point_latitude;
	u32 turn_point_longitude;
	u8 section_width;
	JTT808t_RouteSectionProperty section_property;
	u16 section_length_threshold;
	u16 section_inadequate_threshold;
	u16 section_top_speed;
	u8 section_speeding_time;
	u8 start_time[6];
	u8 end_time[6];
} JTT808t_RouteTurnPoint;


typedef struct
{
	u16 depend_time:1;
	u16 reserve1:1;
	u16 in2driver:1;
	u16 in2platform:1;
	u16 out2driver:1;
	u16 out2platform:1;
	u16 reserve2:10;
} JTT808t_RouteProperty;

typedef struct
{
	u32 id;
	JTT808t_RouteProperty property;
	u8 start_time[6];
	u8 end_time[6];
	u16 turn_point_cnt;
} JTT808t_RouteSet;



/*��ʻ��¼�ɼ�����, ������ 2013-07-02*/
typedef u8 JTT808t_DriveRecordCollectCmd;

/*��ʻ��¼�ϴ�, ������ 2013-07-02*/
typedef __packed struct
{
	u16 serial_no;
	JTT808t_DriveRecordCollectCmd cmd;
	void * content;
} JTT808t_DriveRecordUp;

/*��ʻ��¼�´�����, ������ 2013-07-02*/
typedef __packed struct
{
	JTT808t_DriveRecordCollectCmd cmd;
	void * content;
} JTT808t_DriveRecordParamDown;

/*�����˵��ϱ�, ������ 2013-07-02*/
typedef struct
{
	u32 size;
	void * content;
} JTT808t_WayBillUp;

/*��ʻԱ�����Ϣ�ɼ��ϱ�, ������ 2013-07-02*/
typedef __packed struct
{
	u8 name_size;
	void * name;
	u8 id[20];
	u8 qualification_lic[40];
	u8 lic_org_size;
	void *lic_org;
} JTT808t_DriverIdUp;

/*��ý���¼��ϴ�, ������ 2013-07-02*/
typedef struct
{
	u32 id;
	u8 type;
	u8 form;	// 0: jpeg; 1: tif; 2: mp3; 3: wav; 4: wmv
	u8 event_mod;	//0: platform download; 1: at time; 2: rob alarm; 3: carsh alarm
	u8 channel_id;
} JTT808t_MultimediaEventUp;

/*��ý�������ϴ�, ������ 2013-07-02*/
typedef __packed struct
{
	u32 id;
	u8 type;
	u8 form;	// 0: jpeg; 1: tif; 2: mp3; 3: wav; 4: wmv
	u8 event_mod;	//0: platform download; 1: at time; 2: rob alarm; 3: carsh alarm
	u8 channel_id;
	//JTT808t_LocationBasic locat_report;
	//void * package;
} JTT808t_MultimediaUp;

/*��ý�������ϴ�Ӧ��, ������ 2013-07-02*/
typedef __packed struct
{
	u32 id;
	u8 reload_size;
	u32 reload_id[125];
} JTT808t_MultimediaUpACT;

/*����ͷ������������, ������ 2013-07-02*/
typedef __packed struct
{
	u8 channel_id;
	u16 cmd;
	u16 time;
	u8 save_flag;
	u8 resolution;
	// 0x1: 320*240
	// 0x2: 640*480
	// 0x3: 800*600
	// 0x4: 1024*768
	// 0x5: 176*144[qcif]
	// 0x6: 352*288[cif]
	// 0x7: 704*288[half d1]
	// 0x8: 704*576[d1]
	u8 quality;
	u8 brightness;
	u8 contrast;
	u8 saturation;
	u8 chroma;
} JTT808t_CameraShoot;

/*�洢��ý�����ݼ���, ������ 2013-07-02*/
typedef struct
{
	u8 type;
	u8 channel_id;
	u8 event_mod;	//0: platform download; 1: at time; 2: rob alarm; 3: carsh alarm
	u8 start_time[6];
	u8 end_time[6];
} JTT808t_StoreMeltimediaIndex;

/*�洢��ý�����Ӧ��, ������ 2013-07-02*/
typedef  struct
{
	u32 id;
	u8 type;
	u8 channel_id;
	u8 event_mod;	//0: platform download; 1: at time; 2: rob alarm; 3: carsh alarm
	JTT808t_LocationBasic locat_report;
} JTT808t_StoreMeltimediaIndexACTItem;

typedef struct
{
	u16 serial_no;
	u16 cnt;
	JTT808t_StoreMeltimediaIndexACTItem *index_list;
} JTT808t_StoreMeltimediaIndexACT;

/*��ý�������ϴ�����, ������ 2013-07-02*/
typedef struct
{
	u8 type;
	u8 channel_id;
	u8 event_mod;	//0: platform download; 1: at time; 2: rob alarm; 3: carsh alarm
	u8 start_time[6];
	u8 end_time[6];
	u8 del_flag;
} JTT808t_StoreMultimediaUp;

/*¼����ʼ����, ������ 2013-07-02*/
typedef struct
{
	u8 cmd;	// 0: stop; 1: start
	u16 time;
	u8 save_flag;
	u8 sampling_rate;	//0: 8k; 1: 11k; 2: 23k; 3: 32k
} JTT808t_AudioRecord;

/*������/����͸��, ������ 2013-07-02*/
typedef struct
{
	u8 type;
	void * content;
} JTT808t_Transparent;

/*����ѹ���ϱ�, ������ 2013-07-02*/
typedef struct
{
	u32 size;
	void *body;
} JTT808t_ZipUp;

/*ƽ̨/�ն�RSA��Կ, ������ 2013-07-02*/
typedef struct
{
	u32 e_key;
	u8 n_key[128];
} JTT808t_RSAPubkey;



typedef enum
{
    JTT808_UP_NON=0x00,        //��������Ϣ
    JTT808_UP_START,           //APP���������������д�뿪ʼ����-->��������IAPʱ��д��APP���ⲿflash-->��ת����APP
    JTT808_UP_SUCCEED,         //��APP�����ɹ�  -->�ϴ������ɹ���Ϣ��-->дIAP��ϢΪ��������Ϣ
    JTT808_UP_FAIL,            //��APP����ʧ�ܣ�-->��������IAPʱ��д��APP���ڲ�flash-->�ϴ�����ʧ����Ϣ��-->дIAP��ϢΪ��������Ϣ
}IAP_INFO_t;



typedef struct
{
	u32 key_len;
	u8 key[128];
	u8 use_key;
}RSA_type;




typedef struct
{
    u8 buf[JTT808_MSG_MAX_SIZE];
    int16_t index;
}JTT808_RX_BUF;






extern char JTT808_Flag_Authentication;                     //��Ȩ״̬
extern char dirver_login_status;                            //��ʻԱ��¼״̬�� 0 ��δ�忨   1:�Ѳ忨
extern JTT808t_DialBack Dial;                               //�绰�ز�������
extern RSA_type secrt_key;                                  //RSA����


extern JTT808t_Msg jtt808_msg;
extern u8 jtt808_global_buf[4096];
extern u8 jtt808_global_buf_new[4096];



int JTT808_MsgEscape(char flag,const u8* src,u8* dst,int size);
u8 JTT808_Check(const u8 * src, u32 size);
u32 JTT808_MsgEncode(const JTT808t_Msg *src, u8 *dst);
JTT808t_Err JTT808_MsgDecode(const u8 *src, u32 src_size, JTT808t_Msg *dst);
JTT808t_Err JTT808_MsgPacket(u16 cmd, const void * body_src, u32 body_size, JTT808t_MsgPackage package, u8 prio);
void JTT808_Process(void);
void JTT808_init(void);







#endif
