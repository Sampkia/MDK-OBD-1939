#include "include.h"


void JTT808_TerminalReg(void)           //终端注册
{
    if(!API_get_gprs_state())return;
    if(strcmp((char*)JTT808_Terminal_Param.authorization_no,"00000000000000000000000000000000")==0)
    {
		static u32 tim=0;

		if(TimerSecond-tim>=30)
        {
            u8 *msg_buf=jtt808_global_buf;                          //注册缓冲区
            JTT808t_TerminalReg reg={0};                             //定义注册结构体
            JTT808t_MsgPackage package={0};                          //分包

			tim=TimerSecond;
				
            reg.province_id=htons(atoi((char*)JTT808_Terminal_Param.province_id));//省域ID
            reg.city_id=htons(atoi((char*)JTT808_Terminal_Param.city_id));        //市域ID
            memcpy(reg.producer_id,PRODUCTOR_ID,5);                    			  //制造商ID
            memcpy(reg.model_type,MODULE_TYPE,6);                      			  //终端型号
            memcpy(reg.terminal_id,JTT808_Terminal_Param.terminalID,7);			  //终端ID
            reg.lic_plate_color=JTT808_Terminal_Param.lic_plate_color; 			  //车牌颜色
            strcpy((char*)reg.lic_plate,(char*)JTT808_Terminal_Param.lic_plate);  //机动车牌号

            memcpy(msg_buf, &reg, sizeof(JTT808t_TerminalReg) - sizeof(reg.lic_plate)+strlen((const char*)reg.lic_plate));
            JTT808_MsgPacket(JTT808_CMD_TERMINAL_REG, msg_buf, sizeof(JTT808t_TerminalReg) - sizeof(reg.lic_plate) + strlen((const char*)reg.lic_plate),package, 1);
        }
    }
}

void JTT808_TerminalReg_back(void)          
{
    u8 *msg_buf=jtt808_global_buf;                          //注册缓冲区
    JTT808t_TerminalReg reg={0};                            //定义注册结构体
    JTT808t_MsgPackage package={0};                         //分包

		
    reg.province_id=htons(atoi((char*)JTT808_Terminal_Param.province_id));//省域ID
    reg.city_id=htons(atoi((char*)JTT808_Terminal_Param.city_id));        //市域ID
    memcpy(reg.producer_id,PRODUCTOR_ID,5);                    			  //制造商ID
    memcpy(reg.model_type,MODULE_TYPE,6);                      			  //终端型号
    memcpy(reg.terminal_id,JTT808_Terminal_Param.terminalID,7);			  //终端ID
    reg.lic_plate_color=JTT808_Terminal_Param.lic_plate_color; 			  //车牌颜色
    strcpy((char*)reg.lic_plate,(char*)JTT808_Terminal_Param.lic_plate);  //机动车牌号

    memcpy(msg_buf, &reg, sizeof(JTT808t_TerminalReg) - sizeof(reg.lic_plate)+strlen((const char*)reg.lic_plate));
    JTT808_MsgPacket(JTT808_CMD_TERMINAL_REG, msg_buf, sizeof(JTT808t_TerminalReg) - sizeof(reg.lic_plate) + strlen((const char*)reg.lic_plate),package, 1);
}


void JTT808_TerminalAuthentication(void)  	//终端鉴权
{
    if(!API_get_gprs_state())return;    	//GPRS未上线则退出
    if(strcmp((char*)JTT808_Terminal_Param.authorization_no,"00000000000000000000000000000000")==0)return;  //终端未注册则退出
    if(!JTT808_Flag_Authentication)     	//终端未鉴权
    {
		static u32 tim=0;

		if(TimerSecond-tim>=30)
        {
            JTT808t_MsgPackage package={0}; //分包
			tim=TimerSecond;

            JTT808_MsgPacket(JTT808_CMD_TERMINAL_AUTHENTICATION,(char*)JTT808_Terminal_Param.authorization_no,strlen((const char*)JTT808_Terminal_Param.authorization_no),package, 1);
        }
    }  
}


void JTT808_TerminalBeat(void)    //终端心跳
{
    static u32 last_beat_sec=0;
    if(!API_get_gprs_state())return;    		//GPRS未上线则退出
    if(strcmp((char*)JTT808_Terminal_Param.authorization_no,"00000000000000000000000000000000")==0)return;  //终端未注册则退出
    if(!JTT808_Flag_Authentication)return;      //终端未鉴权则退出

    if(TimerSecond-last_beat_sec>=JTT808_Terminal_Param.beat_interval)
    {
        JTT808t_MsgPackage package={0}; 		//分包

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
//		u32 can_id:29;				 //bit28-bit0 表示 CAN 总线 ID
//	u32 sample_type:1;			 //bit29 表示数据采集方式，0：原始数据，1：采集区间的计算值；
//	u32 frame_type:1;			 //bit30 表示帧类型，0：标准帧，1：扩展帧；
//	u32 can_channel:1;			 //bit31 表示 CAN 通道号，0：CAN1，1：CAN2；
//	char can_data[8];			 //CAN 数据
}


int obd_count = 0;
void JTT808_CAN_bus_scan(void)
{
	static u32 last_upload_time=0;		 //按s计时
	static u32 tim=0;

	if(tim == TimerSecond)return;
	tim = TimerSecond;
		
//		memcpy(&Can_ID_Set,JTT808_Terminal_Param.can_ID_samp_set[i],8);
		
	if(TimerSecond-last_upload_time >= 15)//时间到
	{
		last_upload_time = TimerSecond;
		API_get_Can_ID_data();
	}	
}


void JTT808_TerminalDereg(void)         //终端注销
{
    JTT808t_MsgPackage package={0};     //分包
    JTT808_MsgPacket(JTT808_CMD_TERMINAL_DEREG,0,0,package, 1);
}


void JTT808_TransData(char* src,u16 len)
{
	JTT808t_MsgPackage package={0};     //分包
	JTT808_MsgPacket(JTT808_CMD_UPSTREAM_DATA_TRANSPARENT,src,len,package, 1);
}

/*---------------2015.7.13-sindge-----------------*/
void JTT808_TransPlace(char* src,u16 len)
{
	JTT808t_MsgPackage package={0};     //分包
	JTT808_MsgPacket(JTT808_PLACE_UP, src, len, package, 1);
}



void JTT808_InitiativeCmd(void)
{
    JTT808_TerminalReg();           //终端注册
    JTT808_TerminalAuthentication();//终端鉴权
    JTT808_TerminalBeat();          //终端心跳
		JTT808_LocationReport();        //终端位置信息汇报
//    JTT808_turning_report();        //拐点补传
//	JTT808_CAN_bus_scan();
}
