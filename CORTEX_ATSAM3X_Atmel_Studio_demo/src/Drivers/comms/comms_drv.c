/*
 * comms_drv.c
 *
 * Created: 21-02-2015 2:56:06 PM
 *  Author: Blue
 */
#include "comms_drv.h"

/** 
 * \brief Initializes the Usart in master mode.
 *
 * \param p_usart  Base address of the USART instance.
 * \param opt      Options needed to set up RS232 communication (see
 * \ref usart_options_t).
 */
void comms_serial_init(comms_instance p_usart, usart_serial_options_t *opt)
{
	sam_uart_opt_t uart_settings;
	uart_settings.ul_mck		= sysclk_get_peripheral_hz();
	uart_settings.ul_baudrate	= opt->baudrate;
	uart_settings.ul_mode		= opt->paritytype;
	uart_settings.ul_chmode		= UART_MR_CHMODE_NORMAL;

	sam_usart_opt_t usart_settings;
	usart_settings.baudrate		= opt->baudrate;
	usart_settings.char_length	= opt->charlength;
	usart_settings.parity_type	= opt->paritytype;
	usart_settings.stop_bits	= opt->stopbits;
	usart_settings.channel_mode	= US_MR_CHMODE_NORMAL;
	
	switch ((uint32_t)p_usart)
	{
		case (uint32_t)UART:
			sysclk_enable_peripheral_clock(ID_UART);
			break;
		case (uint32_t)USART0:
			sysclk_enable_peripheral_clock(ID_USART0);
			break;
		case (uint32_t)USART1:
			sysclk_enable_peripheral_clock(ID_USART1);
			break;
		case (uint32_t)USART2:
			sysclk_enable_peripheral_clock(ID_USART2);
			break;
		case (uint32_t)USART3:
			sysclk_enable_peripheral_clock(ID_USART3);
			break;
	}
	
	switch ((uint32_t)p_usart)
	{
		case (uint32_t)UART:
			uart_init((Uart*)p_usart, &uart_settings);
			break;
		case (uint32_t)USART0:
		case (uint32_t)USART1:
		case (uint32_t)USART2:
		case (uint32_t)USART3:
			usart_init_rs232((Usart*)p_usart, &usart_settings, sysclk_get_peripheral_hz());
			usart_enable_tx((Usart*)p_usart);
			usart_enable_rx((Usart*)p_usart);
			break;
	}
}

/**
 * Sends a character.
 *
 * \param p_usart   Base address of the peripheral instance.
 * \param c			Character to write.
 *
 * \return Status.
 *   \retval 1  The character was written.
 *   \retval 0  The function timed out before the USART transmitter became
 * ready to send.
 */
uint32_t comms_serial_putchar(comms_instance p_usart, const uint8_t c)
{
	switch ((uint32_t)p_usart) 
	{
		case (uint32_t)UART:
			while (uart_write((Uart*)p_usart, c)!=0);
			return 1;
		case (uint32_t)USART0:
		case (uint32_t)USART1:
		case (uint32_t)USART2:
		case (uint32_t)USART3:
			while (usart_write((Usart*)p_usart, c)!=0);
			return 1;
	}

	return 0;
}

/**
 * \brief Waits until a character is received, and returns it.
 *
 * \param p_usart   Base address of the USART instance.
 * \param data		Data to read
 *
 */
void comms_serial_getchar(comms_instance p_usart, uint8_t *data)
{
	uint32_t val;
	
	switch((uint32_t)p_usart) 
	{
		case (uint32_t)UART:
			uart_read((Uart*)p_usart, data);
			break;
		case (uint32_t)USART0:
		case (uint32_t)USART1:
		case (uint32_t)USART2:
		case (uint32_t)USART3:
			while (usart_read((Usart*)p_usart, &val));
			*data = (uint8_t)(val & 0xFF);
			break;
	}
}

/**
 * \brief Check if Received data is ready.
 *
 * \param p_usart   Base address of the USART instance.
 *
 * \retval 1 One data has been received.
 * \retval 0 No data has been received.
 */
uint32_t comms_serial_is_rx_ready(comms_instance p_usart)
{
	switch((uint32_t)p_usart) 
	{
		case (uint32_t)UART:
			return uart_is_rx_ready((Uart*)p_usart);
			break;
		case (uint32_t)USART0:
		case (uint32_t)USART1:
		case (uint32_t)USART2:
		case (uint32_t)USART3:
			return usart_is_rx_ready((Usart*)p_usart);
			break;
	}

	return 0;
}


/**
 * \brief Send a sequence of bytes to USART device
 *
 * \param usart  Base address of the USART instance.
 * \param data   Data buffer to read
 * \param len    Length of data
 *
 */
status_code_t comms_serial_write_packet(comms_instance usart, const uint8_t *data, size_t len)
{
	while (len) {
		comms_serial_putchar(usart, *data);
		len--;
		data++;
	}
	return STATUS_OK;
}

/**
 * \brief Receive a sequence of bytes from USART device
 *
 * \param usart  Base address of the USART instance.
 * \param data   Data buffer to write
 * \param len    Length of data
 *
 */
status_code_t comms_serial_read_packet(comms_instance usart, uint8_t *data, size_t len)
{
	while (len) {
		comms_serial_getchar(usart, data);
		len--;
		data++;
	}
	return STATUS_OK;
}