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

void configure_uart(void);
uint32_t uart_print_string_to_serial(char *c);

#endif /* SERIAL_PRINT_DRV_H_ */