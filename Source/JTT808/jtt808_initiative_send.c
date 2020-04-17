#include "include.h"


void JTT808_TerminalReg(void)           //�ն�ע��
{
    if(!API_get_gprs_state())return;
    if(strcmp((char*)JTT808_Terminal_Param.authorization_no,"00000000000000000000000000000000")==0)
    {
		static u32 tim=0;

		if(TimerSecond-tim>=30)
        {
            u8 *msg_buf=jtt808_global_buf;                          //ע�Ỻ����
            JTT808t_TerminalReg reg={0};                             //����ע��ṹ��
            JTT808t_MsgPackage package={0};                          //�ְ�

			tim=TimerSecond;
				
            reg.province_id=htons(atoi((char*)JTT808_Terminal_Param.province_id));//ʡ��ID
            reg.city_id=htons(atoi((char*)JTT808_Terminal_Param.city_id));        //����ID
            memcpy(reg.producer_id,PRODUCTOR_ID,5);                    			  //������ID
            memcpy(reg.model_type,MODULE_TYPE,6);                      			  //�ն��ͺ�
            memcpy(reg.terminal_id,JTT808_Terminal_Param.terminalID,7);			  //�ն�ID
            reg.lic_plate_color=JTT808_Terminal_Param.lic_plate_color; 			  //������ɫ
            strcpy((char*)reg.lic_plate,(char*)JTT808_Terminal_Param.lic_plate);  //�������ƺ�

            memcpy(msg_buf, &reg, sizeof(JTT808t_TerminalReg) - sizeof(reg.lic_plate)+strlen((const char*)reg.lic_plate));
            JTT808_MsgPacket(JTT808_CMD_TERMINAL_REG, msg_buf, sizeof(JTT808t_TerminalReg) - sizeof(reg.lic_plate) + strlen((const char*)reg.lic_plate),package, 1);
        }
    }
}

void JTT808_TerminalReg_back(void)          
{
    u8 *msg_buf=jtt808_global_buf;                          //ע�Ỻ����
    JTT808t_TerminalReg reg={0};                            //����ע��ṹ��
    JTT808t_MsgPackage package={0};                         //�ְ�

		
    reg.province_id=htons(atoi((char*)JTT808_Terminal_Param.province_id));//ʡ��ID
    reg.city_id=htons(atoi((char*)JTT808_Terminal_Param.city_id));        //����ID
    memcpy(reg.producer_id,PRODUCTOR_ID,5);                    			  //������ID
    memcpy(reg.model_type,MODULE_TYPE,6);                      			  //�ն��ͺ�
    memcpy(reg.terminal_id,JTT808_Terminal_Param.terminalID,7);			  //�ն�ID
    reg.lic_plate_color=JTT808_Terminal_Param.lic_plate_color; 			  //������ɫ
    strcpy((char*)reg.lic_plate,(char*)JTT808_Terminal_Param.lic_plate);  //�������ƺ�

    memcpy(msg_buf, &reg, sizeof(JTT808t_TerminalReg) - sizeof(reg.lic_plate)+strlen((const char*)reg.lic_plate));
    JTT808_MsgPacket(JTT808_CMD_TERMINAL_REG, msg_buf, sizeof(JTT808t_TerminalReg) - sizeof(reg.lic_plate) + strlen((const char*)reg.lic_plate),package, 1);
}


void JTT808_TerminalAuthentication(void)  	//�ն˼�Ȩ
{
    if(!API_get_gprs_state())return;    	//GPRSδ�������˳�
    if(strcmp((char*)JTT808_Terminal_Param.authorization_no,"00000000000000000000000000000000")==0)return;  //�ն�δע�����˳�
    if(!JTT808_Flag_Authentication)     	//�ն�δ��Ȩ
    {
		static u32 tim=0;

		if(TimerSecond-tim>=30)
        {
            JTT808t_MsgPackage package={0}; //�ְ�
			tim=TimerSecond;

            JTT808_MsgPacket(JTT808_CMD_TERMINAL_AUTHENTICATION,(char*)JTT808_Terminal_Param.authorization_no,strlen((const char*)JTT808_Terminal_Param.authorization_no),package, 1);
        }
    }  
}


void JTT808_TerminalBeat(void)    //�ն�����
{
    static u32 last_beat_sec=0;
    if(!API_get_gprs_state())return;    		//GPRSδ�������˳�
    if(strcmp((char*)JTT808_Terminal_Param.authorization_no,"00000000000000000000000000000000")==0)return;  //�ն�δע�����˳�
    if(!JTT808_Flag_Authentication)return;      //�ն�δ��Ȩ���˳�

    if(TimerSecond-last_beat_sec>=JTT808_Terminal_Param.beat_interval)
    {
        JTT808t_MsgPackage package={0}; 		//�ְ�

        last_beat_sec=TimerSecond;
        JTT808_MsgPacket(JTT808_CMD_TERMINAL_BEAT,0,0,package, 1);
    }
}

void JTT808_CAN_data_upload(u32 can_id,u8 sample_type,u8 frame_type,u8 can_channel,char* buf)
{
//	JTT808t_CanID_Upload can_id_upload={0};

//	can_id_upload.can_id=can_id;
//	can_id_upload.
//	can_id_upload.
//	can_id_upload.
//	can_id_upload.
//		u32 can_id:29;				 //bit28-bit0 ��ʾ CAN ���� ID
//	u32 sample_type:1;			 //bit29 ��ʾ���ݲɼ���ʽ��0��ԭʼ���ݣ�1���ɼ�����ļ���ֵ��
//	u32 frame_type:1;			 //bit30 ��ʾ֡���ͣ�0����׼֡��1����չ֡��
//	u32 can_channel:1;			 //bit31 ��ʾ CAN ͨ���ţ�0��CAN1��1��CAN2��
//	char can_data[8];			 //CAN ����
}


int obd_count = 0;
void JTT808_CAN_bus_scan(void)
{
	static u32 last_upload_time=0;		 //��s��ʱ
	static u32 tim=0;

	if(tim == TimerSecond)return;
	tim = TimerSecond;
		
//		memcpy(&Can_ID_Set,JTT808_Terminal_Param.can_ID_samp_set[i],8);
		
	if(TimerSecond-last_upload_time >= 15)//ʱ�䵽
	{
		last_upload_time = TimerSecond;
		API_get_Can_ID_data();
	}	
}


void JTT808_TerminalDereg(void)         //�ն�ע��
{
    JTT808t_MsgPackage package={0};     //�ְ�
    JTT808_MsgPacket(JTT808_CMD_TERMINAL_DEREG,0,0,package, 1);
}


void JTT808_TransData(char* src,u16 len)
{
	JTT808t_MsgPackage package={0};     //�ְ�
	JTT808_MsgPacket(JTT808_CMD_UPSTREAM_DATA_TRANSPARENT,src,len,package, 1);
}

/*---------------2015.7.13-sindge-----------------*/
void JTT808_TransPlace(char* src,u16 len)
{
	JTT808t_MsgPackage package={0};     //�ְ�
	JTT808_MsgPacket(JTT808_PLACE_UP, src, len, package, 1);
}



void JTT808_InitiativeCmd(void)
{
    JTT808_TerminalReg();           //�ն�ע��
    JTT808_TerminalAuthentication();//�ն˼�Ȩ
    JTT808_TerminalBeat();          //�ն�����
		JTT808_LocationReport();        //�ն�λ����Ϣ�㱨
//    JTT808_turning_report();        //�յ㲹��
//	JTT808_CAN_bus_scan();
}
