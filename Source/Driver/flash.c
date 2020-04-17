#include "flash.h"

Flash_Para flash_para;
/*---------------------------------------
��������ReadFlash
��  �룺addr����ַ
				buf������
				len, ����
��  ������
��  ������Flash
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
��������WriteFlash
��  �룺addr����ַ
				buf������
				len, ����
��  ������
��  ����дFlash
----------------------------------------*/
void WriteFlash(uint32_t addr, uint8_t* buf, uint32_t len)
{
	int i=0;
	
	__disable_irq();   //���ж�
	FLASH_Unlock();
	FLASH_ClearFlag(FLASH_FLAG_BSY | FLASH_FLAG_EOP | FLASH_FLAG_PGERR | FLASH_FLAG_WRPRTERR);
	FLASH_ErasePage(addr);
	for(i=0;i<len;i+=4)
	{
			FLASH_ProgramWord(addr+i,buf[i+3]<<24|buf[i+2]<<16|buf[i+1]<<8|buf[i]);
	}
	FLASH_Lock();
	__enable_irq();   //���ж�
}

/*---------------------------------------
��������Read_Flash_Para
��  �룺��
��  ������
��  ������ȡ�洢�ı���
----------------------------------------*/
void Read_Flash_Para(void)
{
	ReadFlash(FLASH_PARA_ADDRESS, (uint8_t*)&flash_para, sizeof(flash_para));
}

/*---------------------------------------
��������Write_Flash_Para
��  �룺��
��  ������
��  �����洢Flash����
----------------------------------------*/
void Write_Flash_Para(void)
{
	WriteFlash(FLASH_PARA_ADDRESS, (uint8_t*)&flash_para, sizeof(flash_para));
}


