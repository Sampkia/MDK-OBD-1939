/*-------------------------------------------------
�ļ�����SPI.C
������SPI��ʼ����SD����Flash���ֿ�ȶ���ʹ��SPI����
  -----By Sindge
---------------------------------------------------*/

#include <stdio.h>
#include "spi.h"
#include "debug.h"

/*---------------------------------------
��������SPI_GPIO_Configuration
��  �룺��
��  ������
��  ��������SPI IO��
---------------------------------------*/
static void SPI_GPIO_Configuration(void)
{
	/*SPI1 IO������*/
	GPIO_InitTypeDef GPIO_InitStructure;
#if USE_SPI1
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE );
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(GPIOA, &GPIO_InitStructure);
  GPIO_SetBits(GPIOA, GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7);
#endif
	
	/*SPI2 IO������*/
#if USE_SPI2	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(GPIOB, &GPIO_InitStructure);
  GPIO_SetBits(GPIOB, GPIO_Pin_13|GPIO_Pin_14|GPIO_Pin_15);     //PB13/14/15����
#endif
	
#if USE_SPI3
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3 | GPIO_Pin_4 | GPIO_Pin_5;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(GPIOB, &GPIO_InitStructure);
  GPIO_SetBits(GPIOB, GPIO_Pin_3|GPIO_Pin_4|GPIO_Pin_5);     //PB3/4/5����
#endif
}

/*---------------------------------------
��������SPI_RCC_Configuration
��  �룺��
��  ������
��  ����SPIʱ��ʹ��
---------------------------------------*/
static void SPI_RCC_Configuration(void)
{
	/*-----------------SPI1ʱ��ʹ��-----------------*/
#if USE_SPI1
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1,  ENABLE );
#endif	
	
/*-----------------SPI2ʱ��ʹ��-----------------*/
#if USE_SPI2
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI2,  ENABLE );
#endif
	
/*-----------------SPI3ʱ��ʹ��-----------------*/
#if USE_SPI3
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI3,  ENABLE );
#endif
}

/*---------------------------------------
��������SPI1_Config
���룺SPIn��SPI�ӿڣ�eg.SPI1, SPI2
			SPI_BaudRatePrescaler��SPI�����ٶ�
			ȡֵ��SPI_BaudRatePrescaler_2
						SPI_BaudRatePrescaler_4
						...
�������
���𣺳�ʼ��SPI1
---------------------------------------*/
void SPI_Configuration(SPI_TypeDef* SPIn, uint8_t SPI_BaudRatePrescaler)
{
  SPI_InitTypeDef  SPI_InitStructure;
  
  SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;            //����SPIģʽ
  SPI_InitStructure.SPI_Mode = SPI_Mode_Master;		        //����SPI����ģʽ
  SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;				//����SPI�����ݴ�С
  SPI_InitStructure.SPI_CPOL = SPI_CPOL_High;		        	//����ͬ��ʱ�ӵĿ���״̬Ϊ�ߵ�ƽ
  SPI_InitStructure.SPI_CPHA = SPI_CPHA_2Edge;	          //����ͬ��ʱ�ӵĵڶ��������أ��������½������ݱ�����
  SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;		        		//NSS�ź�
  SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler;							//���岨����Ԥ��Ƶ��ֵ:������Ԥ��ƵֵΪ256
  SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;	    //ָ�����ݴ����MSBλ��ʼ
  SPI_InitStructure.SPI_CRCPolynomial = 7;	              //CRCֵ����Ķ���ʽ
  SPI_Init(SPIn, &SPI_InitStructure);
	
  SPI_Cmd(SPIn, ENABLE);        													//ʹ��SPI����
  SPI_ReadWriteByte(SPIn, 0xFF);													//��������
}

/*---------------------------------------
��������SPI_Initialization
��  �룺��
��  ������
��  ����SPI��ʼ��
---------------------------------------*/
void SPI_Initialization(SPI_TypeDef* SPIn)
{
	SPI_GPIO_Configuration();
	SPI_RCC_Configuration();
	SPI_Configuration(SPIn, SPI_BaudRatePrescaler_256);
}

/*---------------------------------------
��������SPI_SetSpeed
���룺SPI_BaudRatePrescaler
�������
��������SPI2�Ĵ��䲨����
---------------------------------------*/
void SPI_SetSpeed(SPI_TypeDef* SPIn, uint8_t SPI_BaudRatePrescaler)
{
  assert_param(IS_SPI_BAUDRATE_PRESCALER(SPI_BaudRatePrescaler));
#if USE_SPI1 
  SPI1->CR1 &= 0XFFC7;
  SPI1->CR1 |= SPI_BaudRatePrescaler;	//����SPI1�ٶ�
#endif

#if USE_SPI2
  SPI2->CR1&=0XFFC7;
  SPI2->CR1|=SPI_BaudRatePrescaler;	//����SPI2�ٶ� 
#endif
	
#if USE_SPI3
  SPI3->CR1&=0XFFC7;
  SPI3->CR1|=SPI_BaudRatePrescaler;	//����SPI3�ٶ� 
#endif
  SPI_Cmd(SPIn,ENABLE); 
}


/*---------------------------------------
��������SPI_ReadWriteByte
���룺TxData,Ҫд����ֽ�
�������ȡ�����ֽ�
����ͨ������������շ��ص�����
---------------------------------------*/
uint8_t SPI_ReadWriteByte(SPI_TypeDef *SPIx, uint8_t TxData)
{
  u8 time_count=0;

  while (SPI_I2S_GetFlagStatus(SPIx, SPI_I2S_FLAG_TXE) == RESET)
  {
    time_count++;
    if(time_count >= 200)       //��ʱ
    {
      return 0;
    }
  }
  
  SPI_I2S_SendData(SPIx, TxData);
  time_count = 0;
  
  while (SPI_I2S_GetFlagStatus(SPIx, SPI_I2S_FLAG_RXNE) == RESET)
  {
    time_count++;
    if(time_count >= 200)       //��ʱ
    {
      return 0;
    }
  }
  return SPI_I2S_ReceiveData(SPIx);
}

