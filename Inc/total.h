//main.c
#include "system.h"
#include "bsp.h"

int main(void)
{
	system_init();
	bsp_init();

	master_received	=	SPI1_master_transfer(0x55);

	//SPI2の場合
	while(!(SPI2->SR & SR_RXNE)){}

	slave_received =	(uint8_t)SPI2->DR;

	if((master_received == 0xAA) && (slave_received == 0x55))
	{
		test_ok =	1;
	}
	else
	{
		test_ok =	0;
	}

	while(1)
	{

	}
}

void Error_Handler(void)
{
	GPIOA->ODR	&=	0X6000,
	GPIOB->ODR = 0,
	GPIOC->ODR = 0,
	GPIOD->ODR = 0,
	GPIOE->ODR = 0;

	__disable_irq();
	while(1)
	{

	}
}
//data.h
#ifndef __DATA_H__
#define __DATA_H__

#include "stm32f4xx.h"

extern volatile uint8_t master_received;
extern volatile uint8_t slave_received;
extern volatile uint8_t test_ok;

#endif
//data.c
#include "data.h"

//Global variables
volatile uint8_t master_received = 0;
volatile uint8_t slave_received = 0;
volatile uint8_t test_ok = 0;
//bsp.h
#ifndef __BSP_H__
#define __BSP_H__

#include "stm32f4xx.h"
#include "gpio.h"
#include "data.h"
#include "spi.h"

void bsp_init(void);

#endif
//bsp.c
#include "bsp.h"

void bsp_init(void)
{
	gpio_init();
	spi_init();
}
//gpio.h
#ifndef __GPIO_H__
#define __GPIO_H__

#include "stm32f4xx.h"

void gpio_init(void);

#endif
//gpio.c
#include "gpio.h"
//RCC AHB1
#define		GPIOAEN			(1U << 0)
#define 	GPIOBEN			(1U << 1)
//GPIOA, B
#define		MODER567_AF		(0x2A << 10)
#define		MODER131415_AF	(0x2A << 26)
#define		AFRL567_RST		~(0xFFF << 20)
#define		AFRL567_AF5		(0xAAA << 20)
#define 	AFRH_131415_RST	~(0xFFF << 20)
#define		AFRH_131415_AF5	(0xAAA << 20)

void gpio_init(void)
{
	RCC->AHB1ENR	|=	(GPIOAEN | GPIOBEN);
	GPIOA->MODER	|=	MODER567_AF;
	GPIOB->MODER	|=	MODER131415_AF;
	GPIOA->AFR[0]	&=	AFRL567_RST;
	GPIOA->AFR[0]	|=	AFRL567_AF5;
	GPIOB->AFR[1]	&=	AFRH_131415_RST;
	GPIOB->AFR[1]	|=	AFRH_131415_AF5;
}
//system.h
#ifndef __SYSTEM_H__
#define __SYSTEM_H__

#include "stm32f4xx.h"

void system_init(void);

extern void Error_Handler(void);

#endif
//system.c
#include "system.h"
//Time limit
#define		SW_TIMEOUT_LMT	((uint16_t)0X100)

//FLASH 16MHz HSI, latency, Data cache, Instruction cache
#define 	ACR_LATENCY_0	~(0xF << 0)
#define		ACR_DCEN		(1U << 10)
#define		ACR_ICEN		(1U << 9)
//RCC
#define		CR_HSION		(1U <<0)
#define		CFGR_PPRE2_RST	~(7U << 13)
#define 	CFGR_PPRE1_RST	~(7U << 10)
#define 	CFGR_HPRE_REST	~(0xF << 4)
#define 	CFGR_SW_HSI		~(3U << 0)
#define 	CFGR_SWS_CHK	(3U << 2)

void system_init(void)
{
	uint32_t	counter = 0;

	FLASH->ACR	&=	ACR_LATENCY_0;
	FLASH->ACR	|=	(ACR_DCEN & ACR_ICEN);
	RCC->CR		|=	CR_HSION;
	RCC->CFGR	&=	(	CFGR_PPRE2_RST
					&	CFGR_PPRE1_RST
					&	CFGR_HPRE_REST
					&	CFGR_SW_HSI		);
	while((RCC->CFGR & CFGR_SWS_CHK)!= 0)
	{
		counter++;
		if(counter >=	SW_TIMEOUT_LMT)
		{
			Error_Handler();
		}
	}

}
//spi.h
#ifndef __SPI_H__
#define __SPI_H__

#include "stm32f4xx.h"

#define		SR_TXE			(1U << 1)	//Transmit buffer empty
#define		SR_RXNE			(1U << 0)	//Received buffer not empty

void spi_init(void);
uint8_t SPI1_master_transfer(uint8_t data);

#endif
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
	SPI1->CR2		=	0;				//Initialize
	SPI1->CR1		|=	(CR1_MSTR | CR1_SSM | CR1_SSI);
	// Master, NSS software control, internal NSS high
	SPI2->CR1		=	0;				//Initialize
	SPI2->CR2		=	0;				//Initialize
	SPI2->CR1		=	CR1_SSM;
	// Slave, NSS software control, internal NSS low
	// NSS lowは通信可能の状態で待つという事。
	SPI2->DR		=	0xAA;	//(10101010)
	// Masterからの送信後に直ぐ送り返すデータ
	SPI2->CR1		|=	CR1_SPE;
	//稼働は、Slave、Masterの順
	SPI1->CR1		|=	CR1_SPE;
}


uint8_t SPI1_master_transfer(uint8_t data) //Use at main
{
	while(!(SPI1->SR & SR_TXE)){}

	SPI1->DR	=	data;

	//

	while(!(SPI1->SR & SR_RXNE)){}

	return (uint8_t)SPI1->DR;
}
