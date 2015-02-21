/*
 * comms_usart_drv.c
 *
 * Created: 21-02-2015 11:34:20 AM
 *  Author: Blue
 */ 

#include <asf.h>
#include "comms_usart_drv.h"

void configure_usart() 
{
	sysclk_init();
	
	pio_configure(PIOA, PIO_PERIPH_A, PIO_PB26A_CTS0, 0);
	pio_configure(PIOA, PIO_PERIPH_A, PIO_PB25A_RTS0, 0);
	pio_configure(PIOA, PIO_PERIPH_A, PIO_PA10A_RXD0, 0);
	pio_configure(PIOA, PIO_PERIPH_B, PIO_PA17B_SCK0, 0);
	pio_configure(PIOA, PIO_PERIPH_A, PIO_PA11A_TXD0, PIO_PULLUP);
	
	sam_usart_opt_t usart_settings;
	usart_settings.baudrate		= 9600;
	usart_settings.char_length	= US_MR_CHRL_8_BIT;
	usart_settings.parity_type	= US_MR_PAR_NO;
	usart_settings.stop_bits	= US_MR_NBSTOP_1_BIT;
	usart_settings.channel_mode	= US_MR_CHMODE_NORMAL;
	
	pmc_enable_periph_clk(ID_USART0);
	
	usart_init_rs232(USART0, &usart_settings, sysclk_get_main_hz());
	usart_enable_tx(USART0);
	usart_enable_rx(USART0);
}