/*-----------------------------------------------------------------------
文件描述：can.c,主要完成CAN模块的初始化和最基本的发送，接收方式采用中断处理
          详细操作请移步中断函数！
          具体函数配置请参考相应的注释和官方文档，有详细说明
          
          By Sindge
-------------------------------------------------------------------------*/

/*-------------include-------------------*/
#include "can.h"
#include "debug.h"
#include "include.h"

uint8_t CAN_RX_Flag = 0;
CanRxMsg RxMessage;

/*----------------------内部函数申明--------------------*/
static void CAN_GPIO_Init(void);
static void CAN_NVIC_Config(void);
static void CAN_Config(CAN_TypeDef * CANx);


/*--------------------------------
函数名：can_gpio_init
输入：无
输出：无
描述：初始化CAN总线的IO口，内部调用
			PB12:CANRX
			PB13:CANTX
---------------------------------*/
static void CAN_GPIO_Init(void)
{
	  GPIO_InitTypeDef GPIO_InitStructure;
	
		//外设时钟设置
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
	  //重映射
  	GPIO_PinRemapConfig(GPIO_Remap2_CAN1, ENABLE);
#endif	

	  //初始化CAN的接收引脚
	  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
    GPIO_InitStructure.GPIO_Pin = CAN_RX_GPIO_PIN;
	  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	  GPIO_Init(CAN_RX_GPIO_GROUP, &GPIO_InitStructure);
	
		//初始化CAN的发送引脚
	  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	  GPIO_InitStructure.GPIO_Pin = CAN_TX_GPIO_PIN;
	  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(CAN_TX_GPIO_GROUP, &GPIO_InitStructure);
}



/*--------------------------------
函数名：CAN_NVIC_Config
输入：无
输出：无
描述：初始化CAN总线接收中断，内部调用
---------------------------------*/
static void CAN_NVIC_Config(void)
{
  NVIC_InitTypeDef NVIC_InitStructure;
        
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);			//抢占优先组

#if USE_CAN1
  NVIC_InitStructure.NVIC_IRQChannel = CAN1_RX0_IRQn;            //CAN接收中断
#else
//	NVIC_InitStructure.NVIC_IRQChannel = CAN2_RX0_IRQn;
#endif
	
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;		//抢占优先级
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;			//子优先级
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
}



/*--------------------------------
函数名：CAN_Config
输入：无
输出：无
描述：初始化CAN总线，内部调用,CAN总线初始化
两部分：CAN_InitStructure结构体
       CAN_FilterInitStructure结构体
---------------------------------*/
static void CAN_Config(CAN_TypeDef * CANx)
{
	CAN_InitTypeDef CAN_InitStructure;
	CAN_FilterInitTypeDef CAN_FilterInitStructure;
        
	CAN_DeInit(CANx);
	CAN_InitStructure.CAN_TTCM = DISABLE;		//时间触发模式
	CAN_InitStructure.CAN_ABOM = DISABLE;		//自动离线管理
	CAN_InitStructure.CAN_AWUM = DISABLE;		//自动唤醒模式
	CAN_InitStructure.CAN_NART = DISABLE;		//自动重传模式
	CAN_InitStructure.CAN_RFLM = DISABLE;  	        //接收FIFO锁定模式
	CAN_InitStructure.CAN_TXFP = DISABLE; 	        //发送FIFO优先级
	
#ifdef CAN_MODE_NORMAL
        CAN_InitStructure.CAN_Mode = CAN_Mode_Normal;		//CAN普通模式
#endif

#ifdef CAN_MODE_LOOPBACK
        CAN_InitStructure.CAN_Mode = CAN_Mode_LoopBack;	        //CAN回环模式，调试使用
#endif
        
        //传输波特率相关 
        //speed = 36/((CAN_SJW+CAN_BS1+CAN_BS2)*CAN_Prescaler)
	CAN_InitStructure.CAN_SJW = CAN_SJW_1tq;		//重新同步跳跃宽度
	CAN_InitStructure.CAN_BS1 = CAN_BS1_12tq;		//时间段1占用12个时间单元
	CAN_InitStructure.CAN_BS2 = CAN_BS2_5tq;		//时间段2占用5个时间单元
        
#ifdef  CAN_SPEED_250
        CAN_InitStructure.CAN_Prescaler = 8;			//BTR-BRP 波特率分频器 设定时间单位
#endif
        
#ifdef CAN_SPEED_500
       CAN_InitStructure.CAN_Prescaler = 4;
#endif
       
        CAN_Init(CANx, &CAN_InitStructure);
	
	//初始化Filter
	CAN_FilterInitStructure.CAN_FilterNumber = 0;		//指定过滤器
	CAN_FilterInitStructure.CAN_FilterMode = CAN_FilterMode_IdMask;	        //屏蔽位模式
	CAN_FilterInitStructure.CAN_FilterScale = CAN_FilterScale_32bit;        //过滤器位宽
	CAN_FilterInitStructure.CAN_FilterIdHigh = 0x0000;	//设定过滤器标识符
	CAN_FilterInitStructure.CAN_FilterIdLow = 0x0001;
	CAN_FilterInitStructure.CAN_FilterMaskIdHigh = 0x0000;	//设定屏蔽标识符
	CAN_FilterInitStructure.CAN_FilterMaskIdLow = 0x0000;
	CAN_FilterInitStructure.CAN_FilterFIFOAssignment = CAN_FilterFIFO0;	//过滤器指向FIFO0
	CAN_FilterInitStructure.CAN_FilterActivation = ENABLE;
	CAN_FilterInit(&CAN_FilterInitStructure);
	CAN_ITConfig(CANx, CAN_IT_FMP0, ENABLE);	        //FIFO0消息挂号  接收
}

/*------------------------------------------------------
函数名：CAN_Conduct_Filter
输入：CANx
			state
输出：无
描述：设置Filter
-------------------------------------------------------*/
void CAN_Conduct_Filter(CAN_TypeDef * CANx, FunctionalState state)
{
  CAN_FilterInitTypeDef CAN_FilterInitStructure;
  
  CAN_FilterInitStructure.CAN_FilterNumber = 0;		//指定过滤器
  CAN_FilterInitStructure.CAN_FilterActivation = state;
  CAN_FilterInit(&CAN_FilterInitStructure);
  
  CAN_FIFORelease(CANx, CAN_FIFO0);     //释放Filter0
}


/*------------------------------------------------------
函数名：CAN_SetFilterExt
输入：Identifier
输出：无
描述：设置Filter的过滤ID
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
  printf("此时过滤器能识别的ID为：%ld \n", Identifier);
#endif
	
  CAN_FilterInit(&CAN_FilterInitStructure);
}


/*---------------------------------------------
函数名：CANx_Init
输入：无
输出：无
描述：初始化CAN模块，包括IO口、CAN中断、CAN配置的初始化
      main函数中调用
-----------------------------------------------*/
void CANx_Init(CAN_TypeDef* CANx)
{
	  CAN_GPIO_Init();
	  CAN_NVIC_Config();
    CAN_Config(CANx);
}


/*-----------------------------------------------------------
函数名：CAN_Send
输入：CANx，CAN端口
      TxMessage结构体成员：
      TxMessage.StdId = 0x00; 	        //标准标识符
      TxMessage.ExtId = 0xf0f0;	        //扩展标识符
      TxMessage.IDE = CAN_ID_STD; 	//消息类型，使用标准标识符
      TxMessage.RTR = CAN_RTR_DATA; 	//数据类型，数据帧
      TxMessage.DLC = 2; 		//消息帧长度
      TxMessage.Data[0] = 0xAA; 	//数据
      TxMessage.Data[1] = 0x55;

输出：status，返回发送消息的状态，CANTXOK，
        CANTXPENDING，CAN_TxStatus_Failed三者之一。
描叙：其实就是对CAN_Transmit()函数的重写，只是增加了
      状态检测，和发送等待时间
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
