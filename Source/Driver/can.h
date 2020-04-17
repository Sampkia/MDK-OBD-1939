#ifndef __CAN_H
#define __CAN_H

#include "stm32f10x.h"
#include "stm32f10x_can.h"
#include "SysTick.h"

//CAN引脚复用？
#define USE_REMAP_CAN						0
#define USE_CAN1								0
#define USE_CAN2								1


//CAN工作模式
#define CAN_MODE_NORMAL         1
//#define CAN_MODE_LOOPBACK       1

//传输速度
#define CAN_SPEED_250           1
//#define CAN_SPEED_500         1

extern uint8_t CAN_RX_Flag;
extern CanRxMsg RxMessage;

/*-------------------函数申明-----------------*/
void CANx_Init(CAN_TypeDef* CANx);
uint8_t CAN_Send(CAN_TypeDef* CANx, 
                 CanTxMsg* TxMessage);
void CAN_SetFilter(u32 Identifier);
void CAN_Conduct_Filter(CAN_TypeDef * CANx, FunctionalState state);

#endif
