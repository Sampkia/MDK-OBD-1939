#include "include.h"
#include "common.h"

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

                    ioUart.printf(USART1, (uint8_t *)"\r\nThe recive data of USART1:\r\n");
                    for(i=0; i<tmpserial.index; i++)
                    {
                        ioUart.printf(USART1, (uint8_t *)"%p ",tmpserial.buff[i]);
                    }
                    ioUart.printf(USART1,  (uint8_t *)"\r\n");

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

    for(j=0; j<len; j++)//ת�廹ԭ
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

        k=DecodeSerialData(tmpserial.buff, buf_tmp1, tmpserial.index);			//ת�廹ԭ��У�� ,����buf_tmp1��
        if(k!=0) 
        {
            for(i=0,j=10;j<k; i++,j++)
            {
                buf_tmp2[i] = buf_tmp1[j];					//�û�����
            }

            switch(buf_tmp1[6])
            {
                case 0x01:													/* ��ҵ��Ϣ�ն˻� */
                break;
                case 0x02: 													/* ������ʾ�� */
                    switch(buf_tmp1[8])
                    {
                        case 0x01: 								  /* �����ն�ID */
                            strcpy((char *)JTT808_Terminal_Param.terminalID,(const char*)buf_tmp2);
                            jtt808_save_para();
                        break;
                        case 0x02:  								/* ���÷������˿� */
                            itoa((int)(buf_tmp2[0]<<8|buf_tmp2[1]), (char *)buf_tmp3, 10);
                            strcpy((char *)JTT808_Terminal_Param.server_tcp_port,(const char*)buf_tmp3);
														jtt808_save_para();
#if USE_TL
							control_center_ip_port_set(JTT808_Terminal_Param.main_server_domain,JTT808_Terminal_Param.server_tcp_port);
#endif
                            
                        break;
                        case 0x03:  								/*������������IP*/;
                            strcpy((char *)JTT808_Terminal_Param.main_server_domain,(const char*)buf_tmp2);
														jtt808_save_para();
#if USE_TL
							control_center_ip_port_set(JTT808_Terminal_Param.main_server_domain,JTT808_Terminal_Param.server_tcp_port);
#endif
                            
                        break;
                        case 0x04:  								/*������APN*/
                            strcpy((char *)JTT808_Terminal_Param.main_server_apn,(const char*)buf_tmp2);
                            jtt808_save_para();
                        break;
                        case 0x05:  								/*�����û���*/
                            strcpy((char *)JTT808_Terminal_Param.main_server_user,(const char*)buf_tmp2);
                            jtt808_save_para();
                        break;
                        case 0x06:  								/*��������*/
                            strcpy((char *)JTT808_Terminal_Param.main_server_psw,(const char*)buf_tmp2);
                            jtt808_save_para();
                        break;
                        case 0x07:  								/*����ʡ��ID*/
                            strcpy((char *)JTT808_Terminal_Param.province_id,(const char*)buf_tmp2);
                            jtt808_save_para();
                        break;
                        case 0x08:  								/*��������ID*/
                            strcpy((char *)JTT808_Terminal_Param.city_id,(const char*)buf_tmp2);
                            jtt808_save_para();
                        break;
                        case 0x09:  								/*���ó�����ɫ*/
                            JTT808_Terminal_Param.lic_plate_color=buf_tmp2[0];
                            jtt808_save_para();
                        break;
                        case 0x0A: 								 	/*���ó��ƺ���*/
                            strcpy((char *)JTT808_Terminal_Param.lic_plate,(const char*)buf_tmp2);
														jtt808_save_para();
                        break;
                        case 0x0B:  								/*���ñ���SIM����*/
                            strcpy((char *)JTT808_Terminal_Param.local_tel,(const char*)buf_tmp2);
                            jtt808_save_para();
                        break;
                        case 0x0C:  								/*���ü������SIM����*/
                            strcpy((char *)JTT808_Terminal_Param.monitor_platform_tel,(const char*)buf_tmp2);
                            jtt808_save_para();
                        break;
                        case 0x0D:  								/*���ò���ϵ��*/
                            JTT808_Terminal_Param.wheel_coefficient=buf_tmp2[0]<<8|buf_tmp2[1];
                            jtt808_save_para();
                        break;
                        case 0x0E:  								/*�������*/
                            JTT808_Terminal_Param.mileage=(buf_tmp2[0]<<24|buf_tmp2[1]<<16|buf_tmp2[2]<<8|buf_tmp2[3])*10;
                            jtt808_save_para();
                        break;
                        case 0x0F:  								/*������������*/
                            JTT808_Terminal_Param.fuel_tank_capacity=buf_tmp2[0]<<8|buf_tmp2[1];
                            jtt808_save_para();
                        break;
                        case 0x10:  								/*��������*/
                            strcpy((char *)JTT808_Terminal_Param.main_server_domain,(const char*)buf_tmp2);
                            jtt808_save_para();
                        break;
#if USE_TL
						case 0x71:															/*������������*/
							JTT808_Terminal_Param.TL_network_type=buf_tmp2[0];
							_3g_switch_set(JTT808_Terminal_Param.TL_network_type);
							jtt808_save_para();
						break;
						case 0x72:															/*������Ƶ������IP*/
							strcpy((char *)JTT808_Terminal_Param.TL_video_ip,(const char*)buf_tmp2);
							service_center_set(JTT808_Terminal_Param.TL_video_ip,JTT808_Terminal_Param.TL_video_control_port,JTT808_Terminal_Param.TL_video_data_port);
							jtt808_save_para();
						break;
						case 0x73:															/*������Ƶ���������ƶ˿�*/
							strcpy((char *)JTT808_Terminal_Param.TL_video_control_port,(const char*)buf_tmp2);
							service_center_set(JTT808_Terminal_Param.TL_video_ip,JTT808_Terminal_Param.TL_video_control_port,JTT808_Terminal_Param.TL_video_data_port);
							jtt808_save_para();
						break;
						case 0x74:															/*������Ƶ���������ݶ˿�*/
							strcpy((char *)JTT808_Terminal_Param.TL_video_data_port,(const char*)buf_tmp2);
							service_center_set(JTT808_Terminal_Param.TL_video_ip,JTT808_Terminal_Param.TL_video_control_port,JTT808_Terminal_Param.TL_video_data_port);
							jtt808_save_para();
						break;
#endif
                        case 0x31:    
                        break;
                        case 0x30:
                        break;
                        case 0x33:
                        break;
                        case 0x32:
                        break;

                        case 0x11:  								/* ��ȡ�ն�ID */
                            strcpy((char*)buf_tmp4,(const char*)JTT808_Terminal_Param.terminalID);
                            paralen=strlen((const char*)buf_tmp4);
                        break;
                        case 0x12:  								/* ��ȡ�������˿� */
                            strcpy((char*)buf_tmp4,(const char*)JTT808_Terminal_Param.server_tcp_port);
#if	USE_TL
							control_center_ip_port_check();
#endif
                            i=atoi((const char*)buf_tmp4);
                            buf_tmp4[0]=i>>8;
                            buf_tmp4[1]=i&0xff;
                            buf_tmp4[2]=0;
                            paralen=2;
                        break;
                        case 0x13:  								/*��ȡ��������IP*/;
                            strcpy((char*)buf_tmp4,(const char*)JTT808_Terminal_Param.main_server_domain);
#if	USE_TL
							control_center_ip_port_check();
#endif
                            paralen=strlen((const char*)buf_tmp4);
                        break;
                        case 0x14: 								 	/*��ȡ��APN*/
                            strcpy((char*)buf_tmp4,(const char*)JTT808_Terminal_Param.main_server_apn);
                            paralen=strlen((const char*)buf_tmp4);
                        break;
                        case 0x15:  								/*��ȡ�û���*/
                            strcpy((char*)buf_tmp4,(const char*)JTT808_Terminal_Param.main_server_user);
                            paralen=strlen((const char*)buf_tmp4);
                        break;
                        case 0x16:  								/*��ȡ����*/
                            strcpy((char*)buf_tmp4,(const char*)JTT808_Terminal_Param.main_server_psw);
                            paralen=strlen((const char*)buf_tmp4);
                        break;
                        case 0x17:  								/*��ȡʡ��ID*/
                            strcpy((char*)buf_tmp4,(const char*)JTT808_Terminal_Param.province_id);
                            paralen=strlen((const char*)buf_tmp4);
                        break;
                        case 0x18:  								/*��ȡ����ID*/
                            strcpy((char*)buf_tmp4,(const char*)JTT808_Terminal_Param.city_id);
                            paralen=strlen((const char*)buf_tmp4);
                        break;
                        case 0x19:  								/*��ȡ������ɫ*/
                            buf_tmp4[0]=JTT808_Terminal_Param.lic_plate_color;
                            buf_tmp4[1]=0;
                            paralen=1;
                        break;
                        case 0x1A:  								/*��ȡ���ƺ���*/
                            strcpy((char*)buf_tmp4,(const char*)JTT808_Terminal_Param.lic_plate);       
		                    paralen=strlen((const char*)buf_tmp4);
                        break;
                        case 0x1B:  								/*��ȡ����SIM����*/
                            strcpy((char*)buf_tmp4,(const char*)JTT808_Terminal_Param.local_tel);
                            paralen=strlen((const char*)buf_tmp4);
                        break;
                        case 0x1C:  								/*��ȡ�������SIM����*/
                            strcpy((char*)buf_tmp4,(const char*)JTT808_Terminal_Param.monitor_platform_tel);
                            paralen=strlen((const char*)buf_tmp4);
                        break;
                        case 0x1D:  								/*��ȡ����ϵ��*/
                            buf_tmp4[0]=(JTT808_Terminal_Param.wheel_coefficient>>8)&0xFF;
                            buf_tmp4[1]=JTT808_Terminal_Param.wheel_coefficient&0xFF;
                            buf_tmp4[2]=0;
                            paralen=2;
                        break;
                        case 0x1E:  								/*��ȡ���*/
                            buf_tmp4[0]=((JTT808_Terminal_Param.mileage/10)>>24)&0xFF;
                            buf_tmp4[1]=((JTT808_Terminal_Param.mileage/10)>>16)&0xFF;
                            buf_tmp4[2]=((JTT808_Terminal_Param.mileage/10)>>8)&0xFF;
                            buf_tmp4[3]=(JTT808_Terminal_Param.mileage/10)&0xFF;
                            buf_tmp4[4]=0;
                            paralen=4;
                        break;
                        case 0x1F:  								/*��ȡ��������*/
                            buf_tmp4[0]=(JTT808_Terminal_Param.fuel_tank_capacity>>8)&0xFF;
                            buf_tmp4[1]=JTT808_Terminal_Param.fuel_tank_capacity&0xFF;
                            buf_tmp4[2]=0;
                            paralen=2;
                        break;

                        case 0x20:  								/*��ȡ����*/
                            strcpy((char*)buf_tmp4,(const char*)JTT808_Terminal_Param.main_server_domain);
                            paralen=strlen((const char*)buf_tmp4);
                        break;
                        case 0x21:
                        break;
                        case 0x22:
                        break;
                        case 0x23:
                        break;

                        case 0x38:   								/*�ն�ע��*/
                            strcpy((char *)JTT808_Terminal_Param.authorization_no,"00000000000000000000000000000000");
                            jtt808_save_para();
                            //JTT808_TerminalDereg();
                            NVIC_SystemReset();    
                        break;

#if USE_TL
						case 0x81:														/*��ȡ��������*/
							buf_tmp4[0]=JTT808_Terminal_Param.TL_network_type;
                            buf_tmp4[1]=0;
							_3g_switch_check();
                            paralen=1;
						break;
						case 0x82:	/*��ȡ��Ƶ������IP*/ 
							strcpy((char*)buf_tmp4,(const char*)JTT808_Terminal_Param.TL_video_ip);
							service_center_check();
                            paralen=strlen((const char*)buf_tmp4);
						break;
						case 0x83:	/*��ȡ��Ƶ���������ƶ˿�*/
							strcpy((char*)buf_tmp4,(const char*)JTT808_Terminal_Param.TL_video_control_port);
							service_center_check();
                            paralen=strlen((const char*)buf_tmp4);
						break;
						case 0x84:	/*��ȡ��Ƶ���������ݶ˿�*/
							strcpy((char*)buf_tmp4,(const char*)JTT808_Terminal_Param.TL_video_data_port);
							service_center_check();
                            paralen=strlen((const char*)buf_tmp4);
						break;
#endif

                        
                    }
                break;
                case 0x03:  /* ���ص�����ʾ�� */
                break;
                case 0x04:  /* ��������� */
                break;
                case 0x05:  /* ���ٶȼ���� */
                break;
                case 0x06:  /* ���������� */
                break;
                case 0x07:  /* �ӿ���չ�� */
                break;
                case 0x08:  /* ���ؼ���� */
                break;
                case 0x09:  /* ��������� */
                break;
                case 0x0A:  /* ͨ�ô����� */
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
