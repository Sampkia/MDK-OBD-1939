#include "include.h"


JTT808t_LocationTrackCtrl track={0};       					 //����

u32 LOACTION_READ_ADDR=LOACTION_START_ADDR;                  //�ϵ���������ַ
u32 LOACTION_WRITE_ADDR=LOACTION_START_ADDR;                 //�ϵ�����д��ַ



/****************************************************************
*  ��ȡһ��GPS����
*  ����1:  msg_buf        GPS���ݽ��ջ�����
*  ����2�� msg_buf_len    ��ȡ��GPS���ݳ���
*  ����3�� type           ����       1��ʾ��Ӹ�����Ϣ��0Ϊ����Ӹ�����Ϣ                  
******************************************************************/
void JTT808_LocatParse(u8* msg_buf,u32 *msg_buf_len, int type)
{
    JTT808t_LocationBasic msg={0};
    JTT808t_LocationAlarm  JTT808_Location_Alarm_up={0};
    *msg_buf_len=0;

    
    /*****************ȷ���ϱ��ı���״̬********************/
    *(u32*)&JTT808_Location_Alarm_up=(*(u32*)&JTT808_Location_Alarm)&(~JTT808_Terminal_Param.alarm_mask);//���б���״̬ ��λ�� ����������


    /************************��msg��ֵ****************************/
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

    /********************��С��ת��************************/
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

    if(!(LOACTION_WRITE_ADDR%4096))W25X_EraseSector(LOACTION_WRITE_ADDR/4096);  //��ҳ������
    W25X_Write_NoCheck(buf,LOACTION_WRITE_ADDR,32);
    printf("\r\n �ϵ�洢 \r\n");

    LOACTION_WRITE_ADDR+=32;
     
    if(LOACTION_WRITE_ADDR>=LOACTION_END_ADDR)
    {
        LOACTION_WRITE_ADDR=LOACTION_START_ADDR;   
    }

    BKP_WriteBackupRegister(BKP_DR5, LOACTION_WRITE_ADDR>>16);
    BKP_WriteBackupRegister(BKP_DR6, LOACTION_WRITE_ADDR&0xFFFF);       //�ϵ�����д��ַ  
}


void JTT808_LocatReportStored(void)
{
	u8 *buf_tmp=jtt808_global_buf;
    JTT808t_MsgPackage package={0}; //�ְ�
                                           

    if(LOACTION_READ_ADDR!=LOACTION_WRITE_ADDR)
    {
        W25X_Read(&buf_tmp[1],LOACTION_READ_ADDR,32);
        LOACTION_READ_ADDR+=32;

        if(LOACTION_READ_ADDR>=LOACTION_END_ADDR)
        {
            LOACTION_READ_ADDR=LOACTION_START_ADDR;   
        }

        BKP_WriteBackupRegister(BKP_DR3, LOACTION_READ_ADDR>>16);  
        BKP_WriteBackupRegister(BKP_DR4, LOACTION_READ_ADDR&0xFFFF);       //�ϵ���������ַ

        if(buf_tmp[1]==0x01&&buf_tmp[2]==0x01&&buf_tmp[3]==0x01&&buf_tmp[4]==0x01)
        {
						buf_tmp[0]=0x00;
						buf_tmp[1]=0x01;
						buf_tmp[2]=0x01;
						buf_tmp[3]=(28>>8)&0xFF;
						buf_tmp[4]=28&0xFF;
						JTT808_MsgPacket(JTT808_CMD_LOCATION_BULK_UPLOAD,buf_tmp,33,package, 1); //�������� 
						printf("\r\n �ϵ����� \r\n");
        }
    }
}



void JTT808_LocationReport(void)
{
    static JTT808t_LocationState  Last_JTT808_Location_State={0};   //��һ�εĶ�λ״̬
    static JTT808t_LocationAlarm  Last_JTT808_Location_Alarm={0};   //��һ�εı���״̬
    static u32 send_sec=0;                                          //��һ�η��͵�ʱ��
    static u32 Last_latitude=0,Last_longitude=0;                    //��һ�η��͵ľ�γ��
		static u32 check_sec=0;
    char to_send=0;                                                 //�Ƿ���Ҫ�ϱ�
    u32 interval_tim=0,interval_dst=0;                              //ȷ����ǰ�ϱ���ʱ�䡢������

		if(TimerSecond == check_sec)return;							    //ÿ����һ�Σ�ʱ�䲻���������
		check_sec=TimerSecond;
		
		/*-----------���ڼӵ�------------*/
//		if(!JTT808_Location_State.is_location) return;			//����λ���ϴ�
		
#if !USE_INS
		switch(Up_Flag)
		{
			  static u32 tim=0;
				//��Ч����
				case 0:
					tim=0;
					return;
				
				//5���ϴ�һ������
				case 1:
						tim=0;
					break;
				
				//2min�ϴ�һ������
				case 2:
					tim++;
				  if(tim>=60)JTT808_Terminal_Param.default_report_interval = 120;
					break;
		}
#endif
		/*-------------------------------*/

    /*************************��ǰ��λ״̬������״̬ͬ��һ�β�һ��ʱ�����ϱ�***************************/
    if(*(int*)&Last_JTT808_Location_State^*(int*)&JTT808_Location_State)     //��ǰλ��״̬ͬ��һ�εĲ�ͬ
    {
        to_send=1;
    }
    if(*(int*)&Last_JTT808_Location_Alarm^*(int*)&JTT808_Location_Alarm)     //��ǰ����״̬ͬ��һ�εĲ�ͬ
    {
        to_send=1;
    }
    /***************************************************************************************************/


    /****************************ȷ���㱨���*******************************************/
    if(JTT808_Location_Alarm.emergency)                        //��������
    {
        interval_tim=JTT808_Terminal_Param.report_at_alarm_interval;
        interval_dst=JTT808_Terminal_Param.report_at_alarm_dst_interval;
    }
    else if(track.interval&&track.valid&&track.start_sec)      //����
    {
        interval_tim=track.interval;
		interval_dst=0xFFFFFFFF;
    }
    else if(!JTT808_Location_State.acc)                        //ACC��
    {
        interval_tim=JTT808_Terminal_Param.report_at_sleep_interval;
        interval_dst=JTT808_Terminal_Param.report_at_sleep_dst_interval;
    }
    else if(JTT808_Terminal_Param.location_report_program==1&&!dirver_login_status)//���ݵ�¼״̬��ACC״̬
    {                                                                              //���ACC���Ҽ�ʻԱδ�忨
        interval_tim=JTT808_Terminal_Param.report_driver_unlogin_interval;
        interval_dst=JTT808_Terminal_Param.report_driver_unlogin_dst_interval;
    }
    else                                                       //Ĭ�ϼ��
    {
        interval_tim=JTT808_Terminal_Param.default_report_interval;
        interval_dst=JTT808_Terminal_Param.default_report_dst_interval;
    }
    /*******************************************************************************/


    /****************************ȷ���㱨����*******************************************/
    if(JTT808_Terminal_Param.location_report_strategy==0)  //��ʱ�㱨
    {
        if((TimerSecond-send_sec)>=interval_tim)    //���ʱ�䵽
        {
            to_send=1;
        }
    }
    else if(JTT808_Terminal_Param.location_report_strategy==1)//����㱨  
    {
        if(track.interval&&track.valid&&track.start_sec)      //����㱨��������жϸ��٣�����޸������жϾ���
        {
            if((TimerSecond-send_sec)>=interval_tim)          //���ʱ�䵽
            {
                to_send=1;
            }
        }
        else if(two_point_distance(Last_latitude,0,Last_longitude,0,gps_location.latitude,0,gps_location.longitude,0)>=interval_dst)  //�����ǰ��ͬ��һ�ζ�λ������ڲ���ʱ�����ϱ�
        {
            to_send=1;
        }
    }
    else if(JTT808_Terminal_Param.location_report_strategy==2)//��ʱ������
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
        JTT808t_MsgPackage package={0}; //�ְ�


        if(track.interval&&track.valid&&track.start_sec)    //����и���
        {
            if(track.valid<=(TimerSecond-track.start_sec))
            {
                track.interval=0;                //���ټ����ֵΪ0
                track.valid=0;
                track.start_sec=0;
            }
        }


        /****************����һ�εĻ㱨״̬��ֵ***********************/
        send_sec=TimerSecond;
        Last_latitude=gps_location.latitude;
        Last_longitude=gps_location.longitude;
        memcpy((char*)&Last_JTT808_Location_State,(char*)&JTT808_Location_State,sizeof(JTT808t_LocationState));
        memcpy((char*)&Last_JTT808_Location_Alarm,(char*)&JTT808_Location_Alarm,sizeof(JTT808t_LocationAlarm));
        /**************************************************************/
        

        if(!API_get_gprs_state()||!JTT808_Flag_Authentication||!API_Get_singal())//�ϵ�洢   GPRSδ���ӡ��ն�δ��Ȩ���ն����ź�
        {
            JTT808_LocatParse(msg_buf, &msg_buf_len, 0);
            JTT808_LocatReport_Save(msg_buf,msg_buf_len);                        //���浽flash
            return; 
        }
        else
        {
            JTT808_LocatParse(msg_buf, &msg_buf_len, 1);
            JTT808_MsgPacket(JTT808_CMD_LOCATION_REPORT,msg_buf,msg_buf_len,package, 1); //��������
        }
    }

    if(JTT808_Flag_Authentication&&API_get_gprs_state()&&API_Get_singal()&&!API_get_send_buf_len())       //�ϵ�����   
    {
        JTT808_LocatReportStored();
    }
}


//void JTT808_turning_report(void) //�յ㲹��
//{
//    static u32 check_time=0;     //�ϴ�ɨ��ʱ��
//    static u16 last_azimuth=0;   //�ϴκ����
//    static u8 turning_location_data[3][28]={0};//�յ�ǰ3�������
//    static u16 times=0;          //�յ����ʱ��

//    int tmp=0;

//	if(check_time==TimerSecond)return;   		 //һ����ɨ��һ��
//    if((!JTT808_Location_State.acc)||(!JTT808_Location_State.is_location)||(gps_location.speed<=100))	 //ACC�رա�δ��λ���ٶ�С��10KM/H    �����յ㲹��
//	{
//		check_time=TimerSecond;
//		times=0;
//		last_azimuth=gps_location.azimuth;
//		return;	
//	}
//	

//    check_time=TimerSecond;

//    //ȷ�Ϻ����
//    tmp=abs(gps_location.azimuth-last_azimuth);
//    if(tmp>180)tmp=360-tmp;
//    last_azimuth=gps_location.azimuth;

//    if(tmp>=JTT808_Terminal_Param.inflect_point_complement_biography)times++;
//    else times=0;

//    if(times)
//    {
//		u8   *msg_buf=jtt808_global_buf;
//        u32  msg_buf_len=0;
//        JTT808t_MsgPackage package={0}; //�ְ�

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
//                printf("\r\n �յ㲹���� \r\n");
//                JTT808_MsgPacket(JTT808_CMD_LOCATION_REPORT,turning_location_data[i],28,package, 1); //��������    
//            }
//        }

//        if(times>3)
//        {
//            printf("\r\n �յ㲹���� \r\n");
//            JTT808_MsgPacket(JTT808_CMD_LOCATION_REPORT,msg_buf,28,package, 1); //��������
//        }
//    }
//}



