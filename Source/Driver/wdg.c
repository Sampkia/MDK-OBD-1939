#include "wdg.h"

/*---------------------------------------------------------
��������IWDGInit
��  �룺prer��Ԥ��Ƶֵ��������ֵ 7����3λ��Ч
				Reload_Value,�ؼ���ֵ��������ֵ 2344����11λ��Ч
��  ������
��  ������ʼ�����Ź�
ʱ�����(���):Tout=((4*2^prer)*rlr)/40 (ms).
Tout = 256*2344/40 = 15001.6 ms = 15��
----------------------------------------------------------*/
void IWDGInit(uint8_t Prescaler,uint16_t Reload_Value) 
{
	//IWDG_WriteAccess_Enable:ʹ�ܶԼĴ���IWDG_PR��IWDG_RLR��д����
	IWDG_WriteAccessCmd(IWDG_WriteAccess_Enable);
	
	//����IWDGԤ��Ƶֵ:����IWDGԤ��ƵֵΪ256
	IWDG_SetPrescaler(Prescaler);  
		
	//����IWDG��װ��ֵ Set counter reload value to 2344 
	IWDG_SetReload(Reload_Value);
	
	// Reload IWDG counter
	IWDG_ReloadCounter(); 		//����IWDG��װ�ؼĴ�����ֵ��װ��IWDG������

	IWDG_Enable();						//enable wdg
}

/*---------------------------------------
��������IWDGFeed
��  �룺��
��  ������
��  ����ι��
---------------------------------------*/
void IWDGFeed(void)
{	
	IWDG_ReloadCounter();	 //����IWDG��װ�ؼĴ�����ֵ��װ��IWDG									   
}
