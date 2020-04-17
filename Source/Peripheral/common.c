#include "include.h"

TmpSerialData_Type tmpserial={0};


unsigned char Getccommon1Data(void)
{
    unsigned int size=0,i;
    char val;
    static unsigned char count=0;

    size = ioUart.rxBuffCount(1);

    if(ioUart.rxBuffOverFlow(1))
    {
        ioUart.rxBuffRst(1);
        tmpserial.index=0;
        return 0;
    }

    while(size--)
    {
        val = ioUart.getChar(1);

        switch(val)
        {
            case 0x7e:
                count++;
                if(count==1)
                {
                    tmpserial.index=0;
                    memset(tmpserial.buff,0x00,SERIALDATA_SIZE);
                    tmpserial.buff[tmpserial.index++]=val;
                }
                else if(count==2)
                {
                    count=0;
                    tmpserial.buff[tmpserial.index++]=val;
                    tmpserial.buff[tmpserial.index]=0;

                    ioUart.printf(USART1, "\r\n串口1接收到数据:\r\n");
                    for(i=0; i<tmpserial.index; i++)
                    {
                        ioUart.printf(USART1, "%p ",tmpserial.buff[i]);
                    }
                    ioUart.printf(USART1, "\r\n");

                    return 1;
                }
                break;
            default:
                if(count==1)
                {
                    tmpserial.buff[tmpserial.index++]=val;
                }
                else
                {
                    tmpserial.index=0;
                }
        }
    }
    return 0;
}



unsigned int  DecodeSerialData(u8* src,u8* dest,int len)
{
    int i=0,j=0;
    u8 verify=0;

    for(j=0; j<len; j++)//转义还原
    {
        if( (src[j]==0x7D) && (src[j+1]==0x01) )
        {
            dest[i++] = 0x7D;
            j++;
        }
        else if( (src[j]==0x7D) && (src[j+1]==0x02) )
        {
            dest[i++] = 0x7E;
            j++;
        }
        else
        {
            dest[i++] = src[j];
        }
    }
    dest[i]=0;

    for(i=4; i<j-1; i++)
    {
        verify += dest[i];
    }

    if(verify == dest[1])
    {
        return i;
    }

    return 0;
}

static int EncodeSerialData(u8* src, u8 *dest,int len)
{
    int i=0,j=0,k=0;
    u8 verify=0;

    for(k=4;k<len-1; k++)
    {
        verify += src[k];
    }
    src[1]=verify;

    dest[0]=src[0];

    for(i=1,j=1;j<len-1;i++,j++)
    {
        if(src[j]==0x7E)
        {
            dest[i++]=0x7D;
            dest[i]=0x02;
        }
        else if(src[j]==0x7D)
        {
            dest[i++]=0x7D;
            dest[i]=0x01;
        }
        else
        {
            dest[i]=src[j];
        }
    }
    dest[i++]=src[j++];
    dest[i]=0;
    return i;
}



void CCommon1Process(void)
{
    if(Getccommon1Data() == 1)
    {
        u8 buf_tmp1[100]={0},buf_tmp2[100]={0},buf_tmp3[100]={0},buf_tmp4[100]={0};
        u32 paralen=0,i,j,k;

        k=DecodeSerialData(tmpserial.buff,buf_tmp1,tmpserial.index);//转义还原、校验 ,放在buf_tmp1里
        if(k!=0) 
        {
            for(i=0,j=10;j<k; i++,j++)
            {
                buf_tmp2[i] = buf_tmp1[j];//用户数据
            }

            switch(buf_tmp1[6])
            {
                case 0x01:/* 行业信息终端机 */
                break;
                case 0x02: /* 调度显示屏 */
                    switch(buf_tmp1[8])
                    {
                        case 0x01:  /* 设置终端ID */
                            strcpy((char *)JTT808_Terminal_Param.terminalID,(const char*)buf_tmp2);
                            JTT808_rereg();
                        break;
                        case 0x02:  /* 设置服务器端口 */
                            itoa((int)(buf_tmp2[0]<<8|buf_tmp2[1]), (char *)buf_tmp3, 10);
                            strcpy((char *)JTT808_Terminal_Param.server_tcp_port,(const char*)buf_tmp3);
							JTT808_rereg();    
							API_server_ip_port_set(0,JTT808_Terminal_Param.main_server_domain,JTT808_Terminal_Param.server_tcp_port,0);	                   
                        break;
                        case 0x03:  /*设置主服务器IP*/;
                            strcpy((char *)JTT808_Terminal_Param.main_server_domain,(const char*)buf_tmp2);
							JTT808_rereg();  
							API_server_ip_port_set(0,JTT808_Terminal_Param.main_server_domain,JTT808_Terminal_Param.server_tcp_port,0);	                       
                        break;
                        case 0x04:  /*设置主APN*/
                            strcpy((char *)JTT808_Terminal_Param.main_server_apn,(const char*)buf_tmp2);
                            jtt808_save_para();
							API_vpdn_set((char*)JTT808_Terminal_Param.main_server_apn,(char*)JTT808_Terminal_Param.main_server_user,(char*)JTT808_Terminal_Param.main_server_psw);		 //设置apn参数
                        break;
                        case 0x05:  /*设置用户名*/
                            strcpy((char *)JTT808_Terminal_Param.main_server_user,(const char*)buf_tmp2);
                            jtt808_save_para();
							API_vpdn_set((char*)JTT808_Terminal_Param.main_server_apn,(char*)JTT808_Terminal_Param.main_server_user,(char*)JTT808_Terminal_Param.main_server_psw);		 //设置apn参数
                        break;
                        case 0x06:  /*设置密码*/
                            strcpy((char *)JTT808_Terminal_Param.main_server_psw,(const char*)buf_tmp2);
                            jtt808_save_para();
							API_vpdn_set((char*)JTT808_Terminal_Param.main_server_apn,(char*)JTT808_Terminal_Param.main_server_user,(char*)JTT808_Terminal_Param.main_server_psw);		 //设置apn参数
                        break;
                        case 0x07:  /*设置省域ID*/
                            strcpy((char *)JTT808_Terminal_Param.province_id,(const char*)buf_tmp2);
                            JTT808_rereg();
                        break;
                        case 0x08:  /*设置市域ID*/
                            strcpy((char *)JTT808_Terminal_Param.city_id,(const char*)buf_tmp2);
                            JTT808_rereg();
                        break;
                        case 0x09:  /*设置车牌颜色*/
                            JTT808_Terminal_Param.lic_plate_color=buf_tmp2[0];
                            JTT808_rereg();
                        break;
                        case 0x0A:  /*设置车牌号码*/
                            strcpy((char *)JTT808_Terminal_Param.lic_plate,(const char*)buf_tmp2);
							JTT808_rereg();
                        break;
                        case 0x0B:  /*设置本地SIM卡号*/
                            strcpy((char *)JTT808_Terminal_Param.local_tel,(const char*)buf_tmp2);
                            JTT808_rereg();
                        break;
                        case 0x0C:  /*设置监控中心SIM卡号*/
                            strcpy((char *)JTT808_Terminal_Param.monitor_platform_tel,(const char*)buf_tmp2);
                            jtt808_save_para();
                        break;
                        case 0x0D:  /*设置测速系数*/
                            JTT808_Terminal_Param.wheel_coefficient=buf_tmp2[0]<<8|buf_tmp2[1];
                            jtt808_save_para();
                        break;
                        case 0x0E:  /*设置里程*/
                            JTT808_Terminal_Param.mileage=(buf_tmp2[0]<<24|buf_tmp2[1]<<16|buf_tmp2[2]<<8|buf_tmp2[3])*10;
                            jtt808_save_para();
                        break;
                        case 0x0F:  /*设置油箱容量*/
                            JTT808_Terminal_Param.fuel_tank_capacity=buf_tmp2[0]<<8|buf_tmp2[1];
                            jtt808_save_para();
                        break;
                        case 0x10:  /*设置域名*/
                            strcpy((char *)JTT808_Terminal_Param.main_server_domain,(const char*)buf_tmp2);
                            jtt808_save_para();
							API_server_ip_port_set(0,JTT808_Terminal_Param.main_server_domain,JTT808_Terminal_Param.server_tcp_port,0);	
                        break;

                        case 0x31:    
                        break;
                        case 0x30:
                        break;
                        case 0x33:
                        break;
                        case 0x32:
                        break;

                        case 0x11:  /* 读取终端ID */
                            strcpy((char*)buf_tmp4,(const char*)JTT808_Terminal_Param.terminalID);
                            paralen=strlen((const char*)buf_tmp4);
                        break;
                        case 0x12:  /* 读取服务器端口 */
                            strcpy((char*)buf_tmp4,(const char*)JTT808_Terminal_Param.server_tcp_port);
                            i=atoi((const char*)buf_tmp4);
                            buf_tmp4[0]=i>>8;
                            buf_tmp4[1]=i&0xff;
                            buf_tmp4[2]=0;
                            paralen=2;
                        break;
                        case 0x13:  /*读取主服务器IP*/;
                            strcpy((char*)buf_tmp4,(const char*)JTT808_Terminal_Param.main_server_domain);
                            paralen=strlen((const char*)buf_tmp4);
                        break;
                        case 0x14:  /*读取主APN*/
                            strcpy((char*)buf_tmp4,(const char*)JTT808_Terminal_Param.main_server_apn);
                            paralen=strlen((const char*)buf_tmp4);
                        break;
                        case 0x15:  /*读取用户名*/
                            strcpy((char*)buf_tmp4,(const char*)JTT808_Terminal_Param.main_server_user);
                            paralen=strlen((const char*)buf_tmp4);
                        break;
                        case 0x16:  /*读取密码*/
                            strcpy((char*)buf_tmp4,(const char*)JTT808_Terminal_Param.main_server_psw);
                            paralen=strlen((const char*)buf_tmp4);
                        break;
                        case 0x17:  /*读取省域ID*/
                            strcpy((char*)buf_tmp4,(const char*)JTT808_Terminal_Param.province_id);
                            paralen=strlen((const char*)buf_tmp4);
                        break;
                        case 0x18:  /*读取市域ID*/
                            strcpy((char*)buf_tmp4,(const char*)JTT808_Terminal_Param.city_id);
                            paralen=strlen((const char*)buf_tmp4);
                        break;
                        case 0x19:  /*读取车牌颜色*/
                            buf_tmp4[0]=JTT808_Terminal_Param.lic_plate_color;
                            buf_tmp4[1]=0;
                            paralen=1;
                        break;
                        case 0x1A:  /*读取车牌号码*/
                            strcpy((char*)buf_tmp4,(const char*)JTT808_Terminal_Param.lic_plate);       
		                    paralen=strlen((const char*)buf_tmp4);
                        break;
                        case 0x1B:  /*读取本地SIM卡号*/
                            strcpy((char*)buf_tmp4,(const char*)JTT808_Terminal_Param.local_tel);
                            paralen=strlen((const char*)buf_tmp4);
                        break;
                        case 0x1C:  /*读取监控中心SIM卡号*/
                            strcpy((char*)buf_tmp4,(const char*)JTT808_Terminal_Param.monitor_platform_tel);
                            paralen=strlen((const char*)buf_tmp4);
                        break;
                        case 0x1D:  /*读取测速系数*/
                            buf_tmp4[0]=(JTT808_Terminal_Param.wheel_coefficient>>8)&0xFF;
                            buf_tmp4[1]=JTT808_Terminal_Param.wheel_coefficient&0xFF;
                            buf_tmp4[2]=0;
                            paralen=2;
                        break;
                        case 0x1E:  /*读取里程*/
                            buf_tmp4[0]=((JTT808_Terminal_Param.mileage/10)>>24)&0xFF;
                            buf_tmp4[1]=((JTT808_Terminal_Param.mileage/10)>>16)&0xFF;
                            buf_tmp4[2]=((JTT808_Terminal_Param.mileage/10)>>8)&0xFF;
                            buf_tmp4[3]=(JTT808_Terminal_Param.mileage/10)&0xFF;
                            buf_tmp4[4]=0;
                            paralen=4;
                        break;
                        case 0x1F:  /*读取邮箱容量*/
                            buf_tmp4[0]=(JTT808_Terminal_Param.fuel_tank_capacity>>8)&0xFF;
                            buf_tmp4[1]=JTT808_Terminal_Param.fuel_tank_capacity&0xFF;
                            buf_tmp4[2]=0;
                            paralen=2;
                        break;

                        case 0x20:  /*读取域名*/
                            strcpy((char*)buf_tmp4,(const char*)JTT808_Terminal_Param.main_server_domain);
                            paralen=strlen((const char*)buf_tmp4);
                        break;
                        case 0x21:
                        break;
                        case 0x22:
                        break;
                        case 0x23:
                        break;

                        case 0x38:   /*终端注销*/
                            JTT808_rereg();
                            NVIC_SystemReset();    
                        break;


                        
                    }
                break;
                case 0x03:  /* 车载导航显示屏 */
                break;
                case 0x04:  /* 油量检测器 */
                break;
                case 0x05:  /* 加速度检测器 */
                break;
                case 0x06:  /* 防盗报警器 */
                break;
                case 0x07:  /* 接口扩展器 */
                break;
                case 0x08:  /* 载重检测器 */
                break;
                case 0x09:  /* 客流检测器 */
                break;
                case 0x0A:  /* 通用传感器 */
                break;
                default:
                break;
            }
            if(paralen!=0)
            {
                int i=0,txlen=0;
                u8 buf_back[100]={0},buf_up[100]={0};
                for(i=0;i<9;i++)
                {
                    buf_back[i]=buf_tmp1[i];
                }
                buf_back[9]=paralen;
                for(i=0;i<paralen;i++)
                {
                    buf_back[10+i]=buf_tmp4[i];
                }
                buf_back[10+paralen]=0x7e;
                txlen=EncodeSerialData(buf_back,buf_up,11+paralen);
                ioUart.putChars(1, buf_up, txlen);
            }
        } 
    }
}
