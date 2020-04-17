#include "include.h"

#define JTT808_PARAM_ADDRESS    0x0803FC00            //第246页的起始地址



JTT808t_TerminalParamFlash JTT808_Terminal_Param={0};


void JTT808_rereg(void)
{
	memset((char *)JTT808_Terminal_Param.authorization_no,0x00,sizeof(JTT808_Terminal_Param.authorization_no));
	strcpy((char *)JTT808_Terminal_Param.authorization_no,"00000000000000000000000000000000");
	jtt808_save_para();
	JTT808_Flag_Authentication=0;
}

/**********************************************************
*                    参数设置、获取
* 参数：ID : 参数ID
*     size ：参数长度
*       buf：参数缓冲区
*      prio：设置、获取状态     0：获取   1：设置
*************************************************************/

signed char JTT808_Param_exec(u32 ID, u32* size,u8* buf,u8 prio)
{
    switch(ID)
    {     												
        case JTT808_TERMINAL_PARAM_BEAT:               //0x0001//终端心跳间隔
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
        case JTT808_TERMINAL_PARAM_TCP_TIMEOUT:        // 0x0002//TCP应答超时时间
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
        case JTT808_TERMINAL_PARAM_TCP_REPEAT_TIMES:   // 0x0003//TCP重传次数
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
        case JTT808_TERMINAL_PARAM_UDP_TIMEOUT:        // 0x0004//UDP应答超时时间
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
        case JTT808_TERMINAL_PARAM_UDP_REPEAT_TIMES:   // 0x0005//UDP重传次数
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
        case JTT808_TERMINAL_PARAM_SMS_TIMEOUT:        // 0x0006//SMS应答超时时间
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
        case JTT808_TERMINAL_PARAM_SMS_REPEAT_TIMES:   // 0x0007//SMS重传次数
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



 

        case JTT808_TERMINAL_PARAM_MAIN_SERVER_APN:    // 0x0010//主服务器APN
            if(prio)
            {
                memset(JTT808_Terminal_Param.main_server_apn,0x00,LEN_PARAM_SERVER_APN);
                memcpy(JTT808_Terminal_Param.main_server_apn,buf,*size);
				API_vpdn_set((char*)JTT808_Terminal_Param.main_server_apn,(char*)JTT808_Terminal_Param.main_server_user,(char*)JTT808_Terminal_Param.main_server_psw);		 //设置apn参数
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
        case JTT808_TERMINAL_PARAM_MAIN_SERVER_USER:   // 0x0011//主服务器拨号用户名
            if(prio)
            {
                memset(JTT808_Terminal_Param.main_server_user,0x00,LEN_PARAM_SERVER_USER);
                memcpy(JTT808_Terminal_Param.main_server_user,buf,*size);
				API_vpdn_set((char*)JTT808_Terminal_Param.main_server_apn,(char*)JTT808_Terminal_Param.main_server_user,(char*)JTT808_Terminal_Param.main_server_psw);		 //设置apn参数
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
        case JTT808_TERMINAL_PARAM_MAIN_SERVER_PSW:    // 0x0012//主服务器拨号密码
            if(prio)
            {
                memset(JTT808_Terminal_Param.main_server_psw,0x00,LEN_PARAM_SERVER_PSW);
                memcpy(JTT808_Terminal_Param.main_server_psw,buf,*size);
				API_vpdn_set((char*)JTT808_Terminal_Param.main_server_apn,(char*)JTT808_Terminal_Param.main_server_user,(char*)JTT808_Terminal_Param.main_server_psw);		 //设置apn参数
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
        case JTT808_TERMINAL_PARAM_MAIN_SERVER_DOMAIN: // 0x0013//主服务器域名或IP
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
        case JTT808_TERMINAL_PARAM_BACKUP_SERVER_APN:  // 0x0014//备份服务器APN
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
        case JTT808_TERMINAL_PARAM_BACKUP_SERVER_USER: // 0x0015//备份服务器拨号用户名
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
        case JTT808_TERMINAL_PARAM_BACKUP_SERVER_PSW:  // 0x0016//备份服务器拨号密码
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
        case JTT808_TERMINAL_PARAM_BACKUP_SERVER_DOMAIN:// 0x0017//备份服务器域名或IP
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
        case JTT808_TERMINAL_PARAM_SERVER_TCP_PORT:    // 0x0018//TCP端口
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
        case JTT808_TERMINAL_PARAM_SERVER_UDP_PORT:    // 0x0019//UDP端口
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
        case JTT808_TERMINAL_PARAM_IC_AUTHENTICATION_MAIN_SERVER_DOMAIN:  // 0x001A//道路运输证IC卡认证主服务器APN
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
        case JTT808_TERMINAL_PARAM_IC_AUTHENTICATION_TCP_PORT:            // 0x001B//道路运输证IC卡认证主服务器TCP端口
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
        case JTT808_TERMINAL_PARAM_IC_AUTHENTICATION_UDP_PORT:            // 0x001C//道路运输证IC卡认证主服务器UDP端口
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
        case JTT808_TERMINAL_PARAM_IC_AUTHENTICATION_BACKUP_SERVER_DOMAIN:// 0x001D//道路运输证IC卡认证备份服务器APN
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

			
            
            				    

        case JTT808_TERMINAL_PARAM_LOCATION_REPORT_STRATEGY:       // 0x0020,//位置汇报策略
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
        case JTT808_TERMINAL_PARAM_LOCATION_REPORT_PROGRAM:        // 0x0021//位置汇报方案
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
        case JTT808_TERMINAL_PARAM_REPORT_DRIVER_UNLOGIN_INTERVAL: // 0x0022//驾驶员未登录汇报时间间隔
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


        case JTT808_TERMINAL_PARAM_REPORT_AT_SLEEP_INTERVAL:       // 0x0027,//休眠汇报时间间隔
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
        case JTT808_TERMINAL_PARAM_REPORT_AT_ALARM_INTERVAL:       // 0x0028//紧急报警汇报时间间隔
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
        case JTT808_TERMINAL_PARAM_DEFAULT_REPORT_INTERVAL:        // 0x0029//缺省时间汇报间隔
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


        case JTT808_TERMINAL_PARAM_DEFAULT_REPORT_DST_INTERVAL:        	// 0x002C,//缺省距离汇报间隔
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
        case JTT808_TERMINAL_PARAM_REPORT_DRIVER_UNLOGIN_DST_INTERVAL:  // 0x002D//驾驶员未登录汇报距离间隔
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
        case JTT808_TERMINAL_PARAM_REPORT_AT_SLEEP_DST_INTERVAL:        // 0x002E//休眠汇报距离间隔
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
        case JTT808_TERMINAL_PARAM_REPORT_AT_ALARM_DST_INTERVAL:        // 0x002F//紧急报警汇报距离间隔
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
        case JTT808_TERMINAL_PARAM_INFLECT_POINT_COMPLEMENT_BIOGRAPHY:  // 0x0030//拐点补传角度
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
        case JTT808_TERMINAL_PARAM_BOUND_RADIUS:                        //0x0031//电子围栏半径
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

	    

        case JTT808_TERMINAL_PARAM_MONITOR_PLATFORM_TEL:      // 0x0040,//监控平台电话号码
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
        case JTT808_TERMINAL_PARAM_RESET_TEL:                 // 0x0041//复位电话号码
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
        case JTT808_TERMINAL_PARAM_RESTORE_FACTORY_TEL:       // 0x0042//恢复出厂设置电话号码
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
        case JTT808_TERMINAL_PARAM_MONITOR_PLATFORM_SMS_TEL:  // 0x0043//监控平台SMS电话号码
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
        case JTT808_TERMINAL_PARAM_SMS_ALARM_TEL:             // 0x0044//接收终端SMS文本报警号码
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
        case JTT808_TERMINAL_PARAM_PHONE_ANSWER_STRATEGY:     // 0x0045//终端电话接听策略
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
        case JTT808_TERMINAL_PARAM_TALK_MAX_TIME:             // 0x0046//每次最长通话时间
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
        case JTT808_TERMINAL_PARAM_TALK_MAX_TIME_IN_MON:      // 0x0047//每月最长通话时间
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
        case JTT808_TERMINAL_PARAM_MONITOR_TEL:               // 0x0048//监听电话号码
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
        case JTT808_TERMINAL_PARAM_MONITOR_PLATFORM_PRIVILEGE_SMS_TEL:  // 0x0049//监管平台特权短信号码
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

                     

        case JTT808_TERMINAL_PARAM_ALARM_MASK:            // 0x0050,//报警屏蔽字
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
        case JTT808_TERMINAL_PARAM_ALARM_SMS_SWITCH:      // 0x0051//报警发送文本SMS开关
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
        case JTT808_TERMINAL_PARAM_ALARM_SHOOT_SWITCH:    // 0x0052//报警拍摄开关
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
        case JTT808_TERMINAL_PARAM_ALARM_STORE_FLAG:      // 0x0053//报警拍摄储存标志
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
        case JTT808_TERMINAL_PARAM_KEY_FLAG:              // 0x0054//关键标志
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
        case JTT808_TERMINAL_PARAM_TOP_SPEED:             // 0x0055//最高速度
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
        case JTT808_TERMINAL_PARAM_SPEEDING_DURATION:     // 0x0056//超速持续时间
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
        case JTT808_TERMINAL_PARAM_CONTINOUOUS_DRIVE_TIME:// 0x0057//连续驾驶时间门限
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
        case JTT808_TERMINAL_PARAM_DRIVE_TIME_IN_DAY:     // 0x0058//当天累计驾驶时间门限
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
        case JTT808_TERMINAL_PARAM_REST_MIN_TIME:         // 0x0059//最小休息时间
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
        case JTT808_TERMINAL_PARAM_STOP_MAX_TIME:         // 0x005A//最长停车时间
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
        case JTT808_TERMINAL_PARAM_OVERSPEED_ALARM_DIFF:  // 0x005B//超速报警预警差值
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
        case JTT808_TERMINAL_PARAM_FATIGUE_ALARM_DIFF:    // 0x005C//疲劳驾驶预警差值
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
        case JTT808_TERMINAL_PARAM_COLLISION_ALARM_SET:   // 0x005D//碰撞报警参数设置
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
        case JTT808_TERMINAL_PARAM_ROLLOVER_ALARM_SET:    // 0x005E//侧翻报警参数设置
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

									

        case JTT808_TERMINAL_PARAM_TIMING_PHOTOGRAPHY:    // 0x0064,//定时拍照控制
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
        case JTT808_TERMINAL_PARAM_DISTANCE_PHOTOGRAPHY:  // 0x0065//定距拍照控制
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


        case JTT808_TERMINAL_PARAM_MULTIMEDIA_QUALITY:    // 0x0070,//图像质量1~10
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
        case JTT808_TERMINAL_PARAM_BRIGHTNESS:            // 0x0071//亮度0~255
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
        case JTT808_TERMINAL_PARAM_CONTRAST:              // 0x0072//对比度0~127
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
        case JTT808_TERMINAL_PARAM_SATURATION:            // 0x0073//饱和度0~127
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
        case JTT808_TERMINAL_PARAM_CHROMA:                // 0x0074//色度0~255
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


        case JTT808_TERMINAL_PARAM_MILEAGE:             // 0x0080,//车辆里程表读数
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
        case JTT808_TERMINAL_PARAM_PROVINCE_ID:         // 0x0081//省ID
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
        case JTT808_TERMINAL_PARAM_CITY_ID:             // 0x0082//市ID
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
        case JTT808_TERMINAL_PARAM_LICENSE_PLATE:       // 0x0083//车牌
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
        case JTT808_TERMINAL_PARAM_LICENSE_PLATE_COLOR: // 0x0084//车牌颜色
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
                      


        case JTT808_TERMINAL_PARAM_GNSS_POSITION_TYPE:  // 0x0090,//GNSS模块定位模式
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
        case JTT808_TERMINAL_PARAM_GNSS_BAUDRATE:       // 0x0091//GNSS波特率
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
        case JTT808_TERMINAL_PARAM_GNSS_OUTPUT_RATE:    // 0x0092//GNSS模块详细定位数据输出频率
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
        case JTT808_TERMINAL_PARAM_GNSS_SAMP_FREQUENCY: // 0x0093//GNSS模块详细定位数据采集频率
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
        case JTT808_TERMINAL_PARAM_GNSS_UPLOAD_TYPE:    // 0x0094//GNSS模块详细定位数据上传方式
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
        case JTT808_TERMINAL_PARAM_GNSS_UPLOAD_SET:     // 0x0095//GNSS模块详细定位数据上传设置
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

                          

        case JTT808_TERMINAL_PARAM_CAN1_SAMP_INTERVAL:   //0x0100,//CAN总线通道1采集时间间隔
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
        case JTT808_TERMINAL_PARAM_CAN1_UPLOAD_INTERVAL: // 0x0101//CAN总线通道1上传时间间隔
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
        case JTT808_TERMINAL_PARAM_CAN2_SAMP_INTERVAL:   // 0x0102//CAN总线通道2采集时间间隔
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
        case JTT808_TERMINAL_PARAM_CAN2_UPLOAD_INTERVAL: // 0x0103//CAN总线通道2上传时间间隔
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
//        case JTT808_TERMINAL_PARAM_CAN_ID_SAMP_SET:      // 0x0110//CAN总线ID单独采集设置
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
			if(ID>=0x0110&&ID<=0x1FF)  //表示为CAN 总线 ID 单独采集设置：
			{
				int index=ID-0x0110;
				
				if(index>=0&&index<CAN_ID_TOTAL)      //目前最多支持50个CAN_ID
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
    u8 total=*pdata++;     //参数总数
    
    
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
    JTT808_Terminal_Param.initial      = 0x01010101;  //初始化标志

    JTT808_Terminal_Param.beat_interval= 30;          //终端心跳发送间隔，单位为秒(s)
    JTT808_Terminal_Param.tcp_timeout  = 30;          //TCP消息应答超时时间，单位为秒(s)
    JTT808_Terminal_Param.tcp_repeat_times=3;         //TCP消息重传次数
    JTT808_Terminal_Param.udp_timeout  = 30;          //UDP消息应答超时时间，单位为秒(s)
    JTT808_Terminal_Param.udp_repeat_times=3;         //UDP消息重传次数
    JTT808_Terminal_Param.sms_timeout  = 30;          //SMS消息应答超时时间，单位为秒(s)
    JTT808_Terminal_Param.sms_repeat_times=3;         //SMS消息重传次数
    
    strcpy((char *)JTT808_Terminal_Param.main_server_apn,"cmnet");                          //主服务器APN，无线通信拨号访问点。若网络制式为CDMA，则该处为PPP拨号号码
    strcpy((char *)JTT808_Terminal_Param.main_server_user,"user");                          //主服务器无线通信拨号用户名
    strcpy((char *)JTT808_Terminal_Param.main_server_psw,"pwd");                            //主服务器无线通信拨号密码
    strcpy((char *)JTT808_Terminal_Param.main_server_domain,"183.62.9.117");             	  //主服务器地址，IP或域名
    strcpy((char *)JTT808_Terminal_Param.backup_server_apn,"#777");                         //备份服务器APN，无线通信拨号访问点
    strcpy((char *)JTT808_Terminal_Param.backup_server_user,"card");                        //备份服务器无线通信拨号用户名
    strcpy((char *)JTT808_Terminal_Param.backup_server_psw,"card");                         //备份服务器无线通信拨号密码
    strcpy((char *)JTT808_Terminal_Param.backup_server_domain,"183.62.9.117");              //备份服务器地址，IP或域名
    strcpy((char *)JTT808_Terminal_Param.server_tcp_port,"6033");                           //服务器TCP端口
    strcpy((char *)JTT808_Terminal_Param.server_udp_port,"6035");                           //服务器UDP端口
    strcpy((char *)JTT808_Terminal_Param.ic_license_authentication_main_server_domain,"");  //道路运输证IC卡认证主服务器IP地址或域名
    strcpy((char *)JTT808_Terminal_Param.ic_license_authentication_main_server_tcp_port,"");//道路运输证IC卡认证主服务器TCP端口
    strcpy((char *)JTT808_Terminal_Param.ic_license_authentication_main_server_udp_port,"");//道路运输证IC卡认证主服务器UDP端口
    strcpy((char *)JTT808_Terminal_Param.ic_license_authentication_backup_server_domain,"");//道路运输证IC卡认证备份服务器IP地址或域名，端口同主服务器
    
    JTT808_Terminal_Param.location_report_strategy=0x00;     //位置汇报策略，0：定时汇报；1：定距汇报；2：定时和定距汇报
    JTT808_Terminal_Param.location_report_program=0x00;      //位置汇报方案，0：根据ACC状态；1：根据登录状态和ACC状态，先判断登录状态，若登录再根据ACC状态
    JTT808_Terminal_Param.report_driver_unlogin_interval=60; //驾驶员未登录汇报时间间隔，单位为秒(s),>0
    
    JTT808_Terminal_Param.report_at_sleep_interval=60;       //休眠时汇报时间间隔，单位为秒(s),>0
    JTT808_Terminal_Param.report_at_alarm_interval=1;        //紧急报警时汇报时间间隔，单位为秒(s),>0
    JTT808_Terminal_Param.default_report_interval=5;         //缺省时间汇报间隔，单位为秒(s),>0
    
    JTT808_Terminal_Param.default_report_dst_interval=300;        //缺省距离汇报间隔，单位为米(m),>0
    JTT808_Terminal_Param.report_driver_unlogin_dst_interval=400; //驾驶员未登录汇报距离间隔，单位为米(m),>0
    JTT808_Terminal_Param.report_at_sleep_dst_interval=1000;      //休眠时汇报距离间隔，单位为米(m),>0
    JTT808_Terminal_Param.report_at_alarm_dst_interval=50;        //紧急报警时汇报距离间隔，单位为米(m),>0
    JTT808_Terminal_Param.inflect_point_complement_biography=15;  //拐点补传角度，<180°
    JTT808_Terminal_Param.bound_radius=30;                        //电子围栏半径（非法位移阀值），单位为米
    
    strcpy((char *)JTT808_Terminal_Param.monitor_platform_tel,"10086");    //监控平台电话号码
    strcpy((char *)JTT808_Terminal_Param.reset_tel,"10086");               //复位电话号码，可采用此电话号码拨打终端电话让终端复位
    strcpy((char *)JTT808_Terminal_Param.restore_factory_tel,"10086");     //恢复出厂设置电话号码，可采用此电话号码拨打终端电话让终端恢复出厂设置
    strcpy((char *)JTT808_Terminal_Param.monitor_platform_sms_tel,"10086");//监控平台SMS电话号码
    strcpy((char *)JTT808_Terminal_Param.sms_alarm_tel,"10086");           //接收终端SMS文本报警号码
    JTT808_Terminal_Param.phone_answer_strategy=0x00;                 //终端电话接听策略，0：自动接听；1：ACC ON时自动接听，OFF时手动接听
    JTT808_Terminal_Param.talk_max_time=0xFFFFFFFF;                   //每次最长通话时间，单位为秒(s),0为不允许通话，0xFFFFFFFF为不限制
    JTT808_Terminal_Param.talk_max_time_in_mon=0xFFFFFFFF;            //当月最长通话时间，单位为秒(s),0为不允许通话，0xFFFFFFFF为不限制
    strcpy((char *)JTT808_Terminal_Param.monitor_tel,"");             //监听电话号码
    strcpy((char *)JTT808_Terminal_Param.monitor_platform_privilege_sms_tel,"10086");//监管平台特权短信号码
    
    JTT808_Terminal_Param.alarm_mask=0x00;              //报警屏蔽字。与位置信息汇报消息中的报警标志相对应，相应位为1则相应报警被屏蔽
    JTT808_Terminal_Param.alarm_sms_switch=0x00;        //报警发送文本SMS开关，与位置信息汇报消息中的报警标志相对应，相应位为1则相应报警时发送文本SMS
    JTT808_Terminal_Param.alarm_shoot_switch=0x00;      //报警拍摄开关，与位置信息汇报消息中的报警标志相对应，相应位为1则相应报警时摄像头拍摄
    JTT808_Terminal_Param.alarm_store_flag=0x00;        //报警拍摄存储标志，与位置信息汇报消息中的报警标志相对应，相应位为1则对相应报警时拍的照片进行存储，否则实时上传
    JTT808_Terminal_Param.key_flag=0x00;                //关键标志，与位置信息汇报消息中的报警标志相对应，相应位为1则对相应报警为关键报警
    JTT808_Terminal_Param.top_speed=180;                //最高速度，单位为公里每小时(km/h)
    JTT808_Terminal_Param.speeding_duration=5;          //超速持续时间，单位为秒(s)
    JTT808_Terminal_Param.continouous_drive_time=0xFFFFFFFF; //连续驾驶时间门限，单位为秒(s)
    JTT808_Terminal_Param.drive_time_in_day=0xFFFFFFFF;      //当天累计驾驶时间门限，单位为秒(s)
    JTT808_Terminal_Param.rest_min_time=1200;           //最小休息时间，单位为秒(s)
    JTT808_Terminal_Param.stop_max_time=0xFFFFFFFF;     //最长停车时间，单位为秒(s)
    JTT808_Terminal_Param.overspeed_alarm_diff=200;     //超速报警预警差值，单位为0.1km/h
    JTT808_Terminal_Param.fatigue_driving_diff=3600;    //疲劳驾驶预警差值，单位为秒(s),>0
    JTT808_Terminal_Param.collision_alarm_param=0x3030; //碰撞报警参数设置：b7-b0:碰撞时间，单位4ms；b15-b8:碰撞加速度，单位0.1g，设置范围在0-79之间，默认为10；
    JTT808_Terminal_Param.rollover_alarm_param=30;      //侧翻报警参数设置：侧翻角度，单位为度，默认为30度；
    
    JTT808_Terminal_Param.tim_photo=0x00;               //定时拍照控制
    JTT808_Terminal_Param.dis_photo=0x00;               //定距拍照控制
    
    JTT808_Terminal_Param.multimedia_quality=5;         //图像(视频)质量，1～10,1最好
    JTT808_Terminal_Param.brightness=127;               //亮度，0～255
    JTT808_Terminal_Param.contrast=63;                  //对比度，0～127
    JTT808_Terminal_Param.saturation=63;                //饱和度，0～127
    JTT808_Terminal_Param.chroma=127;                   //色度，0～255
    
    JTT808_Terminal_Param.mileage=0x00;                         //车辆里程表读数，1/10km
    strcpy((char *)JTT808_Terminal_Param.province_id,"44");     //车辆所在的省域ID，
    strcpy((char *)JTT808_Terminal_Param.city_id,"0100");       //车辆所在的市域ID，
    strcpy((char *)JTT808_Terminal_Param.lic_plate,"粤A88001"); //公安交通管理部门颁发的机动车号牌
    JTT808_Terminal_Param.lic_plate_color=1;                    //车牌颜色，按照JT/T415-2006的5.4.12
    
    JTT808_Terminal_Param.gnss_position_type=0x0F;    //GNSS定位模式，  bit0，0：禁用GPS，1：启用GPS；      bit1，0：禁用北斗，1：启用北斗；    bit2，0：禁用GLONASS，1：启用GLONASS；      bit3，0：禁用Galileo，1：启用Galileo；
    JTT808_Terminal_Param.gnss_baudrate=0x01;         //GNSS波特率，    0x00:4800;  0x01:9600;  0x02:19200; 0x03:38400; 0x04:47600; 0x05:115200;
    JTT808_Terminal_Param.gnss_output_rate=0x01;      //GNSS模块详细定位数据输出频率， 0x00:500ms;  0x01:1000ms(默认);  0x02:2000ms;    0x03:3000ms;    0x04:4000ms;
    JTT808_Terminal_Param.gnss_samp_frequency=0x01;   //GNSS模块详细定位数据采集频率，单位为秒，默认1；
    JTT808_Terminal_Param.gnss_upload_type=0x01;      //GNSS模块详细定位数据上传方式，0x00:本地存储，不上传（默认值）；  0x01：按时间间隔上传；    0x02:按距离间隔上传； 0x0b：按累积时间上传，达到传输时间后自动停止上传；  0x0c：按累积距离上传，达到距离后自动停止上传；  0x0d：按累积条数上传，达到上传条数后自动停止上传；
    JTT808_Terminal_Param.gnss_upload_set=0x01;       //GNSS模块详细定位数据上传设置，0x01:单位为秒；0x02：单位为米；0x0b:单位为秒；0x0c：单位为米；0x0d：单位为条
    
    JTT808_Terminal_Param.can_1_samp_time_interval=0;               //can总线通道1采集时间间隔（ms），0表示不采集
    JTT808_Terminal_Param.can_1_upload_time_interval=0;             //can总线通道1上传时间间隔（s），0表示不上传
    JTT808_Terminal_Param.can_2_samp_time_interval=0;               //can总线通道2采集时间间隔（ms），0表示不采集
    JTT808_Terminal_Param.can_2_upload_time_interval=0;             //can总线通道2上传时间间隔（s），0表示不上传
//    strcpy((char *)JTT808_Terminal_Param.can_ID_samp_set,"");       //can总线ID单独采集设置，bit63-bit32：表示此ID采集时间间隔（ms），0表示不采集；bit31：canID通道号，0-can1，1-can2；bit30：帧类型，0-标准帧，1-扩展帧；bit29：数据采集方式，0-原始数据看，1-采集区间的计算值；bit28-bit0：can总线ID；
//    strcpy((char *)JTT808_Terminal_Param.other_can_ID_samp_set,""); //用于其他can总线ID单独采集设置。
//		memset(JTT808_Terminal_Param.can_ID_samp_set,0x00,CAN_ID_TOTAL*8);

//		for(i=0; i<CAN_ID_TOTAL; i++)
//		{
//				//OBD_Id结构体的赋值.
//				MSG_Id.priority = P6;
//				MSG_Id.exp_data_page = 0;
//				MSG_Id.data_page = 0;
//				MSG_Id.PDU_format = 0xEA;
//				MSG_Id.PDU_specific = 0x00;    //目标地址，或者广播地址
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
    
    strcpy((char *)JTT808_Terminal_Param.terminalID,"8188001");                //终端ID
    strcpy((char *)JTT808_Terminal_Param.local_tel,"13456788001");             //终端手机号
    JTT808_Terminal_Param.wheel_coefficient=0;                                 //车轮系数
    JTT808_Terminal_Param.fuel_tank_capacity=50;                               //油箱容量
    strcpy((char *)JTT808_Terminal_Param.authorization_no,"00000000000000000000000000000000"); //鉴权码

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
	API_vpdn_set((char*)JTT808_Terminal_Param.main_server_apn,(char*)JTT808_Terminal_Param.main_server_user,(char*)JTT808_Terminal_Param.main_server_psw);		 //设置apn参数
	API_server_ip_port_set(0,JTT808_Terminal_Param.main_server_domain,JTT808_Terminal_Param.server_tcp_port,0);								 //设置IP、端口参数
}


void jtt808_read_para(void)
{
    ReadFlash(JTT808_PARAM_ADDRESS,(u8*)&JTT808_Terminal_Param,sizeof(JTT808_Terminal_Param));
}

void jtt808_save_para(void)
{
    WriteFlash(JTT808_PARAM_ADDRESS,(u8*)&JTT808_Terminal_Param,sizeof(JTT808_Terminal_Param));
}





