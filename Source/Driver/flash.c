#include "flash.h"

Flash_Para flash_para;
/*---------------------------------------
函数名：ReadFlash
输  入：addr，地址
				buf，内容
				len, 长度
输  出：无
描  述：读Flash
----------------------------------------*/
void ReadFlash(uint32_t addr, uint8_t* buf, uint32_t len)
{
    int i=0;
    for(i=0;i<len;i++)
    {
        buf[i]=*((uint8_t *)(addr + i));
    }
}

/*---------------------------------------
函数名：WriteFlash
输  入：addr，地址
				buf，内容
				len, 长度
输  出：无
描  述：写Flash
----------------------------------------*/
void WriteFlash(uint32_t addr, uint8_t* buf, uint32_t len)
{
	int i=0;
	
	__disable_irq();   //关中断
	FLASH_Unlock();
	FLASH_ClearFlag(FLASH_FLAG_BSY | FLASH_FLAG_EOP | FLASH_FLAG_PGERR | FLASH_FLAG_WRPRTERR);
	FLASH_ErasePage(addr);
	for(i=0;i<len;i+=4)
	{
			FLASH_ProgramWord(addr+i,buf[i+3]<<24|buf[i+2]<<16|buf[i+1]<<8|buf[i]);
	}
	FLASH_Lock();
	__enable_irq();   //开中断
}

/*---------------------------------------
函数名：Read_Flash_Para
输  入：无
输  出：无
描  述：读取存储的变量
----------------------------------------*/
void Read_Flash_Para(void)
{
	ReadFlash(FLASH_PARA_ADDRESS, (uint8_t*)&flash_para, sizeof(flash_para));
}

/*---------------------------------------
函数名：Write_Flash_Para
输  入：无
输  出：无
描  述：存储Flash变量
----------------------------------------*/
void Write_Flash_Para(void)
{
	WriteFlash(FLASH_PARA_ADDRESS, (uint8_t*)&flash_para, sizeof(flash_para));
}


