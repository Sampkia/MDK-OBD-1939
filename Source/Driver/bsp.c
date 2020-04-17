#include "bsp.h"
#include "include.h"
#include "debug.h"


/*---------------------------------------
��������RCC_Configuration
��  �룺��
��  ������
��  ����ʱ������
---------------------------------------*/
static void RCC_Configuration(void)
{
	/*----------------ʹ��IO��ʱ��-----------------*/
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB | RCC_APB2Periph_GPIOC |
	                       RCC_APB2Periph_GPIOD, ENABLE);
	
	/*----------------ʹ�ܴ���ʱ��-----------------*/
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2 | RCC_APB1Periph_USART3, ENABLE);
	
	/*----------------ʹ��ADCʱ��-----------------*/
//	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);
	
	/*----------------ʹ��DMA1ͨ��ʱ��-----------------*/
//	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);
}

/*---------------------------------------
��������GPIO_Configuration
��  �룺��
��  ������
��  ����IO������
---------------------------------------*/
static void GPIO_Configuration(void)
{
	  GPIO_InitTypeDef GPIO_InitStructure;

	  /* +++++++++++++++����1--���Լ浼����++++++++++++++ */
	  GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_9;							/* USART1 TX */
	  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	  GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AF_PP;        /* ����������� */
	  GPIO_Init(GPIOA, &GPIO_InitStructure);

	  GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_10;	        	/* USART1 RX */
	  GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_IN_FLOATING;  /* ���ø������� */
	  GPIO_Init(GPIOA, &GPIO_InitStructure);

	  /* +++++++++++++++����2--MG2639+++++++++++++++++++++ */
	  GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_2;							/* USART2 TX */
	  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	  GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AF_PP;        /* ����������� */
	  GPIO_Init(GPIOA, &GPIO_InitStructure);

	  GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_3;							/* USART2 RX */
	  GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_IN_FLOATING;  /* ���ø������� */
	  GPIO_Init(GPIOA, &GPIO_InitStructure);

	  /* +++++++++++++++����3��TX--TTS,RX--GPS��+++++++++ */
	  GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_10;	        	/* USART3 TX(TX--TTS) */
	  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	  GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AF_PP;        /* ����������� */
	  GPIO_Init(GPIOB, &GPIO_InitStructure);

	  GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_11;						/* USART3 RX(RX--GPS) */
	  GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_IN_FLOATING;  /* ���ø������� */
	  GPIO_Init(GPIOB, &GPIO_InitStructure);
	
		/**************************************************************************/
	  /*-----------------GPRS����----------------*/
	  GPIO_InitStructure.GPIO_Pin   = GPRS_PWR_GPIO_PIN;	        	 	/* GPRS���� */
	  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	  GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_Out_PP;       			 	/* ������� */
	  GPIO_Init(GPRS_PWR_GPIO_GROUP, &GPIO_InitStructure);
	
	  /*-----------------GPS����-----------------*/
	  GPIO_InitStructure.GPIO_Pin   = GPS_PWR_GPIO_PIN;	        		 	/* GPS���� */
	  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	  GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_Out_PP;        				/* ������� */
	  GPIO_Init(GPRS_PWR_GPIO_GROUP, &GPIO_InitStructure);
}

/*---------------------------------------
��������NVIC_Configuration
��  �룺��
��  ������
��  �����ж�����
---------------------------------------*/
static void NVIC_Configuration(void)
{
	  NVIC_InitTypeDef NVIC_InitStructure;

	  // Set the Vector Table base location at 0x20000000
	  NVIC_SetVectorTable(NVIC_VectTab_RAM, 0x0);
	  // Set the Vector Table base location at 0x08000000
	  NVIC_SetVectorTable(NVIC_VectTab_FLASH, 0x0);

	  //NVIC_SetVectorTable(NVIC_VectTab_FLASH, 0x10000);

	  /* ------------------�����ж�����------------------- */
	  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);
	  /*------- ���ô���1�ж� -------*/
	  NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
	  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	  NVIC_Init(&NVIC_InitStructure);
	
	  /*------ ���ô���2�ж� ------*/
	  NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;			     
	  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
	  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	  NVIC_Init(&NVIC_InitStructure);
	
   	/*------ ���ô���3�ж� ------*/
	  NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn;			    
	  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;
	  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	  NVIC_Init(&NVIC_InitStructure);
}


/*---------------------------------------
��������BSP_Init
��  �룺��
��  ������
��  ������ʼ������
---------------------------------------*/
void BSP_Init(void)
{
		SysTick_Init();
  	RCC_Configuration();
		GPIO_Configuration();
		NVIC_Configuration();
		__enable_irq();
		
//		IWDGInit(7, 2344);
	
		USART_Config(USART1, 115200);		/* ����1��ʼ��--���Լ浼���� */
		USART_Config(USART2, 9600);		/* ����2��ʼ��--GPRSģ��(M26) */
#if USE_INS
		USART_Config(USART3, 115200);		/*�ߵ�ģ��*/
#else
		USART_Config(USART3, 9600);			/* ����3��ʼ��(GPSģ��)*/
#endif
//		USART_printf(USART1, (uint8_t*)"\r\n ***Project_OBD_1939*** \r\n");
		
		Timer_Initialization();
		CANx_Init(CAN2);
		
	  W25X_Init();
		
		/*USB ��ʼ��*/
		Set_System();
		USB_Interrupt_Init();
		USB_Clock_Init();
		USB_Init();
		
		GPS_PowerOn();
		GPRS_StartUp();
}
