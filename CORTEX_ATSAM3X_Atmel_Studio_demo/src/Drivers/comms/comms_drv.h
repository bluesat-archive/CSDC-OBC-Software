/*
 * comms_drv.h
 *
 * Created: 21-02-2015 2:56:14 PM
 *  Author: Blue
 */ 

#ifndef COMMS_DRV_H_
#define COMMS_DRV_H_

#include <parts.h>
#include "status_codes.h"
#include "compiler.h"
#include "sysclk.h"
#include "uart.h"

typedef struct uart_rs232_options {
	uint32_t baudrate;
	uint32_t charlength;	// Number of bits to transmit as a character (5 to 9)
	uint32_t paritytype;	// Parity type: USART_PMODE_DISABLED_gc, USART_PMODE_EVEN_gc, USART_PMODE_ODD_gc.
	uint32_t stopbits;		// true: 2 stop bits, false: 1 stop bit
} usart_rs232_options_t;

typedef usart_rs232_options_t	usart_serial_options_t;
typedef void*					comms_instance;

void			comms_serial_init			(comms_instance p_usart, usart_serial_options_t *opt);
uint32_t		comms_serial_putchar		(comms_instance p_usart, const uint8_t c);
void			comms_serial_getchar		(comms_instance p_usart, uint8_t *data);
uint32_t		comms_serial_is_rx_ready	(comms_instance p_usart);
status_code_t	comms_serial_write_packet	(comms_instance usart, const uint8_t *data, size_t len);
status_code_t	comms_serial_read_packet	(comms_instance usart, uint8_t *data, size_t len);

#endif /* COMMS_DRV_H_ */