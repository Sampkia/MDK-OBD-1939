#ifndef USART_H_
#define USART_H_

#include "stm32f10x.h"
#include <stdio.h>
#include "systick.h"
#include <math.h>
#include <stdarg.h>
#include "public/public.h"

#define USART1_CS_HIGH()			GPIO_SetBits(GPIOB, GPIO_Pin_1);
#define USART1_CS_LOW()				GPIO_ResetBits(GPIOB, GPIO_Pin_1);

#define USART2_CS_HIGH()			GPIO_SetBits(GPIOB, GPIO_Pin_6);
#define USART2_CS_LOW()				GPIO_ResetBits(GPIOB, GPIO_Pin_6);

typedef struct
{
   unsigned char *buffP;
   unsigned int  buffSize;
   unsigned int *rIndexP;
   unsigned int *wIndexP;
   unsigned int *lockP; //互斥锁，向缓冲区写数据的时候不能读取数据，读取数据的时候不能写入数据
   unsigned int *timerSPushToBuffP;
   unsigned int *overFlowP; //表示缓冲区是否满
} S_CQueue;


typedef struct
{
  void (*rxBuffRst)(unsigned char num);
  unsigned int (*rxBuffCount)(unsigned char num);
  unsigned int (*rxBuffOverFlow)(unsigned char num);
  unsigned char (*getChar)(unsigned char num);
  unsigned int (*getChars)(unsigned char num, unsigned char *dest, unsigned int len);
  
  void (*putChar)(unsigned char num, unsigned char c);
  void (*putChars)(unsigned char num, unsigned char *c, unsigned int length);
  void (*puts)(unsigned char num, unsigned char *c);
  void (*printf)(USART_TypeDef*, uint8_t *, ...);
} S_IO;

extern S_CQueue const uart1RxQ;
extern S_CQueue const uart1TxQ;
extern S_CQueue const uart2RxQ;
extern S_CQueue const uart2TxQ;
extern S_CQueue const uart3RxQ;
extern S_CQueue const uart3TxQ;
extern S_CQueue const uart4RxQ;
extern S_CQueue const uart4TxQ;
extern S_CQueue const uart5RxQ;
extern S_CQueue const uart5TxQ;

extern S_IO const ioUart;

void Disable_IT(void);
void Enable_IT(void);
void USART_Config(USART_TypeDef* USARTx,int BaudRate);
void QueueInit(S_CQueue const *q);

unsigned int QueueGetDataLen(S_CQueue const *q);
unsigned int QueueGetFreeLen(S_CQueue const *q);
unsigned int QueueGetchar(S_CQueue const *q, char *dst);
unsigned int QueueGetchars(S_CQueue const *q, 
                           char *dst, 
						               unsigned int len);
													 
unsigned int QueuePutchar(S_CQueue const *q, 
                          char c, 
                          unsigned int waitToAcce);
unsigned int QueuePutchars(S_CQueue const *q, 
                           char *val, 
						               unsigned int len, 
						               unsigned int waitToAcce);
													 
unsigned int Queue_GetValidPacketLen(S_CQueue const *q, 
                                     unsigned char val, 
                                     unsigned int maxlen);
																		 



void USART_printf(USART_TypeDef* USARTx, uint8_t *Data,...);
void USART_All_Init(void);

#endif


