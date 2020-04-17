#ifndef __DEBUG_H
#define	__DEBUG_H

/*---------------INCLUDE--------------*/
#include "stm32f10x.h" 
/*---------------DEFINE---------------*/
#define USE_CDMA			0

#define USE_SPI1			1
#define USE_SPI2			0

#define USE_INS				1				//惯导模块
	
/*-------------------------IO口使用---------------------*/
/*--------------W25X--------------*/
#define W25X_SPI									SPI1
#define W25X_CS_GPIO_GROUP				GPIOC
#define W25X_CS_GPIO_PIN					GPIO_Pin_4

/*-----------CAN总线引脚----------*/
#define CAN_RX_GPIO_GROUP					GPIOB
#define CAN_RX_GPIO_PIN     			GPIO_Pin_12

#define CAN_TX_GPIO_GROUP					GPIOB
#define CAN_TX_GPIO_PIN     			GPIO_Pin_13

/*------------供电管脚-------------*/
//GPRS-POWER
#define GPRS_PWR_GPIO_GROUP				GPIOB
#define GPRS_PWR_GPIO_PIN					GPIO_Pin_6
//GPS-POWER
#define GPS_PWR_GPIO_GROUP				GPIOB
#define GPS_PWR_GPIO_PIN					GPIO_Pin_7


/*---------------STRUCT---------------*/
/*----------EXTERN VARIABLE-----------*/
/*--------------FUNCTION--------------*/

#endif
