#include "uart.h"
//RCC usart2 APB1, (pa2 usart_tx af07: gpio.c)
#define 	USART2EN		(1U << 17)
#define		APB1_CLK		16000000U
#define		BAUDRATE		115200U
#define		CR1_UE			(1U << 13)
#define		CR1_TE			(1U << 3)

void uart_init(void)
{
	RCC->APB1ENR	|=	USART2EN;
	USART2->BRR		=	(uint32_t)((APB1_CLK / BAUDRATE) + 0.5);
	USART2->CR1		|=	(CR1_UE | CR1_TE);
}

void uart_tx_write(int ch)
{
	while(!(USART2->SR & SR_TXE)){}
	USART2->DR	= (ch & 0xFF);
}

int __io_putchar(int ch)
{
	uart_tx_write(ch);
	return ch;
}
