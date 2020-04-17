#ifndef __SPI_H
#define __SPI_H

/*---------------INCLUDE--------------*/
#include "stm32f10x.h" 
/*---------------DEFINE---------------*/
/*---------------STRUCT---------------*/
/*----------EXTERN VARIABLE-----------*/
/*--------------FUNCTION--------------*/
void SPI_Initialization(SPI_TypeDef* SPIn);
void SPI_SetSpeed(SPI_TypeDef* SPIn, uint8_t SPI_BaudRatePrescaler);
uint8_t SPI_ReadWriteByte(SPI_TypeDef *SPIn, uint8_t TxData);

#endif
