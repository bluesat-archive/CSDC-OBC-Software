/*
 * serial_print_drv.h
 *
 * Created: 28/01/2015 09:54:22
 *  Author: John
 */ 


#ifndef SERIAL_PRINT_DRV_H_
#define SERIAL_PRINT_DRV_H_

int uart_getchar(uint8_t *c);
int uart_putchar(const uint8_t c);
void configure_uart(void);

#endif /* SERIAL_PRINT_DRV_H_ */