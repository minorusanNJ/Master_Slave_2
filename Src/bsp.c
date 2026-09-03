//bsp.c
#include "bsp.h"

void bsp_init(void)
{
	gpio_init();
	spi_init();

}
