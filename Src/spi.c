//spi.c
#include "spi.h"
//RCC SPI1 APB2, SPI2 APB1
#define 	APB1_SPI2EN		(1U << 14)
#define		APB2_SPI1EN		(1U << 12)
//SPI
#define		CR1_MSTR		(1U << 2)	//Set as master
#define 	CR1_SSM			(1U << 9)	//Software slave mgt
#define		CR1_SSI			(1U << 8)	//If 1 NSS high
#define		CR1_SPE			(1U << 6)	//SPI enable

void spi_init(void)
{
	RCC->APB1ENR 	|=	APB1_SPI2EN;
	RCC->APB2ENR	|=	APB2_SPI1EN;
	SPI1->CR1		=	0;				//Initialize
	SPI1->CR2		=	0;				//Initialize//***
	SPI1->CR1		|=	(CR1_MSTR | CR1_SSM | CR1_SSI);
	// Master, NSS software control, internal NSS high
	SPI2->CR1		=	0;				//Initialize
	SPI2->CR2		=	0;				//Initialize//***
	SPI2->CR1		|=	CR1_SSM;
	// Slave, NSS software control, internal NSS low
	// NSS lowは通信可能の状態で待つという事。
	SPI2->CR1		|=	CR1_SPE;
	//稼働は、Slave、Masterの順
	SPI1->CR1		|=	CR1_SPE;
}


uint8_t SPI1_master_transfer(uint8_t data) //Use at main
{
	while(!(SPI1->SR & SR_TXE)){}

	SPI1->DR	=	data;

	while(!(SPI1->SR & SR_RXNE)){}

	return (uint8_t)SPI1->DR;
}

uint8_t SPI2_slave_receive(void)
{
	while(!(SPI2->SR & SR_RXNE)){}

	return (uint8_t)SPI2->DR;
}
