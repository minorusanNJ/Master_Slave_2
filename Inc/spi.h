//spi.h
#ifndef __SPI_H__
#define __SPI_H__

#include "stm32f4xx.h"

#define		SR_TXE			(1U << 1)	//Transmit buffer empty
#define		SR_RXNE			(1U << 0)	//Received buffer not empty

void spi_init(void);
uint8_t SPI1_master_transfer(uint8_t data);

#endif
