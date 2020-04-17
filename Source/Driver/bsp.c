#include "bsp.h"
#include "include.h"
#include "debug.h"


/*---------------------------------------
函数名：RCC_Configuration
输  入：无
输  出：无
描  述：时钟配置
---------------------------------------*/
static void RCC_Configuration(void)
{
	/*----------------使能IO口时钟-----------------*/
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB | RCC_APB2Periph_GPIOC |
	                       RCC_APB2Periph_GPIOD, ENABLE);
	
	/*----------------使能串口时钟-----------------*/
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2 | RCC_APB1Periph_USART3, ENABLE);
	
	/*----------------使能ADC时钟-----------------*/
//	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);
	
	/*----------------使能DMA1通道时钟-----------------*/
//	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);
}

/*---------------------------------------
函数名：GPIO_Configuration
输  入：无
输  出：无
描  述：IO口配置
---------------------------------------*/
static void GPIO_Configuration(void)
{
	  GPIO_InitTypeDef GPIO_InitStructure;

	  /* +++++++++++++++串口1--调试兼导航屏++++++++++++++ */
	  GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_9;							/* USART1 TX */
	  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	  GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AF_PP;        /* 复用推挽输出 */
	  GPIO_Init(GPIOA, &GPIO_InitStructure);

	  GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_10;	        	/* USART1 RX */
	  GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_IN_FLOATING;  /* 复用浮空输入 */
	  GPIO_Init(GPIOA, &GPIO_InitStructure);

	  /* +++++++++++++++串口2--MG2639+++++++++++++++++++++ */
	  GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_2;							/* USART2 TX */
	  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	  GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AF_PP;        /* 复用推挽输出 */
	  GPIO_Init(GPIOA, &GPIO_InitStructure);

	  GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_3;							/* USART2 RX */
	  GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_IN_FLOATING;  /* 复用浮空输入 */
	  GPIO_Init(GPIOA, &GPIO_InitStructure);

	  /* +++++++++++++++串口3（TX--TTS,RX--GPS）+++++++++ */
	  GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_10;	        	/* USART3 TX(TX--TTS) */
	  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	  GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AF_PP;        /* 复用推挽输出 */
	  GPIO_Init(GPIOB, &GPIO_InitStructure);

	  GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_11;						/* USART3 RX(RX--GPS) */
	  GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_IN_FLOATING;  /* 复用浮空输入 */
	  GPIO_Init(GPIOB, &GPIO_InitStructure);
	
		/**************************************************************************/
	  /*-----------------GPRS供电----------------*/
	  GPIO_InitStructure.GPIO_Pin   = GPRS_PWR_GPIO_PIN;	        	 	/* GPRS供电 */
	  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	  GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_Out_PP;       			 	/* 推挽输出 */
	  GPIO_Init(GPRS_PWR_GPIO_GROUP, &GPIO_InitStructure);
	
	  /*-----------------GPS供电-----------------*/
	  GPIO_InitStructure.GPIO_Pin   = GPS_PWR_GPIO_PIN;	        		 	/* GPS供电 */
	  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	  GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_Out_PP;        				/* 推挽输出 */
	  GPIO_Init(GPRS_PWR_GPIO_GROUP, &GPIO_InitStructure);
}

/*---------------------------------------
函数名：NVIC_Configuration
输  入：无
输  出：无
描  述：中断配置
---------------------------------------*/
static void NVIC_Configuration(void)
{
	  NVIC_InitTypeDef NVIC_InitStructure;

	  // Set the Vector Table base location at 0x20000000
	  NVIC_SetVectorTable(NVIC_VectTab_RAM, 0x0);
	  // Set the Vector Table base location at 0x08000000
	  NVIC_SetVectorTable(NVIC_VectTab_FLASH, 0x0);

	  //NVIC_SetVectorTable(NVIC_VectTab_FLASH, 0x10000);

	  /* ------------------串口中断配置------------------- */
	  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);
	  /*------- 设置串口1中断 -------*/
	  NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
	  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	  NVIC_Init(&NVIC_InitStructure);
	
	  /*------ 设置串口2中断 ------*/
	  NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;			     
	  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
	  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	  NVIC_Init(&NVIC_InitStructure);
	
   	/*------ 设置串口3中断 ------*/
	  NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn;			    
	  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;
	  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	  NVIC_Init(&NVIC_InitStructure);
}


/*---------------------------------------
函数名：BSP_Init
输  入：无
输  出：无
描  述：初始化设置
---------------------------------------*/
void BSP_Init(void)
{
		SysTick_Init();
  	RCC_Configuration();
		GPIO_Configuration();
		NVIC_Configuration();
		__enable_irq();
		
//		IWDGInit(7, 2344);
	
		USART_Config(USART1, 115200);		/* 串口1初始化--调试兼导航屏 */
		USART_Config(USART2, 9600);		/* 串口2初始化--GPRS模块(M26) */
#if USE_INS
		USART_Config(USART3, 115200);		/*惯导模块*/
#else
		USART_Config(USART3, 9600);			/* 串口3初始化(GPS模块)*/
#endif
//		USART_printf(USART1, (uint8_t*)"\r\n ***Project_OBD_1939*** \r\n");
		
		Timer_Initialization();
		CANx_Init(CAN2);
		
	  W25X_Init();
		
		/*USB 初始化*/
		Set_System();
		USB_Interrupt_Init();
		USB_Clock_Init();
		USB_Init();
		
		GPS_PowerOn();
		GPRS_StartUp();
}
