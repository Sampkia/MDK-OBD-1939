/*-----------------------------------------------------------------------*/
/* Low level disk I/O module skeleton for FatFs     (C)ChaN, 2014        */
/*-----------------------------------------------------------------------*/
/* If a working storage control module is available, it should be        */
/* attached to the FatFs via a glue function rather than modifying it.   */
/* This is an example of glue functions to attach various exsisting      */
/* storage control modules to the FatFs module with a defined API.       */
/*-----------------------------------------------------------------------*/

#include "diskio.h"		/* FatFs lower layer API */
#include "Peripheral/w25x.h"		/* Example: Header file of existing MMC/SDC contorl module */
#include "debug.h"
//#include "Driver/rtc.h"

//#include "usb_conf.h"
//#include "usbh_msc_core.h"

/* Definitions of physical drive number for each drive */
#define ATA		0	/* Example: Map ATA harddisk to physical drive 0 */
#define MMC		1	/* Example: Map MMC/SD card to physical drive 1 */
#define USB		2	/* Example: Map USB MSD to physical drive 2 */




static volatile DSTATUS Stat = STA_NOINIT;	/* Disk status */
extern uint8_t SDA_STA;
#define FLASH_SECTOR_SIZE       4096     	//和USB中的Block_size对应起来
#define FLASH_BLOCK_NUM         8       	//每个BLOCK有8个SECTOR
uint16_t Flash_sector_count = 0;

//extern USB_OTG_CORE_HANDLE          USB_OTG_Core;
//extern USBH_HOST                     USB_Host;


/*-----------------------------------------------------------------------*/
/* Get Drive Status                                                      */
/*-----------------------------------------------------------------------*/

DSTATUS disk_status (
	BYTE pdrv		/* Physical drive nmuber to identify the drive */
)
{
		DSTATUS stat=STA_NOINIT;
		
		stat = 0;
		return stat;
}



/*-----------------------------------------------------------------------*/
/* Inidialize a Drive                                                    */
/*-----------------------------------------------------------------------*/

DSTATUS disk_initialize (
	BYTE pdrv				/* Physical drive nmuber to identify the drive */
)
{	
		//Flash
	  DSTATUS stat;
		uint16_t Flash_ID;
  

		W25X_Init();
		Flash_ID = W25X_ReadId();
		switch(Flash_ID)
		{
			case W25Q64:
				Flash_sector_count = 2048;
				break;
			case W25Q32:
				Flash_sector_count = 1024/2;
				break;
			case W25Q16:
				Flash_sector_count = 512/2;
				break;
		}
  stat = 0;
  return stat;        //成功初始化
	
	/*
	//SD 卡
	DSTATUS stat=STA_NOINIT;
			stat = SD_Init();
			return stat;
	*/
//	return STA_NOINIT;
}



/*-----------------------------------------------------------------------*/
/* Read Sector(s)                                                        */
/*-----------------------------------------------------------------------*/

DRESULT disk_read (
	BYTE pdrv,		/* Physical drive nmuber to identify the drive */
	BYTE *buff,		/* Data buffer to store read data */
	DWORD sector,	/* Sector address in LBA */
	UINT count		/* Number of sectors to read */
)
{
//	DRESULT res=RES_PARERR;

		if(count <= 0)
		{
#ifdef  DEBUG
			printf("count <= 0, 文件系统读错误\n");
#endif
			return RES_ERROR;
		}
		
		else  //count有效
		{
			for( ; count>0; count--)
			{
				switch(Flash_ID)
					{
						 case W25Q32:
							 W25X_Read((u8 *)buff,                     //缓冲区
												(sector+W25Q32_OFFSET/4096)*FLASH_SECTOR_SIZE,       //读取地址    
			//									sector*FLASH_SECTOR_SIZE,
												FLASH_SECTOR_SIZE);
							 break;
						 
						 case W25Q16:
							W25X_Read((u8 *)buff,                     //缓冲区
												(sector+W25Q16_OFFSET/4096)*FLASH_SECTOR_SIZE,       //读取地址
//											sector*FLASH_SECTOR_SIZE,
												FLASH_SECTOR_SIZE);  
							 break;
					}
				           //读取字节数
				sector++;
				buff += FLASH_SECTOR_SIZE;
			}
			return RES_OK;
		}
	/*
			//SD卡
			if(count <= 0)
			{
#ifdef  DEBUG
				printf("count <= 0, 文件系统读错误\n");
#endif
				return RES_ERROR;
			}
			
			else  //count有效
			{
				for( ; count>0; count--)
				{
					SD_Read_Disk((uint8_t*)buff, sector, count);
				}
				return RES_OK;
			}
	*/
//	return RES_PARERR;
}



/*-----------------------------------------------------------------------*/
/* Write Sector(s)                                                       */
/*-----------------------------------------------------------------------*/

#if _USE_WRITE
DRESULT disk_write (
	BYTE pdrv,			/* Physical drive nmuber to identify the drive */
	const BYTE *buff,	/* Data to be written */
	DWORD sector,		/* Sector address in LBA */
	UINT count			/* Number of sectors to write */
)
{
	//	DRESULT res= RES_PARERR;
	
		if(count <= 0)
		{
#ifdef  DEBUG
				printf("count <= 0, 文件系统写错误\n");
#endif
				return RES_ERROR;
		}
		
		else  //count有效
		{
				for( ; count>0; count--)
				{
					switch(Flash_ID)
					{
						 case W25Q32:
							 W25X_Write_NoCheck((u8 *)buff,                    //缓冲区
													(sector+W25Q32_OFFSET/4096)*FLASH_SECTOR_SIZE,      //读取地址   
//										 		sector*FLASH_SECTOR_SIZE,
													FLASH_SECTOR_SIZE);            //读取字节数
							 break;
						 
						 case W25Q16:
							 W25X_Write_NoCheck((u8 *)buff,                    //缓冲区
													(sector+W25Q16_OFFSET/4096)*FLASH_SECTOR_SIZE,      //读取地址 
//										 		 sector*FLASH_SECTOR_SIZE,
													 FLASH_SECTOR_SIZE);            //读取字节数
							 break;
					}
					
					sector++;
					buff += FLASH_SECTOR_SIZE;
				}
				return RES_OK;
		}
	/*
		//SD卡
			if(count <= 0)
			{
#ifdef  DEBUG
				printf("count <= 0, 文件系统写错误\n");
#endif
				return RES_ERROR;
			}
			
			else  //count有效
			{
				for( ; count>0; count--)
				{
					SD_Write_Disk((uint8_t *)buff, sector, count);
				}
				return RES_OK;
			}
	*/
//	return RES_PARERR;
}
#endif


/*-----------------------------------------------------------------------*/
/* Miscellaneous Functions                                               */
/*-----------------------------------------------------------------------*/

#if _USE_IOCTL
DRESULT disk_ioctl (
	BYTE pdrv,		/* Physical drive nmuber (0..) */
	BYTE cmd,		/* Control code */
	void *buff		/* Buffer to send/receive control data */
)
{
		DRESULT stat;
  
		switch(cmd)
		{
				case CTRL_SYNC:
					stat = RES_OK;
					break;
				
				case GET_SECTOR_COUNT:
					*(DWORD*)buff = Flash_sector_count;
					stat = RES_OK;
					break;
				
				case GET_SECTOR_SIZE:
					*(DWORD*)buff = FLASH_SECTOR_SIZE;
					stat = RES_OK;
					break;
				
				case GET_BLOCK_SIZE:
//					*(DWORD*)buff = FLASH_BLOCK_NUM*FLASH_SECTOR_SIZE;
				*(DWORD*)buff = FLASH_SECTOR_SIZE;
					stat = RES_OK;
					break;
					
				default:
					stat = RES_PARERR;
					break;
		}
		return stat;
	
	/*
	//SD卡
	DRESULT res= RES_PARERR;
			switch(cmd)
			{
				case CTRL_SYNC:
					SET_CS_LOW();
					if(SD_WaitReady() == 0)
					{
						res = RES_OK;
					}
					else
					{
						res = RES_ERROR;
					}
					SET_CS_HIGH();
					break;
				
				case GET_SECTOR_COUNT:
					*(DWORD*)buff = SD_GetSectorCount();
					res = RES_OK;
					break;
				
				case GET_SECTOR_SIZE:
					*(DWORD*)buff = 512;
					res = RES_OK;
					break;
				
				case GET_BLOCK_SIZE:
					*(DWORD*)buff = 512;
					res = RES_OK;
					break;
				
				default:
					res = RES_PARERR;
					break;
			}
			return res;
	*/
//	return RES_PARERR;
}
#endif

DWORD get_fattime (void)
{
	#if USE_RTC
    return   ((DWORD)((calendar.w_year-1980) << 25))| ((DWORD)calendar.w_month << 21)| ((DWORD)calendar.w_date << 16)| ((DWORD)calendar.hour << 11)| ((DWORD)calendar.min << 5)| (DWORD)(calendar.sec >> 1);
	#else
		return 0;
	#endif
}

