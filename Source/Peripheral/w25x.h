#ifndef __W25X_H
#define __W25X_H

/*---------------INCLUDE--------------*/
#include "stm32f10x.h"
#include "Driver/spi.h"

/*---------------DEFINE---------------*/
#define W25Q16 	0XEF14
#define W25Q32 	0XEF15
#define W25Q64 	0XEF16

#define W25Q16_OFFSET				0x100000
#define W25Q32_OFFSET				0x200000

#define W25Q16_BLOCK_NUM        32
#define W25Q64_BLOCK_NUM        0x300

//#define FLASH_BLOCK_SIZE        0x200


#define SPI_FLASH_CS_HIGH()     GPIO_SetBits(W25X_CS_GPIO_GROUP, W25X_CS_GPIO_PIN)
#define SPI_FLASH_CS_LOW()      GPIO_ResetBits(W25X_CS_GPIO_GROUP, W25X_CS_GPIO_PIN)

//FLASH指令
#define W25X_WriteEnable		0x06 
#define W25X_WriteDisable		0x04 
#define W25X_ReadStatusReg		0x05 
#define W25X_WriteStatusReg		0x01 
#define W25X_ReadData			0x03 
#define W25X_FastReadData		0x0B 
#define W25X_FastReadDual		0x3B 
#define W25X_PageProgram		0x02 
#define W25X_BlockErase			0xD8 
#define W25X_SectorErase		0x20 
#define W25X_ChipErase			0xC7 
#define W25X_PowerDown			0xB9 
#define W25X_ReleasePowerDown	        0xAB 
#define W25X_DeviceID			0xAB 
#define W25X_ManufactDeviceID	        0x90 
#define W25X_JedecDeviceID		0x9F
/*---------------STRUCT---------------*/
/*----------EXTERN VARIABLE-----------*/
extern uint16_t Flash_ID;
/*--------------FUNCTION--------------*/
void W25X_Init(void);
u8 SPI_Flash_ReadSR(void);
void SPI_FLASH_Write_SR(u8 sr_byte);
void SPI_FLASH_Write_Enable(void);
void SPI_FLASH_Write_Disable(void);
uint16_t W25X_ReadId(void);

void W25X_Read(u8* pBuffer,
               u32 ReadAddr,       //24位有效地址
               u16 NumByteToRead);

void W25x_Read_sector(uint8_t* buff, 
                      uint32_t sector,
                      uint8_t count);

void SPI_Flash_Write_Page(u8* pBuffer,
                          u32 WriteAddr,
                          u16 NumByteToWrite);
													
void W25X_Write_NoCheck(u8* pBuffer,
                        u32 WriteAddr,
                        u16 NumByteToWrite);

void W25X_Write(u8* pBuffer,
                u32 WriteAddr,
                u16 NumByteToWrite);

void W25x_Write_sector(uint8_t* buff, 
                       uint32_t sector,
                       uint8_t count);
void W25X_EraseChip(void);
void W25X_EraseSector(u32 Dst_Addr);
void SPI_Flash_Wait_Busy(void);

#endif
