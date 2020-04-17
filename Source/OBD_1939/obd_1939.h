#ifndef __OBD_1939_H
#define __OBD_1939_H

/*-----includes-----*/
#include "include.h"

/*-------extern--------*/
extern uint8_t CAN_RX_Flag;
extern CanRxMsg RxMessage;
extern uint8_t CAN_Data_Flag;


/*--------------------struct--------------------*/
typedef struct OBD_J1939_Identifier
{
  uint8_t priority;             //优先级
  uint8_t exp_data_page;        //扩展页
  uint8_t data_page;            //数据页
  uint8_t PDU_format;           //PDU格式
  uint8_t PDU_specific;         //PDU地址，或者。。。
  uint8_t source_add;           //源地址
}OBD_Id;


typedef struct DTC_data
{
  unsigned SPN : 19;    //可疑参数编号
  unsigned FMI : 5;     //故障模式标志
  unsigned CM : 1;      //发生次数
  unsigned OC : 7;      //转化方式
}dtc_data;


typedef struct OBD_1939_data
{
  uint8_t data1;
  uint8_t data2;
  dtc_data DTC;
  uint8_t data7;
  uint8_t data8;
}OBD_data;



//请求状态
typedef enum Request_status
{
  Request_error = 0,
  Request_ok = 1
}Req_status;



typedef enum Response_ACK_status
{
  ACK = 0,
  NACK = 1,
  DE_ACCESS = 2,
  MSG_ERROR = 3
}ACK_status;


/*--------------------宏定义---------------------*/
//#define STD_11BIT               //标准模式
#define EXT_29BIT               	//扩展模式

#define MAX_DATA_LEN    8



/*---------------------OBD_Id中的priority---------------------*/
#define P1      1
#define P2      2
#define P3      3
#define P4      4
#define P5      5
#define P6      6
#define P7      7

/*----------------------OBD_Id中的data_page--------------------*/
#define DP0     0
#define DP1     1
  
/*-------------------------地址定义------------------------*/
#define GLOBAL_ADDRESS          0xFF
#define NULL_ADDRESS            0xFE
#define SOURCE_ADDRESS          0xF9
#define SOURCE_ADDRESS_1        0xFA

/*------------------------OBD J1939 ID定义-----------------------*/
#define ACK_ID          0x18E8FF00


/*--------------------------函数申明--------------------------*/
uint32_t OBD_ID_to_EXT_ID(OBD_Id * obd_id);
OBD_Id * EXT_ID_to_OBD_ID(uint32_t Identifier);
OBD_data* MSG_Data_to_OBD_Data(uint8_t* Data);

void Set_filter_Receive(uint8_t priority, 
                        uint16_t PGN, 
                        uint8_t target_address);

CanTxMsg* obd_set_TxMessage(uint32_t Identifier,
                            uint8_t data_len,
                            uint8_t* data);


void obd_request_address_declare(void);


uint8_t obd_send_message(uint32_t Identifier,
                         uint8_t data_len,
                         uint8_t* data);

Req_status J1939_Request_MSG(uint8_t priority,
                             uint32_t PGN,
                             uint8_t target_address);

Req_status J1939_Request_special(uint8_t target_address,        //专用请求
                                 uint8_t data_len,
                                 uint8_t * data);

Req_status J1939_Send_Acknowledment(ACK_status status,          //发送应答信息
                                    uint32_t PGN);

ACK_status J1939_Receive_Acknowledgment(CanRxMsg RxMessage,     //接收应答处理
                                        uint32_t Req_PGN);



void OBD_Request_DM1(void);
void OBD_Request_DM2(void);
void OBD_Request_DM5(void);
void OBD_Request_DM11(void);
void OBD_Request_ET1(void);     //发动机温度
void OBD_Request_LFC(void);     //燃料消费
void OBD_Request_RD(void);      //行车距离
void OBD_Request_Speed(void);   //车速
void OBD_Request_Weight(void);  //车重
void OBD_Request_Brake(void);   //刹车
void OBD_Request_EEC1(void);    //发动机转速
void OBD_Request_Eng_Str(void);
void OBD_Request_EFL(void);
void OBD_Request_Eng_Time(void);
void OBD_Request_ET(void);
void OBD_Request_ELOP(void);
void OBD_Request_ESLF(void);
void OBD_Request_ESC(void);
void OBD_Request_EES(void);
void OBD_Request_EM(void);
void OBD_Request_ELP(void);
#endif
