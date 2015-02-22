/*
 * serial_uart_drv.c
 *
 * Created: 28/01/2015 09:53:42
 *  Author: John
 */

/* Atmel Library */
#include <asf.h>
#include "comms_drv.h"
#include "comms_uart_drv.h"

void configure_uart(void)
{	
	pio_configure(PIOA, PIO_PERIPH_A, PIO_PA8A_URXD, PIO_PULLUP);
	pio_configure(PIOA, PIO_PERIPH_A, PIO_PA9A_UTXD, PIO_PULLUP);
	
	usart_serial_options_t uart_opt;
	uart_opt.baudrate	= 9600;
	uart_opt.paritytype	= UART_MR_PAR_NO;
	
	comms_serial_init(UART, &uart_opt);
}

uint32_t uart_print_string_to_serial(char *c)
{
	uint32_t x;
	
	for (x = 0; c[x] != '\0'; x++) {
		comms_serial_putchar(UART, c[x]);
		comms_serial_putchar(USART0, 'z');
	}
	
	return 0;
}


