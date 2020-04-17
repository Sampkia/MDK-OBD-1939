#include "usart.h"

#define Rx_buff_SIZE				600
#define Tx_buff_SIZE				128
#define Rx_buff_SIZE4				2046

static unsigned char _Rxbuff1[Rx_buff_SIZE];	/* ���ն������� */
static unsigned char _Txbuff1[Tx_buff_SIZE];	/* ���Ͷ������� */
static unsigned int _rxrIndex1=0;				/* ��ȡ���ݵ�λ�����������ն��У� */
static unsigned int _txrIndex1=0;				/* ��ȡ���ݵ�λ�����������Ͷ��У� */
static unsigned int _rxwIndex1=0;				/* д�����ݵ�λ�����������ն��У� */
static unsigned int _txwIndex1=0;				/* д�����ݵ�λ�����������Ͷ��У� */
static unsigned int _rxlock1=0;					/* �����������ն��У� */
static unsigned int _txlock1=0;					/* �����������Ͷ��У� */
static unsigned int _rxtimerSPushToBuff1=0;		/* ���������ʱ���־�����ն��У� */
static unsigned int _txtimerSPushToBuff1=0;		/* ���������ʱ���־�����Ͷ��У� */
static unsigned int _rxoverFlow1=0;			/* �����־�����ն��У� */
static unsigned int _txoverFlow1=0;           /* �����־�����Ͷ��У� */


static unsigned char _Rxbuff2[Rx_buff_SIZE];
static unsigned char _Txbuff2[Tx_buff_SIZE];
static unsigned int _rxrIndex2=0;
static unsigned int _txrIndex2=0;
static unsigned int _rxwIndex2=0;
static unsigned int _txwIndex2=0;
static unsigned int _rxlock2=0;
static unsigned int _txlock2=0;
static unsigned int _rxtimerSPushToBuff2=0;
static unsigned int _txtimerSPushToBuff2=0;
static unsigned int _rxoverFlow2=0;
static unsigned int _txoverFlow2=0;


static unsigned char _Rxbuff3[Rx_buff_SIZE];
static unsigned char _Txbuff3[Tx_buff_SIZE];
static unsigned int _rxrIndex3=0;
static unsigned int _txrIndex3=0;
static unsigned int _rxwIndex3=0;
static unsigned int _txwIndex3=0;
static unsigned int _rxlock3=0;
static unsigned int _txlock3=0;
static unsigned int _rxtimerSPushToBuff3=0;
static unsigned int _txtimerSPushToBuff3=0;
static unsigned int _rxoverFlow3=0;
static unsigned int _txoverFlow3=0;


/* ++++++����1++++++ */
S_CQueue const uart1RxQ =
{
  (unsigned char *)_Rxbuff1,
  Rx_buff_SIZE,
  &_rxrIndex1,
  &_rxwIndex1,
  &_rxlock1,
  &_rxtimerSPushToBuff1,
  &_rxoverFlow1
};

S_CQueue const uart1TxQ =
{
	(unsigned char *)_Txbuff1,
	Tx_buff_SIZE,
	&_txrIndex1,
	&_txwIndex1,
	&_txlock1,
	&_txtimerSPushToBuff1,
	&_txoverFlow1
};

/* ++++++����2++++++ */
S_CQueue const uart2RxQ =
{
	(unsigned char *)_Rxbuff2,
	Rx_buff_SIZE,
	&_rxrIndex2,
	&_rxwIndex2,
	&_rxlock2,
	&_rxtimerSPushToBuff2,
	&_rxoverFlow2
};

S_CQueue const uart2TxQ =
{
	(unsigned char *)_Txbuff2,
	Tx_buff_SIZE,
	&_txrIndex2,
	&_txwIndex2,
	&_txlock2,
	&_txtimerSPushToBuff2,
	&_txoverFlow2
};

/* ++++++����3++++++ */
S_CQueue const uart3RxQ =
{
	(unsigned char *)_Rxbuff3,
	Rx_buff_SIZE,
	&_rxrIndex3,
	&_rxwIndex3,
	&_rxlock3,
	&_rxtimerSPushToBuff3,
	&_rxoverFlow3
};

S_CQueue const uart3TxQ =
{
	(unsigned char *)_Txbuff3,
	Tx_buff_SIZE,
	&_txrIndex3,	
	&_txwIndex3,
	&_txlock3,
	&_txtimerSPushToBuff3,
	&_txoverFlow3
};

void Disable_IT(void)
{
	USART_ITConfig(USART1, USART_IT_RXNE,DISABLE);
	USART_ITConfig(USART1, USART_IT_TXE, DISABLE);

	USART_ITConfig(USART2, USART_IT_RXNE,DISABLE);
	USART_ITConfig(USART2, USART_IT_TXE, DISABLE);

	USART_ITConfig(USART3, USART_IT_RXNE,DISABLE);
	USART_ITConfig(USART3, USART_IT_TXE, DISABLE);
}


void Enable_IT(void)
{
	USART_ITConfig(USART1, USART_IT_RXNE,ENABLE);
	USART_ITConfig(USART1, USART_IT_TXE, ENABLE);

	USART_ITConfig(USART2, USART_IT_RXNE,ENABLE);
	USART_ITConfig(USART2, USART_IT_TXE, ENABLE);

	USART_ITConfig(USART3, USART_IT_RXNE,ENABLE);
	USART_ITConfig(USART3, USART_IT_TXE, ENABLE);
}

void QueueInit(S_CQueue const *q)
{
	Disable_IT();
	*(q->rIndexP) = 0;
	*(q->wIndexP) = 0;
	*(q->lockP) = 0;
	*(q->overFlowP) = 0;
	Enable_IT();
}

/*********************************************************
 *�������ƣ� QueueGetDataLen
 *�������ܣ� ��ȡ���������ݵĳ���
**********************************************************/
unsigned int QueueGetDataLen(S_CQueue const *q)
{
	unsigned int ui;
	unsigned int rIndex, wIndex;

	ui = 0;
	rIndex = *(q->rIndexP);
	wIndex = *(q->wIndexP);

	if(wIndex >= rIndex)
	{
		ui = wIndex - rIndex;
	}
	else
	{
		ui = rIndex - wIndex;
		ui = q->buffSize - ui;
	}

	if( (ui>=q->buffSize) || (rIndex>=q->buffSize) || (wIndex>=q->buffSize) )
	{
		QueueInit(q);
		ui = 0;
	}

	return (ui);
}

/*********************************************************
 *�������ƣ� QueueGetFreeLen
 *�������ܣ� ��ȡ���п��ÿռ��С
**********************************************************/
unsigned int QueueGetFreeLen(S_CQueue const *q)
{
	return (q->buffSize - QueueGetDataLen(q) - 1);
}

/*********************************************************
 *�������ƣ� QueueGetchar
 *�������ܣ� �Ӷ�����ȡ��һ���ֽ�����
**********************************************************/
unsigned int QueueGetchar(S_CQueue const *q, char *dst)
{
	if(QueueGetDataLen(q) == 0)
	{
		return 0;
	}
	else
	{
		*dst = *(q->buffP + *(q->rIndexP));
		if(++(*(q->rIndexP)) >= q->buffSize)
		{
			*(q->rIndexP) = 0;
		}

		return 1;
	}
}

/*********************************************************
 *�������ƣ� QueueGetchars
 *�������ܣ� �Ӷ�����ȡ������ֽڵ�����
**********************************************************/
unsigned int QueueGetchars(S_CQueue const *q, char *dst, unsigned int len)
{
    unsigned int validLen;
    unsigned int ui;

    validLen = min(len, QueueGetDataLen(q));
    for(ui = 0; ui < validLen; ui++)
    {
            QueueGetchar(q, dst+ui);
    }

    return validLen;
}

/*********************************************************
 *�������ƣ� QueuePutchar
 *�������ܣ� ������д���һ���ֽڵ�����
**********************************************************/
unsigned int QueuePutchar(S_CQueue const *q, char c, unsigned int waitToAcce)
{
	unsigned int ui;

	*(q->timerSPushToBuffP) = TimerSecond;

	if(QueueGetFreeLen(q) == 0)
	{
		if(waitToAcce == 1)
		{
			while(1)
			{
				ui = QueueGetFreeLen(q);
				if(ui)
				{
					break;
				}
			}
		}
		else
		{
			*(q->overFlowP) = 1;
			return 0;
		}
	}
	*(q->buffP + *(q->wIndexP)) = c;
	if(++(*(q->wIndexP)) >= q->buffSize)
	{
		*(q->wIndexP)=0;
	}

	return 1;
}

unsigned int QueuePutchars(S_CQueue const *q, char *val, unsigned int len, unsigned int waitToAcce)
{
	unsigned int ui;

	*(q->timerSPushToBuffP) = TimerSecond;
	if(QueueGetFreeLen(q) < len)
	{
		if(waitToAcce == 0)
		{
			*(q->overFlowP) = 1;
			return 0;
		}
	}

	for(ui = 0; ui < len; ui++)
	{
		QueuePutchar(q, *(val+ui), waitToAcce);
	}
	return len;
}

/*------------------------------------------
*��������	: Queue_GetValidPacketLen
 *��������	: ���޶�����󳤶��ڲ��ҵ�ǰ�������������ݰ��ĳ��ȡ�ע�����ݰ�
 *			  ��֡ͷ��֡β������ͬһ����ͬ���ַ�����֡ͷ��֡β֮������ݲ�
 *			  ��û��֡ͷ֡β�ؼ��ַ�
 *�������	: q-------������ݰ��Ķ���
 *			  val-----֡ͷ��֡β�Ĺؼ���
 *			  maxlen--�޶�����󳤶�
 *����ֵ	: һ���������������ݰ�����
--------------------------------------------*/
unsigned int Queue_GetValidPacketLen(S_CQueue const *q,
																		 unsigned char val, 
																		 unsigned int maxlen)
{
    unsigned int QueueDataLen=0, count=0, validpacketlen=0;
    unsigned int i, querylen;

    QueueDataLen = QueueGetDataLen(q);

    querylen = min(QueueDataLen, maxlen);

    for(i=0; i<querylen; i++)
    {
        if( (*(q->rIndexP) + i) < (q->buffSize) )
        {
            if(*(q->buffP + *(q->rIndexP) + i) == val)
            {
                count++;
                if(count%2 == 0)
                {
                    validpacketlen = i+1;
                }
            }
        }
        else
        {
            if(*(q->buffP + *(q->rIndexP) + i - (q->buffSize)) == val)
            {
                count++;
                if(count%2 == 0)
                {
                    validpacketlen = i+1;
                }
            }
        }
    }

    if((querylen==QueueDataLen) && (count%2 != 0))
    {
        QueueInit(q);
        validpacketlen = 0;
    }

    return validpacketlen;
}

/*------------------------------------------
��������QueueDelchars
��  �룺q������
				len,����
��  �����ɹ�����1
��  ��
--------------------------------------------*/
unsigned int QueueDelchars(S_CQueue const *q, unsigned int len)
{
    if( len > QueueGetDataLen(q) )
    {
        return 0;
    }
    else
    {
        if( *(q->rIndexP) >= q->buffSize - len )
        {
            *(q->rIndexP) = len - (q->buffSize - *(q->rIndexP));
        }
        else
        {
            *(q->rIndexP) = *(q->rIndexP) + len;
        }
        return 1;
    }
}



/*****************************************************************************
 *��������	: Queue_CopyData
 *��������	: ��ָ���Ķ����п�����ָ�����ȵ�����
 *�������	: q-------������ݰ��Ķ���
 *			  dest----��ſ����������ݵĵ�ַ
 *			  len-----�������ݵĳ���
 *����ֵ	: �ɹ����������ݳ���
******************************************************************************/
unsigned int Queue_CopyData(S_CQueue const *q, unsigned char *dest, unsigned int len)
{
	unsigned int i, rIndex=0;

	if( len > QueueGetDataLen(q) )
	{
		return 0;
	}
	else
	{
		rIndex = *(q->rIndexP);

		for(i=0; i<len; i++)
		{
			*dest++ = *(q->buffP + rIndex++);
			if(rIndex >= q->buffSize)
			{
				rIndex = rIndex - q->buffSize;
			}
		}
	}

	return i;
}

/* ++++++++++++++++++++++ ���ڵ�Ӧ�ú��� ++++++++++++++++++++++++ */
static void uartRxBuffRst(unsigned char num)
{
	switch(num)
	{
	case 1:
		QueueInit(&uart1RxQ);
		break;

	case 2:
		QueueInit(&uart2RxQ);
		break;

	case 3:
		QueueInit(&uart3RxQ);
		break;

	case 4:
#if USE_UART4
		QueueInit(&uart4RxQ);
#endif
		break;

	case 5:
#if USE_UART5
		QueueInit(&uart5RxQ);
#endif
		break;
	}
}

static unsigned int uartRxBuffCount(unsigned char num)
{
	switch(num)
	{
	case 1:
		return (QueueGetDataLen(&uart1RxQ));

	case 2:
		return (QueueGetDataLen(&uart2RxQ));

	case 3:
		return (QueueGetDataLen(&uart3RxQ));
	}

	return 0;
}

static unsigned int uartRxBuffOverFlow(unsigned char num)
{
	switch(num)
	{
	case 1:
		return (*(uart1RxQ.overFlowP));

	case 2:
		return (*(uart2RxQ.overFlowP));

	case 3:
		return (*(uart3RxQ.overFlowP));
	}

	return 0;
}

static unsigned char uartGetChar(unsigned char num)
{
	unsigned char uc=0;

	switch(num)
	{
	case 1:
		QueueGetchar(&uart1RxQ, (char *)(&uc));
		break;

	case 2:
		QueueGetchar(&uart2RxQ, (char *)(&uc));
		break;

	case 3:
		QueueGetchar(&uart3RxQ, (char *)(&uc));
		break;
	}

	return (uc);
}

static unsigned int uartGetChars(unsigned char num, unsigned char *dest, unsigned int len)
{
	switch(num)
	{
	case 1:
		return (QueueGetchars(&uart1RxQ, (char *)dest, len));

	case 2:
		return (QueueGetchars(&uart2RxQ, (char *)dest, len));

	case 3:
		return (QueueGetchars(&uart3RxQ, (char *)dest, len));
	}

	return 0;
}

static unsigned int uartTrigToSend(unsigned char num)
{
	unsigned int tB=0;
	unsigned char uc;

	switch(num)
	{
	case 1:
		if(QueueGetDataLen(&uart1TxQ))
		{
			QueueGetchar(&uart1TxQ, (char *)(&uc));
			USART_SendData(USART1, uc);
			while(USART_GetFlagStatus(USART1, USART_FLAG_TC)==RESET);
			tB = 1;
		}
		break;

	case 2:
		if(QueueGetDataLen(&uart2TxQ))
		{
			QueueGetchar(&uart2TxQ, (char *)(&uc));
			USART_SendData(USART2, uc);
			while(USART_GetFlagStatus(USART2, USART_FLAG_TC)==RESET);
			tB = 1;
		}
		break;

	case 3:
		if(QueueGetDataLen(&uart3TxQ))
		{
			QueueGetchar(&uart3TxQ, (char *)(&uc));
			USART_SendData(USART3, uc);
			while(USART_GetFlagStatus(USART3, USART_FLAG_TC)==RESET);
			tB = 1;
		}
		break;
	}

	return (tB);
}

static void uartPutChar(unsigned char num, unsigned char c)
{
	switch(num)
	{
	case 1:
		QueuePutchar(&uart1TxQ, c, 1);
		uartTrigToSend(1);
		break;

	case 2:
		QueuePutchar(&uart2TxQ, c, 1);
		uartTrigToSend(2);
		break;

	case 3:
		QueuePutchar(&uart3TxQ, c, 1);
		uartTrigToSend(3);
		break;
	}
}

static void uartPutChars(unsigned char num, unsigned char *c, unsigned int length)
{
	switch(num)
	{
	case 1:
		while(length)
		{
			uartPutChar(1, *(c++));
			length--;
		}
		break;

	case 2:
		while(length)
		{
			uartPutChar(2, *(c++));
			length--;
		}
		break;

	case 3:
		while(length)
		{
			uartPutChar(3, *(c++));
			length--;
		}
		break;
	}
}

static void uartPuts(unsigned char num, unsigned char *c)
{
  switch(num)
  {
  case 1:
    while(*c)
    {
      uartPutChar(1, *(c++));
    }
    break;

  case 2:
    while(*c)
    {
      uartPutChar(2, *(c++));
    }
    break;

  case 3:
    while(*c)
    {
      uartPutChar(3, *(c++));
    }
    break;
  }
}

void USART_printf(USART_TypeDef* USARTx, uint8_t *Data,...)
{
  const char *s;
  int d, p;
  char buf[16];
  va_list ap;
  va_start(ap, Data);

  while(*Data!=0)				/* �ж��Ƿ񵽴��ַ��������� */
  {
    if(*Data==0x5c)			/* '\' */
    {
      switch(*++Data)
      {
      case 'r':								/* �س��� */
        USART_SendData(USARTx, 0x0d);
        Data++;
        break;

      case 'n':							    /* ���з� */
        USART_SendData(USARTx, 0x0a);
        Data++;
        break;

      default:
        Data++;
        break;
      }
    }
    else if(*Data=='%')
    {
      switch(*++Data)
      {
      case 's':							/* �ַ��� */
        s = va_arg(ap, const char *);
        for ( ; *s; s++)
        {
          USART_SendData(USARTx,*s);
          while(USART_GetFlagStatus(USARTx, USART_FLAG_TC)==RESET);
        }
        Data++;
        break;

      case 'd':							/* ʮ���� */
        d = va_arg(ap, int);
        itoa(d, buf, 10);
        for (s = buf; *s; s++)
        {
          USART_SendData(USARTx,*s);
          while(USART_GetFlagStatus(USARTx, USART_FLAG_TC)==RESET);
        }
        Data++;
        break;

      case 'p':							/* ʮ������ */
        p = va_arg(ap, unsigned int);
        hex_to_string(p, buf);
        for (s = buf; *s; s++)
        {
          USART_SendData(USARTx,*s);
          while(USART_GetFlagStatus(USARTx, USART_FLAG_TC)==RESET);
        }
        Data++;
        break;

      default:
        Data++;
        break;
      }
    }
    else
    {
            USART_SendData(USARTx, *Data++);
    }
    while(USART_GetFlagStatus(USARTx, USART_FLAG_TC)==RESET);
  }
}

/*---------------------------------------
��������USART_GPIO_Init
���룺��
�������
���𣺴���IO�˿ڳ�ʼ����ʹ��ģ��ʱ��
      USART2��PA2��������
              PA3��������
              PB9����485�շ����ƶ˿�
           
      USART1��PA9��������
              PA10��������
----------------------------------------*/
void USART_GPIO_Config(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB,
                         ENABLE);          //GPIOAʱ��ʹ��
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2 | RCC_APB1Periph_USART3, ENABLE);        //USARTʱ��
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);        //USARTʱ��
  
  //USART1 USART2 - TX
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2 | GPIO_Pin_9;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(GPIOA, &GPIO_InitStructure);
  
  //USART3-TX
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(GPIOB, &GPIO_InitStructure);
  
  //USART1 USART2 - RX
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3 | GPIO_Pin_10;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;     //����˿ڣ��ٶȲ��Ǳ����
  GPIO_Init(GPIOA, &GPIO_InitStructure);
  
  //USART3-RX
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(GPIOB, &GPIO_InitStructure);
  
  //GPS��Դ����
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(GPIOB, &GPIO_InitStructure);
  
  GPIO_SetBits(GPIOB, GPIO_Pin_9);
	
	//USART1-PB1   USART2-PB6 485��������
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1 | GPIO_Pin_6;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(GPIOB, &GPIO_InitStructure);
	GPIO_ResetBits(GPIOB, GPIO_Pin_1 | GPIO_Pin_6);
}

/*---------------------------------------
��������USART_INT_Config
���룺��
�������
���𣺴����ж�����
----------------------------------------*/
void USART_INT_Config(void)
{
  NVIC_InitTypeDef NVIC_InitStructure;
  
  NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 5;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
  
  NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 4;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
  
  NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
}

/*********************************************************
*Function Name     :USART_InitConfig
*Copyright         :���ݴ�Ӯ
*CPU               :STM32
*Create Date       :2012/12/12
*Author            :Lin Lifeng
*Abstract          :��ʼ������
*Input             :USARTx  :���ں�
                    BaudRate:������
*Output            :void
*History           :2012/12/12 �¹�����
*********************************************************/
void USART_Config(USART_TypeDef* USARTx,int BaudRate)
{
    USART_InitTypeDef USART_InitStructure;

    USART_InitStructure.USART_BaudRate   = BaudRate;			    		/* ���������� */
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;		/* ����λ8λ  */
    USART_InitStructure.USART_StopBits = USART_StopBits_1;				/* ֹͣλ1λ  */
    USART_InitStructure.USART_Parity   = USART_Parity_No;					/* ��У��λ   */
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;   /* ��Ӳ������ */
    USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	  								/* �շ�ģʽ  */

    /* Configure USARTx */
    USART_Init(USARTx, &USART_InitStructure);						  				/* ���ô��ڲ������� */

    /* Enable USARTx Receive and Transmit interrupts */
    USART_ITConfig(USARTx, USART_IT_RXNE,ENABLE);            			/* ʹ�ܽ����ж� */
    USART_ITConfig(USARTx, USART_IT_TXE, ENABLE);					  			/* ʹ�ܷ��ͻ�����ж� */
	USART_ClearITPendingBit(USARTx, USART_IT_RXNE);
    /* Enable the USARTx */
    USART_Cmd(USARTx, ENABLE);
}


S_IO const ioUart =
{
  uartRxBuffRst,
  uartRxBuffCount,
  uartRxBuffOverFlow,
  uartGetChar,
  uartGetChars,
  uartPutChar,
  uartPutChars,
  uartPuts,
  USART_printf
};


#if 1
int fputc(int ch, FILE* f)
{
  USART_SendData(USART1, (uint8_t)ch);
  while (USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET);
  return ch;
}
#endif



