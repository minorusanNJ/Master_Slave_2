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

