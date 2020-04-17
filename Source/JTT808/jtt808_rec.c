#include "include.h"

u8 to_lock_car=0;

void JTT808_terminal_act(u16 serial_no,u16 msg_id,char res)  //�ն�ͨ��Ӧ��
{
	u8 *msg_buf=jtt808_global_buf;
    JTT808t_ACT act={0};
    JTT808t_MsgPackage package={0};

    act.serial_no=htons(serial_no);
    act.ACT_id=htons(msg_id);
    act.ret=res;
    memcpy(msg_buf,(char*)&act,sizeof(JTT808t_ACT));
    JTT808_MsgPacket(JTT808_CMD_TERMINAL_ACT,msg_buf,sizeof(JTT808t_ACT),package,0);
}


void JTT808_PlatformACT(JTT808t_Msg* msg)               //0x8001	ƽ̨ͨ��Ӧ��
{
    JTT808t_ACT act={0};

    memcpy((char*)&act,msg->body,msg->head.property.size);
    act.serial_no=ntohs(act.serial_no);                //��С��ת��
    act.ACT_id=ntohs(act.ACT_id);                      //��С��ת��

    printf("ƽ̨ͨ��Ӧ��  Ӧ����ˮ��:0x%04X  Ӧ����ϢID:0x%04X  ������:%d\r\n",act.serial_no,act.ACT_id,act.ret);

    switch(act.ACT_id)
    {
        case JTT808_CMD_TERMINAL_BEAT:                 //�ն�����        0x0002           
        break;
        case JTT808_CMD_TERMINAL_DEREG:                //�ն�ע��        0x0003
            if(act.ret==JTT808_ACT_RET_OK)             
            {
                memset(JTT808_Terminal_Param.authorization_no,'0',sizeof(JTT808_Terminal_Param.authorization_no));
                jtt808_save_para();                                    //���ü�Ȩ��
                printf("\r\n ע���ɹ���\r\n");
                NVIC_SystemReset();
            }
            else printf("\r\n ע��ʧ�ܣ�\r\n");
        break;
        case JTT808_CMD_TERMINAL_AUTHENTICATION:       //�ն˼�Ȩ        0x0102
			{
				static u8 tim=0;

	            if(act.ret==JTT808_ACT_RET_OK)
	            {
									tim=0;
	                printf("\r\n ��Ȩ�ɹ���\r\n");
	                JTT808_Flag_Authentication=1;
	            }
	            else
	            {
									tim++;
	                printf("\r\n ��Ȩʧ�ܣ�\r\n");
	                JTT808_Flag_Authentication=0;
									if(tim>=10)
									{
											tim=0;
											printf("\r\n����10�μ�Ȩʧ�ܣ�����ע�ᣡ��\r\n");
											JTT808_rereg();   
									}
	            }
			}
        break;
        case JTT808_CMD_LOCATION_REPORT:               //λ����Ϣ�㱨    0x0200
        break;

        default:
        break;
    }
}


void JTT808_TerminalRegAct(JTT808t_Msg* msg)            //0x8100	�ն�ע��Ӧ��
{
    u8 *pbody=msg->body;
//    u16 act_serial_no;
    u8 res;

//    act_serial_no=ntohs(*(u16*)pbody);
    pbody+=2;
    res=*pbody++;

    if(res==0)//ע��ɹ�
    {
        printf("\r\n ע��ɹ���\r\n");
        memset(JTT808_Terminal_Param.authorization_no,0x00,sizeof(JTT808_Terminal_Param.authorization_no));
        strncpy((char*)JTT808_Terminal_Param.authorization_no,(char*)pbody,msg->head.property.size-3);
        jtt808_save_para();
    }
    else
    {
        printf("\r\n ע��ʧ�ܣ�\r\n");
    }
}

void JTT808_SetTerminalParam(JTT808t_Msg* msg)          //0x8103	�����ն˲���
{
    JTT808_Set_Terminal_Param(msg->body, msg->head.property.size);

    JTT808_terminal_act(msg->head.serial_no,msg->head.id,JTT808_ACT_RET_OK); //�����ն�ͨ��Ӧ��
}


void JTT808_Queryterminalparam(JTT808t_Msg* msg)        //0x8104	��ѯ�ն˲���
{
    u8 *p_msg_buf=jtt808_global_buf;
    u32 size=3;             //Ԥ��3���ֽ�
    u8 total=0;
    u32 ID=0;

    u16 serial_no;
    JTT808t_MsgPackage package={0};

    p_msg_buf+=3;           //Ԥ��3���ֽ�

    while(ID<=0x0111)
    {
        signed char rec;

        rec = JTT808_Param_exec(ID,&size,p_msg_buf,0);
        ID++;

        if(rec==0)
        {
            p_msg_buf+=(5+p_msg_buf[4]);
            total++;
        }
        else if(rec==-1)
        {
            continue;
        }
    }

    serial_no=htons(msg->head.serial_no);
    memcpy(jtt808_global_buf,(char*)&serial_no,2);

    memcpy(&jtt808_global_buf[2],&total,1);

    JTT808_MsgPacket(JTT808_CMD_QUERY_TERMINAL_PARAM_ACT,jtt808_global_buf,size,package,0);   
}


void JTT808_Queryappointterminalparam(JTT808t_Msg* msg) //0x8106    ��ѯָ���ն˲���
{
    u8 *p_msg_buf=jtt808_global_buf,*pbody=msg->body;

    u32 size=3;             //Ԥ��3���ֽ�,��Ϣ���ܴ�С
    u8 total_rec=*pbody++;  //���յ���Ҫ��ѯ�Ĳ�������
    u8 total_send=0;        //�������ϱ��Ĳ�������
    

    u16 serial_no=htons(msg->head.serial_no);  //Ӧ����Ϣ��ˮ�� ��С��ת��
    JTT808t_MsgPackage package={0};            //�ְ�

    p_msg_buf+=3;           //Ԥ��3���ֽ�

    while(total_rec--)
    {
        signed char rec;                                       //״̬��������0ʱ��ʾ�иò���������-1ʱ��ʾ�޸ò���
        u32 ID=ntohl(*(u32*)pbody);                     //ȡ������ID
        pbody+=4;

        rec = JTT808_Param_exec(ID,&size,p_msg_buf,0);

        if(rec==0)
        {
            p_msg_buf+=(5+p_msg_buf[4]);
            total_send++;                              //���ز�������++
        }
        else if(rec==-1)
        {
            continue;
        }
    }

    memcpy(jtt808_global_buf,(char*)&serial_no,2);
    memcpy(&jtt808_global_buf[2],&total_send,1);

    JTT808_MsgPacket(JTT808_CMD_QUERY_TERMINAL_PARAM_ACT,jtt808_global_buf,size,package,0); 
}



void JTT808_Terminalctrl(JTT808t_Msg* msg)              //0x8105	�ն˿���
{
    char *pbody=msg->body,cmd=*pbody++;

    switch(cmd)
    {
        case 1:        //��������
        break;
        case 2:        //�����ն�����ָ���ķ�����
        break;
        case 3:        //�ն˹ػ�
            Terminal_PowerDown();
        break;
        case 4:        //�ն˸�λ
            NVIC_SystemReset();
        break;
        case 5:        //�ָ���������
            JTT808_ParamRestoreFactory();
            NVIC_SystemReset();
        break;
        case 6:        //�ر�����ͨ��
            JTT808_Gprs_Disconnect();
        break;
        case 7:        //�ر���������ͨ��
            JTT808_Disconnect_All();
        break;
        default:
        break;
    }
	JTT808_terminal_act(msg->head.serial_no,msg->head.id,JTT808_ACT_RET_OK); //�����ն�ͨ��Ӧ��
}


void JTT808_Queryterminalproperty(JTT808t_Msg* msg)     //0x8107    ��ѯ�ն�����
{
    u8 *msg_buf=jtt808_global_buf,*pdata=msg_buf;
    u32  msg_buf_len=0;
    JTT808t_MsgPackage package={0};                     //�ְ�

    u16 terminal_type=htons(TERMINAL_TYPE);

    memcpy(pdata,(char*)&terminal_type,2);
    pdata+=2;
    msg_buf_len+=2;                                //�ն�����

    strcpy((char*)pdata,PRODUCTOR_ID);
    pdata+=5;
    msg_buf_len+=5;                                //������ID

    strcpy((char*)pdata,MODULE_TYPE);
    pdata+=20;
    msg_buf_len+=20;                               //�ն��ͺ�

    memcpy(pdata,JTT808_Terminal_Param.terminalID,7);
    pdata+=7;
    msg_buf_len+=7;                                //�ն�ID

	getBCDFromStr((unsigned char*)pdata,(char*)SIM_ICCID);
    pdata+=10;
    msg_buf_len+=10;                               //�ն�SIM��ICCID

    memset(pdata,strlen((const char*)HARDWARE_VERSION),1);
    pdata+=1;
    msg_buf_len+=1;                                //�ն�Ӳ���汾�ų���

    memcpy(pdata,HARDWARE_VERSION,strlen((const char*)HARDWARE_VERSION));
    pdata+=strlen((const char*)HARDWARE_VERSION);
    msg_buf_len+=strlen((const char*)HARDWARE_VERSION);         //�ն�Ӳ���汾��

    memset(pdata,strlen((const char*)SOFTWARE_VERSION),1);
    pdata+=1;
    msg_buf_len+=1;                                //�ն˹̼��汾�ų���

    memcpy(pdata,SOFTWARE_VERSION,strlen((const char*)SOFTWARE_VERSION));
    pdata+=strlen((const char*)SOFTWARE_VERSION);
    msg_buf_len+=strlen((const char*)SOFTWARE_VERSION);         //�ն˹̼��汾��

    memset(pdata,GPS_MODULE_TYPE,1);
    pdata+=1;
    msg_buf_len+=1;                                //GNSSģ������

    memset(pdata, GPRS_MODULE_TYPE, 1);
    pdata+=1;
    msg_buf_len+=1;

    JTT808_MsgPacket(JTT808_CMD_QUERY_TERMINAL_PROPERTY_ACT,msg_buf,msg_buf_len,package,0);
}


void JTT808_Locationquery(JTT808t_Msg* msg)             //0x8201	λ����Ϣ��ѯ
{
    u8 *msg_buf=jtt808_global_buf;
    u32  msg_buf_len=0;
    JTT808t_MsgPackage package={0};                    //�ְ�
    u16 seral_no=htons(msg->head.serial_no);           //Ӧ����ˮ��

    memcpy(msg_buf,(char*)&seral_no,2);

    JTT808_LocatParse(&msg_buf[2],&msg_buf_len,1);       //��ȡ�����Ķ�λ��Ϣ��

    JTT808_MsgPacket(JTT808_CMD_LOCATION_QUERY_ACT,msg_buf,msg_buf_len+2,package,0); //�������
}


void JTT808_Locationtrackctrl(JTT808t_Msg* msg)         //0x8202	��ʱλ�ø��ٿ���
{
    memcpy((char*)&track,msg->body,msg->head.property.size);
    track.interval=ntohs(track.interval);
    track.valid=ntohl(track.valid);
    track.start_sec=TimerSecond;

    JTT808_terminal_act(msg->head.serial_no,msg->head.id,JTT808_ACT_RET_OK);   //�����ն�ͨ��Ӧ��
}


void JTT808_Txtmsgdown(JTT808t_Msg* msg)                //0x8300	�ı���Ϣ�·�
{
    u8* pbody=msg->body;
    u8 cmd=*pbody++;


    if(cmd&0x01)      //1������
    {
    }
    if(cmd&0x02)      //����
    {
    }
    if(cmd&0x04)      //1:�ն���ʾ����ʾ
    {
    }
		
    if(cmd&0x08)      //1:�ն�TTS����
    {
				if(strstr((const char*)pbody,"@RESETGPS@"))
				{
//					reset_gps();
				}
				else
				{
#if !USE_NAVI_F0
						API_Play_TTS((char*)pbody);
#endif
				}
		}
		
    if(cmd&0x10)      //1:�������ʾ
    {
    }
    if(cmd&0x20)      //0:���ĵ�����Ϣ��1��CAN��������Ϣ
    {
    }
    if(cmd&0x40)	  //����
    {
    }
    if(cmd&0x80)	  //����
    {
    }
#if	USE_NAVI_F0
	NaviEncodedle(0xA6,pbody,msg->head.property.size-1);
#endif
    JTT808_terminal_act(msg->head.serial_no,msg->head.id,JTT808_ACT_RET_OK); //�����ն�ͨ��Ӧ��
}



void JTT808_Dialback(JTT808t_Msg* msg)                  //0x8400	�绰�ز�
{
    u8 *pbody=msg->body;

    Dial.flag=*pbody++;                                //0:��ͨ�绰  1������

    memset(Dial.tel,0x00,sizeof(Dial.tel));                         
    memcpy(Dial.tel,pbody,msg->head.property.size-1);  //�绰����
         
    API_make_phone_call();    					       //��֧�ִ�绰

    JTT808_terminal_act(msg->head.serial_no,msg->head.id,JTT808_ACT_RET_OK); //�����ն�ͨ��Ӧ��
} 



void JTT808_Carctrl(JTT808t_Msg* msg)                   //0x8500	��������
{
    u8 *pbody=msg->body;
    u8 type=*pbody;
    
    switch(type)
    {
        case 0:
            to_lock_car=0;
	        JTT808_Terminal_Param.lock_car=0;
	        jtt808_save_para();
	        UnLockCar();   
        break;

        case 1:
            to_lock_car=1;
            JTT808_Terminal_Param.lock_car=1;
            jtt808_save_para();
        break;

        default:
        break;
    }

    JTT808_terminal_act(msg->head.serial_no,msg->head.id,JTT808_ACT_RET_OK); //�����ն�ͨ��Ӧ��      
}


void JTT808_Downlinkdatatransparent(JTT808t_Msg* msg)   //0x8900	��������͸��
{
    u8 *pbody=msg->body;
    u8 type=*pbody++;
		uint16_t Req_pgn;
    
    switch(type)
    {
        case 0x00:             //GNSSģ����ϸ��λ����
        break;

        case 0x0B:             //��·����֤IC��
        break;

        case 0x41:             //����1
        break;

        case 0x42:             //����2
        break;

		case 'M':
			if(strstr((const char*)pbody,"@RESETGPS@"))
			{
				reset_gps();	
			}
			else
			{

#if	USE_NAVI_F0
				NaviEncodedle(0xA6,pbody,msg->head.property.size-1);
#endif	
			}
		break;

		case 'O':
			if(strstr((const char*)pbody,"@RESETGPS@"))
			{
					reset_gps();	
			}
			else
			{
#if	USE_NAVI_F0
//			band_msg_rec(pbody,msg->head.property.size-1);
					NaviEncodedle(0xA6,pbody,msg->head.property.size-1);
#endif
			}
		break;
			
			case 'C':			//CAN����OBD����
				
				if(strstr((const char*)pbody,"@REQUESTOBD@"))
				{
						pbody += 12;
						Req_pgn = (*pbody)<<8 | *(pbody+1);
//						Req_pgn |= (*pbody)<<8;
//						pbody ++;
//						Req_pgn |= (*pbody);
					
						switch(Req_pgn)
						{
								case 0xFECA:
									OBD_Request_DM1();
									break;
								
								case 0xFECB:
									OBD_Request_DM2();
									break;
								
								case 0xFEE9:
									OBD_Request_LFC();
									break;
								
								case 0xFEE8:
									OBD_Request_Speed();
									break;
								
								case 0xFEEA:
									OBD_Request_Weight();
									break;
								
								case 0xFEE0:
									OBD_Request_RD();
									break;
								
								case 0xFEFA:
									OBD_Request_Brake();
									break;
								
								case 0xFEE3:
									OBD_Request_Eng_Str();
									break;
								
								case 0xFEE5:
									OBD_Request_Eng_Time();
								  break;
								
								case 0xFEEE:
									OBD_Request_ET();
								  break;
								
								case 0xFEEF:
									OBD_Request_ELP();
									break;
								
								case 0xFEDB:
									OBD_Request_ELOP();
									break;
								
								case 0xFEB7:
									OBD_Request_ESLF();
									break;
								
								case 0xFE94:
									OBD_Request_ESC();
									break;
								
								case 0xFE93:
									OBD_Request_EES();
								  break;
								
								case 0xFE92:
									OBD_Request_EM();
									break;
								
								case 0xFE6A:
									OBD_Request_EFL();
									break;
								
								default:
									break;
						}
				}
				break;
		
    }
    JTT808_terminal_act(msg->head.serial_no,msg->head.id,JTT808_ACT_RET_OK); //�����ն�ͨ��Ӧ��    
}


void JTT808_Plantformrsapubkey(JTT808t_Msg* msg)        //0x8A00	ƽ̨RSA��Կ
{
//	u8* pbody=msg->body;
//
//
//	secrt_key.key_len=ntohl(*(u32*)pbody);
//	pbody+=4;
//
//	if((secrt_key.key_len+4)!=msg->head.property.size)
//	{
//		memset(&secrt_key,0x00,sizeof(secrt_key));
//		JTT808_terminal_act(msg->head.serial_no,msg->head.id,JTT808_ACT_RET_ERR);
//		return;		
//	}
//
//	memcpy(secrt_key.key,pbody,secrt_key.key_len);
//	secrt_key.use_key=1;

    JTT808_terminal_act(msg->head.serial_no,msg->head.id,JTT808_ACT_RET_INVALID); //�����ն�ͨ��Ӧ��
}



void JTT808_msg_prase(JTT808_RX_BUF* rx)
{
    JTT808t_Msg *msg = &jtt808_msg;

    if(JTT808_MsgDecode((u8*)rx->buf, rx->index, msg)!=JTT808_OK)
    {
        return;
    }
    switch(msg->head.id)
    {
        case JTT808_CMD_PLATFORM_ACT:                  //0x8001	ƽ̨ͨ��Ӧ��
            JTT808_PlatformACT(msg);
        break;
        case JTT808_CMD_TERMINAL_REGACT:               //0x8100	�ն�ע��Ӧ��
            JTT808_TerminalRegAct(msg);
        break;
        case JTT808_CMD_SET_TERMINAL_PARAM:            //0x8103	�����ն˲���
            JTT808_SetTerminalParam(msg);
        break;
        case JTT808_CMD_QUERY_TERMINAL_PARAM:          //0x8104	��ѯ�ն˲���
            JTT808_Queryterminalparam(msg);
        break;
        case JTT808_CMD_QUERY_APPOINT_TERMINAL_PARAM:  //0x8106 ��ѯָ���ն˲���
            JTT808_Queryappointterminalparam(msg);
        break;
        case JTT808_CMD_TERMINAL_CTRL:                 //0x8105	�ն˿���
            JTT808_Terminalctrl(msg);
        break;
        case JTT808_CMD_QUERY_TERMINAL_PROPERTY:       //0x8107 ��ѯ�ն�����
            JTT808_Queryterminalproperty(msg);
        break;
        case JTT808_CMD_LOCATION_QUERY:                //0x8201	λ����Ϣ��ѯ
            JTT808_Locationquery(msg);
        break;
        case JTT808_CMD_LOCATION_TRACK_CTRL:           //0x8202	��ʱλ�ø��ٿ���
            JTT808_Locationtrackctrl(msg);
        break;
        case JTT808_CMD_TXTMSG_DOWN:                   //0x8300	�ı���Ϣ�·�
            JTT808_Txtmsgdown(msg);
        break;
        case JTT808_CMD_DIAL_BACK:                     //0x8400	�绰�ز�
            JTT808_Dialback(msg);
        break;
        case JTT808_CMD_CAR_CTRL:                      //0x8500	��������
            JTT808_Carctrl(msg);
        break;
        case JTT808_CMD_AREA_CYCLE_SET:                //0x8600	����Բ������
            JTT808_Areacycleset(msg);
        break;
        case JTT808_CMD_AREA_CYCLE_DEL:                //0x8601	ɾ��Բ������
            JTT808_Areacycledel(msg);
        break;
        case JTT808_CMD_AREA_RECTANGLE_SET:            //0x8602	���þ�������
            JTT808_Arearectangleset(msg);
        break;
        case JTT808_CMD_AREA_RECTANGLE_DEL:            //0x8603	ɾ����������
            JTT808_Arearectangledel(msg);
        break;
        case JTT808_CMD_AREA_POLYGON_SET:              //0x8604	���ö��������
            JTT808_Areapolygonset(msg);
        break;
        case JTT808_CMD_AREA_POLYGON_DEL:              //0x8605	ɾ�����������
            JTT808_Areapolygondel(msg);
        break;
        case JTT808_CMD_ROUTE_SET:                     //0x8606	����·��
            JTT808_Routeset(msg);
        break;
        case JTT808_CMD_ROUTE_DEL:                     //0x8607	ɾ��·��
            JTT808_Routedel(msg);
        break;
        case JTT808_CMD_DOWNLINK_DATA_TRANSPARENT:     //0x8900	��������͸��
            JTT808_Downlinkdatatransparent(msg);
        break;
        case JTT808_CMD_PLATFORM_RSA_PUBKEY:           //0x8A00	ƽ̨RSA��Կ
            JTT808_Plantformrsapubkey(msg);
        break;

				/*----------2015.7.13-sindge-------*/

				case 0x8B00:
					if(1)
					{
//						u8 *pbody=msg->body;
//						Store_Place("PLACE.TXT", (char*)pbody, msg->head.property.size);
					}
					break;
				/*----------end---------*/
					
        default:
            JTT808_terminal_act(msg->head.serial_no,msg->head.id,JTT808_ACT_RET_INVALID);
        break;
    }
}


int JTT808_get_msg(JTT808_RX_BUF *rx)
{
    static u32 rece_time=0;
	static u8 head_flag = 0;
	char val=0;

	while(API_server_data_recv(0,&val))
    {
        switch(val)
        {
            case JTT808_MSG_TAG:
                if(!head_flag)
                {
                    head_flag=1;
					rece_time=Timer100ms;
                    rx->index=0;
                    rx->buf[rx->index++]=val;
					rx->buf[rx->index]=0x00;
                }
                else
                {
                    head_flag=0;
					rece_time=0; 
                    rx->buf[rx->index++]=val; 
                    rx->buf[rx->index]=0x00; 
                    return 1;
                }
            break;

            default:
				if(head_flag)
				{
	                rx->buf[rx->index++]=val;
					rx->buf[rx->index]=0x00; 
	                if(rx->index>=JTT808_MSG_MAX_SIZE)
					{
						head_flag=0;
						rece_time=0;
						rx->index=0;
					}
				}
            break;
        }
    }

    if(head_flag&&rece_time&&(timerSecondSub(Timer100ms, rece_time) > JTT808_RECE_TIMEOUT))
    {
		head_flag = 0;
		rece_time=0;
		rx->index=0;
    }
	return 0;
}

