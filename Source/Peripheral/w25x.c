/*------------------------�ļ�����---------------------------
�ļ�����w25x.c
��  ��������SPI���ߵ��ⲿFlash��ʼ��������д
ʱ  �䣺2014��7��        
      By Sindge
-----------------------------------------------------------*/

/*------------------------INCLUDE---------------------------*/
#include "w25x.h"
#include "debug.h"

uint16_t Flash_ID;

/*------------------------FUNCTION--------------------------*/
/*---------------------------------------
��������W25X_Init
���룺��
�������
���𣺳�ʼ���ⲿFLASH��IO�ڳ�ʼ����SPI��ʼ��
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
��������W25X_ReadSR
���룺��
�����byte
���𣺶�ȡFLASH״̬�Ĵ���
      BIT7  6   5   4   3   2   1   0
      SPR   RV  TB BP2 BP1 BP0 WEL BUSY
      Ĭ�ϣ�0x00
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
��������SPI_FLASH_Write_SR
���룺sr_byte
�������
����дFLASH״̬�Ĵ���
---------------------------------------*/
void SPI_FLASH_Write_SR(u8 sr_byte)
{
  SPI_FLASH_CS_LOW();
  SPI_ReadWriteByte(W25X_SPI, W25X_WriteStatusReg);
  SPI_ReadWriteByte(W25X_SPI, sr_byte);
  SPI_FLASH_CS_HIGH();
}


/*---------------------------------------
��������SPI_FLASH_Write_Enable
���룺��
�������
����дʹ��
---------------------------------------*/
void SPI_FLASH_Write_Enable(void)   
{
  SPI_FLASH_CS_LOW();
  SPI_ReadWriteByte(W25X_SPI, W25X_WriteEnable);
  SPI_FLASH_CS_HIGH();
}


/*---------------------------------------
��������SPI_FLASH_Write_Disable
���룺��
�������
����дʧ��
---------------------------------------*/
void SPI_FLASH_Write_Disable(void)
{  
    SPI_FLASH_CS_LOW();  
    SPI_ReadWriteByte(W25X_SPI, W25X_WriteDisable);        
    SPI_FLASH_CS_HIGH();   	      
} 			    

/*---------------------------------------
��������W25X_ReadID
���룺��
�����Temp�� FLASH ID��
���𣺶�ȡFLASH��ID��
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
��������W25X_Read
���룺pBuffer,��Ŷ�ȡ���ݻ���
      ReadAddr,��ȡ��ַ
      NumByteToRead����ȡ���ֽ���
�������
���𣺴�FLASH��ָ����ReadAddr��ַ��ȡָ����
      NumByteToRead�ֽ����洢��pBuffer
---------------------------------------------------*/
void W25X_Read(u8* pBuffer,
               u32 ReadAddr,       //24λ��Ч��ַ
               u16 NumByteToRead)
{
		uint16_t i;    												    
		SPI_FLASH_CS_LOW();                            //   
		SPI_ReadWriteByte(W25X_SPI, W25X_ReadData);
		SPI_ReadWriteByte(W25X_SPI, ((ReadAddr)>>16));     //��8λ
		SPI_ReadWriteByte(W25X_SPI, ((ReadAddr)>>8));         
		SPI_ReadWriteByte(W25X_SPI, ReadAddr);             //��8λ��ַ
		
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
��������W25X_Write_Page
���룺pBuffer,��Ŷ�ȡ���ݻ���
      WriteAddr,д���ַ
      NumByteToWrite��д������
�������
������FLASH��ָ���ĵ�ַWriteAddr��
      д��NumByteToWrite��pBuffer�е�����
---------------------------------------*/	 
void W25X_Write_Page(u8* pBuffer,
                     u32 WriteAddr,        //24λ��Ч��ַ
                     u16 NumByteToWrite)
{
  u16 i;  
	
  SPI_FLASH_Write_Enable();                     //дʹ�� 
  SPI_FLASH_CS_LOW();                          
  SPI_ReadWriteByte(W25X_SPI, W25X_PageProgram);         //д��ַָ��
  SPI_ReadWriteByte(W25X_SPI, ((WriteAddr&0xFF0000)>>16));     
  SPI_ReadWriteByte(W25X_SPI, ((WriteAddr&0x00FF00)>>8));   
  SPI_ReadWriteByte(W25X_SPI, WriteAddr&0x0000FF);            //��8λ
  for(i=0;i<NumByteToWrite;i++)
  {
    SPI_ReadWriteByte(W25X_SPI, pBuffer[i]);             //дһ���ֽ�
  }
  SPI_FLASH_CS_HIGH();
  SPI_Flash_Wait_Busy();                        //�ȴ�д�����
} 


/*---------------------------------------
��������W25X_Write_NoCheck
���룺pBuffer,��Ŷ�ȡ���ݻ���
      ReadAddr,��ȡ��ַ
      NumByteToRead����ȡ���ֽ���
�������
������FLASH��ָ���ĵ�ַWriteAddr��
      д��NumByteToWrite��pBuffer�е�����
      ����У��
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
	
  pageremain = 256 - WriteAddr % 256;   //��ҳʣ���ֽ���	 	    
  if(NumByteToWrite <= pageremain)
  {
    pageremain = NumByteToWrite;        //��ҳ��Ҫд���ֽ���
  }
	
  while(1)
  {
    W25X_Write_Page(pBuffer,
                    WriteAddr,
                    pageremain);
    if(NumByteToWrite == pageremain)
    {
      break;    //�˳�ѭ��
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
��������W25X_Write
���룺pBuffer,��Ŷ�ȡ���ݻ���
      WriteAddr,д��ַ
      NumByteToWrite��д���ֽ���
�������
������FLASH��ָ���ĵ�ַWriteAddr��
      д��NumByteToWrite��pBuffer�е�����
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

  secpos = WriteAddr / 4096;      //��n������
  secoff = WriteAddr % 4096;      //�����ڵ�ƫ��
  secremain = 4096 - secoff;      //����ʣ��ռ�

  if(NumByteToWrite <= secremain)
  {
    secremain = NumByteToWrite;   //С��4096����
  }
  
  while(1) 
  {
		if(!(WriteAddr % 4096))
		{
			W25X_EraseSector(secpos);
		}
		
    W25X_Read(SPI_FLASH_BUF,       //����������
              secpos*4096,
              4096);
    for(i=0;i<secremain;i++)
    {
      if(SPI_FLASH_BUF[secoff+i] != 0xFF)
      {
        break;                          //�˳�ѭ��
      }
    }
    if(i<secremain)
    {
      W25X_EraseSector(secpos);           //��������
      for(i=0; i<secremain; i++)	        //����
      {
        SPI_FLASH_BUF[i+secoff] = pBuffer[i];	  
      }
      W25X_Write_NoCheck(SPI_FLASH_BUF,    //д����
                         secpos*4096,
                         4096);
    }
    else 
    {
      W25X_Write_NoCheck(pBuffer,          //д����
                         WriteAddr,
                         secremain);
    }
    if(NumByteToWrite == secremain)
    {
      break;    //�˳�
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
��������W25X_EraseChip
���룺��
�������
���𣺲���оƬ
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
��������W25X_EraseSector
���룺��
�������
���𣺲���ָ����ַ������
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
��������SPI_Flash_Wait_Busy
���룺��
�������
���𣺵ȴ������
---------------------------------------*/
void SPI_Flash_Wait_Busy(void)
{
  while((W25X_ReadSR()&0x01) == 0x01); 
}
