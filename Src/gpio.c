//gpio.c
#include "gpio.h"
//RCC AHB1
#define		GPIOAEN			(1U << 0)
#define 	GPIOBEN			(1U << 1)
//GPIOA, B
#define		MODER567_AF		(0x2A << 10)
#define		MODER131415_AF	(0x2A << 26)
#define		AFRL567_RST		~(0xFFF << 20)
#define		AFRL567_AF5		(0x555 << 20)
#define 	AFRH_131415_RST	~(0xFFF << 20)
#define		AFRH_131415_AF5	(0x555 << 20)

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
