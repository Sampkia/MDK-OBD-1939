#include "include.h"

#define JTT808_PARAM_ADDRESS    0x0803FC00            //��246ҳ����ʼ��ַ



JTT808t_TerminalParamFlash JTT808_Terminal_Param={0};


void JTT808_rereg(void)
{
	memset((char *)JTT808_Terminal_Param.authorization_no,0x00,sizeof(JTT808_Terminal_Param.authorization_no));
	strcpy((char *)JTT808_Terminal_Param.authorization_no,"00000000000000000000000000000000");
	jtt808_save_para();
	JTT808_Flag_Authentication=0;
}

/**********************************************************
*                    �������á���ȡ
* ������ID : ����ID
*     size ����������
*       buf������������
*      prio�����á���ȡ״̬     0����ȡ   1������
*************************************************************/

signed char JTT808_Param_exec(u32 ID, u32* size,u8* buf,u8 prio)
{
    switch(ID)
    {     												
        case JTT808_TERMINAL_PARAM_BEAT:               //0x0001//�ն��������
            if(prio)JTT808_Terminal_Param.beat_interval=ntohl(*(u32*)buf);
            else
            {
                u32 tmp=htonl(JTT808_Terminal_Param.beat_interval);
                u32 id=htonl(ID);
                memcpy(buf,(char*)&id,4);
                memset(buf+4,4,1);
                memcpy(buf+5,(char*)&tmp,4);
                *size+=9;
            }
        break;
        case JTT808_TERMINAL_PARAM_TCP_TIMEOUT:        // 0x0002//TCPӦ��ʱʱ��
            if(prio)JTT808_Terminal_Param.tcp_timeout=ntohl(*(u32*)buf);
            else
            {
                u32 tmp=htonl(JTT808_Terminal_Param.tcp_timeout);
                u32 id=htonl(ID);
                memcpy(buf,(char*)&id,4);
                memset(buf+4,4,1);
                memcpy(buf+5,(char*)&tmp,4);
                *size+=9;
            }
        break;
        case JTT808_TERMINAL_PARAM_TCP_REPEAT_TIMES:   // 0x0003//TCP�ش�����
            if(prio)JTT808_Terminal_Param.tcp_repeat_times=ntohl(*(u32*)buf);
            else
            {
                u32 tmp=htonl(JTT808_Terminal_Param.tcp_repeat_times);
                u32 id=htonl(ID);
                memcpy(buf,(char*)&id,4);
                memset(buf+4,4,1);
                memcpy(buf+5,(char*)&tmp,4);
                *size+=9;
            }
        break;
        case JTT808_TERMINAL_PARAM_UDP_TIMEOUT:        // 0x0004//UDPӦ��ʱʱ��
            if(prio)JTT808_Terminal_Param.udp_timeout=ntohl(*(u32*)buf);
            else
            {
                u32 tmp=htonl(JTT808_Terminal_Param.udp_timeout);
                u32 id=htonl(ID);
                memcpy(buf,(char*)&id,4);
                memset(buf+4,4,1);
                memcpy(buf+5,(char*)&tmp,4);
                *size+=9;
            }
        break;
        case JTT808_TERMINAL_PARAM_UDP_REPEAT_TIMES:   // 0x0005//UDP�ش�����
            if(prio)JTT808_Terminal_Param.udp_repeat_times=ntohl(*(u32*)buf);
            else
            {
                u32 tmp=htonl(JTT808_Terminal_Param.udp_repeat_times);
                u32 id=htonl(ID);
                memcpy(buf,(char*)&id,4);
                memset(buf+4,4,1);
                memcpy(buf+5,(char*)&tmp,4);
                *size+=9;
            }
        break;
        case JTT808_TERMINAL_PARAM_SMS_TIMEOUT:        // 0x0006//SMSӦ��ʱʱ��
            if(prio)JTT808_Terminal_Param.sms_timeout=ntohl(*(u32*)buf);
            else
            {
                u32 tmp=htonl(JTT808_Terminal_Param.sms_timeout);
                u32 id=htonl(ID);
                memcpy(buf,(char*)&id,4);
                memset(buf+4,4,1);
                memcpy(buf+5,(char*)&tmp,4);
                *size+=9;
            }
        break;
        case JTT808_TERMINAL_PARAM_SMS_REPEAT_TIMES:   // 0x0007//SMS�ش�����
            if(prio)JTT808_Terminal_Param.sms_repeat_times=ntohl(*(u32*)buf);
            else
            {
                u32 tmp=htonl(JTT808_Terminal_Param.sms_repeat_times);
                u32 id=htonl(ID);
                memcpy(buf,(char*)&id,4);
                memset(buf+4,4,1);
                memcpy(buf+5,(char*)&tmp,4);
                *size+=9;
            }
        break;



 

        case JTT808_TERMINAL_PARAM_MAIN_SERVER_APN:    // 0x0010//��������APN
            if(prio)
            {
                memset(JTT808_Terminal_Param.main_server_apn,0x00,LEN_PARAM_SERVER_APN);
                memcpy(JTT808_Terminal_Param.main_server_apn,buf,*size);
				API_vpdn_set((char*)JTT808_Terminal_Param.main_server_apn,(char*)JTT808_Terminal_Param.main_server_user,(char*)JTT808_Terminal_Param.main_server_psw);		 //����apn����
            }
            else
            {
                u32 id=htonl(ID);
                memcpy(buf,(char*)&id,4);

                memset(buf+4,strlen((const char*)JTT808_Terminal_Param.main_server_apn),1);

                memcpy(buf+5,JTT808_Terminal_Param.main_server_apn,strlen((const char*)JTT808_Terminal_Param.main_server_apn));
                *size+=(5+strlen((const char*)JTT808_Terminal_Param.main_server_apn));
            }
        break;
        case JTT808_TERMINAL_PARAM_MAIN_SERVER_USER:   // 0x0011//�������������û���
            if(prio)
            {
                memset(JTT808_Terminal_Param.main_server_user,0x00,LEN_PARAM_SERVER_USER);
                memcpy(JTT808_Terminal_Param.main_server_user,buf,*size);
				API_vpdn_set((char*)JTT808_Terminal_Param.main_server_apn,(char*)JTT808_Terminal_Param.main_server_user,(char*)JTT808_Terminal_Param.main_server_psw);		 //����apn����
            }
            else
            {
                u32 id=htonl(ID);
                memcpy(buf,(char*)&id,4);

                memset(buf+4,strlen((const char*)JTT808_Terminal_Param.main_server_user),1);

                memcpy(buf+5,JTT808_Terminal_Param.main_server_user,strlen((const char*)JTT808_Terminal_Param.main_server_user));
                *size+=(5+strlen((const char*)JTT808_Terminal_Param.main_server_user));
            }
        break;
        case JTT808_TERMINAL_PARAM_MAIN_SERVER_PSW:    // 0x0012//����������������
            if(prio)
            {
                memset(JTT808_Terminal_Param.main_server_psw,0x00,LEN_PARAM_SERVER_PSW);
                memcpy(JTT808_Terminal_Param.main_server_psw,buf,*size);
				API_vpdn_set((char*)JTT808_Terminal_Param.main_server_apn,(char*)JTT808_Terminal_Param.main_server_user,(char*)JTT808_Terminal_Param.main_server_psw);		 //����apn����
            }
            else
            {
                u32 id=htonl(ID);
                memcpy(buf,(char*)&id,4);

                memset(buf+4,strlen((const char*)JTT808_Terminal_Param.main_server_psw),1);

                memcpy(buf+5,JTT808_Terminal_Param.main_server_psw,strlen((const char*)JTT808_Terminal_Param.main_server_psw));
                *size+=(5+strlen((const char*)JTT808_Terminal_Param.main_server_psw));
            }
        break;
        case JTT808_TERMINAL_PARAM_MAIN_SERVER_DOMAIN: // 0x0013//��������������IP
            if(prio)
            {
                memset(JTT808_Terminal_Param.main_server_domain,0x00,LEN_PARAM_SERVER_DOMAIN);
                memcpy(JTT808_Terminal_Param.main_server_domain,buf,*size);
				JTT808_rereg();
				API_server_ip_port_set(0,JTT808_Terminal_Param.main_server_domain,JTT808_Terminal_Param.server_tcp_port,0);	
            }
            else
            {
                u32 id=htonl(ID);
                memcpy(buf,(char*)&id,4);

                memset(buf+4,strlen((const char*)JTT808_Terminal_Param.main_server_domain),1);

                memcpy(buf+5,JTT808_Terminal_Param.main_server_domain,strlen((const char*)JTT808_Terminal_Param.main_server_domain));
                *size+=(5+strlen((const char*)JTT808_Terminal_Param.main_server_domain));
            }
        break;
        case JTT808_TERMINAL_PARAM_BACKUP_SERVER_APN:  // 0x0014//���ݷ�����APN
            if(prio)
            {
                memset(JTT808_Terminal_Param.backup_server_apn,0x00,LEN_PARAM_SERVER_APN);
                memcpy(JTT808_Terminal_Param.backup_server_apn,buf,*size);
            }
            else
            {
                u32 id=htonl(ID);
                memcpy(buf,(char*)&id,4);

                memset(buf+4,strlen((const char*)JTT808_Terminal_Param.backup_server_apn),1);

                memcpy(buf+5,JTT808_Terminal_Param.backup_server_apn,strlen((const char*)JTT808_Terminal_Param.backup_server_apn));
                *size+=(5+strlen((const char*)JTT808_Terminal_Param.backup_server_apn));
            }
        break;
        case JTT808_TERMINAL_PARAM_BACKUP_SERVER_USER: // 0x0015//���ݷ����������û���
            if(prio)
            {
                memset(JTT808_Terminal_Param.backup_server_user,0x00,LEN_PARAM_SERVER_USER);
                memcpy(JTT808_Terminal_Param.backup_server_user,buf,*size);
            }
            else
            {
                u32 id=htonl(ID);
                memcpy(buf,(char*)&id,4);

                memset(buf+4,strlen((const char*)JTT808_Terminal_Param.backup_server_user),1);

                memcpy(buf+5,JTT808_Terminal_Param.backup_server_user,strlen((const char*)JTT808_Terminal_Param.backup_server_user));
                *size+=(5+strlen((const char*)JTT808_Terminal_Param.backup_server_user));
            }
        break;
        case JTT808_TERMINAL_PARAM_BACKUP_SERVER_PSW:  // 0x0016//���ݷ�������������
            if(prio)
            {
                memset(JTT808_Terminal_Param.backup_server_psw,0x00,LEN_PARAM_SERVER_PSW);
                memcpy(JTT808_Terminal_Param.backup_server_psw,buf,*size);
            }
            else
            {
                u32 id=htonl(ID);
                memcpy(buf,(char*)&id,4);

                memset(buf+4,strlen((const char*)JTT808_Terminal_Param.backup_server_psw),1);

                memcpy(buf+5,JTT808_Terminal_Param.backup_server_psw,strlen((const char*)JTT808_Terminal_Param.backup_server_psw));
                *size+=(5+strlen((const char*)JTT808_Terminal_Param.backup_server_psw));
            }
        break;
        case JTT808_TERMINAL_PARAM_BACKUP_SERVER_DOMAIN:// 0x0017//���ݷ�����������IP
            if(prio)
            {
                memset(JTT808_Terminal_Param.backup_server_domain,0x00,LEN_PARAM_SERVER_DOMAIN);
                memcpy(JTT808_Terminal_Param.backup_server_domain,buf,*size);
            }
            else
            {
                u32 id=htonl(ID);
                memcpy(buf,(char*)&id,4);

                memset(buf+4,strlen((const char*)JTT808_Terminal_Param.backup_server_domain),1);

                memcpy(buf+5,JTT808_Terminal_Param.backup_server_domain,strlen((const char*)JTT808_Terminal_Param.backup_server_domain));
                *size+=(5+strlen((const char*)JTT808_Terminal_Param.backup_server_domain));
            }
        break;
        case JTT808_TERMINAL_PARAM_SERVER_TCP_PORT:    // 0x0018//TCP�˿�
            if(prio)
            {
                 char tmp[40]={0};
                 itoa(ntohl(*(u32*)buf),tmp, 10);
                 memcpy(JTT808_Terminal_Param.server_tcp_port,tmp,LEN_PARAM_SERVER_PORT);
				 JTT808_rereg();
				API_server_ip_port_set(0,JTT808_Terminal_Param.main_server_domain,JTT808_Terminal_Param.server_tcp_port,0);	
            }
            else
            {
                u32 id=htonl(ID);
                u32 port;
                memcpy(buf,(char*)&id,4);

                memset(buf+4,4,1);

                port=htonl(atoi((const char*)JTT808_Terminal_Param.server_tcp_port));
                memcpy(buf+5,(char*)&port,4);
                *size+=9;
            }
        break;
        case JTT808_TERMINAL_PARAM_SERVER_UDP_PORT:    // 0x0019//UDP�˿�
            if(prio)
            {
                 char tmp[40]={0};
                 itoa(ntohl(*(u32*)buf),tmp, 10);
                 memcpy(JTT808_Terminal_Param.server_udp_port,tmp,LEN_PARAM_SERVER_PORT);
            }
            else
            {
                u32 id=htonl(ID);
                u32 port;
                memcpy(buf,(char*)&id,4);

                memset(buf+4,4,1);

                port=htonl(atoi((const char*)JTT808_Terminal_Param.server_udp_port));
                memcpy(buf+5,(char*)&port,4);
                *size+=9;
            }
        break;
        case JTT808_TERMINAL_PARAM_IC_AUTHENTICATION_MAIN_SERVER_DOMAIN:  // 0x001A//��·����֤IC����֤��������APN
            if(prio)
            {
                memset(JTT808_Terminal_Param.ic_license_authentication_main_server_domain,0x00,LEN_PARAM_SERVER_DOMAIN);
                memcpy(JTT808_Terminal_Param.ic_license_authentication_main_server_domain,buf,*size);
            }
            else
            {
                u32 id=htonl(ID);
                memcpy(buf,(char*)&id,4);

                memset(buf+4,strlen((const char*)JTT808_Terminal_Param.ic_license_authentication_main_server_domain),1);

                memcpy(buf+5,JTT808_Terminal_Param.ic_license_authentication_main_server_domain,strlen((const char*)JTT808_Terminal_Param.ic_license_authentication_main_server_domain));
                *size+=(5+strlen((const char*)JTT808_Terminal_Param.ic_license_authentication_main_server_domain));
            }
        break;
        case JTT808_TERMINAL_PARAM_IC_AUTHENTICATION_TCP_PORT:            // 0x001B//��·����֤IC����֤��������TCP�˿�
            if(prio)
            {
                 char tmp[40]={0};
                 itoa(ntohl(*(u32*)buf),tmp, 10);
                 memcpy(JTT808_Terminal_Param.ic_license_authentication_main_server_tcp_port,tmp,LEN_PARAM_SERVER_PORT);
            }
            else
            {
                u32 id=htonl(ID);
                u32 port;
                memcpy(buf,(char*)&id,4);

                memset(buf+4,4,1);

                port=htonl(atoi((const char*)JTT808_Terminal_Param.ic_license_authentication_main_server_tcp_port));
                memcpy(buf+5,(char*)&port,4);
                *size+=9;
            }
        break;
        case JTT808_TERMINAL_PARAM_IC_AUTHENTICATION_UDP_PORT:            // 0x001C//��·����֤IC����֤��������UDP�˿�
            if(prio)
            {
                 char tmp[40]={0};
                 itoa(ntohl(*(u32*)buf),tmp, 10);
                 memcpy(JTT808_Terminal_Param.ic_license_authentication_main_server_udp_port,tmp,LEN_PARAM_SERVER_PORT);
            }
            else
            {
                u32 id=htonl(ID);
                u32 port;
                memcpy(buf,(char*)&id,4);

                memset(buf+4,4,1);

                port=htonl(atoi((const char*)JTT808_Terminal_Param.ic_license_authentication_main_server_udp_port));
                memcpy(buf+5,(char*)&port,4);
                *size+=9;
            }
        break;
        case JTT808_TERMINAL_PARAM_IC_AUTHENTICATION_BACKUP_SERVER_DOMAIN:// 0x001D//��·����֤IC����֤���ݷ�����APN
            if(prio)
            {
                memset(JTT808_Terminal_Param.ic_license_authentication_backup_server_domain,0x00,LEN_PARAM_SERVER_DOMAIN);
                memcpy(JTT808_Terminal_Param.ic_license_authentication_backup_server_domain,buf,*size);
            }
            else
            {
                u32 id=htonl(ID);
                memcpy(buf,(char*)&id,4);

                memset(buf+4,strlen((const char*)JTT808_Terminal_Param.ic_license_authentication_backup_server_domain),1);

                memcpy(buf+5,JTT808_Terminal_Param.ic_license_authentication_backup_server_domain,strlen((const char*)JTT808_Terminal_Param.ic_license_authentication_backup_server_domain));
                *size+=(5+strlen((const char*)JTT808_Terminal_Param.ic_license_authentication_backup_server_domain));
            }
        break;

			
            
            				    

        case JTT808_TERMINAL_PARAM_LOCATION_REPORT_STRATEGY:       // 0x0020,//λ�û㱨����
            if(prio)JTT808_Terminal_Param.location_report_strategy=ntohl(*(u32*)buf);
            else
            {
                u32 tmp=htonl(JTT808_Terminal_Param.location_report_strategy);
                u32 id=htonl(ID);
                memcpy(buf,(char*)&id,4);
                memset(buf+4,4,1);
                memcpy(buf+5,(char*)&tmp,4);
                *size+=9;
            }
        break;
        case JTT808_TERMINAL_PARAM_LOCATION_REPORT_PROGRAM:        // 0x0021//λ�û㱨����
            if(prio)JTT808_Terminal_Param.location_report_program=ntohl(*(u32*)buf);
            else
            {
                u32 tmp=htonl(JTT808_Terminal_Param.location_report_program);
                u32 id=htonl(ID);
                memcpy(buf,(char*)&id,4);
                memset(buf+4,4,1);
                memcpy(buf+5,(char*)&tmp,4);
                *size+=9;
            }
        break;
        case JTT808_TERMINAL_PARAM_REPORT_DRIVER_UNLOGIN_INTERVAL: // 0x0022//��ʻԱδ��¼�㱨ʱ����
            if(prio)JTT808_Terminal_Param.report_driver_unlogin_interval=ntohl(*(u32*)buf);
            else
            {
                u32 tmp=htonl(JTT808_Terminal_Param.report_driver_unlogin_interval);
                u32 id=htonl(ID);
                memcpy(buf,(char*)&id,4);
                memset(buf+4,4,1);
                memcpy(buf+5,(char*)&tmp,4);
                *size+=9;
            }
        break;


        case JTT808_TERMINAL_PARAM_REPORT_AT_SLEEP_INTERVAL:       // 0x0027,//���߻㱨ʱ����
            if(prio)JTT808_Terminal_Param.report_at_sleep_interval=ntohl(*(u32*)buf);
            else
            {
                u32 tmp=htonl(JTT808_Terminal_Param.report_at_sleep_interval);
                u32 id=htonl(ID);
                memcpy(buf,(char*)&id,4);
                memset(buf+4,4,1);
                memcpy(buf+5,(char*)&tmp,4);
                *size+=9;
            }
        break;
        case JTT808_TERMINAL_PARAM_REPORT_AT_ALARM_INTERVAL:       // 0x0028//���������㱨ʱ����
            if(prio)JTT808_Terminal_Param.report_at_alarm_interval=ntohl(*(u32*)buf);
            else
            {
                u32 tmp=htonl(JTT808_Terminal_Param.report_at_alarm_interval);
                u32 id=htonl(ID);
                memcpy(buf,(char*)&id,4);
                memset(buf+4,4,1);
                memcpy(buf+5,(char*)&tmp,4);
                *size+=9;
            }
        break;
        case JTT808_TERMINAL_PARAM_DEFAULT_REPORT_INTERVAL:        // 0x0029//ȱʡʱ��㱨���
            if(prio)JTT808_Terminal_Param.default_report_interval=ntohl(*(u32*)buf);
            else
            {
                u32 tmp=htonl(JTT808_Terminal_Param.default_report_interval);
                u32 id=htonl(ID);
                memcpy(buf,(char*)&id,4);
                memset(buf+4,4,1);
                memcpy(buf+5,(char*)&tmp,4);
                *size+=9;
            }
        break;


        case JTT808_TERMINAL_PARAM_DEFAULT_REPORT_DST_INTERVAL:        	// 0x002C,//ȱʡ����㱨���
            if(prio)JTT808_Terminal_Param.default_report_dst_interval=ntohl(*(u32*)buf);
            else
            {
                u32 tmp=htonl(JTT808_Terminal_Param.default_report_dst_interval);
                u32 id=htonl(ID);
                memcpy(buf,(char*)&id,4);
                memset(buf+4,4,1);
                memcpy(buf+5,(char*)&tmp,4);
                *size+=9;
            }
        break;
        case JTT808_TERMINAL_PARAM_REPORT_DRIVER_UNLOGIN_DST_INTERVAL:  // 0x002D//��ʻԱδ��¼�㱨������
            if(prio)JTT808_Terminal_Param.report_driver_unlogin_dst_interval=ntohl(*(u32*)buf);
            else
            {
                u32 tmp=htonl(JTT808_Terminal_Param.report_driver_unlogin_dst_interval);
                u32 id=htonl(ID);
                memcpy(buf,(char*)&id,4);
                memset(buf+4,4,1);
                memcpy(buf+5,(char*)&tmp,4);
                *size+=9;
            }
        break;
        case JTT808_TERMINAL_PARAM_REPORT_AT_SLEEP_DST_INTERVAL:        // 0x002E//���߻㱨������
            if(prio)JTT808_Terminal_Param.report_at_sleep_dst_interval=ntohl(*(u32*)buf);
            else
            {
                u32 tmp=htonl(JTT808_Terminal_Param.report_at_sleep_dst_interval);
                u32 id=htonl(ID);
                memcpy(buf,(char*)&id,4);
                memset(buf+4,4,1);
                memcpy(buf+5,(char*)&tmp,4);
                *size+=9;
            }
        break;
        case JTT808_TERMINAL_PARAM_REPORT_AT_ALARM_DST_INTERVAL:        // 0x002F//���������㱨������
            if(prio)JTT808_Terminal_Param.report_at_alarm_dst_interval=ntohl(*(u32*)buf);
            else
            {
                u32 tmp=htonl(JTT808_Terminal_Param.report_at_alarm_dst_interval);
                u32 id=htonl(ID);
                memcpy(buf,(char*)&id,4);
                memset(buf+4,4,1);
                memcpy(buf+5,(char*)&tmp,4);
                *size+=9;
            }
        break;
        case JTT808_TERMINAL_PARAM_INFLECT_POINT_COMPLEMENT_BIOGRAPHY:  // 0x0030//�յ㲹���Ƕ�
            if(prio)JTT808_Terminal_Param.inflect_point_complement_biography=ntohl(*(u32*)buf);
            else
            {
                u32 tmp=htonl(JTT808_Terminal_Param.inflect_point_complement_biography);
                u32 id=htonl(ID);
                memcpy(buf,(char*)&id,4);
                memset(buf+4,4,1);
                memcpy(buf+5,(char*)&tmp,4);
                *size+=9;
            }
        break;
        case JTT808_TERMINAL_PARAM_BOUND_RADIUS:                        //0x0031//����Χ���뾶
            if(prio)JTT808_Terminal_Param.bound_radius=ntohs(*(u16*)buf);
            else
            {
                u32 tmp=htonl(JTT808_Terminal_Param.bound_radius);
                u32 id=htonl(ID);
                memcpy(buf,(char*)&id,4);
                memset(buf+4,4,1);
                memcpy(buf+5,(char*)&tmp,4);
                *size+=9;
            }
        break;

	    

        case JTT808_TERMINAL_PARAM_MONITOR_PLATFORM_TEL:      // 0x0040,//���ƽ̨�绰����
            if(prio)
            {
                memset(JTT808_Terminal_Param.monitor_platform_tel,0x00,LEN_TEL);
                memcpy(JTT808_Terminal_Param.monitor_platform_tel,buf,*size);
            }
            else
            {
                u32 id=htonl(ID);
                memcpy(buf,(char*)&id,4);

                memset(buf+4,strlen((const char*)JTT808_Terminal_Param.monitor_platform_tel),1);

                memcpy(buf+5,JTT808_Terminal_Param.monitor_platform_tel,strlen((const char*)JTT808_Terminal_Param.monitor_platform_tel));
                *size+=(5+strlen((const char*)JTT808_Terminal_Param.monitor_platform_tel));
            }    
        break;
        case JTT808_TERMINAL_PARAM_RESET_TEL:                 // 0x0041//��λ�绰����
            if(prio)
            {
                memset(JTT808_Terminal_Param.reset_tel,0x00,LEN_TEL);
                memcpy(JTT808_Terminal_Param.reset_tel,buf,*size);
            }
            else
            {
                u32 id=htonl(ID);
                memcpy(buf,(char*)&id,4);

                memset(buf+4,strlen((const char*)JTT808_Terminal_Param.reset_tel),1);

                memcpy(buf+5,JTT808_Terminal_Param.reset_tel,strlen((const char*)JTT808_Terminal_Param.reset_tel));
                *size+=(5+strlen((const char*)JTT808_Terminal_Param.reset_tel));
            }
        break;
        case JTT808_TERMINAL_PARAM_RESTORE_FACTORY_TEL:       // 0x0042//�ָ��������õ绰����
            if(prio)
            {
                memset(JTT808_Terminal_Param.restore_factory_tel,0x00,LEN_TEL);
                memcpy(JTT808_Terminal_Param.restore_factory_tel,buf,*size);
            }
            else
            {
                u32 id=htonl(ID);
                memcpy(buf,(char*)&id,4);

                memset(buf+4,strlen((const char*)JTT808_Terminal_Param.restore_factory_tel),1);

                memcpy(buf+5,JTT808_Terminal_Param.restore_factory_tel,strlen((const char*)JTT808_Terminal_Param.restore_factory_tel));
                *size+=(5+strlen((const char*)JTT808_Terminal_Param.restore_factory_tel));
            }
        break;
        case JTT808_TERMINAL_PARAM_MONITOR_PLATFORM_SMS_TEL:  // 0x0043//���ƽ̨SMS�绰����
            if(prio)
            {
                memset(JTT808_Terminal_Param.monitor_platform_sms_tel,0x00,LEN_TEL);
                memcpy(JTT808_Terminal_Param.monitor_platform_sms_tel,buf,*size);
            }
            else
            {
                u32 id=htonl(ID);
                memcpy(buf,(char*)&id,4);

                memset(buf+4,strlen((const char*)JTT808_Terminal_Param.monitor_platform_sms_tel),1);

                memcpy(buf+5,JTT808_Terminal_Param.monitor_platform_sms_tel,strlen((const char*)JTT808_Terminal_Param.monitor_platform_sms_tel));
                *size+=(5+strlen((const char*)JTT808_Terminal_Param.monitor_platform_sms_tel));
            }
        break;
        case JTT808_TERMINAL_PARAM_SMS_ALARM_TEL:             // 0x0044//�����ն�SMS�ı���������
            if(prio)
            {
                memset(JTT808_Terminal_Param.sms_alarm_tel,0x00,LEN_TEL);
                memcpy(JTT808_Terminal_Param.sms_alarm_tel,buf,*size);
            }
            else
            {
                u32 id=htonl(ID);
                memcpy(buf,(char*)&id,4);

                memset(buf+4,strlen((const char*)JTT808_Terminal_Param.sms_alarm_tel),1);

                memcpy(buf+5,JTT808_Terminal_Param.sms_alarm_tel,strlen((const char*)JTT808_Terminal_Param.sms_alarm_tel));
                *size+=(5+strlen((const char*)JTT808_Terminal_Param.sms_alarm_tel));
            }
        break;
        case JTT808_TERMINAL_PARAM_PHONE_ANSWER_STRATEGY:     // 0x0045//�ն˵绰��������
            if(prio)JTT808_Terminal_Param.phone_answer_strategy=ntohl(*(u32*)buf);
            else
            {
                u32 tmp=htonl(JTT808_Terminal_Param.phone_answer_strategy);
                u32 id=htonl(ID);
                memcpy(buf,(char*)&id,4);
                memset(buf+4,4,1);
                memcpy(buf+5,(char*)&tmp,4);
                *size+=9;
            }
        break;
        case JTT808_TERMINAL_PARAM_TALK_MAX_TIME:             // 0x0046//ÿ���ͨ��ʱ��
            if(prio)JTT808_Terminal_Param.talk_max_time=ntohl(*(u32*)buf);
            else
            {
                u32 tmp=htonl(JTT808_Terminal_Param.talk_max_time);
                u32 id=htonl(ID);
                memcpy(buf,(char*)&id,4);
                memset(buf+4,4,1);
                memcpy(buf+5,(char*)&tmp,4);
                *size+=9;
            }
        break;
        case JTT808_TERMINAL_PARAM_TALK_MAX_TIME_IN_MON:      // 0x0047//ÿ���ͨ��ʱ��
            if(prio)JTT808_Terminal_Param.talk_max_time_in_mon=ntohl(*(u32*)buf);
            else
            {
                u32 tmp=htonl(JTT808_Terminal_Param.talk_max_time_in_mon);
                u32 id=htonl(ID);
                memcpy(buf,(char*)&id,4);
                memset(buf+4,4,1);
                memcpy(buf+5,(char*)&tmp,4);
                *size+=9;
            }
        break;
        case JTT808_TERMINAL_PARAM_MONITOR_TEL:               // 0x0048//�����绰����
            if(prio)
            {
                memset(JTT808_Terminal_Param.monitor_tel,0x00,LEN_TEL);
                memcpy(JTT808_Terminal_Param.monitor_tel,buf,*size);
            }
            else
            {
                u32 id=htonl(ID);
                memcpy(buf,(char*)&id,4);

                memset(buf+4,strlen((const char*)JTT808_Terminal_Param.monitor_tel),1);

                memcpy(buf+5,JTT808_Terminal_Param.monitor_tel,strlen((const char*)JTT808_Terminal_Param.monitor_tel));
                *size+=(5+strlen((const char*)JTT808_Terminal_Param.monitor_tel));
            }
        break;
        case JTT808_TERMINAL_PARAM_MONITOR_PLATFORM_PRIVILEGE_SMS_TEL:  // 0x0049//���ƽ̨��Ȩ���ź���
            if(prio)
            {
                memset(JTT808_Terminal_Param.monitor_platform_privilege_sms_tel,0x00,LEN_TEL);
                memcpy(JTT808_Terminal_Param.monitor_platform_privilege_sms_tel,buf,*size);
            }
            else
            {
                u32 id=htonl(ID);
                memcpy(buf,(char*)&id,4);

                memset(buf+4,strlen((const char*)JTT808_Terminal_Param.monitor_platform_privilege_sms_tel),1);

                memcpy(buf+5,JTT808_Terminal_Param.monitor_platform_privilege_sms_tel,strlen((const char*)JTT808_Terminal_Param.monitor_platform_privilege_sms_tel));
                *size+=(5+strlen((const char*)JTT808_Terminal_Param.monitor_platform_privilege_sms_tel));
            }
        break;

                     

        case JTT808_TERMINAL_PARAM_ALARM_MASK:            // 0x0050,//����������
            if(prio)JTT808_Terminal_Param.alarm_mask=ntohl(*(u32*)buf);
            else
            {
                u32 tmp=htonl(JTT808_Terminal_Param.alarm_mask);
                u32 id=htonl(ID);
                memcpy(buf,(char*)&id,4);
                memset(buf+4,4,1);
                memcpy(buf+5,(char*)&tmp,4);
                *size+=9;
            }
        break;
        case JTT808_TERMINAL_PARAM_ALARM_SMS_SWITCH:      // 0x0051//���������ı�SMS����
            if(prio)JTT808_Terminal_Param.alarm_sms_switch=ntohl(*(u32*)buf);
            else
            {
                u32 tmp=htonl(JTT808_Terminal_Param.alarm_sms_switch);
                u32 id=htonl(ID);
                memcpy(buf,(char*)&id,4);
                memset(buf+4,4,1);
                memcpy(buf+5,(char*)&tmp,4);
                *size+=9;
            }
        break;
        case JTT808_TERMINAL_PARAM_ALARM_SHOOT_SWITCH:    // 0x0052//�������㿪��
            if(prio)JTT808_Terminal_Param.alarm_shoot_switch=ntohl(*(u32*)buf);
            else
            {
                u32 tmp=htonl(JTT808_Terminal_Param.alarm_shoot_switch);
                u32 id=htonl(ID);
                memcpy(buf,(char*)&id,4);
                memset(buf+4,4,1);
                memcpy(buf+5,(char*)&tmp,4);
                *size+=9;
            }
        break;
        case JTT808_TERMINAL_PARAM_ALARM_STORE_FLAG:      // 0x0053//�������㴢���־
            if(prio)JTT808_Terminal_Param.alarm_store_flag=ntohl(*(u32*)buf);
            else
            {
                u32 tmp=htonl(JTT808_Terminal_Param.alarm_store_flag);
                u32 id=htonl(ID);
                memcpy(buf,(char*)&id,4);
                memset(buf+4,4,1);
                memcpy(buf+5,(char*)&tmp,4);
                *size+=9;
            }
        break;
        case JTT808_TERMINAL_PARAM_KEY_FLAG:              // 0x0054//�ؼ���־
            if(prio)JTT808_Terminal_Param.key_flag=ntohl(*(u32*)buf);
            else
            {
                u32 tmp=htonl(JTT808_Terminal_Param.key_flag);
                u32 id=htonl(ID);
                memcpy(buf,(char*)&id,4);
                memset(buf+4,4,1);
                memcpy(buf+5,(char*)&tmp,4);
                *size+=9;
            }
        break;
        case JTT808_TERMINAL_PARAM_TOP_SPEED:             // 0x0055//����ٶ�
            if(prio)JTT808_Terminal_Param.top_speed=ntohl(*(u32*)buf);
            else
            {
                u32 tmp=htonl(JTT808_Terminal_Param.top_speed);
                u32 id=htonl(ID);
                memcpy(buf,(char*)&id,4);
                memset(buf+4,4,1);
                memcpy(buf+5,(char*)&tmp,4);
                *size+=9;
            }
        break;
        case JTT808_TERMINAL_PARAM_SPEEDING_DURATION:     // 0x0056//���ٳ���ʱ��
            if(prio)JTT808_Terminal_Param.speeding_duration=ntohl(*(u32*)buf);
            else
            {
                u32 tmp=htonl(JTT808_Terminal_Param.speeding_duration);
                u32 id=htonl(ID);
                memcpy(buf,(char*)&id,4);
                memset(buf+4,4,1);
                memcpy(buf+5,(char*)&tmp,4);
                *size+=9;
            }
        break;
        case JTT808_TERMINAL_PARAM_CONTINOUOUS_DRIVE_TIME:// 0x0057//������ʻʱ������
            if(prio)JTT808_Terminal_Param.continouous_drive_time=ntohl(*(u32*)buf);
            else
            {
                u32 tmp=htonl(JTT808_Terminal_Param.continouous_drive_time);
                u32 id=htonl(ID);
                memcpy(buf,(char*)&id,4);
                memset(buf+4,4,1);
                memcpy(buf+5,(char*)&tmp,4);
                *size+=9;
            }
        break;
        case JTT808_TERMINAL_PARAM_DRIVE_TIME_IN_DAY:     // 0x0058//�����ۼƼ�ʻʱ������
            if(prio)JTT808_Terminal_Param.drive_time_in_day=ntohl(*(u32*)buf);
            else
            {
                u32 tmp=htonl(JTT808_Terminal_Param.drive_time_in_day);
                u32 id=htonl(ID);
                memcpy(buf,(char*)&id,4);
                memset(buf+4,4,1);
                memcpy(buf+5,(char*)&tmp,4);
                *size+=9;
            }
        break;
        case JTT808_TERMINAL_PARAM_REST_MIN_TIME:         // 0x0059//��С��Ϣʱ��
            if(prio)JTT808_Terminal_Param.rest_min_time=ntohl(*(u32*)buf);
            else
            {
                u32 tmp=htonl(JTT808_Terminal_Param.rest_min_time);
                u32 id=htonl(ID);
                memcpy(buf,(char*)&id,4);
                memset(buf+4,4,1);
                memcpy(buf+5,(char*)&tmp,4);
                *size+=9;
            }
        break;
        case JTT808_TERMINAL_PARAM_STOP_MAX_TIME:         // 0x005A//�ͣ��ʱ��
            if(prio)JTT808_Terminal_Param.stop_max_time=ntohl(*(u32*)buf);
            else
            {
                u32 tmp=htonl(JTT808_Terminal_Param.stop_max_time);
                u32 id=htonl(ID);
                memcpy(buf,(char*)&id,4);
                memset(buf+4,4,1);
                memcpy(buf+5,(char*)&tmp,4);
                *size+=9;
            }
        break;
        case JTT808_TERMINAL_PARAM_OVERSPEED_ALARM_DIFF:  // 0x005B//���ٱ���Ԥ����ֵ
            if(prio)JTT808_Terminal_Param.overspeed_alarm_diff=ntohs(*(u16*)buf);
            else
            {
                u16 tmp=htons(JTT808_Terminal_Param.overspeed_alarm_diff);
                u32 id=htonl(ID);
                memcpy(buf,(char*)&id,4);
                memset(buf+4,2,1);
                memcpy(buf+5,(char*)&tmp,2);
                *size+=7;
            }
        break;
        case JTT808_TERMINAL_PARAM_FATIGUE_ALARM_DIFF:    // 0x005C//ƣ�ͼ�ʻԤ����ֵ
            if(prio)JTT808_Terminal_Param.fatigue_driving_diff=ntohs(*(u16*)buf);
            else
            {
                u16 tmp=htons(JTT808_Terminal_Param.fatigue_driving_diff);
                u32 id=htonl(ID);
                memcpy(buf,(char*)&id,4);
                memset(buf+4,2,1);
                memcpy(buf+5,(char*)&tmp,2);
                *size+=7;
            }
        break;
        case JTT808_TERMINAL_PARAM_COLLISION_ALARM_SET:   // 0x005D//��ײ������������
            if(prio)JTT808_Terminal_Param.collision_alarm_param=ntohs(*(u16*)buf);
            else
            {
                u16 tmp=htons(JTT808_Terminal_Param.collision_alarm_param);
                u32 id=htonl(ID);
                memcpy(buf,(char*)&id,4);
                memset(buf+4,2,1);
                memcpy(buf+5,(char*)&tmp,2);
                *size+=7;
            }
        break;
        case JTT808_TERMINAL_PARAM_ROLLOVER_ALARM_SET:    // 0x005E//�෭������������
            if(prio)JTT808_Terminal_Param.rollover_alarm_param=ntohs(*(u16*)buf);
            else
            {
                u16 tmp=htons(JTT808_Terminal_Param.rollover_alarm_param);
                u32 id=htonl(ID);
                memcpy(buf,(char*)&id,4);
                memset(buf+4,2,1);
                memcpy(buf+5,(char*)&tmp,2);
                *size+=7;
            }
        break;

									

        case JTT808_TERMINAL_PARAM_TIMING_PHOTOGRAPHY:    // 0x0064,//��ʱ���տ���
            if(prio)JTT808_Terminal_Param.tim_photo=ntohl(*(u32*)buf);
            else
            {
                u32 tmp=htonl(JTT808_Terminal_Param.tim_photo);
                u32 id=htonl(ID);
                memcpy(buf,(char*)&id,4);
                memset(buf+4,4,1);
                memcpy(buf+5,(char*)&tmp,4);
                *size+=9;
            }
        break;
        case JTT808_TERMINAL_PARAM_DISTANCE_PHOTOGRAPHY:  // 0x0065//�������տ���
            if(prio)JTT808_Terminal_Param.dis_photo=ntohl(*(u32*)buf);
            else
            {
                u32 tmp=htonl(JTT808_Terminal_Param.dis_photo);
                u32 id=htonl(ID);
                memcpy(buf,(char*)&id,4);
                memset(buf+4,4,1);
                memcpy(buf+5,(char*)&tmp,4);
                *size+=9;
            }
        break;


        case JTT808_TERMINAL_PARAM_MULTIMEDIA_QUALITY:    // 0x0070,//ͼ������1~10
            if(prio)JTT808_Terminal_Param.multimedia_quality=ntohl(*(u32*)buf);
            else
            {
                u32 tmp=htonl(JTT808_Terminal_Param.multimedia_quality);
                u32 id=htonl(ID);
                memcpy(buf,(char*)&id,4);
                memset(buf+4,4,1);
                memcpy(buf+5,(char*)&tmp,4);
                *size+=9;
            }
        break;
        case JTT808_TERMINAL_PARAM_BRIGHTNESS:            // 0x0071//����0~255
            if(prio)JTT808_Terminal_Param.brightness=ntohl(*(u32*)buf);
            else
            {
                u32 tmp=htonl(JTT808_Terminal_Param.brightness);
                u32 id=htonl(ID);
                memcpy(buf,(char*)&id,4);
                memset(buf+4,4,1);
                memcpy(buf+5,(char*)&tmp,4);
                *size+=9;
            }
        break;
        case JTT808_TERMINAL_PARAM_CONTRAST:              // 0x0072//�Աȶ�0~127
            if(prio)JTT808_Terminal_Param.contrast=ntohl(*(u32*)buf);
            else
            {
                u32 tmp=htonl(JTT808_Terminal_Param.contrast);
                u32 id=htonl(ID);
                memcpy(buf,(char*)&id,4);
                memset(buf+4,4,1);
                memcpy(buf+5,(char*)&tmp,4);
                *size+=9;
            }
        break;
        case JTT808_TERMINAL_PARAM_SATURATION:            // 0x0073//���Ͷ�0~127
            if(prio)JTT808_Terminal_Param.saturation=ntohl(*(u32*)buf);
            else
            {
                u32 tmp=htonl(JTT808_Terminal_Param.saturation);
                u32 id=htonl(ID);
                memcpy(buf,(char*)&id,4);
                memset(buf+4,4,1);
                memcpy(buf+5,(char*)&tmp,4);
                *size+=9;
            }
        break;
        case JTT808_TERMINAL_PARAM_CHROMA:                // 0x0074//ɫ��0~255
            if(prio)JTT808_Terminal_Param.chroma=ntohl(*(u32*)buf);
            else
            {
                u32 tmp=htonl(JTT808_Terminal_Param.chroma);
                u32 id=htonl(ID);
                memcpy(buf,(char*)&id,4);
                memset(buf+4,4,1);
                memcpy(buf+5,(char*)&tmp,4);
                *size+=9;
            }
        break;


        case JTT808_TERMINAL_PARAM_MILEAGE:             // 0x0080,//������̱����
            if(prio)JTT808_Terminal_Param.mileage=ntohl(*(u32*)buf);
            else
            {
                u32 tmp=htonl(JTT808_Terminal_Param.mileage);
                u32 id=htonl(ID);
                memcpy(buf,(char*)&id,4);
                memset(buf+4,4,1);
                memcpy(buf+5,(char*)&tmp,4);
                *size+=9;
            }
        break;
        case JTT808_TERMINAL_PARAM_PROVINCE_ID:         // 0x0081//ʡID
            if(prio)
            {
                 char tmp[40]={0};
                 itoa(ntohs(*(u16*)buf),tmp, 10);
                 memcpy(JTT808_Terminal_Param.province_id,tmp,LEN_PARAM_PROVINCE_ID);
				 JTT808_rereg();
            }
            else
            {
                u16 tmp=htons(atoi((const char*)JTT808_Terminal_Param.province_id));
                u32 id=htonl(ID);
                memcpy(buf,(char*)&id,4);
                memset(buf+4,2,1);
                memcpy(buf+5,(char*)&tmp,2);
                *size+=7;
            }
        break;
        case JTT808_TERMINAL_PARAM_CITY_ID:             // 0x0082//��ID
            if(prio)
            {
                 char tmp[40]={0};
                 itoa(ntohs(*(u16*)buf),tmp, 10);
                 memcpy(JTT808_Terminal_Param.city_id,tmp,LEN_PARAM_CITY_ID);
				 JTT808_rereg();
            }
            else
            {
                u16 tmp=htons(atoi((const char*)JTT808_Terminal_Param.city_id));
                u32 id=htonl(ID);
                memcpy(buf,(char*)&id,4);
                memset(buf+4,2,1);
                memcpy(buf+5,(char*)&tmp,2);
                *size+=7;
            }
        break;
        case JTT808_TERMINAL_PARAM_LICENSE_PLATE:       // 0x0083//����
            if(prio)
            {
                memset(JTT808_Terminal_Param.lic_plate,0x00,LEN_PARAM_LICENSE_PLATE);
                memcpy(JTT808_Terminal_Param.lic_plate,buf,*size);
				JTT808_rereg();
            }
            else
            {
                u32 id=htonl(ID);
                memcpy(buf,(char*)&id,4);

                memset(buf+4,strlen((const char*)JTT808_Terminal_Param.lic_plate),1);

                memcpy(buf+5,JTT808_Terminal_Param.lic_plate,strlen((const char*)JTT808_Terminal_Param.lic_plate));
                *size+=(5+strlen((const char*)JTT808_Terminal_Param.lic_plate));
            }
        break;
        case JTT808_TERMINAL_PARAM_LICENSE_PLATE_COLOR: // 0x0084//������ɫ
            if(prio)
			{
				JTT808_Terminal_Param.lic_plate_color=*buf;
				JTT808_rereg();
			}
            else
            {
                u32 id=htonl(ID);
                memcpy(buf,(char*)&id,4);
                memset(buf+4,1,1);
                memset(buf+5,JTT808_Terminal_Param.lic_plate_color,1);
                *size+=6;
            }
        break;
                      


        case JTT808_TERMINAL_PARAM_GNSS_POSITION_TYPE:  // 0x0090,//GNSSģ�鶨λģʽ
            if(prio)JTT808_Terminal_Param.gnss_position_type=*buf;
            else
            {
                u32 id=htonl(ID);
                memcpy(buf,(char*)&id,4);
                memset(buf+4,1,1);
                memset(buf+5,JTT808_Terminal_Param.gnss_position_type,1);
                *size+=6;
            }
        break;
        case JTT808_TERMINAL_PARAM_GNSS_BAUDRATE:       // 0x0091//GNSS������
            if(prio)JTT808_Terminal_Param.gnss_baudrate=*buf;
            else
            {
                u32 id=htonl(ID);
                memcpy(buf,(char*)&id,4);
                memset(buf+4,1,1);
                memset(buf+5,JTT808_Terminal_Param.gnss_baudrate,1);
                *size+=6;
            }
        break;
        case JTT808_TERMINAL_PARAM_GNSS_OUTPUT_RATE:    // 0x0092//GNSSģ����ϸ��λ�������Ƶ��
            if(prio)JTT808_Terminal_Param.gnss_output_rate=*buf;
            else
            {
                u32 id=htonl(ID);
                memcpy(buf,(char*)&id,4);
                memset(buf+4,1,1);
                memset(buf+5,JTT808_Terminal_Param.gnss_output_rate,1);
                *size+=6;
            }
        break;
        case JTT808_TERMINAL_PARAM_GNSS_SAMP_FREQUENCY: // 0x0093//GNSSģ����ϸ��λ���ݲɼ�Ƶ��
            if(prio)JTT808_Terminal_Param.gnss_samp_frequency=ntohl(*(u32*)buf);
            else
            {
                u32 tmp=htonl(JTT808_Terminal_Param.gnss_samp_frequency);
                u32 id=htonl(ID);
                memcpy(buf,(char*)&id,4);
                memset(buf+4,4,1);
                memcpy(buf+5,(char*)&tmp,4);
                *size+=9;
            }
        break;
        case JTT808_TERMINAL_PARAM_GNSS_UPLOAD_TYPE:    // 0x0094//GNSSģ����ϸ��λ�����ϴ���ʽ
            if(prio)JTT808_Terminal_Param.gnss_upload_type=*buf;
            else
            {
                u32 id=htonl(ID);
                memcpy(buf,(char*)&id,4);
                memset(buf+4,1,1);
                memset(buf+5,JTT808_Terminal_Param.gnss_upload_type,1);
                *size+=6;
            }
        break;
        case JTT808_TERMINAL_PARAM_GNSS_UPLOAD_SET:     // 0x0095//GNSSģ����ϸ��λ�����ϴ�����
            if(prio)JTT808_Terminal_Param.gnss_upload_set=ntohl(*(u32*)buf);
            else
            {
                u32 tmp=htonl(JTT808_Terminal_Param.gnss_upload_set);
                u32 id=htonl(ID);
                memcpy(buf,(char*)&id,4);
                memset(buf+4,4,1);
                memcpy(buf+5,(char*)&tmp,4);
                *size+=9;
            }
        break;

                          

        case JTT808_TERMINAL_PARAM_CAN1_SAMP_INTERVAL:   //0x0100,//CAN����ͨ��1�ɼ�ʱ����
            if(prio)JTT808_Terminal_Param.can_1_samp_time_interval=ntohl(*(u32*)buf);
            else
            {
                u32 tmp=htonl(JTT808_Terminal_Param.can_1_samp_time_interval);
                u32 id=htonl(ID);
                memcpy(buf,(char*)&id,4);
                memset(buf+4,4,1);
                memcpy(buf+5,(char*)&tmp,4);
                *size+=9;
            }
        break;
        case JTT808_TERMINAL_PARAM_CAN1_UPLOAD_INTERVAL: // 0x0101//CAN����ͨ��1�ϴ�ʱ����
            if(prio)JTT808_Terminal_Param.can_1_upload_time_interval=ntohs(*(u16*)buf);
            else
            {
                u16 tmp=htons(JTT808_Terminal_Param.can_1_upload_time_interval);
                u32 id=htonl(ID);
                memcpy(buf,(char*)&id,4);
                memset(buf+4,2,1);
                memcpy(buf+5,(char*)&tmp,2);
                *size+=7;
            }
        break;
        case JTT808_TERMINAL_PARAM_CAN2_SAMP_INTERVAL:   // 0x0102//CAN����ͨ��2�ɼ�ʱ����
            if(prio)JTT808_Terminal_Param.can_2_samp_time_interval=ntohl(*(u32*)buf);
            else
            {
                u32 tmp=htonl(JTT808_Terminal_Param.can_2_samp_time_interval);
                u32 id=htonl(ID);
                memcpy(buf,(char*)&id,4);
                memset(buf+4,4,1);
                memcpy(buf+5,(char*)&tmp,4);
                *size+=9;
            }
        break;
        case JTT808_TERMINAL_PARAM_CAN2_UPLOAD_INTERVAL: // 0x0103//CAN����ͨ��2�ϴ�ʱ����
            if(prio)JTT808_Terminal_Param.can_2_upload_time_interval=ntohs(*(u16*)buf);
            else
            {
                u16 tmp=htons(JTT808_Terminal_Param.can_2_upload_time_interval);
                u32 id=htonl(ID);
                memcpy(buf,(char*)&id,4);
                memset(buf+4,2,1);
                memcpy(buf+5,(char*)&tmp,2);
                *size+=7;
            }
        break;
//        case JTT808_TERMINAL_PARAM_CAN_ID_SAMP_SET:      // 0x0110//CAN����ID�����ɼ�����
//            if(prio)
//            {
//                memset(JTT808_Terminal_Param.can_ID_samp_set,0x00,8);
//                memcpy(JTT808_Terminal_Param.can_ID_samp_set,buf,*size);
//            }
//            else
//            {
//                u32 id=htonl(ID);
//                memcpy(buf,(char*)&id,4);
//
//                memset(buf+4,strlen((const char*)JTT808_Terminal_Param.can_ID_samp_set),1);
//
//                memcpy(buf+5,JTT808_Terminal_Param.can_ID_samp_set,strlen((const char*)JTT808_Terminal_Param.can_ID_samp_set));
//                *size+=(5+strlen((const char*)JTT808_Terminal_Param.can_ID_samp_set));
//            }
//        break;


        default:
			if(ID>=0x0110&&ID<=0x1FF)  //��ʾΪCAN ���� ID �����ɼ����ã�
			{
				int index=ID-0x0110;
				
				if(index>=0&&index<CAN_ID_TOTAL)      //Ŀǰ���֧��50��CAN_ID
				{
					if(prio)
					{
							memset(JTT808_Terminal_Param.can_ID_samp_set[index], 0x00, 8);
							memcpy(JTT808_Terminal_Param.can_ID_samp_set[index], buf, *size);
					}
					else
					{	
							u32 id=htonl(ID);
							memcpy(buf,(char*)&id,4);

							memset(buf+4,0x08,1);

							memcpy(buf+5, JTT808_Terminal_Param.can_ID_samp_set[index], 8);
							*size+=(5+8);
					}
				}	
			}
			else
			{
            	return -1;
			}
    }
    return 0;   
}



void JTT808_Set_Terminal_Param(u8 *pdata, u32 size)
{
    u8 total=*pdata++;     //��������
    
    
    while(total--)
    {
         u32 ID=0;
         u32 len=0;
         u8 buf[40]={0};

         ID=ntohl(*(u32*)pdata);
         pdata+=4;
         len=*pdata++;
         memcpy(buf,pdata,len);
         pdata+=len;

         JTT808_Param_exec(ID,&len,buf,1);
    }
    jtt808_save_para();
}



void JTT808_ParamRestoreFactory(void)
{
    JTT808_Terminal_Param.initial      = 0x01010101;  //��ʼ����־

    JTT808_Terminal_Param.beat_interval= 30;          //�ն��������ͼ������λΪ��(s)
    JTT808_Terminal_Param.tcp_timeout  = 30;          //TCP��ϢӦ��ʱʱ�䣬��λΪ��(s)
    JTT808_Terminal_Param.tcp_repeat_times=3;         //TCP��Ϣ�ش�����
    JTT808_Terminal_Param.udp_timeout  = 30;          //UDP��ϢӦ��ʱʱ�䣬��λΪ��(s)
    JTT808_Terminal_Param.udp_repeat_times=3;         //UDP��Ϣ�ش�����
    JTT808_Terminal_Param.sms_timeout  = 30;          //SMS��ϢӦ��ʱʱ�䣬��λΪ��(s)
    JTT808_Terminal_Param.sms_repeat_times=3;         //SMS��Ϣ�ش�����
    
    strcpy((char *)JTT808_Terminal_Param.main_server_apn,"cmnet");                          //��������APN������ͨ�Ų��ŷ��ʵ㡣��������ʽΪCDMA����ô�ΪPPP���ź���
    strcpy((char *)JTT808_Terminal_Param.main_server_user,"user");                          //������������ͨ�Ų����û���
    strcpy((char *)JTT808_Terminal_Param.main_server_psw,"pwd");                            //������������ͨ�Ų�������
    strcpy((char *)JTT808_Terminal_Param.main_server_domain,"183.62.9.117");             	  //����������ַ��IP������
    strcpy((char *)JTT808_Terminal_Param.backup_server_apn,"#777");                         //���ݷ�����APN������ͨ�Ų��ŷ��ʵ�
    strcpy((char *)JTT808_Terminal_Param.backup_server_user,"card");                        //���ݷ���������ͨ�Ų����û���
    strcpy((char *)JTT808_Terminal_Param.backup_server_psw,"card");                         //���ݷ���������ͨ�Ų�������
    strcpy((char *)JTT808_Terminal_Param.backup_server_domain,"183.62.9.117");              //���ݷ�������ַ��IP������
    strcpy((char *)JTT808_Terminal_Param.server_tcp_port,"6033");                           //������TCP�˿�
    strcpy((char *)JTT808_Terminal_Param.server_udp_port,"6035");                           //������UDP�˿�
    strcpy((char *)JTT808_Terminal_Param.ic_license_authentication_main_server_domain,"");  //��·����֤IC����֤��������IP��ַ������
    strcpy((char *)JTT808_Terminal_Param.ic_license_authentication_main_server_tcp_port,"");//��·����֤IC����֤��������TCP�˿�
    strcpy((char *)JTT808_Terminal_Param.ic_license_authentication_main_server_udp_port,"");//��·����֤IC����֤��������UDP�˿�
    strcpy((char *)JTT808_Terminal_Param.ic_license_authentication_backup_server_domain,"");//��·����֤IC����֤���ݷ�����IP��ַ���������˿�ͬ��������
    
    JTT808_Terminal_Param.location_report_strategy=0x00;     //λ�û㱨���ԣ�0����ʱ�㱨��1������㱨��2����ʱ�Ͷ���㱨
    JTT808_Terminal_Param.location_report_program=0x00;      //λ�û㱨������0������ACC״̬��1�����ݵ�¼״̬��ACC״̬�����жϵ�¼״̬������¼�ٸ���ACC״̬
    JTT808_Terminal_Param.report_driver_unlogin_interval=60; //��ʻԱδ��¼�㱨ʱ��������λΪ��(s),>0
    
    JTT808_Terminal_Param.report_at_sleep_interval=60;       //����ʱ�㱨ʱ��������λΪ��(s),>0
    JTT808_Terminal_Param.report_at_alarm_interval=1;        //��������ʱ�㱨ʱ��������λΪ��(s),>0
    JTT808_Terminal_Param.default_report_interval=5;         //ȱʡʱ��㱨�������λΪ��(s),>0
    
    JTT808_Terminal_Param.default_report_dst_interval=300;        //ȱʡ����㱨�������λΪ��(m),>0
    JTT808_Terminal_Param.report_driver_unlogin_dst_interval=400; //��ʻԱδ��¼�㱨����������λΪ��(m),>0
    JTT808_Terminal_Param.report_at_sleep_dst_interval=1000;      //����ʱ�㱨����������λΪ��(m),>0
    JTT808_Terminal_Param.report_at_alarm_dst_interval=50;        //��������ʱ�㱨����������λΪ��(m),>0
    JTT808_Terminal_Param.inflect_point_complement_biography=15;  //�յ㲹���Ƕȣ�<180��
    JTT808_Terminal_Param.bound_radius=30;                        //����Χ���뾶���Ƿ�λ�Ʒ�ֵ������λΪ��
    
    strcpy((char *)JTT808_Terminal_Param.monitor_platform_tel,"10086");    //���ƽ̨�绰����
    strcpy((char *)JTT808_Terminal_Param.reset_tel,"10086");               //��λ�绰���룬�ɲ��ô˵绰���벦���ն˵绰���ն˸�λ
    strcpy((char *)JTT808_Terminal_Param.restore_factory_tel,"10086");     //�ָ��������õ绰���룬�ɲ��ô˵绰���벦���ն˵绰���ն˻ָ���������
    strcpy((char *)JTT808_Terminal_Param.monitor_platform_sms_tel,"10086");//���ƽ̨SMS�绰����
    strcpy((char *)JTT808_Terminal_Param.sms_alarm_tel,"10086");           //�����ն�SMS�ı���������
    JTT808_Terminal_Param.phone_answer_strategy=0x00;                 //�ն˵绰�������ԣ�0���Զ�������1��ACC ONʱ�Զ�������OFFʱ�ֶ�����
    JTT808_Terminal_Param.talk_max_time=0xFFFFFFFF;                   //ÿ���ͨ��ʱ�䣬��λΪ��(s),0Ϊ������ͨ����0xFFFFFFFFΪ������
    JTT808_Terminal_Param.talk_max_time_in_mon=0xFFFFFFFF;            //�����ͨ��ʱ�䣬��λΪ��(s),0Ϊ������ͨ����0xFFFFFFFFΪ������
    strcpy((char *)JTT808_Terminal_Param.monitor_tel,"");             //�����绰����
    strcpy((char *)JTT808_Terminal_Param.monitor_platform_privilege_sms_tel,"10086");//���ƽ̨��Ȩ���ź���
    
    JTT808_Terminal_Param.alarm_mask=0x00;              //���������֡���λ����Ϣ�㱨��Ϣ�еı�����־���Ӧ����ӦλΪ1����Ӧ����������
    JTT808_Terminal_Param.alarm_sms_switch=0x00;        //���������ı�SMS���أ���λ����Ϣ�㱨��Ϣ�еı�����־���Ӧ����ӦλΪ1����Ӧ����ʱ�����ı�SMS
    JTT808_Terminal_Param.alarm_shoot_switch=0x00;      //�������㿪�أ���λ����Ϣ�㱨��Ϣ�еı�����־���Ӧ����ӦλΪ1����Ӧ����ʱ����ͷ����
    JTT808_Terminal_Param.alarm_store_flag=0x00;        //��������洢��־����λ����Ϣ�㱨��Ϣ�еı�����־���Ӧ����ӦλΪ1�����Ӧ����ʱ�ĵ���Ƭ���д洢������ʵʱ�ϴ�
    JTT808_Terminal_Param.key_flag=0x00;                //�ؼ���־����λ����Ϣ�㱨��Ϣ�еı�����־���Ӧ����ӦλΪ1�����Ӧ����Ϊ�ؼ�����
    JTT808_Terminal_Param.top_speed=180;                //����ٶȣ���λΪ����ÿСʱ(km/h)
    JTT808_Terminal_Param.speeding_duration=5;          //���ٳ���ʱ�䣬��λΪ��(s)
    JTT808_Terminal_Param.continouous_drive_time=0xFFFFFFFF; //������ʻʱ�����ޣ���λΪ��(s)
    JTT808_Terminal_Param.drive_time_in_day=0xFFFFFFFF;      //�����ۼƼ�ʻʱ�����ޣ���λΪ��(s)
    JTT808_Terminal_Param.rest_min_time=1200;           //��С��Ϣʱ�䣬��λΪ��(s)
    JTT808_Terminal_Param.stop_max_time=0xFFFFFFFF;     //�ͣ��ʱ�䣬��λΪ��(s)
    JTT808_Terminal_Param.overspeed_alarm_diff=200;     //���ٱ���Ԥ����ֵ����λΪ0.1km/h
    JTT808_Terminal_Param.fatigue_driving_diff=3600;    //ƣ�ͼ�ʻԤ����ֵ����λΪ��(s),>0
    JTT808_Terminal_Param.collision_alarm_param=0x3030; //��ײ�����������ã�b7-b0:��ײʱ�䣬��λ4ms��b15-b8:��ײ���ٶȣ���λ0.1g�����÷�Χ��0-79֮�䣬Ĭ��Ϊ10��
    JTT808_Terminal_Param.rollover_alarm_param=30;      //�෭�����������ã��෭�Ƕȣ���λΪ�ȣ�Ĭ��Ϊ30�ȣ�
    
    JTT808_Terminal_Param.tim_photo=0x00;               //��ʱ���տ���
    JTT808_Terminal_Param.dis_photo=0x00;               //�������տ���
    
    JTT808_Terminal_Param.multimedia_quality=5;         //ͼ��(��Ƶ)������1��10,1���
    JTT808_Terminal_Param.brightness=127;               //���ȣ�0��255
    JTT808_Terminal_Param.contrast=63;                  //�Աȶȣ�0��127
    JTT808_Terminal_Param.saturation=63;                //���Ͷȣ�0��127
    JTT808_Terminal_Param.chroma=127;                   //ɫ�ȣ�0��255
    
    JTT808_Terminal_Param.mileage=0x00;                         //������̱������1/10km
    strcpy((char *)JTT808_Terminal_Param.province_id,"44");     //�������ڵ�ʡ��ID��
    strcpy((char *)JTT808_Terminal_Param.city_id,"0100");       //�������ڵ�����ID��
    strcpy((char *)JTT808_Terminal_Param.lic_plate,"��A88001"); //������ͨ�����Ű䷢�Ļ���������
    JTT808_Terminal_Param.lic_plate_color=1;                    //������ɫ������JT/T415-2006��5.4.12
    
    JTT808_Terminal_Param.gnss_position_type=0x0F;    //GNSS��λģʽ��  bit0��0������GPS��1������GPS��      bit1��0�����ñ�����1�����ñ�����    bit2��0������GLONASS��1������GLONASS��      bit3��0������Galileo��1������Galileo��
    JTT808_Terminal_Param.gnss_baudrate=0x01;         //GNSS�����ʣ�    0x00:4800;  0x01:9600;  0x02:19200; 0x03:38400; 0x04:47600; 0x05:115200;
    JTT808_Terminal_Param.gnss_output_rate=0x01;      //GNSSģ����ϸ��λ�������Ƶ�ʣ� 0x00:500ms;  0x01:1000ms(Ĭ��);  0x02:2000ms;    0x03:3000ms;    0x04:4000ms;
    JTT808_Terminal_Param.gnss_samp_frequency=0x01;   //GNSSģ����ϸ��λ���ݲɼ�Ƶ�ʣ���λΪ�룬Ĭ��1��
    JTT808_Terminal_Param.gnss_upload_type=0x01;      //GNSSģ����ϸ��λ�����ϴ���ʽ��0x00:���ش洢�����ϴ���Ĭ��ֵ����  0x01����ʱ�����ϴ���    0x02:���������ϴ��� 0x0b�����ۻ�ʱ���ϴ����ﵽ����ʱ����Զ�ֹͣ�ϴ���  0x0c�����ۻ������ϴ����ﵽ������Զ�ֹͣ�ϴ���  0x0d�����ۻ������ϴ����ﵽ�ϴ��������Զ�ֹͣ�ϴ���
    JTT808_Terminal_Param.gnss_upload_set=0x01;       //GNSSģ����ϸ��λ�����ϴ����ã�0x01:��λΪ�룻0x02����λΪ�ף�0x0b:��λΪ�룻0x0c����λΪ�ף�0x0d����λΪ��
    
    JTT808_Terminal_Param.can_1_samp_time_interval=0;               //can����ͨ��1�ɼ�ʱ������ms����0��ʾ���ɼ�
    JTT808_Terminal_Param.can_1_upload_time_interval=0;             //can����ͨ��1�ϴ�ʱ������s����0��ʾ���ϴ�
    JTT808_Terminal_Param.can_2_samp_time_interval=0;               //can����ͨ��2�ɼ�ʱ������ms����0��ʾ���ɼ�
    JTT808_Terminal_Param.can_2_upload_time_interval=0;             //can����ͨ��2�ϴ�ʱ������s����0��ʾ���ϴ�
//    strcpy((char *)JTT808_Terminal_Param.can_ID_samp_set,"");       //can����ID�����ɼ����ã�bit63-bit32����ʾ��ID�ɼ�ʱ������ms����0��ʾ���ɼ���bit31��canIDͨ���ţ�0-can1��1-can2��bit30��֡���ͣ�0-��׼֡��1-��չ֡��bit29�����ݲɼ���ʽ��0-ԭʼ���ݿ���1-�ɼ�����ļ���ֵ��bit28-bit0��can����ID��
//    strcpy((char *)JTT808_Terminal_Param.other_can_ID_samp_set,""); //��������can����ID�����ɼ����á�
//		memset(JTT808_Terminal_Param.can_ID_samp_set,0x00,CAN_ID_TOTAL*8);

//		for(i=0; i<CAN_ID_TOTAL; i++)
//		{
//				//OBD_Id�ṹ��ĸ�ֵ.
//				MSG_Id.priority = P6;
//				MSG_Id.exp_data_page = 0;
//				MSG_Id.data_page = 0;
//				MSG_Id.PDU_format = 0xEA;
//				MSG_Id.PDU_specific = 0x00;    //Ŀ���ַ�����߹㲥��ַ
//				MSG_Id.source_add = source_address;
//			
//				obd_id.can_id = OBD_ID_to_EXT_ID(&MSG_Id);
//				obd_id.sample_type = 0;
//				obd_id.frame_type = 1;
//				obd_id.can_channel = 1;
//				obd_id.sample_time_interval = 600000;
//				memcpy(JTT808_Terminal_Param.can_ID_samp_set[i], &obd_id, sizeof(obd_id));
//		}
		
//		JTT808_Terminal_Param.can_ID_samp_set[0]=
//		JTT808_Terminal_Param.can_ID_samp_set[1]=
//		JTT808_Terminal_Param.can_ID_samp_set[2]=
//		JTT808_Terminal_Param.can_ID_samp_set[3]=
//		JTT808_Terminal_Param.can_ID_samp_set[4]=
//		JTT808_Terminal_Param.can_ID_samp_set[5]=
//		JTT808_Terminal_Param.can_ID_samp_set[6]=
//		JTT808_Terminal_Param.can_ID_samp_set[7]=
    
    strcpy((char *)JTT808_Terminal_Param.terminalID,"8188001");                //�ն�ID
    strcpy((char *)JTT808_Terminal_Param.local_tel,"13456788001");             //�ն��ֻ���
    JTT808_Terminal_Param.wheel_coefficient=0;                                 //����ϵ��
    JTT808_Terminal_Param.fuel_tank_capacity=50;                               //��������
    strcpy((char *)JTT808_Terminal_Param.authorization_no,"00000000000000000000000000000000"); //��Ȩ��

	JTT808_Terminal_Param.lock_car=0;
    
    jtt808_save_para();
}

void jtt808_param_init(void)
{
     jtt808_read_para();
     if(JTT808_Terminal_Param.initial != 0x01010101)
     {
         JTT808_ParamRestoreFactory();
		 W25X_EraseChip();
     }
     jtt808_read_para(); 

	 if(JTT808_Terminal_Param.lock_car)
     {
        LockCar();
     } 
     else
     {
        UnLockCar();
     }
	API_vpdn_set((char*)JTT808_Terminal_Param.main_server_apn,(char*)JTT808_Terminal_Param.main_server_user,(char*)JTT808_Terminal_Param.main_server_psw);		 //����apn����
	API_server_ip_port_set(0,JTT808_Terminal_Param.main_server_domain,JTT808_Terminal_Param.server_tcp_port,0);								 //����IP���˿ڲ���
}


void jtt808_read_para(void)
{
    ReadFlash(JTT808_PARAM_ADDRESS,(u8*)&JTT808_Terminal_Param,sizeof(JTT808_Terminal_Param));
}

void jtt808_save_para(void)
{
    WriteFlash(JTT808_PARAM_ADDRESS,(u8*)&JTT808_Terminal_Param,sizeof(JTT808_Terminal_Param));
}





