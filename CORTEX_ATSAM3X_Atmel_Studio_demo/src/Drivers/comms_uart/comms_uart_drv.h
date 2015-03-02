/*
 * serial_print_drv.h
 *
 * Created: 28/01/2015 09:54:22
 *  Author: John
 */ 

/*
	Code borrowed and barely modified from: http://codetron.net/uart-interface-sam3x8e-arduino-due/
*/


#ifndef SERIAL_PRINT_DRV_H_
#define SERIAL_PRINT_DRV_H_

#define UART_BUFFER_START			uart_buffer
#define UART_BUFFER(x)				uart_buffer[x]
#define UART_BUFFER_SIZE			256
#define UART_BUFFER_NEXT			uart_buffer_next_available_space

char		UART_BUFFER_START[UART_BUFFER_SIZE];
uint32_t	UART_BUFFER_NEXT;

void configure_uart(void);
uint32_t uart_print_string_to_serial(char *c);

void uart_write_char_to_buffer(char c);
void uart_write_string_to_buffer(char* c);
void uart_push_buffer_to_serial();
void init_uart_buffer();

#endif /* SERIAL_PRINT_DRV_H_ */