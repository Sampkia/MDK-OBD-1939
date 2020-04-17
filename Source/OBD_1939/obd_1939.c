/*-----------------------------------------------------------------------
文件名：obd_1939.c
文件描述：obd_1939.c,基于SAE J1939标准协议完成OBD的请求和应答

          2014年6月
          By Sindge
-------------------------------------------------------------------------*/

#include <stm32f10x.h>
#include "Driver/can.h"
#include "OBD_1939/obd_1939.h"
#include "JTT808/jtt808_initiative_send.h"

uint8_t Request_status = 0;
uint8_t Response_status = 0;
uint8_t source_address = SOURCE_ADDRESS;
uint8_t OBD_MSG[512] = {0};
uint8_t CAN_Data_Flag = 0;

/*----------------------内部函数申明----------------------*/
uint8_t* OBD_Data_to_MSG_Data(OBD_data* data);
const uint16_t Request_ID[30] = {0};

/*-----------------------------------------------------
函数名：OBD_ID_to_EXT_ID
输入：Exit_id 结构体
输出：Identifier，32位的识别号
描述：将OBD_Id的结构体数据转化成32位的能发送的Id
------------------------------------------------------*/
uint32_t OBD_ID_to_EXT_ID(OBD_Id * obd_id)
{
  uint32_t Identifier = 0;

  Identifier = (obd_id->source_add)            |
               (obd_id->PDU_specific << 8)     |
               (obd_id->PDU_format << 16)      |
               (obd_id->data_page << 24)       |
               (obd_id->exp_data_page << 25)   |
               (obd_id->priority << 26);

  return Identifier;
}


/*-----------------------------------------------------
函数名：EXT_ID_to_OBD_ID
输入：Identifier
输出：OBD_Id
描述：将Identifier的结构体数据转化成OBD_Id
------------------------------------------------------*/
OBD_Id * EXT_ID_to_OBD_ID(uint32_t Identifier)
{
  static OBD_Id obd_id;

  obd_id.priority = Identifier & 0x1C000000 >> 26;
  obd_id.exp_data_page = Identifier & 0x02000000 >> 25;
  obd_id.data_page = Identifier & 0x01000000 >> 24;
  obd_id.PDU_format = Identifier & 0x00FF0000 >> 16;
  obd_id.PDU_specific = Identifier & 0x0000FF00 >> 8;
  obd_id.source_add = Identifier & 0x000000FF;

  return &obd_id;
}


/*----------------------------------------------
函数名：OBD_Data_to_MSG_Data
输入：data 结构体，OBD J1939的数据域
输出：msg，数组指针,可发送的数据
描述：将数据域中的OBD_data中的结构体dtc_data转换成
      数据域中的标准数据
------------------------------------------------*/
uint8_t* OBD_Data_to_MSG_Data(OBD_data* data)
{
  static uint8_t msg[8] = {0};

  msg[0] = data->data1;
  msg[1] = data->data2;
  msg[2] = (uint8_t)(data->DTC.SPN & 0x000FF);
  msg[3] = (uint8_t)(data->DTC.SPN & 0x0FF00) >> 8;
  //实际上是先右移16位，然后在左移5位
  msg[4] = (uint8_t)((data->DTC.SPN & 0x70000) >> 11) |
            data->DTC.FMI;
  msg[5] = (data->DTC.CM << 7) | data->DTC.OC;
  msg[6] = data->data7;
  msg[7] = data->data8;

  return msg;
}


/*----------------------------------------------
函数名：MSG_Data_to_OBD_Data
输入：Data
输出：OBD_data
描述：将接收到的CAN数据转换成OBD设备能够识别的
      数据结构模式
------------------------------------------------*/
OBD_data* MSG_Data_to_OBD_Data(uint8_t* Data)
{
  static OBD_data data;

  data.data1 = Data[0];
  data.data2 = Data[1];
  data.DTC.SPN = ((Data[4] & 0xE0) >> 16)       |
                  Data[3] >> 8                  |
                  Data[2];
  data.DTC.FMI = Data[4] & 0x1F;
  data.DTC.CM = Data[5] & 0x80 >> 7;
  data.DTC.OC = Data[5] & 0x7F;
  data.data7 = Data[6];
  data.data8 = Data[7];

  return &data;
}

/*-----------------------------------------------------
函数名：Set_filter_Receive
输入：priority，优先级
      PGN，参数组号
      target_address，目标地址
输出：
描述：依据OBD的具体ID，设置CAN总线的Filter
      过滤掉无用的信息，控制接收
-----------------------------------------------------*/
void Set_filter_Receive(uint8_t priority,
                        uint16_t PGN,
                        uint8_t target_address)
{
  uint32_t temp0 = 0x00000000;

  CAN_Conduct_Filter(CAN1, DISABLE);
  temp0 = temp0 | (priority << 26) | (PGN << 8) | target_address;
  CAN_SetFilter(temp0);
}

/*
void Set_filter_Receive(uint32_t Identifer)
{
  CAN_conduct_Filter(DISABLE);
  CAN_SetFilter(Identifer);
}
*/



/*-----------------------------------------------------
函数名：obd_set_TxMessage
输入：Identifier：32位的ID识别号，29BIT的扩展模式
      data_len：发送数据的长度，即data数组中成员个数
      data：数组指针
输出：CanTxMsg
描述：用函数的方式实现了对TxMessage结构体成员的赋值，
      并返回赋值的结构体
------------------------------------------------------*/
CanTxMsg* obd_set_TxMessage(uint32_t Identifier,
                            uint8_t data_len,
                            uint8_t* data)
{
  uint8_t i;
  static CanTxMsg TxMessage;

#ifdef STD_11BIT
  TxMessage.StdId = Identifier;         //标准标识符0x00-0x7FF
  TxMessage.IDE = CAN_ID_STD;						//消息类型，使用标准标识符
#endif

#ifdef EXT_29BIT
  TxMessage.ExtId = Identifier;         //扩展标识符0x00-0x3FFFF
  TxMessage.IDE = CAN_ID_EXT; 	        //消息类型，使用扩展标识符
#endif

  TxMessage.RTR = CAN_RTR_DATA; 	//数据帧
  TxMessage.DLC = data_len; 		  //消息帧长度

  for(i=0;i<MAX_DATA_LEN; i++)
  {
    TxMessage.Data[i] = 0;
  }

  for(i=0; i<data_len; i++)
  {
    TxMessage.Data[i] = data[i];
  }

  return &TxMessage;
}


/*-----------------------------------------------------
函数名：obd_send_message
输入：
      结构体成员见can.c函数或者库函数
      Identifier：32位的ID识别号，29BIT的扩展模式
      data_len：发送数据的长度，即data数组中成员个数
      data：数组指针
输出：Txstaus，若成功发送，则返回1，否则返回0
描述：
      发送请求信息，信息格式见SAE J1939规范
--------------------------------------------------------*/
uint8_t obd_send_message(uint32_t Identifier,
                         uint8_t data_len,
                         uint8_t* data)
{
  CanTxMsg* TxMessage;
  uint8_t Txstaus;

  TxMessage = obd_set_TxMessage(Identifier,
                                data_len,
                                data);

  Txstaus = CAN_Send(CAN1, TxMessage);
  return Txstaus;
}


/*--------------------------------------------------------------
函数名：obd_request_address_declare
输  入：无
输  出：无
描  述：声明地址消息请求，详细参考SAE J1939
---------------------------------------------------------------*/
void obd_request_address_declare(void)
{
  OBD_Id Add_declare_id;
  uint32_t Identifier;
  uint8_t data_len = 8;
  uint8_t data[8] = {0xFF, 0xFF, 0x00, 0x00,
                     0xFF, 0xFF, 0xFF, 0xFF};

  Set_filter_Receive(P6,
                     0xee00|GLOBAL_ADDRESS,     //设置滤波器
                     source_address);

  Add_declare_id.priority = P6;
  Add_declare_id.exp_data_page = 0;
  Add_declare_id.data_page = DP0;
  Add_declare_id.PDU_format = 0xEE;                     //PDU格式
  Add_declare_id.PDU_specific = GLOBAL_ADDRESS;         //PDU特定域
  Add_declare_id.source_add = source_address;           //源地址
  Identifier = OBD_ID_to_EXT_ID(&Add_declare_id);       //转换成发ID

  obd_send_message(Identifier,       //发送
                   data_len,
                   data);
  __TIM_COUNT = 0;
  while(__TIM_COUNT <= 250 && !CAN_RX_Flag);      //等待

#ifdef J1939_DEBUG
  printf(" 声明地址消息请求 \n");
#endif

  if(CAN_RX_Flag)       //有数据返回
  {
    if((RxMessage.ExtId & 0x000000FF) == NULL_ADDRESS)
    {
    #ifdef J1939_DEBUG
       printf("不能声明地址消息\n");
    #endif
      source_address = SOURCE_ADDRESS;
    }

    else
    {
      #ifdef J1939_DEBUG
        printf("地址冲突，设置成SOURCE_ADDRESS_1\n");
      #endif
      source_address = SOURCE_ADDRESS_1;        //地址冲突
    }
  }

  else
  {
  #ifdef J1939_DEBUG
    printf("声明地址成功，地址: %d \n", SOURCE_ADDRESS);
  #endif
  }
}



/*-------------------------------------------------------
函数名：J1939_Request_MSG

输  入：priority，优先级
			target_address，请求PGN的目标地址。
      PGN，需要请求的PGN。

输  出：返回请求状态
        Request_error ,错误
        Request_ok ,成功

描  叙：请求PGN是否被支持或者请求相关数据，参数群编号为固定的00EA00。
				数据域只有3位，为被请求的PGN，优先级为P6
        LSB格式高8位，中8位，低8位
        默认数据长度3
---------------------------------------------------------*/
Req_status J1939_Request_MSG(uint8_t priority,
                             uint32_t PGN,
                             uint8_t target_address)
{
  OBD_Id MSG_Id;
  uint8_t data[3];
  uint32_t Identifier;
  uint8_t tx_status;

  //OBD_Id结构体的赋值.
  MSG_Id.priority = priority;
  MSG_Id.exp_data_page = 0;
  MSG_Id.data_page = 0;
  MSG_Id.PDU_format = 0xEA;
  MSG_Id.PDU_specific = target_address;    //目标地址，或者广播地址
  MSG_Id.source_add = source_address;

  Identifier = OBD_ID_to_EXT_ID(&MSG_Id);

  data[0] = (PGN & 0x0000FF);
  data[1] = (PGN & 0x00FF00) >> 8;
  data[2] = (PGN & 0xFF0000) >> 16;

  tx_status = obd_send_message(Identifier,
                               3,            //数据长度
                               data);
  if(tx_status == CANTXOK)
  {
    return Request_ok;
  }
  else
  {
    return Request_error;
  }
}


/*--------------------------------------------
函数名：J1939_Request_special
输  入：target_address，目标地址
        data，数据域信息
输  出：Req_status，返回请求状态
        Request_error ,错误
        Request_ok ,成功
描  述：请求专用数据，如厂商自己定义的标准等等
        默认数据长度8，标准协议上说为自定义！
----------------------------------------------*/
Req_status J1939_Request_special(uint8_t target_address,
                                 uint8_t data_len,
                                 uint8_t * data)
{
  OBD_Id Spe_Id;
  uint32_t Identifier;
  uint8_t tx_status;

  Spe_Id.priority = P6;
  Spe_Id.exp_data_page = 0;
  Spe_Id.data_page = DP0;
  Spe_Id.PDU_format = 0xEF;
  Spe_Id.PDU_specific = target_address;
  Spe_Id.source_add = source_address;
  Identifier = OBD_ID_to_EXT_ID(&Spe_Id);

  tx_status = obd_send_message(Identifier,
                               data_len,            //数据长度
                               data);
  if(tx_status == CANTXOK)
  {
    return Request_ok;
  }
  else
  {
    return Request_error;
  }
}


/*----------------------------------------------
函数名：J1939_Send_Acknowledment
输  入：status，应答信息状态
              ACK               0
              NACK              1
              DE_ACCESS         2
              MSG_ERROR         3
        PGN，参数组号
描  述：发送应答信息
----------------------------------------------*/
Req_status J1939_Send_Acknowledment(ACK_status status,
                                    uint32_t PGN)
{
  OBD_Id ACK_Id;
  uint32_t Identifier;
  uint8_t data[8] = {0};
  uint8_t tx_status;

  ACK_Id.priority = P6;
  ACK_Id.exp_data_page = 0;
  ACK_Id.data_page = DP0;
  ACK_Id.PDU_format = 0xE8;
  ACK_Id.PDU_specific = 0xFF;   //广播
  ACK_Id.source_add = source_address;
  Identifier = OBD_ID_to_EXT_ID(&ACK_Id);

  data[0] = status;
  data[1] = 0xFF;       //组功能值
  data[2] = data[3] = data[4] = 0xFF;   //保留

  data[5] = (PGN & 0x0000FF);
  data[6] = (PGN & 0x00FF00) >> 8;
  data[7] = (PGN & 0xFF0000) >> 16;

  tx_status = obd_send_message(Identifier,
                               8,            //数据长度
                               data);
  if(tx_status == CANTXOK)
  {
    return Request_ok;
  }
  else
  {
    return Request_error;
  }
}

/*------------------------------------------------
函数名：J1939_Acknowledgment
输  入：RxMessage ,接收到的ACK信息
        Req_PGN，被请求参数组
描  述：接收来自ECU应答的确认信息
        接收到的数据信息格式查看SAE J1939标准协议
-------------------------------------------------*/
ACK_status J1939_Receive_Acknowledgment(CanRxMsg RxMessage,   //接收的数据帧
                                        uint32_t Req_PGN)     //待验证的PGN
{
  uint32_t PGN = 0;

  PGN = (RxMessage.Data[7] << 16)     |
        (RxMessage.Data[6] << 8)      |
        (RxMessage.Data[5]);

  if(RxMessage.ExtId == ACK_ID && PGN == Req_PGN)
  {

    switch(RxMessage.Data[0])
    {
    case 0:     //肯定确认ACK
      /*此处省略一万字*/

#ifdef  J1939_DEBUG
      printf("PGN: %d 是被支持的，ACK应答\n", PGN);
#endif
      return ACK;

    case 1:
      /*此处省略一万字*/

#ifdef  J1939_DEBUG
      printf("PGN: %d 是不被支持的，NACK应答\n", PGN);
#endif
      return NACK;

    case 2:
      /*此处省略一万字*/

#ifdef  J1939_DEBUG
      printf("PGN: %d 是支持的但拒绝访问\n", PGN);
#endif
      return DE_ACCESS;

    }
  }

  return MSG_ERROR;   //输入信息有错
}


void OBD_Process(void)
{
		if(CAN_Data_Flag)
		{
				CAN_Data_Flag = 0;
			
				__TIM_COUNT = 0;
				while(__TIM_COUNT <= 250 && !CAN_RX_Flag);
				
				if(CAN_RX_Flag)
				{
						CAN_RX_Flag = 0;
						
						/*上传数据*/
						
				}
		}
}

/*上传数据*/
void Up_OBD_Data(void)
{
		JTT808t_CanID_Sample_Set obd_id={0};
		
		obd_id.can_id = RxMessage.ExtId;
		obd_id.sample_type = 0;
		obd_id.frame_type = 1;
		obd_id.can_channel = 1;
	
		JTT808_CAN_data_upload(obd_id.can_id, obd_id.sample_type, obd_id.frame_type, obd_id.can_channel, (char*)RxMessage.Data);
}

/*------------------------------------------------------
函数名：OBD_Request_DM1
输  入：无
输  出：无
描  述：请求DM1诊断数据，当前故障信息
------------------------------------------------------*/
void OBD_Request_DM1(void)
{

		
#ifdef J1939_DEBUG
    uint8_t i = 0;
#endif

    Set_filter_Receive(P6, 0xFECA, 0x00);
    J1939_Request_MSG(P6, 0xFECA, 0x00);
#ifdef J1939_DEBUG
      printf("请求DM1诊断\n");
#endif
    __TIM_COUNT = 0;
    while(__TIM_COUNT <= 250 && !CAN_RX_Flag);
    if(CAN_RX_Flag)
    {
      CAN_RX_Flag = 0;
#ifdef J1939_DEBUG
      printf("DM1诊断ID:%d \n", RxMessage.ExtId);
      printf("数据域： \n");
      for(i=0; i<8; i++)
      {
        printf(" %d", RxMessage.Data[i]);
      }
#endif
				
			Up_OBD_Data();
    }
		
#ifdef J1939_DEBUG
      printf("\n \n");
#endif
}


/*------------------------------------------------------
函数名：OBD_Request_DM2
输  入：无
输  出：无
描  述：请求DM2诊断数据，历史故障信息
------------------------------------------------------*/
void OBD_Request_DM2(void)
{
#ifdef J1939_DEBUG
    uint8_t i = 0;
#endif
	
    Set_filter_Receive(P6, 0xFECB, 0x00);
    J1939_Request_MSG(P6, 0xFECB, 0x00);
    #ifdef J1939_DEBUG
      printf("请求DM2诊断\n");
    #endif
    __TIM_COUNT = 0;
    while(__TIM_COUNT <= 250 && !CAN_RX_Flag);

    if(CAN_RX_Flag)
    {
      CAN_RX_Flag = 0;
    #ifdef J1939_DEBUG
      printf("DM2诊断ID:%d \n", RxMessage.ExtId);
      printf("数据域： \n");
      for(i=0; i<8; i++)
      {
        printf(" %d", RxMessage.Data[i]);
      }
    #endif
			Up_OBD_Data();
    }
    #ifdef J1939_DEBUG
      printf("\n \n");
    #endif
}

/*------------------------------------------------------
函数名：OBD_Request_DM11
输  入：无
输  出：无
描  述：
------------------------------------------------------*/
void OBD_Request_DM11(void)
{
#ifdef J1939_DEBUG
    uint8_t i = 0;
#endif
	
    Set_filter_Receive(P6, 0xFED3, 0x00);
    J1939_Request_MSG(P6, 0xFED3, 0x00);
    #ifdef J1939_DEBUG
      printf("请求DM11诊断\n");
    #endif
    __TIM_COUNT = 0;
    while(__TIM_COUNT <= 250 && !CAN_RX_Flag);

    if(CAN_RX_Flag)
    {
      CAN_RX_Flag = 0;
    #ifdef J1939_DEBUG
      printf("DM11诊断ID:%d \n", RxMessage.ExtId);
      printf("数据域： \n");
      for(i=0; i<8; i++)
      {
        printf(" %d", RxMessage.Data[i]);
      }
    #endif
			Up_OBD_Data();
    }
    #ifdef J1939_DEBUG
      printf("\n \n");
    #endif
}


/*------------------------------------------------------
函数名：OBD_Request_DM5
输  入：无
输  出：无
描  述：请求DM2诊断数据，历史故障信息
------------------------------------------------------*/
void OBD_Request_DM5(void)
{
#ifdef J1939_DEBUG
    uint8_t i = 0;
#endif

    Set_filter_Receive(P6, 0xFECE, 0x00);
    J1939_Request_MSG(P6, 0xFECE, 0x00);
    #ifdef J1939_DEBUG
      printf("请求DM5诊断\n");
    #endif
    __TIM_COUNT = 0;
    while(__TIM_COUNT <= 250 && !CAN_RX_Flag);

    if(CAN_RX_Flag)
    {
      CAN_RX_Flag = 0;
    #ifdef J1939_DEBUG
      printf("DM5诊断ID:%d \n", RxMessage.ExtId);
      printf("数据域： \n");
      for(i=0; i<8; i++)
      {
        printf(" %d", RxMessage.Data[i]);
      }
    #endif
			Up_OBD_Data();
    }
  #ifdef J1939_DEBUG
    printf("\n \n");
  #endif
}


/*---------------------------------------------------
函数名：OBD_Request_LFC
输  入：无
输  出：无
描  述：请求燃料消费信息
---------------------------------------------------*/
void OBD_Request_LFC(void)
{
#ifdef J1939_DEBUG
    uint8_t i = 0;
#endif

    Set_filter_Receive(P6, 0xFEE9, 0x00);
    J1939_Request_MSG(P6, 0xFEE9, 0x00);
    #ifdef J1939_DEBUG
      printf("请求燃料消费数据\n");
    #endif
    __TIM_COUNT = 0;
    while(__TIM_COUNT <= 250 && !CAN_RX_Flag);

    if(CAN_RX_Flag)
    {
      CAN_RX_Flag = 0;
    #ifdef J1939_DEBUG
      printf("燃料消费ID:%d \n", RxMessage.ExtId);
      printf("数据域： \n");
      for(i=0; i<8; i++)
      {
        printf(" %d", RxMessage.Data[i]);
      }
    #endif
			Up_OBD_Data();
    }
    #ifdef J1939_DEBUG
      printf("\n \n");
    #endif
}


/*---------------------------------------------
函数名：OBD_Request_Speed
输  入：无
输  出：无
描  述：请求车速信息
----------------------------------------------*/
void OBD_Request_Speed(void)
{
#ifdef J1939_DEBUG
    uint8_t i = 0;
#endif

    Set_filter_Receive(P6, 0xFEE8, 0x00);
    J1939_Request_MSG(P6, 0xFEE8, 0x00);
	
#ifdef J1939_DEBUG
    printf("请求车速数据\n");
#endif
	
    __TIM_COUNT = 0;
    while(__TIM_COUNT <= 250 && !CAN_RX_Flag);

    if(CAN_RX_Flag)
    {
      CAN_RX_Flag = 0;
			
#ifdef J1939_DEBUG
      printf("车速数据ID:%d \n", RxMessage.ExtId);
      printf("数据域： \n");
      for(i=0; i<8; i++)
      {
        printf(" %d", RxMessage.Data[i]);
      }
#endif
			Up_OBD_Data();
    }
		
#ifdef J1939_DEBUG
      printf("\n \n");
#endif
}


/*---------------------------------------------
函数名：OBD_Request_Speed
输  入：无
输  出：无
描  述：请求车重信息
----------------------------------------------*/
void OBD_Request_Weight(void)
{
#ifdef J1939_DEBUG
    uint8_t i = 0;
#endif
	
    Set_filter_Receive(P6, 0xFEEA, 0x00);
    J1939_Request_MSG(P6, 0xFEEA, 0x00);
    #ifdef J1939_DEBUG
      printf("请求车重数据\n");
    #endif
    __TIM_COUNT = 0;
    while(__TIM_COUNT <= 250 && !CAN_RX_Flag);

    if(CAN_RX_Flag)
    {
      CAN_RX_Flag = 0;
			
			//获取车重信息
    #ifdef J1939_DEBUG
      printf("车重数据ID:%d \n", RxMessage.ExtId);
      printf("数据域： \n");
      for(i=0; i<8; i++)
      {
        printf(" %d", RxMessage.Data[i]);
      }
    #endif
			Up_OBD_Data();
    }
    #ifdef J1939_DEBUG
      printf("\n \n");
    #endif
}


/*-----------------------------------------------------
函数名：OBD_Request_RD
输  入：无
输  出：无
描  述：请求行车距离信息
-----------------------------------------------------*/
void OBD_Request_RD(void)
{
    Set_filter_Receive(P6, 0xFEE0, 0x00);
    J1939_Request_MSG(P6, 0xFEE0, 0x00);
	
    __TIM_COUNT = 0;
    while(__TIM_COUNT <= 250 && !CAN_RX_Flag);

    if(CAN_RX_Flag)
    {
      CAN_RX_Flag = 0;
			Up_OBD_Data();
    }
}


/*---------------------------------------------
函数名：OBD_Request_Brake
输  入：无
输  出：无
描  述：请求刹车信息
        这个不行,猜测是因为模拟板ECU不支持
----------------------------------------------*/
void OBD_Request_Brake(void)
{
#ifdef J1939_DEBUG
    uint8_t i = 0;
#endif

    Set_filter_Receive(P6, 0xFEFA, 0x00);
    J1939_Request_MSG(P6, 0xFEFA, 0x00);
	
#ifdef J1939_DEBUG
      printf("请求刹车数据\n");
#endif
	
    __TIM_COUNT = 0;
    while(__TIM_COUNT <= 250 && !CAN_RX_Flag);

    if(CAN_RX_Flag)
    {
      CAN_RX_Flag = 0;
#ifdef J1939_DEBUG
      printf("刹车相关数据ID:%d \n", RxMessage.ExtId);
      printf("数据域： \n");
      for(i=0; i<8; i++)
      {
        printf(" %d", RxMessage.Data[i]);
      }
#endif
			Up_OBD_Data();
    }
		
#ifdef J1939_DEBUG
      printf("\n \n");
#endif
}



/*-----------------------------------------
函数名：OBD_Request_Brake
输  入：无
输  出：无
描  述：请求发动机控制信息，包括发动机转速
        发动机需求等信息
------------------------------------------*/
void OBD_Request_EEC1(void)
{
    Set_filter_Receive(P3, 0xF004, 0x00);
    J1939_Request_MSG(P3, 0xF004, 0x00);

	
    __TIM_COUNT = 0;
    while(__TIM_COUNT <= 250 && !CAN_RX_Flag);

    if(CAN_RX_Flag)
    {
      CAN_RX_Flag = 0;
      //发送接收到的数据
			Up_OBD_Data();
    }
}


/*-----------------------------------------
函数名：OBD_Request_Eng_Str
输  入：无
输  出：无
描  述：请求发动机发送机结构，该信息的数据28
				个字节，拆分成4个包传输
------------------------------------------*/
void OBD_Request_Eng_Str(void)
{
		uint8_t i;		//拆包数
//		uint8_t offset = 0; 		//偏移地址
		
		Set_filter_Receive(P6, 0xFEE3, 0x00);
		
		//发送请求
    J1939_Request_MSG(P6, 0xFEE3, 0x00);
		//计时
	  for(i=0; i<4; i++)
		{
				__TIM_COUNT = 0;
				while(__TIM_COUNT <= 200 && !CAN_RX_Flag);
			
				if(CAN_RX_Flag)
				{
						CAN_RX_Flag = 0;
						
						//缓存接收的数据
//						memcpy(&RxMessage.Data[1], &OBD_MSG[offset], 7);
//						offset += 7;
					  //发送接收到的数据
						Up_OBD_Data();
				}
		}
}


/*-----------------------------------------
函数名：OBD_Request_Eng_Time
输  入：无
输  出：无
描  述：请求发动机发送机时间和旋转，8字节
------------------------------------------*/
void OBD_Request_Eng_Time(void)
{
		//设置过滤器
		Set_filter_Receive(P6, 0xFEE5, 0x00);
		//发送请求
    J1939_Request_MSG(P6, 0xFEE5, 0x00);
	
		__TIM_COUNT = 0;
		while(__TIM_COUNT <= 250 && !CAN_RX_Flag);
	
		if(CAN_RX_Flag)
		{
				CAN_RX_Flag = 0;
			
				//发送接收到的数据
				Up_OBD_Data();
		}
}


/*------------------------------------------------------
函数名：OBD_Request_ET
输  入：无
输  出：无
描  述：请求发发动机温度信息
------------------------------------------------------*/
void OBD_Request_ET(void)
{
    Set_filter_Receive(P6, 0xFEEE, 0x00);
    J1939_Request_MSG(P6, 0xFEEE, 0x00);
    __TIM_COUNT = 0;
    while(__TIM_COUNT <= 250 && !CAN_RX_Flag);

    if(CAN_RX_Flag)
    {
				CAN_RX_Flag = 0;
			
				//发送数据
				Up_OBD_Data();
    }
}


/*------------------------------------------------------
函数名：OBD_Request_ELP
输  入：无
输  出：无
描  述：请求发发动机液体层压力
------------------------------------------------------*/
void OBD_Request_ELP(void)
{
    Set_filter_Receive(P6, 0xFEEF, 0x00);
    J1939_Request_MSG(P6, 0xFEEF, 0x00);
    __TIM_COUNT = 0;
    while(__TIM_COUNT <= 250 && !CAN_RX_Flag);

    if(CAN_RX_Flag)
    {
				CAN_RX_Flag = 0;
			
				//发送数据
				Up_OBD_Data();
    }
}

/*------------------------------------------------------
函数名：OBD_Request_ELOP
输  入：无
输  出：无
描  述：请求发发动机液油层压力,8字节
------------------------------------------------------*/
void OBD_Request_ELOP(void)
{
    Set_filter_Receive(P6, 0xFEDB, 0x00);
    J1939_Request_MSG(P6, 0xFEDB, 0x00);
    __TIM_COUNT = 0;
    while(__TIM_COUNT <= 250 && !CAN_RX_Flag);

    if(CAN_RX_Flag)
    {
				CAN_RX_Flag = 0;
			
				//发送数据
				Up_OBD_Data();
    }
}

/*------------------------------------------------------
函数名：OBD_Request_ELP
输  入：无
输  出：无
描  述：请求发动机信息和载入因素信息,10字节
------------------------------------------------------*/
void OBD_Request_ESLF(void)
{
		uint8_t i;		//拆包数
//		uint8_t offset = 0; 		//偏移地址
	
    Set_filter_Receive(P6, 0xFEB7, 0x00);
    J1939_Request_MSG(P6, 0xFEB7, 0x00);
	
		
		for(i=0; i<2; i++)
	  {
				__TIM_COUNT = 0;
				while(__TIM_COUNT <= 200 && !CAN_RX_Flag);

				if(CAN_RX_Flag)
				{
						CAN_RX_Flag = 0;
					
						//缓存接收的数据
//						memcpy(&RxMessage.Data[1], &OBD_MSG[offset], 7);
//						offset += 7;
					  //发送接收到的数据
						Up_OBD_Data();
				}
		}
}


/*------------------------------------------------------
函数名：OBD_Request_ESC
输  入：无
输  出：无
描  述：请求发动机辅助冷却剂
------------------------------------------------------*/
void OBD_Request_ESC(void)
{
		Set_filter_Receive(P6, 0xFE94, 0x00);
    J1939_Request_MSG(P6, 0xFE94, 0x00);
	
		__TIM_COUNT = 0;
		while(__TIM_COUNT <= 250 && !CAN_RX_Flag);
	
    if(CAN_RX_Flag)
		{
				CAN_RX_Flag = 0;
			
				//发送接收到的数据
				Up_OBD_Data();
		}
}

/*------------------------------------------------------
函数名：OBD_Request_EES
输  入：无
输  出：无
描  述：请求发动机电子系统、模块信息
------------------------------------------------------*/
void OBD_Request_EES(void)
{
		Set_filter_Receive(P6, 0xFE93, 0x00);
    J1939_Request_MSG(P6, 0xFE93, 0x00);
	
		__TIM_COUNT = 0;
		while(__TIM_COUNT <= 250 && !CAN_RX_Flag);
	
    if(CAN_RX_Flag)
		{
				CAN_RX_Flag = 0;
			
				//发送接收到的数据
				Up_OBD_Data();
		}
}

/*------------------------------------------------------
函数名：OBD_Request_EES
输  入：无
输  出：无
描  述：请求发动机信息
------------------------------------------------------*/
void OBD_Request_EM(void)
{
		Set_filter_Receive(P6, 0xFE92, 0x00);
    J1939_Request_MSG(P6, 0xFE92, 0x00);
	
		__TIM_COUNT = 0;
		while(__TIM_COUNT <= 250 && !CAN_RX_Flag);
	
    if(CAN_RX_Flag)
		{
				CAN_RX_Flag = 0;
			
				//发送接收到的数据
				Up_OBD_Data();
		}
}


/*------------------------------------------------------
函数名：OBD_Request_EES
输  入：无
输  出：无
描  述：请求发动机燃料、润滑剂系统
------------------------------------------------------*/
void OBD_Request_EFL(void)
{
		Set_filter_Receive(P6, 0xFE6A, 0x00);
    J1939_Request_MSG(P6, 0xFE6A, 0x00);
	
		__TIM_COUNT = 0;
		while(__TIM_COUNT <= 250 && !CAN_RX_Flag);
	
    if(CAN_RX_Flag)
		{
				CAN_RX_Flag = 0;
			
				//发送接收到的数据
				Up_OBD_Data();
		}
}

