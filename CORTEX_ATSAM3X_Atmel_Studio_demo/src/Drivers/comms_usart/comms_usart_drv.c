/*
 * comms_usart_drv.c
 *
 * Created: 21-02-2015 11:34:20 AM
 *  Author: Blue
 */ 

#include <asf.h>
#include <comms_usart_drv.h>

#define USART_SERIAL_INSTANCE					USART0
#define USART_SERIAL_PERIPHERAL_ID				ID_USART0

void configure_usart() {
	
	sysclk_init();
	
	// USART0 Peripheral as outlined on page 780
	pio_configure(PIOA, PIO_PERIPH_A, PIO_PB26A_CTS0, 0);
	pio_configure(PIOA, PIO_PERIPH_A, PIO_PB25A_RTS0, 0);
	pio_configure(PIOA, PIO_PERIPH_A, PIO_PA10A_RXD0, 0);
	pio_configure(PIOA, PIO_PERIPH_B, PIO_PA17B_SCK0, 0);
	pio_configure(PIOA, PIO_PERIPH_A, PIO_PA11A_TXD0, PIO_PULLUP);		// pull up is mandatory
	
	// usart settings
	sam_usart_opt_t usart_console_settings;
	usart_console_settings.baudrate		= 9600;
	usart_console_settings.char_length	= US_MR_CHRL_8_BIT;
	usart_console_settings.parity_type	= US_MR_PAR_NO;
	usart_console_settings.stop_bits	= US_MR_NBSTOP_1_BIT;
	usart_console_settings.channel_mode	= US_MR_CHMODE_NORMAL;
	
	// enable peripheral clock
	pmc_enable_periph_clk(USART_SERIAL_PERIPHERAL_ID);
	
	// initiliase and enable usart
	usart_init_rs232(USART_SERIAL_INSTANCE, &usart_console_settings, sysclk_get_main_hz());
	usart_enable_tx(USART_SERIAL_INSTANCE);
	usart_enable_rx(USART_SERIAL_INSTANCE);
}