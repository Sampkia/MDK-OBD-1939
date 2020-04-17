/*------------------------文件描述---------------------------
文件名：w25x.c
描  述：基于SPI总线的外部Flash初始化、读、写
时  间：2014年7月        
      By Sindge
-----------------------------------------------------------*/

/*------------------------INCLUDE---------------------------*/
#include "w25x.h"
#include "debug.h"

uint16_t Flash_ID;

/*------------------------FUNCTION--------------------------*/
/*---------------------------------------
函数名：W25X_Init
输入：无
输出：无
描叙：初始化外部FLASH，IO口初始化，SPI初始化
      CS--PA1
---------------------------------------*/
void W25X_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;

  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	
  GPIO_InitStructure.GPIO_Pin = W25X_CS_GPIO_PIN;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(W25X_CS_GPIO_GROUP, &GPIO_InitStructure);
  GPIO_SetBits(W25X_CS_GPIO_GROUP, W25X_CS_GPIO_PIN);

  SPI_Initialization(W25X_SPI);
	SPI_SetSpeed(W25X_SPI, SPI_BaudRatePrescaler_2);
	Flash_ID = W25X_ReadId();
}


/*---------------------------------------
函数名：W25X_ReadSR
输入：无
输出：byte
描叙：读取FLASH状态寄存器
      BIT7  6   5   4   3   2   1   0
      SPR   RV  TB BP2 BP1 BP0 WEL BUSY
      默认，0x00
---------------------------------------*/
u8 W25X_ReadSR(void)
{  
  u8 byte = 0;   
  SPI_FLASH_CS_LOW();
  SPI_ReadWriteByte(W25X_SPI, W25X_ReadStatusReg);
  byte=SPI_ReadWriteByte(W25X_SPI, 0xff);           
  SPI_FLASH_CS_HIGH();
  return byte;
}


/*---------------------------------------
函数名：SPI_FLASH_Write_SR
输入：sr_byte
输出：无
描叙：写FLASH状态寄存器
---------------------------------------*/
void SPI_FLASH_Write_SR(u8 sr_byte)
{
  SPI_FLASH_CS_LOW();
  SPI_ReadWriteByte(W25X_SPI, W25X_WriteStatusReg);
  SPI_ReadWriteByte(W25X_SPI, sr_byte);
  SPI_FLASH_CS_HIGH();
}


/*---------------------------------------
函数名：SPI_FLASH_Write_Enable
输入：无
输出：无
描叙：写使能
---------------------------------------*/
void SPI_FLASH_Write_Enable(void)   
{
  SPI_FLASH_CS_LOW();
  SPI_ReadWriteByte(W25X_SPI, W25X_WriteEnable);
  SPI_FLASH_CS_HIGH();
}


/*---------------------------------------
函数名：SPI_FLASH_Write_Disable
输入：无
输出：无
描叙：写失能
---------------------------------------*/
void SPI_FLASH_Write_Disable(void)
{  
    SPI_FLASH_CS_LOW();  
    SPI_ReadWriteByte(W25X_SPI, W25X_WriteDisable);        
    SPI_FLASH_CS_HIGH();   	      
} 			    

/*---------------------------------------
函数名：W25X_ReadID
输入：无
输出：Temp， FLASH ID号
描叙：读取FLASH　ID号
---------------------------------------*/
uint16_t W25X_ReadId(void)
{
  u16 Temp = 0;	  
  SPI_FLASH_CS_LOW();				    
  SPI_ReadWriteByte(W25X_SPI, 0x90);
  SPI_ReadWriteByte(W25X_SPI, 0x00);
  SPI_ReadWriteByte(W25X_SPI, 0x00);
  SPI_ReadWriteByte(W25X_SPI, 0x00);
  Temp|=SPI_ReadWriteByte(W25X_SPI, 0xFF)<<8;
  Temp|=SPI_ReadWriteByte(W25X_SPI, 0xFF);	 
  SPI_FLASH_CS_HIGH();
  return Temp;
}


/*--------------------------------------------------
函数名：W25X_Read
输入：pBuffer,存放读取数据缓存
      ReadAddr,读取地址
      NumByteToRead，读取的字节数
输出：无
描叙：从FLASH中指定的ReadAddr地址读取指定的
      NumByteToRead字节数存储在pBuffer
---------------------------------------------------*/
void W25X_Read(u8* pBuffer,
               u32 ReadAddr,       //24位有效地址
               u16 NumByteToRead)
{
		uint16_t i;    												    
		SPI_FLASH_CS_LOW();                            //   
		SPI_ReadWriteByte(W25X_SPI, W25X_ReadData);
		SPI_ReadWriteByte(W25X_SPI, ((ReadAddr)>>16));     //高8位
		SPI_ReadWriteByte(W25X_SPI, ((ReadAddr)>>8));         
		SPI_ReadWriteByte(W25X_SPI, ReadAddr);             //低8位地址
		
		for(i=0; i<NumByteToRead; i++)
		{
				pBuffer[i]=SPI_ReadWriteByte(W25X_SPI, 0xFF);
		}
		SPI_FLASH_CS_HIGH();
}


void W25x_Read_sector(uint8_t* buff, 
                      uint32_t sector,
                      uint8_t count)
{
  for(; count>0; count--)
  {
    W25X_Read(buff,
              sector*4096,
              4096);
    sector++;
    buff += 4096;
  }
}

/*---------------------------------------
函数名：W25X_Write_Page
输入：pBuffer,存放读取数据缓存
      WriteAddr,写入地址
      NumByteToWrite，写入数量
输出：无
描叙：向FLASH中指定的地址WriteAddr，
      写入NumByteToWrite个pBuffer中的内容
---------------------------------------*/	 
void W25X_Write_Page(u8* pBuffer,
                     u32 WriteAddr,        //24位有效地址
                     u16 NumByteToWrite)
{
  u16 i;  
	
  SPI_FLASH_Write_Enable();                     //写使能 
  SPI_FLASH_CS_LOW();                          
  SPI_ReadWriteByte(W25X_SPI, W25X_PageProgram);         //写地址指令
  SPI_ReadWriteByte(W25X_SPI, ((WriteAddr&0xFF0000)>>16));     
  SPI_ReadWriteByte(W25X_SPI, ((WriteAddr&0x00FF00)>>8));   
  SPI_ReadWriteByte(W25X_SPI, WriteAddr&0x0000FF);            //低8位
  for(i=0;i<NumByteToWrite;i++)
  {
    SPI_ReadWriteByte(W25X_SPI, pBuffer[i]);             //写一个字节
  }
  SPI_FLASH_CS_HIGH();
  SPI_Flash_Wait_Busy();                        //等待写入结束
} 


/*---------------------------------------
函数名：W25X_Write_NoCheck
输入：pBuffer,存放读取数据缓存
      ReadAddr,读取地址
      NumByteToRead，读取的字节数
输出：无
描叙：向FLASH中指定的地址WriteAddr，
      写入NumByteToWrite个pBuffer中的内容
      不带校验
---------------------------------------*/
void W25X_Write_NoCheck(u8* pBuffer,
                        u32 WriteAddr,
                        u16 NumByteToWrite)
{
  u16 pageremain;
  
	if(WriteAddr%4096 == 0)
	{
			W25X_EraseSector((int)(WriteAddr/4096));
	}
	
  pageremain = 256 - WriteAddr % 256;   //单页剩余字节数	 	    
  if(NumByteToWrite <= pageremain)
  {
    pageremain = NumByteToWrite;        //单页需要写的字节数
  }
	
  while(1)
  {
    W25X_Write_Page(pBuffer,
                    WriteAddr,
                    pageremain);
    if(NumByteToWrite == pageremain)
    {
      break;    //退出循环
    }
    else //NumByteToWrite>pageremain
    {
      pBuffer += pageremain;
      WriteAddr += pageremain;	
      NumByteToWrite -= pageremain;
      if(NumByteToWrite>256)
      {
        pageremain=256;
      }

      else
      {
        pageremain=NumByteToWrite; 	  //
      }
    }
  }
}


/*---------------------------------------
函数名：W25X_Write
输入：pBuffer,存放读取数据缓存
      WriteAddr,写地址
      NumByteToWrite，写的字节数
输出：无
描叙：向FLASH中指定的地址WriteAddr，
      写入NumByteToWrite个pBuffer中的内容
---------------------------------------*/
void W25X_Write(u8* pBuffer,
                u32 WriteAddr,
                u16 NumByteToWrite)
{
	static uint8_t SPI_FLASH_BUF[4096];
	u32 secpos;
  u16 secoff;
  u16 secremain;
  u16 i;

  secpos = WriteAddr / 4096;      //第n个扇区
  secoff = WriteAddr % 4096;      //扇区内的偏移
  secremain = 4096 - secoff;      //扇区剩余空间

  if(NumByteToWrite <= secremain)
  {
    secremain = NumByteToWrite;   //小于4096个数
  }
  
  while(1) 
  {
		if(!(WriteAddr % 4096))
		{
			W25X_EraseSector(secpos);
		}
		
    W25X_Read(SPI_FLASH_BUF,       //读整个扇区
              secpos*4096,
              4096);
    for(i=0;i<secremain;i++)
    {
      if(SPI_FLASH_BUF[secoff+i] != 0xFF)
      {
        break;                          //退出循环
      }
    }
    if(i<secremain)
    {
      W25X_EraseSector(secpos);           //擦除扇区
      for(i=0; i<secremain; i++)	        //复制
      {
        SPI_FLASH_BUF[i+secoff] = pBuffer[i];	  
      }
      W25X_Write_NoCheck(SPI_FLASH_BUF,    //写扇区
                         secpos*4096,
                         4096);
    }
    else 
    {
      W25X_Write_NoCheck(pBuffer,          //写扇区
                         WriteAddr,
                         secremain);
    }
    if(NumByteToWrite == secremain)
    {
      break;    //退出
    }
    else//
    {
      secpos++;
      secoff = 0;

      pBuffer += secremain;
      WriteAddr += secremain;
      NumByteToWrite -= secremain;
      if(NumByteToWrite > 4096)
      {
        secremain = 4096;
      }
      else
      {
        secremain = NumByteToWrite;
      }
    }
  };
}



void W25x_Write_sector(uint8_t* buff, 
                       uint32_t sector,
                       uint8_t count)
{
  for(; count>0; count--)
  {
    W25X_Write(buff,
              sector*4096,
              4096);
    sector++;
    buff += 4096;
  }
}

/*---------------------------------------
函数名：W25X_EraseChip
输入：无
输出：无
描叙：擦除芯片
---------------------------------------*/
void W25X_EraseChip(void)
{                                             
  SPI_FLASH_Write_Enable();
  SPI_Flash_Wait_Busy();
  SPI_FLASH_CS_LOW();
  SPI_ReadWriteByte(W25X_SPI, W25X_ChipErase);
  SPI_FLASH_CS_HIGH();
  SPI_Flash_Wait_Busy();
}


/*---------------------------------------
函数名：W25X_EraseSector
输入：无
输出：无
描叙：擦除指定地址的内容
---------------------------------------*/
void W25X_EraseSector(u32 Dst_Addr)
{   
  Dst_Addr *= 4096;
  SPI_FLASH_Write_Enable();                  //SET WEL 	 
  SPI_Flash_Wait_Busy();   
  SPI_FLASH_CS_LOW();
  SPI_ReadWriteByte(W25X_SPI, W25X_SectorErase);
  SPI_ReadWriteByte(W25X_SPI, (u8)((Dst_Addr)>>16));
  SPI_ReadWriteByte(W25X_SPI, (u8)((Dst_Addr)>>8));   
  SPI_ReadWriteByte(W25X_SPI, (u8)Dst_Addr);
  SPI_FLASH_CS_HIGH();
  SPI_Flash_Wait_Busy();
}


/*---------------------------------------
函数名：SPI_Flash_Wait_Busy
输入：无
输出：无
描叙：等待读完成
---------------------------------------*/
void SPI_Flash_Wait_Busy(void)
{
  while((W25X_ReadSR()&0x01) == 0x01); 
}
