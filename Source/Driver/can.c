/*-----------------------------------------------------------------------
�ļ�������can.c,��Ҫ���CANģ��ĳ�ʼ����������ķ��ͣ����շ�ʽ�����жϴ���
          ��ϸ�������Ʋ��жϺ�����
          ���庯��������ο���Ӧ��ע�ͺ͹ٷ��ĵ�������ϸ˵��
          
          By Sindge
-------------------------------------------------------------------------*/

/*-------------include-------------------*/
#include "can.h"
#include "debug.h"
#include "include.h"

uint8_t CAN_RX_Flag = 0;
CanRxMsg RxMessage;

/*----------------------�ڲ���������--------------------*/
static void CAN_GPIO_Init(void);
static void CAN_NVIC_Config(void);
static void CAN_Config(CAN_TypeDef * CANx);


/*--------------------------------
��������can_gpio_init
���룺��
�������
��������ʼ��CAN���ߵ�IO�ڣ��ڲ�����
			PB12:CANRX
			PB13:CANTX
---------------------------------*/
static void CAN_GPIO_Init(void)
{
	  GPIO_InitTypeDef GPIO_InitStructure;
	
		//����ʱ������
#if USE_CAN1
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	  RCC_APB1PeriphClockCmd(RCC_APB1Periph_CAN1, ENABLE);
#else
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	  RCC_APB1PeriphClockCmd(RCC_APB1Periph_CAN2, ENABLE);
#endif
	
#if USE_REMAP_CAN && USE_CAN1
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO | RCC_APB2Periph_GPIOD, ENABLE);
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_CAN1, ENABLE);
	  //��ӳ��
  	GPIO_PinRemapConfig(GPIO_Remap2_CAN1, ENABLE);
#endif	

	  //��ʼ��CAN�Ľ�������
	  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
    GPIO_InitStructure.GPIO_Pin = CAN_RX_GPIO_PIN;
	  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	  GPIO_Init(CAN_RX_GPIO_GROUP, &GPIO_InitStructure);
	
		//��ʼ��CAN�ķ�������
	  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	  GPIO_InitStructure.GPIO_Pin = CAN_TX_GPIO_PIN;
	  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(CAN_TX_GPIO_GROUP, &GPIO_InitStructure);
}



/*--------------------------------
��������CAN_NVIC_Config
���룺��
�������
��������ʼ��CAN���߽����жϣ��ڲ�����
---------------------------------*/
static void CAN_NVIC_Config(void)
{
  NVIC_InitTypeDef NVIC_InitStructure;
        
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);			//��ռ������

#if USE_CAN1
  NVIC_InitStructure.NVIC_IRQChannel = CAN1_RX0_IRQn;            //CAN�����ж�
#else
//	NVIC_InitStructure.NVIC_IRQChannel = CAN2_RX0_IRQn;
#endif
	
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;		//��ռ���ȼ�
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;			//�����ȼ�
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
}



/*--------------------------------
��������CAN_Config
���룺��
�������
��������ʼ��CAN���ߣ��ڲ�����,CAN���߳�ʼ��
�����֣�CAN_InitStructure�ṹ��
       CAN_FilterInitStructure�ṹ��
---------------------------------*/
static void CAN_Config(CAN_TypeDef * CANx)
{
	CAN_InitTypeDef CAN_InitStructure;
	CAN_FilterInitTypeDef CAN_FilterInitStructure;
        
	CAN_DeInit(CANx);
	CAN_InitStructure.CAN_TTCM = DISABLE;		//ʱ�䴥��ģʽ
	CAN_InitStructure.CAN_ABOM = DISABLE;		//�Զ����߹���
	CAN_InitStructure.CAN_AWUM = DISABLE;		//�Զ�����ģʽ
	CAN_InitStructure.CAN_NART = DISABLE;		//�Զ��ش�ģʽ
	CAN_InitStructure.CAN_RFLM = DISABLE;  	        //����FIFO����ģʽ
	CAN_InitStructure.CAN_TXFP = DISABLE; 	        //����FIFO���ȼ�
	
#ifdef CAN_MODE_NORMAL
        CAN_InitStructure.CAN_Mode = CAN_Mode_Normal;		//CAN��ͨģʽ
#endif

#ifdef CAN_MODE_LOOPBACK
        CAN_InitStructure.CAN_Mode = CAN_Mode_LoopBack;	        //CAN�ػ�ģʽ������ʹ��
#endif
        
        //���䲨������� 
        //speed = 36/((CAN_SJW+CAN_BS1+CAN_BS2)*CAN_Prescaler)
	CAN_InitStructure.CAN_SJW = CAN_SJW_1tq;		//����ͬ����Ծ���
	CAN_InitStructure.CAN_BS1 = CAN_BS1_12tq;		//ʱ���1ռ��12��ʱ�䵥Ԫ
	CAN_InitStructure.CAN_BS2 = CAN_BS2_5tq;		//ʱ���2ռ��5��ʱ�䵥Ԫ
        
#ifdef  CAN_SPEED_250
        CAN_InitStructure.CAN_Prescaler = 8;			//BTR-BRP �����ʷ�Ƶ�� �趨ʱ�䵥λ
#endif
        
#ifdef CAN_SPEED_500
       CAN_InitStructure.CAN_Prescaler = 4;
#endif
       
        CAN_Init(CANx, &CAN_InitStructure);
	
	//��ʼ��Filter
	CAN_FilterInitStructure.CAN_FilterNumber = 0;		//ָ��������
	CAN_FilterInitStructure.CAN_FilterMode = CAN_FilterMode_IdMask;	        //����λģʽ
	CAN_FilterInitStructure.CAN_FilterScale = CAN_FilterScale_32bit;        //������λ��
	CAN_FilterInitStructure.CAN_FilterIdHigh = 0x0000;	//�趨��������ʶ��
	CAN_FilterInitStructure.CAN_FilterIdLow = 0x0001;
	CAN_FilterInitStructure.CAN_FilterMaskIdHigh = 0x0000;	//�趨���α�ʶ��
	CAN_FilterInitStructure.CAN_FilterMaskIdLow = 0x0000;
	CAN_FilterInitStructure.CAN_FilterFIFOAssignment = CAN_FilterFIFO0;	//������ָ��FIFO0
	CAN_FilterInitStructure.CAN_FilterActivation = ENABLE;
	CAN_FilterInit(&CAN_FilterInitStructure);
	CAN_ITConfig(CANx, CAN_IT_FMP0, ENABLE);	        //FIFO0��Ϣ�Һ�  ����
}

/*------------------------------------------------------
��������CAN_Conduct_Filter
���룺CANx
			state
�������
����������Filter
-------------------------------------------------------*/
void CAN_Conduct_Filter(CAN_TypeDef * CANx, FunctionalState state)
{
  CAN_FilterInitTypeDef CAN_FilterInitStructure;
  
  CAN_FilterInitStructure.CAN_FilterNumber = 0;		//ָ��������
  CAN_FilterInitStructure.CAN_FilterActivation = state;
  CAN_FilterInit(&CAN_FilterInitStructure);
  
  CAN_FIFORelease(CANx, CAN_FIFO0);     //�ͷ�Filter0
}


/*------------------------------------------------------
��������CAN_SetFilterExt
���룺Identifier
�������
����������Filter�Ĺ���ID
-------------------------------------------------------*/
void CAN_SetFilter(u32 Identifier)
{
  CAN_FilterInitTypeDef  CAN_FilterInitStructure;
  CAN_FilterInitStructure.CAN_FilterNumber = 0;
  CAN_FilterInitStructure.CAN_FilterMode = CAN_FilterMode_IdMask;
  CAN_FilterInitStructure.CAN_FilterScale = CAN_FilterScale_32bit;
  CAN_FilterInitStructure.CAN_FilterIdHigh = ((Identifier<<3)&0xFFFF0000) >> 16;
  CAN_FilterInitStructure.CAN_FilterIdLow = ((Identifier<<3)|CAN_ID_EXT|CAN_RTR_DATA) & 0xFFFF;
  CAN_FilterInitStructure.CAN_FilterMaskIdHigh = 0xFFFF;
  CAN_FilterInitStructure.CAN_FilterMaskIdLow = 0xFFF0;
  CAN_FilterInitStructure.CAN_FilterFIFOAssignment = CAN_FIFO0;
  CAN_FilterInitStructure.CAN_FilterActivation = ENABLE;
  
#ifdef J1939_DEBUG
  printf("��ʱ��������ʶ���IDΪ��%ld \n", Identifier);
#endif
	
  CAN_FilterInit(&CAN_FilterInitStructure);
}


/*---------------------------------------------
��������CANx_Init
���룺��
�������
��������ʼ��CANģ�飬����IO�ڡ�CAN�жϡ�CAN���õĳ�ʼ��
      main�����е���
-----------------------------------------------*/
void CANx_Init(CAN_TypeDef* CANx)
{
	  CAN_GPIO_Init();
	  CAN_NVIC_Config();
    CAN_Config(CANx);
}


/*-----------------------------------------------------------
��������CAN_Send
���룺CANx��CAN�˿�
      TxMessage�ṹ���Ա��
      TxMessage.StdId = 0x00; 	        //��׼��ʶ��
      TxMessage.ExtId = 0xf0f0;	        //��չ��ʶ��
      TxMessage.IDE = CAN_ID_STD; 	//��Ϣ���ͣ�ʹ�ñ�׼��ʶ��
      TxMessage.RTR = CAN_RTR_DATA; 	//�������ͣ�����֡
      TxMessage.DLC = 2; 		//��Ϣ֡����
      TxMessage.Data[0] = 0xAA; 	//����
      TxMessage.Data[1] = 0x55;

�����status�����ط�����Ϣ��״̬��CANTXOK��
        CANTXPENDING��CAN_TxStatus_Failed����֮һ��
������ʵ���Ƕ�CAN_Transmit()��������д��ֻ��������
      ״̬��⣬�ͷ��͵ȴ�ʱ��
-----------------------------------------------------------*/
uint8_t CAN_Send(CAN_TypeDef* CANx,
                 CanTxMsg* TxMessage)
{
  uint8_t Send_Mailbox = 0;
  uint8_t Send_Status;
  uint8_t count = 0;
	
  Send_Mailbox = CAN_Transmit(CANx, TxMessage);
  
//  Send_Status = CAN_TransmitStatus(CAN1, Send_Mailbox);
  while(((Send_Status = CAN_TransmitStatus(CANx, Send_Mailbox)) != CANTXOK)
        && count < 255)
  {
    count++;
  }
  return Send_Status;
}

/*
void USB_LP_CAN1_RX0_IRQHandler(void)
{
  
  CAN_Receive(CAN1, CAN_FIFO0, &RxMessage);
        
	//if((RxMessage.ExtId == 0x12)
        //   && (RxMessage.IDE == CAN_ID_EXT))
  if(RxMessage.IDE == CAN_ID_EXT)
  {
    CAN_RX_Flag = 1;
  }
  else
  {
    CAN_RX_Flag = 0;
  }
  CAN_ClearITPendingBit(CAN1, CAN_IT_FMP0);
}
*/
