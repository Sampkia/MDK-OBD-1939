#include "include.h"


JTT808t_LocationTrackCtrl track={0};       					 //跟踪

u32 LOACTION_READ_ADDR=LOACTION_START_ADDR;                  //断点续传读地址
u32 LOACTION_WRITE_ADDR=LOACTION_START_ADDR;                 //断点续传写地址



/****************************************************************
*  获取一条GPS数据
*  参数1:  msg_buf        GPS数据接收缓冲区
*  参数2： msg_buf_len    获取的GPS数据长度
*  参数3： type           类型       1表示添加附加消息，0为不添加附加消息                  
******************************************************************/
void JTT808_LocatParse(u8* msg_buf,u32 *msg_buf_len, int type)
{
    JTT808t_LocationBasic msg={0};
    JTT808t_LocationAlarm  JTT808_Location_Alarm_up={0};
    *msg_buf_len=0;

    
    /*****************确定上报的报警状态********************/
    *(u32*)&JTT808_Location_Alarm_up=(*(u32*)&JTT808_Location_Alarm)&(~JTT808_Terminal_Param.alarm_mask);//现有报警状态 按位与 报警屏蔽字


    /************************对msg赋值****************************/
    memcpy((char*)&msg.alarm_flag,(char*)&JTT808_Location_Alarm_up, sizeof(JTT808t_LocationAlarm));
    memcpy((char*)&msg.state,     (char*)&JTT808_Location_State, sizeof(JTT808t_LocationState));
		msg.latitude  = gprs_dev.status.latitude;//gps_location.latitude;
    msg.longitude = gprs_dev.status.longitude;//gps_location.longitude;
		msg.altitude  = gps_location.altitude;
		msg.speed     = gps_location.speed;
		msg.azimuth   = gps_location.azimuth;
//		memcpy(msg.time,gps_location.bj_tim.bcd_tim, 6);
		memcpy(msg.time,gprs_dev.status.BJ_BCD_tim, 6);
/*
		msg.latitude  = gps_location_bak.latitude;
    msg.longitude = gps_location_bak.longitude;
		msg.altitude  = gps_location_bak.altitude;
		msg.speed     = gps_location_bak.speed;
		msg.azimuth   = gps_location_bak.azimuth;
		memcpy(msg.time,gps_location_bak.bj_tim.bcd_tim, 6);
*/
    /*********************************************************************/

    /********************大小端转换************************/
    *(u32*)&msg.alarm_flag = htonl(*(u32*)&msg.alarm_flag);
    *(u32*)&msg.state      = htonl(*(u32*)&msg.state);
    *(u32*)&msg.latitude   = htonl(*(u32*)&msg.latitude);
    *(u32*)&msg.longitude  = htonl(*(u32*)&msg.longitude);
    *(u16*)&msg.altitude   = htons(*(u16*)&msg.altitude);
    *(u16*)&msg.speed      = htons(*(u16*)&msg.speed);
    *(u16*)&msg.azimuth    = htons(*(u16*)&msg.azimuth);
    /******************************************************/

    memcpy(msg_buf,(char*)&msg,sizeof(JTT808t_LocationBasic));
    *msg_buf_len+=sizeof(JTT808t_LocationBasic);
    msg_buf+=sizeof(JTT808t_LocationBasic);

    if(!type)return;
}


void JTT808_LocatReport_Save(u8* msg_buf,u32 msg_buf_len)
{
    u8 buf[32]={0x01,0x01,0x01,0x01,0};

    memcpy(&buf[4],msg_buf,28);

    if(!(LOACTION_WRITE_ADDR%4096))W25X_EraseSector(LOACTION_WRITE_ADDR/4096);  //新页，擦除
    W25X_Write_NoCheck(buf,LOACTION_WRITE_ADDR,32);
    printf("\r\n 断点存储 \r\n");

    LOACTION_WRITE_ADDR+=32;
     
    if(LOACTION_WRITE_ADDR>=LOACTION_END_ADDR)
    {
        LOACTION_WRITE_ADDR=LOACTION_START_ADDR;   
    }

    BKP_WriteBackupRegister(BKP_DR5, LOACTION_WRITE_ADDR>>16);
    BKP_WriteBackupRegister(BKP_DR6, LOACTION_WRITE_ADDR&0xFFFF);       //断点续传写地址  
}


void JTT808_LocatReportStored(void)
{
	u8 *buf_tmp=jtt808_global_buf;
    JTT808t_MsgPackage package={0}; //分包
                                           

    if(LOACTION_READ_ADDR!=LOACTION_WRITE_ADDR)
    {
        W25X_Read(&buf_tmp[1],LOACTION_READ_ADDR,32);
        LOACTION_READ_ADDR+=32;

        if(LOACTION_READ_ADDR>=LOACTION_END_ADDR)
        {
            LOACTION_READ_ADDR=LOACTION_START_ADDR;   
        }

        BKP_WriteBackupRegister(BKP_DR3, LOACTION_READ_ADDR>>16);  
        BKP_WriteBackupRegister(BKP_DR4, LOACTION_READ_ADDR&0xFFFF);       //断点续传读地址

        if(buf_tmp[1]==0x01&&buf_tmp[2]==0x01&&buf_tmp[3]==0x01&&buf_tmp[4]==0x01)
        {
						buf_tmp[0]=0x00;
						buf_tmp[1]=0x01;
						buf_tmp[2]=0x01;
						buf_tmp[3]=(28>>8)&0xFF;
						buf_tmp[4]=28&0xFF;
						JTT808_MsgPacket(JTT808_CMD_LOCATION_BULK_UPLOAD,buf_tmp,33,package, 1); //立即发送 
						printf("\r\n 断点续传 \r\n");
        }
    }
}



void JTT808_LocationReport(void)
{
    static JTT808t_LocationState  Last_JTT808_Location_State={0};   //上一次的定位状态
    static JTT808t_LocationAlarm  Last_JTT808_Location_Alarm={0};   //上一次的报警状态
    static u32 send_sec=0;                                          //上一次发送的时间
    static u32 Last_latitude=0,Last_longitude=0;                    //上一次发送的经纬度
		static u32 check_sec=0;
    char to_send=0;                                                 //是否需要上报
    u32 interval_tim=0,interval_dst=0;                              //确定当前上报的时间、距离间隔

		if(TimerSecond == check_sec)return;							    //每秒检查一次，时间不到不做检查
		check_sec=TimerSecond;
		
		/*-----------后期加的------------*/
//		if(!JTT808_Location_State.is_location) return;			//不定位不上传
		
#if !USE_INS
		switch(Up_Flag)
		{
			  static u32 tim=0;
				//无效数据
				case 0:
					tim=0;
					return;
				
				//5秒上传一条数据
				case 1:
						tim=0;
					break;
				
				//2min上传一条数据
				case 2:
					tim++;
				  if(tim>=60)JTT808_Terminal_Param.default_report_interval = 120;
					break;
		}
#endif
		/*-------------------------------*/

    /*************************当前定位状态、报警状态同上一次不一致时立即上报***************************/
    if(*(int*)&Last_JTT808_Location_State^*(int*)&JTT808_Location_State)     //当前位置状态同上一次的不同
    {
        to_send=1;
    }
    if(*(int*)&Last_JTT808_Location_Alarm^*(int*)&JTT808_Location_Alarm)     //当前报警状态同上一次的不同
    {
        to_send=1;
    }
    /***************************************************************************************************/


    /****************************确定汇报间隔*******************************************/
    if(JTT808_Location_Alarm.emergency)                        //紧急报警
    {
        interval_tim=JTT808_Terminal_Param.report_at_alarm_interval;
        interval_dst=JTT808_Terminal_Param.report_at_alarm_dst_interval;
    }
    else if(track.interval&&track.valid&&track.start_sec)      //跟踪
    {
        interval_tim=track.interval;
		interval_dst=0xFFFFFFFF;
    }
    else if(!JTT808_Location_State.acc)                        //ACC关
    {
        interval_tim=JTT808_Terminal_Param.report_at_sleep_interval;
        interval_dst=JTT808_Terminal_Param.report_at_sleep_dst_interval;
    }
    else if(JTT808_Terminal_Param.location_report_program==1&&!dirver_login_status)//根据登录状态和ACC状态
    {                                                                              //如果ACC打开且驾驶员未插卡
        interval_tim=JTT808_Terminal_Param.report_driver_unlogin_interval;
        interval_dst=JTT808_Terminal_Param.report_driver_unlogin_dst_interval;
    }
    else                                                       //默认间隔
    {
        interval_tim=JTT808_Terminal_Param.default_report_interval;
        interval_dst=JTT808_Terminal_Param.default_report_dst_interval;
    }
    /*******************************************************************************/


    /****************************确定汇报策略*******************************************/
    if(JTT808_Terminal_Param.location_report_strategy==0)  //定时汇报
    {
        if((TimerSecond-send_sec)>=interval_tim)    //如果时间到
        {
            to_send=1;
        }
    }
    else if(JTT808_Terminal_Param.location_report_strategy==1)//定距汇报  
    {
        if(track.interval&&track.valid&&track.start_sec)      //定距汇报情况下先判断跟踪，如果无跟踪再判断距离
        {
            if((TimerSecond-send_sec)>=interval_tim)          //如果时间到
            {
                to_send=1;
            }
        }
        else if(two_point_distance(Last_latitude,0,Last_longitude,0,gps_location.latitude,0,gps_location.longitude,0)>=interval_dst)  //如果当前点同上一次定位点间距大于参数时，则上报
        {
            to_send=1;
        }
    }
    else if(JTT808_Terminal_Param.location_report_strategy==2)//定时、定距
    {
        if(((TimerSecond-send_sec)>=interval_tim)||(two_point_distance(Last_latitude,0,Last_longitude,0,gps_location.latitude,0,gps_location.longitude,0)>=interval_dst))
        {
            to_send=1;
        }
    }
    /****************************************************************************/


    if(to_send==1)
    {
        u8   *msg_buf=jtt808_global_buf;
        u32  msg_buf_len=0;
        JTT808t_MsgPackage package={0}; //分包


        if(track.interval&&track.valid&&track.start_sec)    //如果有跟踪
        {
            if(track.valid<=(TimerSecond-track.start_sec))
            {
                track.interval=0;                //跟踪间隔赋值为0
                track.valid=0;
                track.start_sec=0;
            }
        }


        /****************对上一次的汇报状态赋值***********************/
        send_sec=TimerSecond;
        Last_latitude=gps_location.latitude;
        Last_longitude=gps_location.longitude;
        memcpy((char*)&Last_JTT808_Location_State,(char*)&JTT808_Location_State,sizeof(JTT808t_LocationState));
        memcpy((char*)&Last_JTT808_Location_Alarm,(char*)&JTT808_Location_Alarm,sizeof(JTT808t_LocationAlarm));
        /**************************************************************/
        

        if(!API_get_gprs_state()||!JTT808_Flag_Authentication||!API_Get_singal())//断点存储   GPRS未连接、终端未鉴权、终端无信号
        {
            JTT808_LocatParse(msg_buf, &msg_buf_len, 0);
            JTT808_LocatReport_Save(msg_buf,msg_buf_len);                        //保存到flash
            return; 
        }
        else
        {
            JTT808_LocatParse(msg_buf, &msg_buf_len, 1);
            JTT808_MsgPacket(JTT808_CMD_LOCATION_REPORT,msg_buf,msg_buf_len,package, 1); //立即发送
        }
    }

    if(JTT808_Flag_Authentication&&API_get_gprs_state()&&API_Get_singal()&&!API_get_send_buf_len())       //断点续传   
    {
        JTT808_LocatReportStored();
    }
}


//void JTT808_turning_report(void) //拐点补传
//{
//    static u32 check_time=0;     //上次扫描时间
//    static u16 last_azimuth=0;   //上次航向角
//    static u8 turning_location_data[3][28]={0};//拐点前3秒的数据
//    static u16 times=0;          //拐点持续时间

//    int tmp=0;

//	if(check_time==TimerSecond)return;   		 //一秒钟扫描一次
//    if((!JTT808_Location_State.acc)||(!JTT808_Location_State.is_location)||(gps_location.speed<=100))	 //ACC关闭、未定位、速度小于10KM/H    不检测拐点补传
//	{
//		check_time=TimerSecond;
//		times=0;
//		last_azimuth=gps_location.azimuth;
//		return;	
//	}
//	

//    check_time=TimerSecond;

//    //确认航向差
//    tmp=abs(gps_location.azimuth-last_azimuth);
//    if(tmp>180)tmp=360-tmp;
//    last_azimuth=gps_location.azimuth;

//    if(tmp>=JTT808_Terminal_Param.inflect_point_complement_biography)times++;
//    else times=0;

//    if(times)
//    {
//		u8   *msg_buf=jtt808_global_buf;
//        u32  msg_buf_len=0;
//        JTT808t_MsgPackage package={0}; //分包

//        JTT808_LocatParse(msg_buf,&msg_buf_len,0);

//        if(times<=3)
//        {
//            memcpy(turning_location_data[times-1],msg_buf,28);
//        }

//        if(times==3)
//        {
//            u8 i=0;

//            for(i=0;i<3;i++)
//            {
//                printf("\r\n 拐点补传！ \r\n");
//                JTT808_MsgPacket(JTT808_CMD_LOCATION_REPORT,turning_location_data[i],28,package, 1); //立即发送    
//            }
//        }

//        if(times>3)
//        {
//            printf("\r\n 拐点补传！ \r\n");
//            JTT808_MsgPacket(JTT808_CMD_LOCATION_REPORT,msg_buf,28,package, 1); //立即发送
//        }
//    }
//}



