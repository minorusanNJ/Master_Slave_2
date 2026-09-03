//main.c
#include "system.h"
#include "bsp.h"

// UART動かしてから
#include <stdio.h>

// ==========================================
// デバッグマクロの定義
// ==========================================
#ifdef DEBUG
    // DEBUGが定義されている場合：通常のprintfとして動作
    #define DEBUG_PRINT(...) printf(__VA_ARGS__)
#else
    // DEBUGが定義されていない場合：空（何もしない）
    #define DEBUG_PRINT(...) ((void)0)
#endif
// ==========================================


int main(void)
{
	system_init();
	bsp_init();

	uint16_t tx_count;
	uint8_t tx_data;
	//uint8_t test_data[] = {0x01, 0x02, 0x03};

	for (tx_count = 1; tx_count <= 0xFF; tx_count++)
	{
		tx_data = (uint8_t)(tx_count);
		SPI1_master_transfer(tx_data);

		while(!(SPI2->SR & SR_RXNE)){}

		slave_received =	(uint8_t)SPI2->DR;

		if(slave_received != tx_data)
			{
				test_ok =	0;
				Error_Handler();
			}
			else
			{
				test_ok =	1;
				DEBUG_PRINT("Debug: Received data = 0x%02X\r\n", slave_received);
			}
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
