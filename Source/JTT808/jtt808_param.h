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
    u32 initial;	       //��ʼ����־

    u32 beat_interval;     //�ն��������ͼ������λΪ��(s)
    u32 tcp_timeout;       //TCP��ϢӦ��ʱʱ�䣬��λΪ��(s)
    u32 tcp_repeat_times;  //TCP��Ϣ�ش�����
    u32 udp_timeout;       //UDP��ϢӦ��ʱʱ�䣬��λΪ��(s)
    u32 udp_repeat_times;  //UDP��Ϣ�ش�����
    u32 sms_timeout;       //SMS��ϢӦ��ʱʱ�䣬��λΪ��(s)
    u32 sms_repeat_times;  //SMS��Ϣ�ش�����


    u8 main_server_apn[LEN_PARAM_SERVER_APN];          //��������APN������ͨ�Ų��ŷ��ʵ㡣��������ʽΪCDMA����ô�ΪPPP���ź���
    u8 main_server_user[LEN_PARAM_SERVER_USER];        //������������ͨ�Ų����û���
    u8 main_server_psw[LEN_PARAM_SERVER_PSW];          //������������ͨ�Ų�������
    u8 main_server_domain[LEN_PARAM_SERVER_DOMAIN];    //����������ַ��IP������
    u8 backup_server_apn[LEN_PARAM_SERVER_APN];        //���ݷ�����APN������ͨ�Ų��ŷ��ʵ�
    u8 backup_server_user[LEN_PARAM_SERVER_USER];      //���ݷ���������ͨ�Ų����û���
    u8 backup_server_psw[LEN_PARAM_SERVER_PSW];        //���ݷ���������ͨ�Ų�������
    u8 backup_server_domain[LEN_PARAM_SERVER_DOMAIN];  //���ݷ�������ַ��IP������
    u8 server_tcp_port[LEN_PARAM_SERVER_PORT];	       //������TCP�˿�
    u8 server_udp_port[LEN_PARAM_SERVER_PORT];         //������UDP�˿�
    u8 ic_license_authentication_main_server_domain[LEN_PARAM_SERVER_DOMAIN];  //��·����֤IC����֤��������IP��ַ������
    u8 ic_license_authentication_main_server_tcp_port[LEN_PARAM_SERVER_PORT];  //��·����֤IC����֤��������TCP�˿�
    u8 ic_license_authentication_main_server_udp_port[LEN_PARAM_SERVER_PORT];  //��·����֤IC����֤��������UDP�˿�
    u8 ic_license_authentication_backup_server_domain[LEN_PARAM_SERVER_DOMAIN];//��·����֤IC����֤���ݷ�����IP��ַ���������˿�ͬ��������

    u32 location_report_strategy;         //λ�û㱨���ԣ�0����ʱ�㱨��1������㱨��2����ʱ�Ͷ���㱨
    u32 location_report_program;          //λ�û㱨������0������ACC״̬��1�����ݵ�¼״̬��ACC״̬�����жϵ�¼״̬������¼�ٸ���ACC״̬
    u32 report_driver_unlogin_interval;   //��ʻԱδ��¼�㱨ʱ��������λΪ��(s),>0

    u32 report_at_sleep_interval;         //����ʱ�㱨ʱ��������λΪ��(s),>0
    u32 report_at_alarm_interval;         //��������ʱ�㱨ʱ��������λΪ��(s),>0
    u32 default_report_interval;          //ȱʡʱ��㱨�������λΪ��(s),>0

    u32 default_report_dst_interval;      //ȱʡ����㱨�������λΪ��(m),>0
    u32 report_driver_unlogin_dst_interval;//��ʻԱδ��¼�㱨����������λΪ��(m),>0
    u32 report_at_sleep_dst_interval;     //����ʱ�㱨����������λΪ��(m),>0
    u32 report_at_alarm_dst_interval;     //��������ʱ�㱨����������λΪ��(m),>0
    u32 inflect_point_complement_biography;//�յ㲹���Ƕȣ�<180��
    u32 bound_radius;                      //����Χ���뾶���Ƿ�λ�Ʒ�ֵ������λΪ��

    u8 monitor_platform_tel[LEN_TEL];               //���ƽ̨�绰����
    u8 reset_tel[LEN_TEL];                          //��λ�绰���룬�ɲ��ô˵绰���벦���ն˵绰���ն˸�λ
    u8 restore_factory_tel[LEN_TEL];                //�ָ��������õ绰���룬�ɲ��ô˵绰���벦���ն˵绰���ն˻ָ���������
    u8 monitor_platform_sms_tel[LEN_TEL];           //���ƽ̨SMS�绰����
    u8 sms_alarm_tel[LEN_TEL];                      //�����ն�SMS�ı���������
    u32 phone_answer_strategy;                      //�ն˵绰�������ԣ�0���Զ�������1��ACC ONʱ�Զ�������OFFʱ�ֶ�����
    u32 talk_max_time;                              //ÿ���ͨ��ʱ�䣬��λΪ��(s),0Ϊ������ͨ����0xFFFFFFFFΪ������
    u32 talk_max_time_in_mon;                       //�����ͨ��ʱ�䣬��λΪ��(s),0Ϊ������ͨ����0xFFFFFFFFΪ������
    u8 monitor_tel[LEN_TEL];                        //�����绰����
    u8 monitor_platform_privilege_sms_tel[LEN_TEL]; //���ƽ̨��Ȩ���ź���

    u32 alarm_mask;          //���������֡���λ����Ϣ�㱨��Ϣ�еı�����־���Ӧ����ӦλΪ1����Ӧ����������
    u32 alarm_sms_switch;    //���������ı�SMS���أ���λ����Ϣ�㱨��Ϣ�еı�����־���Ӧ����ӦλΪ1����Ӧ����ʱ�����ı�SMS
    u32 alarm_shoot_switch;  //�������㿪�أ���λ����Ϣ�㱨��Ϣ�еı�����־���Ӧ����ӦλΪ1����Ӧ����ʱ����ͷ����
    u32 alarm_store_flag;    //��������洢��־����λ����Ϣ�㱨��Ϣ�еı�����־���Ӧ����ӦλΪ1�����Ӧ����ʱ�ĵ���Ƭ���д洢������ʵʱ�ϴ�
    u32 key_flag;            //�ؼ���־����λ����Ϣ�㱨��Ϣ�еı�����־���Ӧ����ӦλΪ1�����Ӧ����Ϊ�ؼ�����
    u32 top_speed;           //����ٶȣ���λΪ����ÿСʱ(km/h)
    u32 speeding_duration;   //���ٳ���ʱ�䣬��λΪ��(s)
    u32 continouous_drive_time;//������ʻʱ�����ޣ���λΪ��(s)
    u32 drive_time_in_day;   //�����ۼƼ�ʻʱ�����ޣ���λΪ��(s)
    u32 rest_min_time;       //��С��Ϣʱ�䣬��λΪ��(s)
    u32 stop_max_time;       //�ͣ��ʱ�䣬��λΪ��(s)
    u32 overspeed_alarm_diff;//���ٱ���Ԥ����ֵ����λΪ0.1km/h
    u32 fatigue_driving_diff;//ƣ�ͼ�ʻԤ����ֵ����λΪ��(s),>0
    u32 collision_alarm_param;//��ײ�����������ã�b7-b0:��ײʱ�䣬��λ4ms��b15-b8:��ײ���ٶȣ���λ0.1g�����÷�Χ��0-79֮�䣬Ĭ��Ϊ10��
    u32 rollover_alarm_param;//�෭�����������ã��෭�Ƕȣ���λΪ�ȣ�Ĭ��Ϊ30�ȣ�

    u32 tim_photo;           //��ʱ���տ���
    u32 dis_photo;           //�������տ���


    u32 multimedia_quality;  //ͼ��(��Ƶ)������1��10,1���
    u32 brightness;          //���ȣ�0��255
    u32 contrast;            //�Աȶȣ�0��127
    u32 saturation;          //���Ͷȣ�0��127
    u32 chroma;              //ɫ�ȣ�0��255

    u32 mileage;                            //������̱������1/10km
    u8 province_id[LEN_PARAM_PROVINCE_ID];	//�������ڵ�ʡ��ID��
    u8 city_id[LEN_PARAM_CITY_ID];	        //�������ڵ�����ID��
    u8 lic_plate[LEN_PARAM_LICENSE_PLATE];  //������ͨ�����Ű䷢�Ļ���������
    u32 lic_plate_color;                    //������ɫ������JT/T415-2006��5.4.12

    u32 gnss_position_type;   //GNSS��λģʽ��  bit0��0������GPS��1������GPS��      bit1��0�����ñ�����1�����ñ�����    bit2��0������GLONASS��1������GLONASS��      bit3��0������Galileo��1������Galileo��
    u32 gnss_baudrate;        //GNSS�����ʣ�    0x00:4800;  0x01:9600;  0x02:19200; 0x03:38400; 0x04:47600; 0x05:115200; 
    u32 gnss_output_rate;     //GNSSģ����ϸ��λ�������Ƶ�ʣ� 0x00:500ms;  0x01:1000ms(Ĭ��);  0x02:2000ms;    0x03:3000ms;    0x04:4000ms;
    u32 gnss_samp_frequency;  //GNSSģ����ϸ��λ���ݲɼ�Ƶ�ʣ���λΪ�룬Ĭ��1��
    u32 gnss_upload_type;     //GNSSģ����ϸ��λ�����ϴ���ʽ��0x00:���ش洢�����ϴ���Ĭ��ֵ����  0x01����ʱ�����ϴ���    0x02:���������ϴ��� 0x0b�����ۻ�ʱ���ϴ����ﵽ����ʱ����Զ�ֹͣ�ϴ���  0x0c�����ۻ������ϴ����ﵽ������Զ�ֹͣ�ϴ���  0x0d�����ۻ������ϴ����ﵽ�ϴ��������Զ�ֹͣ�ϴ���
    u32 gnss_upload_set;      //GNSSģ����ϸ��λ�����ϴ����ã�0x01:��λΪ�룻0x02����λΪ�ף�0x0b:��λΪ�룻0x0c����λΪ�ף�0x0d����λΪ��
    
    u32 can_1_samp_time_interval;   //can����ͨ��1�ɼ�ʱ������ms����0��ʾ���ɼ�
    u32 can_1_upload_time_interval; //can����ͨ��1�ϴ�ʱ������s����0��ʾ���ϴ�
    u32 can_2_samp_time_interval;   //can����ͨ��2�ɼ�ʱ������ms����0��ʾ���ɼ�
    u32 can_2_upload_time_interval; //can����ͨ��2�ϴ�ʱ������s����0��ʾ���ϴ�
    u8 can_ID_samp_set[CAN_ID_TOTAL][8];          //can����ID�����ɼ����ã�bit63-bit32����ʾ��ID�ɼ�ʱ������ms����0��ʾ���ɼ���bit31��canIDͨ���ţ�0-can1��1-can2��bit30��֡���ͣ�0-��׼֡��1-��չ֡��bit29�����ݲɼ���ʽ��0-ԭʼ���ݿ���1-�ɼ�����ļ���ֵ��bit28-bit0��can����ID��

    /*����Ϊ�������Ӳ���*/
    u8 terminalID[12];             //�ն�ID
    u8 local_tel[LEN_TEL];         //�ն�SIM����
    u32 wheel_coefficient;         //����ϵ�������ڼ������
    u32 fuel_tank_capacity;
    u8 authorization_no[32];       //��Ȩ��

	int lock_car;				   //���� 

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


