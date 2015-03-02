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
	init_uart_buffer();
}

uint32_t uart_print_string_to_serial(char *c)
{
	uint32_t x;
	
	for (x = 0; c[x] != '\0'; x++) {
		comms_serial_putchar(UART, c[x]);
	}
	
	return 0;
}

void uart_write_char_to_buffer(char c) {
	UART_BUFFER(UART_BUFFER_NEXT) = c;
	UART_BUFFER_NEXT++;
}
void uart_write_string_to_buffer(char* c) {
	for (uint32_t x = 0; c[x] != '\0'; x++) {
		uart_write_char_to_buffer(c[x]);
	}
	uart_write_char_to_buffer('\n');
	uart_write_char_to_buffer('\r');
}

void uart_push_buffer_to_serial() {
	for (uint32_t pos = 0; pos < UART_BUFFER_NEXT; pos++) {
		comms_serial_putchar(UART, UART_BUFFER(pos));
		UART_BUFFER(pos) = '\0';
	} 
	UART_BUFFER_NEXT = 0;
}

void init_uart_buffer() {
	for (uint32_t pos = 0; pos < UART_BUFFER_SIZE; pos++) {
		UART_BUFFER(pos) = '\0';
	}
	UART_BUFFER_NEXT = 0;
}
