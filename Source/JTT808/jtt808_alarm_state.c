#include "include.h"

JTT808t_LocationState  JTT808_Location_State={0};     //��λ״̬
JTT808t_LocationAlarm  JTT808_Location_Alarm={0};     //����״̬


//void JTT808_speed_check(void)                         //�ٶ�ɨ��
//{
//    static u32 speeding_start_tim=0;                  //���ٿ�ʼʱ��

//    if((!JTT808_Location_State.acc)||(!JTT808_Location_State.is_location))		      //ACC���˳�	δ��λ�˳�
//	{
//		speeding_start_tim=0;
//		return;
//	}


//    if((JTT808_Terminal_Param.top_speed)&&((gps_location.speed/10)>=JTT808_Terminal_Param.top_speed))//���ٱ���
//    {
//        static u32 TTS_tim=0;

//        if(!speeding_start_tim)speeding_start_tim=TimerSecond; //���ٿ�ʼʱ��
//        if((TimerSecond-speeding_start_tim)>=JTT808_Terminal_Param.speeding_duration)//���ٳ���ʱ������趨����ֵ
//        {
//            JTT808_Location_Alarm.speeding=1;

//            if(TimerSecond-TTS_tim>=60)         //ÿ60���ϱ�����
//            {
//                TTS_tim=TimerSecond;
//                API_Play_TTS("���ѳ���");
//            }
//        }
//    }
//    else if((JTT808_Terminal_Param.overspeed_alarm_diff)&&(((gps_location.speed/10)+(JTT808_Terminal_Param.overspeed_alarm_diff/10))>=JTT808_Terminal_Param.top_speed))//����Ԥ��
//    {
//        static u32 TTS_tim=0;

//        speeding_start_tim=0;
//        JTT808_Location_Alarm.speeding_warning=1;

//        if(TimerSecond-TTS_tim>=60)           //ÿ60���ϱ�����
//        {
//            TTS_tim=TimerSecond;
//            API_Play_TTS("����������");
//        }
//    }
//    else
//    {
//        speeding_start_tim=0;
//    }
//}

//void JTT808_driver_tim_check(void)           //��ʻ��ͣ��ʱ��ɨ��  1����ɨ��һ�Σ������еĴ���ʱ��������
//{
//    static u32 driver_tim_in_day=0;          //����������ʻʱ��
//    static u32 driver_tim_continue=0;        //����������ʻʱ��
//    static u32 acc_off_total=0;              //����ͣ��ʱ��
//    static u32 acc_on_tim=0;                 //ACC����ʱ��
//    static u32 acc_off_tim=0;                //ACC�ر�ʱ��

//#if USE_RTC
//    if(calendar.hour==0x00&&calendar.min==0x00&&calendar.sec==0x00)driver_tim_in_day=0; //ÿ�쿪ʼ�����ʻʱ������
//#else
//	if(gps_location.is_location&&gps_location.bj_tim.hour==0x00&&gps_location.bj_tim.minute==0x00&&gps_location.bj_tim.second==0x00)driver_tim_in_day=0; //ÿ�쿪ʼ�����ʻʱ������
//#endif

//    //ȷ��ACC�������رտ�ʼʱ��
//    if(JTT808_Location_State.acc)            //��ǰACC����
//    {
//        if(!acc_on_tim)acc_on_tim=TimerSecond; 
//        acc_off_tim=0;
//        acc_off_total=0;                     //ACC����������ͣ��ʱ������
//    }
//    else                                     //��ǰACC�ر�
//    {
//        if(!acc_off_tim)acc_off_tim=TimerSecond;
//        acc_on_tim=0;
//    }

//    if((acc_off_tim)&&((TimerSecond-acc_off_tim)>=JTT808_Terminal_Param.rest_min_time))driver_tim_continue=0;//���ACC�ر�ʱ�������С��Ϣʱ�� ����յ���������ʻʱ��

//    if(acc_on_tim)                  //���ACC���� �����ʻʱ��
//    {
//        driver_tim_continue++;
//        driver_tim_in_day++;
//    }
//    
//    if(acc_off_tim)acc_off_total++;//���ACC�ر� ����ͣ��ʱ��

//    if((JTT808_Terminal_Param.continouous_drive_time)&&(driver_tim_continue>=JTT808_Terminal_Param.continouous_drive_time))//������ʻʱ�䳬ʱ   ƣ�ͼ�ʻ
//    {
//        static u32 TTS_tim=0;

//        JTT808_Location_Alarm.fatigue_drive=1;  
//        
//        if((JTT808_Location_State.acc)&&((TimerSecond-TTS_tim)>=60))
//        {
//            TTS_tim=TimerSecond;
//            API_Play_TTS("����ƣ�ͼ�ʻ");
//        }  
//    }
//    else if((JTT808_Terminal_Param.fatigue_driving_diff)&&((driver_tim_continue+JTT808_Terminal_Param.fatigue_driving_diff)>=JTT808_Terminal_Param.continouous_drive_time)) //ƣ�ͼ�ʻԤ��
//    {
//        static u32 TTS_tim=0;  
//        
//        JTT808_Location_Alarm.fatigue_drive_warning=1;

//        if((JTT808_Location_State.acc)&&((TimerSecond-TTS_tim)>=60))
//        {
//            TTS_tim=TimerSecond;
//            API_Play_TTS("����ƣ�ͼ�ʻ");
//        }  
//    }

//    if((JTT808_Terminal_Param.drive_time_in_day)&&(driver_tim_in_day>=JTT808_Terminal_Param.drive_time_in_day))//�����ۻ���ʻʱ�䳬ʱ
//    {
//        static u32 TTS_tim=0;

//        JTT808_Location_Alarm.drive_timeout_in_day=1;   

//        if((JTT808_Location_State.acc)&&((TimerSecond-TTS_tim)>=60)) //���ACC�� ��������ʻԱ
//        {
//            TTS_tim=TimerSecond;
//            API_Play_TTS("�����ۻ���ʻʱ�䳬ʱ");
//        }
//    }

//    if((JTT808_Terminal_Param.stop_max_time)&&(acc_off_tim>=JTT808_Terminal_Param.stop_max_time)) //��ʱͣ��
//    {
//        JTT808_Location_Alarm.stop_timeout=1;
//    }
//}

void JTT808_power_state_check(void)
{
//    switch(main_power_alarm_state)
//    {
//        case MAIN_POWER_NORMAL:
//        break;

//        case MAIN_POWER_LOW:
//            JTT808_Location_Alarm.power_down=1;      //����ԴǷѹ
//        break;

//        case MAIN_POWER_LOSS:  
//            JTT808_Location_Alarm.power_off=1;      //����Դ����
//        break;

//        default:
//        break;
//    }
}


void JTT808_alarm_other_check(void)
{
//    JTT808_Location_Alarm.emergency=IO_Dev.bit_EmergencyAlert; 
//		iso_check();			//�����������
    JTT808_Location_Alarm.gnss_err=gps_location.GNSS_module_fault; 
	if(gps_location.antenna_sta==1)JTT808_Location_Alarm.gnss_disconnect=1;
	if(gps_location.antenna_sta==2)JTT808_Location_Alarm.gnss_ant_shortcircuit=1;
    
    /*****************����δ��********************/ 
    JTT808_Location_Alarm.warning=0;                   //Σ��Ԥ��
    JTT808_Location_Alarm.display_err=0;               //�ն�LCD����ʾ������
    JTT808_Location_Alarm.tts_err=0;                   //TTSģ�����
    JTT808_Location_Alarm.camera_err=0;                //����ͷ����
    JTT808_Location_Alarm.IC_err=0;                    //��·����֤IC��ģ����� 
    JTT808_Location_Alarm.vss_err=0;                   //����VSS����
    JTT808_Location_Alarm.abnormal_fuel=0;             //���������쳣
    JTT808_Location_Alarm.stolen=0;                    //��������(ͨ������������)
    JTT808_Location_Alarm.illegal_ignition=0;          //�����Ƿ���� 
    JTT808_Location_Alarm.illegal_move=0;              //�����Ƿ�λ��
    JTT808_Location_Alarm.rollover_crash=0;            //��ײԤ��
    JTT808_Location_Alarm.side_turn=0;                 //�෭Ԥ�� 
    JTT808_Location_Alarm.illegal_dooropen=0;          //�Ƿ������ű���
}


void JTT808_Location_state_check(void)
{
//    JTT808_Location_State.acc=IO_Dev.bit_ACC;                               //0��ACC��      1��ACC��
		JTT808_Location_State.acc = 1;
    JTT808_Location_State.is_location=gps_location.is_location;             //0��GNSSδ��λ 1��GNSS��λ
    JTT808_Location_State.latitude_type=gps_location.latitude_type;         //0����γ       1����γ
    JTT808_Location_State.longitude_type=gps_location.longitude_type;       //0������       1������ 
    JTT808_Location_State.operation=0;                                      //0����Ӫ״̬   1��ͣ��״̬ 
    JTT808_Location_State.coordinate_is_encrypt=0;                          //0: ��γ��δ����  1����γ���Ѽ���

//    JTT808_Location_State.delivery_status1=IO_Dev.bit_Heavy&0x01;           //
//    JTT808_Location_State.delivery_status2=(IO_Dev.bit_Heavy&0x02)>>1;      //00:�ճ�  01������   10������   11������ 

//    JTT808_Location_State.fuel_way=0;                                       //0��������·����     1��������·�Ͽ� 
//    JTT808_Location_State.circuit=0;                                        //0��������·����     1��������·�Ͽ�
//    JTT808_Location_State.door_lock=IO_Dev.bit_Door;                        //0�����Ž���         1�����ż��� 

    JTT808_Location_State.door1=0;                                          //0����1��        1����1����ǰ�ţ�
    JTT808_Location_State.door2=0;                                          //0����2��        1����2�������ţ�
    JTT808_Location_State.door3=0;                                          //0����3��        1����3�������ţ�
    JTT808_Location_State.door4=0;                                          //0����4��        1����4������ʻϯ�ţ�
    JTT808_Location_State.door5=0;                                          //0����5��        1����5�����Զ��壩

    JTT808_Location_State.location_type_GPS=gps_location.location_type_GPS; //0��δʹ�� GPS ���ǽ��ж�λ��1��ʹ�� GPS ���ǽ��ж�λ
    JTT808_Location_State.location_type_BD= gps_location.location_type_BD;  //0��δʹ�� ���� ���ǽ��ж�λ��1��ʹ�� ���� ���ǽ��ж�λ
    JTT808_Location_State.location_type_GLONASS=0;                          //0��δʹ�� GLONASS ���ǽ��ж�λ��1��ʹ�� GLONASS ���ǽ��ж�λ
    JTT808_Location_State.location_type_GALILEO=0;                          //0��δʹ�� Galileo ���ǽ��ж�λ��1��ʹ�� Galileo ���ǽ��ж�λ
}

void JTT808_AlarmStateCheck(void)
{
    static u32 tim=0;
    if(tim==TimerSecond)return;          //������״̬ÿ����ɨ��һ��
    tim=TimerSecond; 


    JTT808_Location_state_check();       //��ǰ��λ״̬

    
    memset((char*)&JTT808_Location_Alarm,0x00,sizeof(JTT808_Location_Alarm)); //��ձ���λ

    JTT808_Area_Alarm_check();           //����ɨ��
//    JTT808_speed_check();                //�ٶ�ɨ��
//    JTT808_driver_tim_check();           //ƣ�ͼ�ʻ���ۼƼ�ʻɨ��
//    JTT808_power_state_check();          //��Դ״̬ɨ��
    JTT808_alarm_other_check();          //��������״̬
}


