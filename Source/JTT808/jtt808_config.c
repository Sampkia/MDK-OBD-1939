#include "include.h"
#include "OBD_1939/obd_1939.h"

u8 SIM_ICCID[21]={0};

extern STATE_GPRS currentGPRSState;


int API_get_gprs_state(void)
{
	if(!API_get_socket_status(0))JTT808_Flag_Authentication=0;
	return API_get_socket_status(0);
}

void API_GPRS_Reset(void)
{
    GPRS_StartUp();
}


/************获取GPRS待发缓冲区大小************/
int API_get_send_buf_len(void)
{
    return API_get_send_data_len(0);
}


/*********数据发送******************/
void JTT808_TXD(u8 *src, u32 size)
{
	API_server_data_send(0,src,size);
}

#if !USE_TL
void JTT808_TXD_BAK(u8 *src, u32 size)
{
//	QueuePutchars(&GprsTx_bak, (char *)src, size, 0);
}
#endif

/*********获取信号强度********/
int API_Get_singal(void)
{
    return 1;
}

/********************************
*       终端关机
* 要求：1、关闭显示屏背光、清屏
*       2、关闭GPS、GPRS模块电源
*       3、关闭所有中断
*       4、关闭所有外设电源（如SPI、打印机、LED灯等）
*       5、程序执行到该函数不再返回
*********************************/
void Terminal_PowerDown(void)
{
}

/*********关闭数据通信***********/
void JTT808_Gprs_Disconnect(void)
{
}

/*********关闭所有无线通信********/
void JTT808_Disconnect_All(void)
{
}

/**********拨打电话***************/
void API_make_phone_call(void)
{
//#if !USE_TL
//    currentGPRSState = GPRS_Talk;
//	currentCmdToGPRS = CMD_GPRS_Null;
//#endif
}


/*********车辆加锁*****************/
void LockCar(void)
{
	GPIO_SetBits(GPIOB, GPIO_Pin_12);
}


/*********车辆解锁**************/
void UnLockCar(void)
{
	GPIO_ResetBits(GPIOB, GPIO_Pin_12);
}


/**************禁止开车门*******************/
void JTT808_Forbidden_opendoor(void)
{
}

/********GNSS采集详细定位数据*********/
void JTT808_GNSS_Sample(void)
{
}


/********TTS语音播报****************/
void API_Play_TTS(char* buf)
{
    printf("%s\r\n",buf);
//    API_PlayTTS((unsigned char*)buf,strlen(buf));
}


u32 two_point_distance(u32 lat1,u8 lat1_type,u32 long1,u8 long1_type,u32 lat2,u8 lat2_type,u32 long2,u8 long2_type)
{
     return sqrt(pow((6371004*3.1415926*((lat1/1000000.0)-(lat2/1000000.0))/180),2)+pow((6371004*3.1415926*((long1/1000000.0)-(long2/1000000.0))*cos(lat1*3.1415926/1000000.0/180)/180),2));
}



/*
	获取CAN_ID数据   
	参数：
	    can_id:      表示 CAN 总线 ID   
		sample_type：表示数据采集方式，0：原始数据，1：采集区间的平均值	  
		frame_type： 表示帧类型，0：标准帧，1：扩展帧	 
		can_channel：表示 CAN 通道号，0：CAN1，1：CAN2  
	返回值：成功返回1，失败返回0
*/
int API_get_Can_ID_data(void)
{
	extern int obd_count;

	/*
		Set_filter_Receive(priority, pgn, 0x00);
		CAN_Data_Flag = 1;
		J1939_Request_MSG(priority, pgn, 0x00);
	*/
		switch(obd_count)
		{
				case 0:			//当前故障信息
					OBD_Request_DM1();
					break;
	
				case 1:			//当前速度
					OBD_Request_Speed();
					break;
				
				case 2:
					OBD_Request_Eng_Str();
					break;
				
				case 3:				//发动机旋转时间
					OBD_Request_Eng_Time();	
					break;
				
				case 4:
					OBD_Request_ET();		//发动机温度
					break;
				
				case 5:
					OBD_Request_ELP();		//发送机液体层压力
					break;
				
				case 6:					//发送机液油层压力
					OBD_Request_ELOP();
					break;
				
				case 7:
					OBD_Request_ESLF();
					break;
				
				case 8:
					OBD_Request_ESC();
				  break;
				
				case 9:
					OBD_Request_EES();
					break;
				
				case 10:
					OBD_Request_EM();
					break;
				
				case 11: 
					OBD_Request_EFL();
					break;
				
				default:
					obd_count = 0;
					return 0;
		}
		
		obd_count++;
		return 0;
}


