#ifndef __FLASH_H
#define __FLASH_H

#include "stm32f10x.h"

#define FLASH_PARA_ADDRESS    0x0801F800		//Flash��ַ 126ҳ


/*-----------------------------------
BaudRate:
					0��1200bps
					1��2400bps
					2��4800bps
					3��9600bps��Ĭ�ϣ�
					4��19200bps
					
data_byte:
					0��7λ
					1��8λ��Ĭ�ϣ�
					
stop_byte:
					0��1λ��Ĭ�ϣ�
					1��2λ
					
check_type:
					0����У��
					1��żУ��
					2����У�飨Ĭ�ϣ�
------------------------------------------*/
typedef struct
{
	uint8_t Init_flag;
	
	//��������
	uint8_t BaudRate;
	uint8_t Data_Bit;
	uint8_t Stop_Bit;
	uint8_t Check_Type;
	
	//ģ������
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

