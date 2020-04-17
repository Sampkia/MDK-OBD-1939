/*-----------------------------------------------------------------------
�ļ�����obd_1939.c
�ļ�������obd_1939.c,����SAE J1939��׼Э�����OBD�������Ӧ��

          2014��6��
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

/*----------------------�ڲ���������----------------------*/
uint8_t* OBD_Data_to_MSG_Data(OBD_data* data);
const uint16_t Request_ID[30] = {0};

/*-----------------------------------------------------
��������OBD_ID_to_EXT_ID
���룺Exit_id �ṹ��
�����Identifier��32λ��ʶ���
��������OBD_Id�Ľṹ������ת����32λ���ܷ��͵�Id
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
��������EXT_ID_to_OBD_ID
���룺Identifier
�����OBD_Id
��������Identifier�Ľṹ������ת����OBD_Id
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
��������OBD_Data_to_MSG_Data
���룺data �ṹ�壬OBD J1939��������
�����msg������ָ��,�ɷ��͵�����
���������������е�OBD_data�еĽṹ��dtc_dataת����
      �������еı�׼����
------------------------------------------------*/
uint8_t* OBD_Data_to_MSG_Data(OBD_data* data)
{
  static uint8_t msg[8] = {0};

  msg[0] = data->data1;
  msg[1] = data->data2;
  msg[2] = (uint8_t)(data->DTC.SPN & 0x000FF);
  msg[3] = (uint8_t)(data->DTC.SPN & 0x0FF00) >> 8;
  //ʵ������������16λ��Ȼ��������5λ
  msg[4] = (uint8_t)((data->DTC.SPN & 0x70000) >> 11) |
            data->DTC.FMI;
  msg[5] = (data->DTC.CM << 7) | data->DTC.OC;
  msg[6] = data->data7;
  msg[7] = data->data8;

  return msg;
}


/*----------------------------------------------
��������MSG_Data_to_OBD_Data
���룺Data
�����OBD_data
�����������յ���CAN����ת����OBD�豸�ܹ�ʶ���
      ���ݽṹģʽ
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
��������Set_filter_Receive
���룺priority�����ȼ�
      PGN���������
      target_address��Ŀ���ַ
�����
����������OBD�ľ���ID������CAN���ߵ�Filter
      ���˵����õ���Ϣ�����ƽ���
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
��������obd_set_TxMessage
���룺Identifier��32λ��IDʶ��ţ�29BIT����չģʽ
      data_len���������ݵĳ��ȣ���data�����г�Ա����
      data������ָ��
�����CanTxMsg
�������ú����ķ�ʽʵ���˶�TxMessage�ṹ���Ա�ĸ�ֵ��
      �����ظ�ֵ�Ľṹ��
------------------------------------------------------*/
CanTxMsg* obd_set_TxMessage(uint32_t Identifier,
                            uint8_t data_len,
                            uint8_t* data)
{
  uint8_t i;
  static CanTxMsg TxMessage;

#ifdef STD_11BIT
  TxMessage.StdId = Identifier;         //��׼��ʶ��0x00-0x7FF
  TxMessage.IDE = CAN_ID_STD;						//��Ϣ���ͣ�ʹ�ñ�׼��ʶ��
#endif

#ifdef EXT_29BIT
  TxMessage.ExtId = Identifier;         //��չ��ʶ��0x00-0x3FFFF
  TxMessage.IDE = CAN_ID_EXT; 	        //��Ϣ���ͣ�ʹ����չ��ʶ��
#endif

  TxMessage.RTR = CAN_RTR_DATA; 	//����֡
  TxMessage.DLC = data_len; 		  //��Ϣ֡����

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
��������obd_send_message
���룺
      �ṹ���Ա��can.c�������߿⺯��
      Identifier��32λ��IDʶ��ţ�29BIT����չģʽ
      data_len���������ݵĳ��ȣ���data�����г�Ա����
      data������ָ��
�����Txstaus�����ɹ����ͣ��򷵻�1�����򷵻�0
������
      ����������Ϣ����Ϣ��ʽ��SAE J1939�淶
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
��������obd_request_address_declare
��  �룺��
��  ������
��  ����������ַ��Ϣ������ϸ�ο�SAE J1939
---------------------------------------------------------------*/
void obd_request_address_declare(void)
{
  OBD_Id Add_declare_id;
  uint32_t Identifier;
  uint8_t data_len = 8;
  uint8_t data[8] = {0xFF, 0xFF, 0x00, 0x00,
                     0xFF, 0xFF, 0xFF, 0xFF};

  Set_filter_Receive(P6,
                     0xee00|GLOBAL_ADDRESS,     //�����˲���
                     source_address);

  Add_declare_id.priority = P6;
  Add_declare_id.exp_data_page = 0;
  Add_declare_id.data_page = DP0;
  Add_declare_id.PDU_format = 0xEE;                     //PDU��ʽ
  Add_declare_id.PDU_specific = GLOBAL_ADDRESS;         //PDU�ض���
  Add_declare_id.source_add = source_address;           //Դ��ַ
  Identifier = OBD_ID_to_EXT_ID(&Add_declare_id);       //ת���ɷ�ID

  obd_send_message(Identifier,       //����
                   data_len,
                   data);
  __TIM_COUNT = 0;
  while(__TIM_COUNT <= 250 && !CAN_RX_Flag);      //�ȴ�

#ifdef J1939_DEBUG
  printf(" ������ַ��Ϣ���� \n");
#endif

  if(CAN_RX_Flag)       //�����ݷ���
  {
    if((RxMessage.ExtId & 0x000000FF) == NULL_ADDRESS)
    {
    #ifdef J1939_DEBUG
       printf("����������ַ��Ϣ\n");
    #endif
      source_address = SOURCE_ADDRESS;
    }

    else
    {
      #ifdef J1939_DEBUG
        printf("��ַ��ͻ�����ó�SOURCE_ADDRESS_1\n");
      #endif
      source_address = SOURCE_ADDRESS_1;        //��ַ��ͻ
    }
  }

  else
  {
  #ifdef J1939_DEBUG
    printf("������ַ�ɹ�����ַ: %d \n", SOURCE_ADDRESS);
  #endif
  }
}



/*-------------------------------------------------------
��������J1939_Request_MSG

��  �룺priority�����ȼ�
			target_address������PGN��Ŀ���ַ��
      PGN����Ҫ�����PGN��

��  ������������״̬
        Request_error ,����
        Request_ok ,�ɹ�

��  ������PGN�Ƿ�֧�ֻ�������������ݣ�����Ⱥ���Ϊ�̶���00EA00��
				������ֻ��3λ��Ϊ�������PGN�����ȼ�ΪP6
        LSB��ʽ��8λ����8λ����8λ
        Ĭ�����ݳ���3
---------------------------------------------------------*/
Req_status J1939_Request_MSG(uint8_t priority,
                             uint32_t PGN,
                             uint8_t target_address)
{
  OBD_Id MSG_Id;
  uint8_t data[3];
  uint32_t Identifier;
  uint8_t tx_status;

  //OBD_Id�ṹ��ĸ�ֵ.
  MSG_Id.priority = priority;
  MSG_Id.exp_data_page = 0;
  MSG_Id.data_page = 0;
  MSG_Id.PDU_format = 0xEA;
  MSG_Id.PDU_specific = target_address;    //Ŀ���ַ�����߹㲥��ַ
  MSG_Id.source_add = source_address;

  Identifier = OBD_ID_to_EXT_ID(&MSG_Id);

  data[0] = (PGN & 0x0000FF);
  data[1] = (PGN & 0x00FF00) >> 8;
  data[2] = (PGN & 0xFF0000) >> 16;

  tx_status = obd_send_message(Identifier,
                               3,            //���ݳ���
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
��������J1939_Request_special
��  �룺target_address��Ŀ���ַ
        data����������Ϣ
��  ����Req_status����������״̬
        Request_error ,����
        Request_ok ,�ɹ�
��  ��������ר�����ݣ��糧���Լ�����ı�׼�ȵ�
        Ĭ�����ݳ���8����׼Э����˵Ϊ�Զ��壡
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
                               data_len,            //���ݳ���
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
��������J1939_Send_Acknowledment
��  �룺status��Ӧ����Ϣ״̬
              ACK               0
              NACK              1
              DE_ACCESS         2
              MSG_ERROR         3
        PGN���������
��  ��������Ӧ����Ϣ
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
  ACK_Id.PDU_specific = 0xFF;   //�㲥
  ACK_Id.source_add = source_address;
  Identifier = OBD_ID_to_EXT_ID(&ACK_Id);

  data[0] = status;
  data[1] = 0xFF;       //�鹦��ֵ
  data[2] = data[3] = data[4] = 0xFF;   //����

  data[5] = (PGN & 0x0000FF);
  data[6] = (PGN & 0x00FF00) >> 8;
  data[7] = (PGN & 0xFF0000) >> 16;

  tx_status = obd_send_message(Identifier,
                               8,            //���ݳ���
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
��������J1939_Acknowledgment
��  �룺RxMessage ,���յ���ACK��Ϣ
        Req_PGN�������������
��  ������������ECUӦ���ȷ����Ϣ
        ���յ���������Ϣ��ʽ�鿴SAE J1939��׼Э��
-------------------------------------------------*/
ACK_status J1939_Receive_Acknowledgment(CanRxMsg RxMessage,   //���յ�����֡
                                        uint32_t Req_PGN)     //����֤��PGN
{
  uint32_t PGN = 0;

  PGN = (RxMessage.Data[7] << 16)     |
        (RxMessage.Data[6] << 8)      |
        (RxMessage.Data[5]);

  if(RxMessage.ExtId == ACK_ID && PGN == Req_PGN)
  {

    switch(RxMessage.Data[0])
    {
    case 0:     //�϶�ȷ��ACK
      /*�˴�ʡ��һ����*/

#ifdef  J1939_DEBUG
      printf("PGN: %d �Ǳ�֧�ֵģ�ACKӦ��\n", PGN);
#endif
      return ACK;

    case 1:
      /*�˴�ʡ��һ����*/

#ifdef  J1939_DEBUG
      printf("PGN: %d �ǲ���֧�ֵģ�NACKӦ��\n", PGN);
#endif
      return NACK;

    case 2:
      /*�˴�ʡ��һ����*/

#ifdef  J1939_DEBUG
      printf("PGN: %d ��֧�ֵĵ��ܾ�����\n", PGN);
#endif
      return DE_ACCESS;

    }
  }

  return MSG_ERROR;   //������Ϣ�д�
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
						
						/*�ϴ�����*/
						
				}
		}
}

/*�ϴ�����*/
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
��������OBD_Request_DM1
��  �룺��
��  ������
��  ��������DM1������ݣ���ǰ������Ϣ
------------------------------------------------------*/
void OBD_Request_DM1(void)
{

		
#ifdef J1939_DEBUG
    uint8_t i = 0;
#endif

    Set_filter_Receive(P6, 0xFECA, 0x00);
    J1939_Request_MSG(P6, 0xFECA, 0x00);
#ifdef J1939_DEBUG
      printf("����DM1���\n");
#endif
    __TIM_COUNT = 0;
    while(__TIM_COUNT <= 250 && !CAN_RX_Flag);
    if(CAN_RX_Flag)
    {
      CAN_RX_Flag = 0;
#ifdef J1939_DEBUG
      printf("DM1���ID:%d \n", RxMessage.ExtId);
      printf("������ \n");
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
��������OBD_Request_DM2
��  �룺��
��  ������
��  ��������DM2������ݣ���ʷ������Ϣ
------------------------------------------------------*/
void OBD_Request_DM2(void)
{
#ifdef J1939_DEBUG
    uint8_t i = 0;
#endif
	
    Set_filter_Receive(P6, 0xFECB, 0x00);
    J1939_Request_MSG(P6, 0xFECB, 0x00);
    #ifdef J1939_DEBUG
      printf("����DM2���\n");
    #endif
    __TIM_COUNT = 0;
    while(__TIM_COUNT <= 250 && !CAN_RX_Flag);

    if(CAN_RX_Flag)
    {
      CAN_RX_Flag = 0;
    #ifdef J1939_DEBUG
      printf("DM2���ID:%d \n", RxMessage.ExtId);
      printf("������ \n");
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
��������OBD_Request_DM11
��  �룺��
��  ������
��  ����
------------------------------------------------------*/
void OBD_Request_DM11(void)
{
#ifdef J1939_DEBUG
    uint8_t i = 0;
#endif
	
    Set_filter_Receive(P6, 0xFED3, 0x00);
    J1939_Request_MSG(P6, 0xFED3, 0x00);
    #ifdef J1939_DEBUG
      printf("����DM11���\n");
    #endif
    __TIM_COUNT = 0;
    while(__TIM_COUNT <= 250 && !CAN_RX_Flag);

    if(CAN_RX_Flag)
    {
      CAN_RX_Flag = 0;
    #ifdef J1939_DEBUG
      printf("DM11���ID:%d \n", RxMessage.ExtId);
      printf("������ \n");
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
��������OBD_Request_DM5
��  �룺��
��  ������
��  ��������DM2������ݣ���ʷ������Ϣ
------------------------------------------------------*/
void OBD_Request_DM5(void)
{
#ifdef J1939_DEBUG
    uint8_t i = 0;
#endif

    Set_filter_Receive(P6, 0xFECE, 0x00);
    J1939_Request_MSG(P6, 0xFECE, 0x00);
    #ifdef J1939_DEBUG
      printf("����DM5���\n");
    #endif
    __TIM_COUNT = 0;
    while(__TIM_COUNT <= 250 && !CAN_RX_Flag);

    if(CAN_RX_Flag)
    {
      CAN_RX_Flag = 0;
    #ifdef J1939_DEBUG
      printf("DM5���ID:%d \n", RxMessage.ExtId);
      printf("������ \n");
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
��������OBD_Request_LFC
��  �룺��
��  ������
��  ��������ȼ��������Ϣ
---------------------------------------------------*/
void OBD_Request_LFC(void)
{
#ifdef J1939_DEBUG
    uint8_t i = 0;
#endif

    Set_filter_Receive(P6, 0xFEE9, 0x00);
    J1939_Request_MSG(P6, 0xFEE9, 0x00);
    #ifdef J1939_DEBUG
      printf("����ȼ����������\n");
    #endif
    __TIM_COUNT = 0;
    while(__TIM_COUNT <= 250 && !CAN_RX_Flag);

    if(CAN_RX_Flag)
    {
      CAN_RX_Flag = 0;
    #ifdef J1939_DEBUG
      printf("ȼ������ID:%d \n", RxMessage.ExtId);
      printf("������ \n");
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
��������OBD_Request_Speed
��  �룺��
��  ������
��  ������������Ϣ
----------------------------------------------*/
void OBD_Request_Speed(void)
{
#ifdef J1939_DEBUG
    uint8_t i = 0;
#endif

    Set_filter_Receive(P6, 0xFEE8, 0x00);
    J1939_Request_MSG(P6, 0xFEE8, 0x00);
	
#ifdef J1939_DEBUG
    printf("����������\n");
#endif
	
    __TIM_COUNT = 0;
    while(__TIM_COUNT <= 250 && !CAN_RX_Flag);

    if(CAN_RX_Flag)
    {
      CAN_RX_Flag = 0;
			
#ifdef J1939_DEBUG
      printf("��������ID:%d \n", RxMessage.ExtId);
      printf("������ \n");
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
��������OBD_Request_Speed
��  �룺��
��  ������
��  ������������Ϣ
----------------------------------------------*/
void OBD_Request_Weight(void)
{
#ifdef J1939_DEBUG
    uint8_t i = 0;
#endif
	
    Set_filter_Receive(P6, 0xFEEA, 0x00);
    J1939_Request_MSG(P6, 0xFEEA, 0x00);
    #ifdef J1939_DEBUG
      printf("����������\n");
    #endif
    __TIM_COUNT = 0;
    while(__TIM_COUNT <= 250 && !CAN_RX_Flag);

    if(CAN_RX_Flag)
    {
      CAN_RX_Flag = 0;
			
			//��ȡ������Ϣ
    #ifdef J1939_DEBUG
      printf("��������ID:%d \n", RxMessage.ExtId);
      printf("������ \n");
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
��������OBD_Request_RD
��  �룺��
��  ������
��  ���������г�������Ϣ
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
��������OBD_Request_Brake
��  �룺��
��  ������
��  ��������ɲ����Ϣ
        �������,�²�����Ϊģ���ECU��֧��
----------------------------------------------*/
void OBD_Request_Brake(void)
{
#ifdef J1939_DEBUG
    uint8_t i = 0;
#endif

    Set_filter_Receive(P6, 0xFEFA, 0x00);
    J1939_Request_MSG(P6, 0xFEFA, 0x00);
	
#ifdef J1939_DEBUG
      printf("����ɲ������\n");
#endif
	
    __TIM_COUNT = 0;
    while(__TIM_COUNT <= 250 && !CAN_RX_Flag);

    if(CAN_RX_Flag)
    {
      CAN_RX_Flag = 0;
#ifdef J1939_DEBUG
      printf("ɲ���������ID:%d \n", RxMessage.ExtId);
      printf("������ \n");
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
��������OBD_Request_Brake
��  �룺��
��  ������
��  �������󷢶���������Ϣ������������ת��
        �������������Ϣ
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
      //���ͽ��յ�������
			Up_OBD_Data();
    }
}


/*-----------------------------------------
��������OBD_Request_Eng_Str
��  �룺��
��  ������
��  �������󷢶������ͻ��ṹ������Ϣ������28
				���ֽڣ���ֳ�4��������
------------------------------------------*/
void OBD_Request_Eng_Str(void)
{
		uint8_t i;		//�����
//		uint8_t offset = 0; 		//ƫ�Ƶ�ַ
		
		Set_filter_Receive(P6, 0xFEE3, 0x00);
		
		//��������
    J1939_Request_MSG(P6, 0xFEE3, 0x00);
		//��ʱ
	  for(i=0; i<4; i++)
		{
				__TIM_COUNT = 0;
				while(__TIM_COUNT <= 200 && !CAN_RX_Flag);
			
				if(CAN_RX_Flag)
				{
						CAN_RX_Flag = 0;
						
						//������յ�����
//						memcpy(&RxMessage.Data[1], &OBD_MSG[offset], 7);
//						offset += 7;
					  //���ͽ��յ�������
						Up_OBD_Data();
				}
		}
}


/*-----------------------------------------
��������OBD_Request_Eng_Time
��  �룺��
��  ������
��  �������󷢶������ͻ�ʱ�����ת��8�ֽ�
------------------------------------------*/
void OBD_Request_Eng_Time(void)
{
		//���ù�����
		Set_filter_Receive(P6, 0xFEE5, 0x00);
		//��������
    J1939_Request_MSG(P6, 0xFEE5, 0x00);
	
		__TIM_COUNT = 0;
		while(__TIM_COUNT <= 250 && !CAN_RX_Flag);
	
		if(CAN_RX_Flag)
		{
				CAN_RX_Flag = 0;
			
				//���ͽ��յ�������
				Up_OBD_Data();
		}
}


/*------------------------------------------------------
��������OBD_Request_ET
��  �룺��
��  ������
��  �������󷢷������¶���Ϣ
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
			
				//��������
				Up_OBD_Data();
    }
}


/*------------------------------------------------------
��������OBD_Request_ELP
��  �룺��
��  ������
��  �������󷢷�����Һ���ѹ��
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
			
				//��������
				Up_OBD_Data();
    }
}

/*------------------------------------------------------
��������OBD_Request_ELOP
��  �룺��
��  ������
��  �������󷢷�����Һ�Ͳ�ѹ��,8�ֽ�
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
			
				//��������
				Up_OBD_Data();
    }
}

/*------------------------------------------------------
��������OBD_Request_ELP
��  �룺��
��  ������
��  �������󷢶�����Ϣ������������Ϣ,10�ֽ�
------------------------------------------------------*/
void OBD_Request_ESLF(void)
{
		uint8_t i;		//�����
//		uint8_t offset = 0; 		//ƫ�Ƶ�ַ
	
    Set_filter_Receive(P6, 0xFEB7, 0x00);
    J1939_Request_MSG(P6, 0xFEB7, 0x00);
	
		
		for(i=0; i<2; i++)
	  {
				__TIM_COUNT = 0;
				while(__TIM_COUNT <= 200 && !CAN_RX_Flag);

				if(CAN_RX_Flag)
				{
						CAN_RX_Flag = 0;
					
						//������յ�����
//						memcpy(&RxMessage.Data[1], &OBD_MSG[offset], 7);
//						offset += 7;
					  //���ͽ��յ�������
						Up_OBD_Data();
				}
		}
}


/*------------------------------------------------------
��������OBD_Request_ESC
��  �룺��
��  ������
��  �������󷢶���������ȴ��
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
			
				//���ͽ��յ�������
				Up_OBD_Data();
		}
}

/*------------------------------------------------------
��������OBD_Request_EES
��  �룺��
��  ������
��  �������󷢶�������ϵͳ��ģ����Ϣ
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
			
				//���ͽ��յ�������
				Up_OBD_Data();
		}
}

/*------------------------------------------------------
��������OBD_Request_EES
��  �룺��
��  ������
��  �������󷢶�����Ϣ
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
			
				//���ͽ��յ�������
				Up_OBD_Data();
		}
}


/*------------------------------------------------------
��������OBD_Request_EES
��  �룺��
��  ������
��  �������󷢶���ȼ�ϡ��󻬼�ϵͳ
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
			
				//���ͽ��յ�������
				Up_OBD_Data();
		}
}

