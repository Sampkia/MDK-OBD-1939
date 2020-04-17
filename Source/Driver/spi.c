/*-------------------------------------------------
文件名：SPI.C
描述：SPI初始化，SD卡、Flash、字库等都是使用SPI控制
  -----By Sindge
---------------------------------------------------*/

#include <stdio.h>
#include "spi.h"
#include "debug.h"

/*---------------------------------------
函数名：SPI_GPIO_Configuration
输  入：无
输  出：无
描  述：配置SPI IO口
---------------------------------------*/
static void SPI_GPIO_Configuration(void)
{
	/*SPI1 IO口配置*/
	GPIO_InitTypeDef GPIO_InitStructure;
#if USE_SPI1
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE );
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(GPIOA, &GPIO_InitStructure);
  GPIO_SetBits(GPIOA, GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7);
#endif
	
	/*SPI2 IO口配置*/
#if USE_SPI2	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(GPIOB, &GPIO_InitStructure);
  GPIO_SetBits(GPIOB, GPIO_Pin_13|GPIO_Pin_14|GPIO_Pin_15);     //PB13/14/15上拉
#endif
	
#if USE_SPI3
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3 | GPIO_Pin_4 | GPIO_Pin_5;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(GPIOB, &GPIO_InitStructure);
  GPIO_SetBits(GPIOB, GPIO_Pin_3|GPIO_Pin_4|GPIO_Pin_5);     //PB3/4/5上拉
#endif
}

/*---------------------------------------
函数名：SPI_RCC_Configuration
输  入：无
输  出：无
描  述：SPI时钟使能
---------------------------------------*/
static void SPI_RCC_Configuration(void)
{
	/*-----------------SPI1时钟使能-----------------*/
#if USE_SPI1
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1,  ENABLE );
#endif	
	
/*-----------------SPI2时钟使能-----------------*/
#if USE_SPI2
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI2,  ENABLE );
#endif
	
/*-----------------SPI3时钟使能-----------------*/
#if USE_SPI3
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI3,  ENABLE );
#endif
}

/*---------------------------------------
函数名：SPI1_Config
输入：SPIn，SPI接口，eg.SPI1, SPI2
			SPI_BaudRatePrescaler，SPI传输速度
			取值：SPI_BaudRatePrescaler_2
						SPI_BaudRatePrescaler_4
						...
输出：无
描叙：初始化SPI1
---------------------------------------*/
void SPI_Configuration(SPI_TypeDef* SPIn, uint8_t SPI_BaudRatePrescaler)
{
  SPI_InitTypeDef  SPI_InitStructure;
  
  SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;            //设置SPI模式
  SPI_InitStructure.SPI_Mode = SPI_Mode_Master;		        //设置SPI工作模式
  SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;				//设置SPI的数据大小
  SPI_InitStructure.SPI_CPOL = SPI_CPOL_High;		        	//串行同步时钟的空闲状态为高电平
  SPI_InitStructure.SPI_CPHA = SPI_CPHA_2Edge;	          //串行同步时钟的第二个跳变沿（上升或下降）数据被采样
  SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;		        		//NSS信号
  SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler;							//定义波特率预分频的值:波特率预分频值为256
  SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;	    //指定数据传输从MSB位开始
  SPI_InitStructure.SPI_CRCPolynomial = 7;	              //CRC值计算的多项式
  SPI_Init(SPIn, &SPI_InitStructure);
	
  SPI_Cmd(SPIn, ENABLE);        													//使能SPI外设
  SPI_ReadWriteByte(SPIn, 0xFF);													//启动传输
}

/*---------------------------------------
函数名：SPI_Initialization
输  入：无
输  出：无
描  述：SPI初始化
---------------------------------------*/
void SPI_Initialization(SPI_TypeDef* SPIn)
{
	SPI_GPIO_Configuration();
	SPI_RCC_Configuration();
	SPI_Configuration(SPIn, SPI_BaudRatePrescaler_256);
}

/*---------------------------------------
函数名：SPI_SetSpeed
输入：SPI_BaudRatePrescaler
输出：无
描叙：设置SPI2的传输波特率
---------------------------------------*/
void SPI_SetSpeed(SPI_TypeDef* SPIn, uint8_t SPI_BaudRatePrescaler)
{
  assert_param(IS_SPI_BAUDRATE_PRESCALER(SPI_BaudRatePrescaler));
#if USE_SPI1 
  SPI1->CR1 &= 0XFFC7;
  SPI1->CR1 |= SPI_BaudRatePrescaler;	//设置SPI1速度
#endif

#if USE_SPI2
  SPI2->CR1&=0XFFC7;
  SPI2->CR1|=SPI_BaudRatePrescaler;	//设置SPI2速度 
#endif
	
#if USE_SPI3
  SPI3->CR1&=0XFFC7;
  SPI3->CR1|=SPI_BaudRatePrescaler;	//设置SPI3速度 
#endif
  SPI_Cmd(SPIn,ENABLE); 
}


/*---------------------------------------
函数名：SPI_ReadWriteByte
输入：TxData,要写入的字节
输出：读取到的字节
描叙：通过发送命令，接收返回的数据
---------------------------------------*/
uint8_t SPI_ReadWriteByte(SPI_TypeDef *SPIx, uint8_t TxData)
{
  u8 time_count=0;

  while (SPI_I2S_GetFlagStatus(SPIx, SPI_I2S_FLAG_TXE) == RESET)
  {
    time_count++;
    if(time_count >= 200)       //超时
    {
      return 0;
    }
  }
  
  SPI_I2S_SendData(SPIx, TxData);
  time_count = 0;
  
  while (SPI_I2S_GetFlagStatus(SPIx, SPI_I2S_FLAG_RXNE) == RESET)
  {
    time_count++;
    if(time_count >= 200)       //超时
    {
      return 0;
    }
  }
  return SPI_I2S_ReceiveData(SPIx);
}

