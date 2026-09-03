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
