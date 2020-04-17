#ifndef __FLASH_H
#define __FLASH_H

#include "stm32f10x.h"

#define FLASH_PARA_ADDRESS    0x0801F800		//Flash地址 126页


/*-----------------------------------
BaudRate:
					0：1200bps
					1：2400bps
					2：4800bps
					3：9600bps（默认）
					4：19200bps
					
data_byte:
					0：7位
					1：8位（默认）
					
stop_byte:
					0：1位（默认）
					1：2位
					
check_type:
					0：奇校验
					1：偶校验
					2：无校验（默认）
------------------------------------------*/
typedef struct
{
	uint8_t Init_flag;
	
	//串口设置
	uint8_t BaudRate;
	uint8_t Data_Bit;
	uint8_t Stop_Bit;
	uint8_t Check_Type;
	
	//模块配置
	uint8_t link_mode;
	uint16_t shake_value;
	long int longitude;
	long int latitude;
	unsigned long altitude;
}Flash_Para;

extern Flash_Para flash_para;

void ReadFlash(uint32_t addr, uint8_t* buf, uint32_t len);
void WriteFlash(uint32_t addr, uint8_t* buf, uint32_t len);
void Write_Flash_Para(void);
void Flash_Para_Init(void);

#endif

