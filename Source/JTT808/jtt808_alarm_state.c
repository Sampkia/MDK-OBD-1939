#include "include.h"

JTT808t_LocationState  JTT808_Location_State={0};     //定位状态
JTT808t_LocationAlarm  JTT808_Location_Alarm={0};     //报警状态


//void JTT808_speed_check(void)                         //速度扫描
//{
//    static u32 speeding_start_tim=0;                  //超速开始时间

//    if((!JTT808_Location_State.acc)||(!JTT808_Location_State.is_location))		      //ACC关退出	未定位退出
//	{
//		speeding_start_tim=0;
//		return;
//	}


//    if((JTT808_Terminal_Param.top_speed)&&((gps_location.speed/10)>=JTT808_Terminal_Param.top_speed))//超速报警
//    {
//        static u32 TTS_tim=0;

//        if(!speeding_start_tim)speeding_start_tim=TimerSecond; //超速开始时间
//        if((TimerSecond-speeding_start_tim)>=JTT808_Terminal_Param.speeding_duration)//超速持续时间大于设定参数值
//        {
//            JTT808_Location_Alarm.speeding=1;

//            if(TimerSecond-TTS_tim>=60)         //每60秒上报超速
//            {
//                TTS_tim=TimerSecond;
//                API_Play_TTS("您已超速");
//            }
//        }
//    }
//    else if((JTT808_Terminal_Param.overspeed_alarm_diff)&&(((gps_location.speed/10)+(JTT808_Terminal_Param.overspeed_alarm_diff/10))>=JTT808_Terminal_Param.top_speed))//超速预警
//    {
//        static u32 TTS_tim=0;

//        speeding_start_tim=0;
//        JTT808_Location_Alarm.speeding_warning=1;

//        if(TimerSecond-TTS_tim>=60)           //每60秒上报超速
//        {
//            TTS_tim=TimerSecond;
//            API_Play_TTS("您即将超速");
//        }
//    }
//    else
//    {
//        speeding_start_tim=0;
//    }
//}

//void JTT808_driver_tim_check(void)           //驾驶、停车时间扫描  1秒钟扫描一次，代码中的大延时不做考虑
//{
//    static u32 driver_tim_in_day=0;          //当天连续驾驶时间
//    static u32 driver_tim_continue=0;        //单次连续驾驶时间
//    static u32 acc_off_total=0;              //连续停车时间
//    static u32 acc_on_tim=0;                 //ACC开启时间
//    static u32 acc_off_tim=0;                //ACC关闭时间

//#if USE_RTC
//    if(calendar.hour==0x00&&calendar.min==0x00&&calendar.sec==0x00)driver_tim_in_day=0; //每天开始当天驾驶时间清零
//#else
//	if(gps_location.is_location&&gps_location.bj_tim.hour==0x00&&gps_location.bj_tim.minute==0x00&&gps_location.bj_tim.second==0x00)driver_tim_in_day=0; //每天开始当天驾驶时间清零
//#endif

//    //确定ACC开启、关闭开始时间
//    if(JTT808_Location_State.acc)            //当前ACC开启
//    {
//        if(!acc_on_tim)acc_on_tim=TimerSecond; 
//        acc_off_tim=0;
//        acc_off_total=0;                     //ACC开启，连续停车时间清零
//    }
//    else                                     //当前ACC关闭
//    {
//        if(!acc_off_tim)acc_off_tim=TimerSecond;
//        acc_on_tim=0;
//    }

//    if((acc_off_tim)&&((TimerSecond-acc_off_tim)>=JTT808_Terminal_Param.rest_min_time))driver_tim_continue=0;//如果ACC关闭时间大于最小休息时间 则清空单次连续驾驶时间

//    if(acc_on_tim)                  //如果ACC开启 计算驾驶时间
//    {
//        driver_tim_continue++;
//        driver_tim_in_day++;
//    }
//    
//    if(acc_off_tim)acc_off_total++;//如果ACC关闭 计算停车时间

//    if((JTT808_Terminal_Param.continouous_drive_time)&&(driver_tim_continue>=JTT808_Terminal_Param.continouous_drive_time))//连续驾驶时间超时   疲劳驾驶
//    {
//        static u32 TTS_tim=0;

//        JTT808_Location_Alarm.fatigue_drive=1;  
//        
//        if((JTT808_Location_State.acc)&&((TimerSecond-TTS_tim)>=60))
//        {
//            TTS_tim=TimerSecond;
//            API_Play_TTS("您已疲劳驾驶");
//        }  
//    }
//    else if((JTT808_Terminal_Param.fatigue_driving_diff)&&((driver_tim_continue+JTT808_Terminal_Param.fatigue_driving_diff)>=JTT808_Terminal_Param.continouous_drive_time)) //疲劳驾驶预警
//    {
//        static u32 TTS_tim=0;  
//        
//        JTT808_Location_Alarm.fatigue_drive_warning=1;

//        if((JTT808_Location_State.acc)&&((TimerSecond-TTS_tim)>=60))
//        {
//            TTS_tim=TimerSecond;
//            API_Play_TTS("您将疲劳驾驶");
//        }  
//    }

//    if((JTT808_Terminal_Param.drive_time_in_day)&&(driver_tim_in_day>=JTT808_Terminal_Param.drive_time_in_day))//当天累积驾驶时间超时
//    {
//        static u32 TTS_tim=0;

//        JTT808_Location_Alarm.drive_timeout_in_day=1;   

//        if((JTT808_Location_State.acc)&&((TimerSecond-TTS_tim)>=60)) //如果ACC打开 报警给驾驶员
//        {
//            TTS_tim=TimerSecond;
//            API_Play_TTS("当天累积驾驶时间超时");
//        }
//    }

//    if((JTT808_Terminal_Param.stop_max_time)&&(acc_off_tim>=JTT808_Terminal_Param.stop_max_time)) //超时停车
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
//            JTT808_Location_Alarm.power_down=1;      //主电源欠压
//        break;

//        case MAIN_POWER_LOSS:  
//            JTT808_Location_Alarm.power_off=1;      //主电源掉电
//        break;

//        default:
//        break;
//    }
}


void JTT808_alarm_other_check(void)
{
//    JTT808_Location_Alarm.emergency=IO_Dev.bit_EmergencyAlert; 
//		iso_check();			//紧急报警检测
    JTT808_Location_Alarm.gnss_err=gps_location.GNSS_module_fault; 
	if(gps_location.antenna_sta==1)JTT808_Location_Alarm.gnss_disconnect=1;
	if(gps_location.antenna_sta==2)JTT808_Location_Alarm.gnss_ant_shortcircuit=1;
    
    /*****************以下未做********************/ 
    JTT808_Location_Alarm.warning=0;                   //危险预警
    JTT808_Location_Alarm.display_err=0;               //终端LCD或显示器故障
    JTT808_Location_Alarm.tts_err=0;                   //TTS模块故障
    JTT808_Location_Alarm.camera_err=0;                //摄像头故障
    JTT808_Location_Alarm.IC_err=0;                    //道路运输证IC卡模块故障 
    JTT808_Location_Alarm.vss_err=0;                   //车辆VSS故障
    JTT808_Location_Alarm.abnormal_fuel=0;             //车辆油量异常
    JTT808_Location_Alarm.stolen=0;                    //车辆被盗(通过车辆防盗器)
    JTT808_Location_Alarm.illegal_ignition=0;          //车辆非法点火 
    JTT808_Location_Alarm.illegal_move=0;              //车辆非法位移
    JTT808_Location_Alarm.rollover_crash=0;            //碰撞预警
    JTT808_Location_Alarm.side_turn=0;                 //侧翻预警 
    JTT808_Location_Alarm.illegal_dooropen=0;          //非法开车门报警
}


void JTT808_Location_state_check(void)
{
//    JTT808_Location_State.acc=IO_Dev.bit_ACC;                               //0：ACC关      1：ACC开
		JTT808_Location_State.acc = 1;
    JTT808_Location_State.is_location=gps_location.is_location;             //0：GNSS未定位 1：GNSS定位
    JTT808_Location_State.latitude_type=gps_location.latitude_type;         //0：北纬       1：南纬
    JTT808_Location_State.longitude_type=gps_location.longitude_type;       //0：东经       1：西经 
    JTT808_Location_State.operation=0;                                      //0：运营状态   1：停运状态 
    JTT808_Location_State.coordinate_is_encrypt=0;                          //0: 经纬度未加密  1：经纬度已加密

//    JTT808_Location_State.delivery_status1=IO_Dev.bit_Heavy&0x01;           //
//    JTT808_Location_State.delivery_status2=(IO_Dev.bit_Heavy&0x02)>>1;      //00:空车  01：半载   10：保留   11：满载 

//    JTT808_Location_State.fuel_way=0;                                       //0：车辆油路正常     1：车辆油路断开 
//    JTT808_Location_State.circuit=0;                                        //0：车辆电路正常     1：车辆电路断开
//    JTT808_Location_State.door_lock=IO_Dev.bit_Door;                        //0：车门解锁         1：车门加锁 

    JTT808_Location_State.door1=0;                                          //0：门1关        1：门1开（前门）
    JTT808_Location_State.door2=0;                                          //0：门2关        1：门2开（中门）
    JTT808_Location_State.door3=0;                                          //0：门3关        1：门3开（后门）
    JTT808_Location_State.door4=0;                                          //0：门4关        1：门4开（驾驶席门）
    JTT808_Location_State.door5=0;                                          //0：门5关        1：门5开（自定义）

    JTT808_Location_State.location_type_GPS=gps_location.location_type_GPS; //0：未使用 GPS 卫星进行定位；1：使用 GPS 卫星进行定位
    JTT808_Location_State.location_type_BD= gps_location.location_type_BD;  //0：未使用 北斗 卫星进行定位；1：使用 北斗 卫星进行定位
    JTT808_Location_State.location_type_GLONASS=0;                          //0：未使用 GLONASS 卫星进行定位；1：使用 GLONASS 卫星进行定位
    JTT808_Location_State.location_type_GALILEO=0;                          //0：未使用 Galileo 卫星进行定位；1：使用 Galileo 卫星进行定位
}

void JTT808_AlarmStateCheck(void)
{
    static u32 tim=0;
    if(tim==TimerSecond)return;          //报警、状态每秒钟扫描一次
    tim=TimerSecond; 


    JTT808_Location_state_check();       //当前定位状态

    
    memset((char*)&JTT808_Location_Alarm,0x00,sizeof(JTT808_Location_Alarm)); //清空报警位

    JTT808_Area_Alarm_check();           //区域扫描
//    JTT808_speed_check();                //速度扫描
//    JTT808_driver_tim_check();           //疲劳驾驶、累计驾驶扫描
//    JTT808_power_state_check();          //电源状态扫描
    JTT808_alarm_other_check();          //其他报警状态
}


