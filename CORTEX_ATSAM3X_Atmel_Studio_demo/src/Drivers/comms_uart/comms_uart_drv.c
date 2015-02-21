/*
 * serial_print_drv.c
 *
 * Created: 28/01/2015 09:53:42
 *  Author: John
 */

/* Atmel Library */
#include <asf.h>
#include "comms_uart_drv.h"
#include "serial.h"

void configure_uart(void)
{
	
	// Enable Peripheral Clock
	pmc_enable_periph_clk(ID_UART);
	
	// Pin Configuration
	pio_configure(PIOA, PIO_PERIPH_A, PIO_PA8A_URXD, PIO_PULLUP);
	pio_configure(PIOA, PIO_PERIPH_A, PIO_PA9A_UTXD, PIO_PULLUP);
	
	// uart settings
	sam_uart_opt_t uart_opt;
	uart_opt.ul_mck			= 84000000;
	uart_opt.ul_baudrate	= 115200;
	uart_opt.ul_mode		= UART_MR_PAR_NO;
	uart_opt.ul_chmode		= UART_MR_CHMODE_NORMAL;
	
	uart_init(UART, &uart_opt);
	uart_enable(UART);
	
	uart_disable_interrupt(UART, 0xFFFFFFFF);
	NVIC_EnableIRQ((IRQn_Type) ID_UART);
	uart_enable_interrupt(UART, UART_IER_RXRDY);
}

uint32_t uart_print_string_to_serial(char *c)
{
	uint32_t x;
	
	for (x = 0; c[x] != '\0'; x++) {
		usart_serial_putchar((usart_if)UART, c[x]);
	}
	
	return 0;
}


