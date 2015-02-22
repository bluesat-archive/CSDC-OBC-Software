/*
 * comms_usart_drv.c
 *
 * Created: 21-02-2015 11:34:20 AM
 *  Author: Blue
 */ 

#include <asf.h>
#include "comms_drv.h"
#include "comms_usart_drv.h"

void configure_usart() 
{	
	pio_configure(PIOA, PIO_PERIPH_A, PIO_PB26A_CTS0, 0);
	pio_configure(PIOA, PIO_PERIPH_A, PIO_PB25A_RTS0, 0);
	pio_configure(PIOA, PIO_PERIPH_A, PIO_PA10A_RXD0, 0);
	pio_configure(PIOA, PIO_PERIPH_B, PIO_PA17B_SCK0, 0);
	pio_configure(PIOA, PIO_PERIPH_A, PIO_PA11A_TXD0, PIO_PULLUP);
	
	usart_rs232_options_t usart_settings;
	usart_settings.baudrate		= 9600;
	usart_settings.charlength	= US_MR_CHRL_8_BIT;
	usart_settings.paritytype	= US_MR_PAR_NO;
	usart_settings.stopbits		= US_MR_NBSTOP_1_BIT;
	
	comms_serial_init(USART0, &usart_settings);
}