#ifndef __UART_H__
#define __UART_H__

#include "stm32f4xx.h"

#define		SR_TXE		(1U << 7)

void uart_init(void);

#endif
