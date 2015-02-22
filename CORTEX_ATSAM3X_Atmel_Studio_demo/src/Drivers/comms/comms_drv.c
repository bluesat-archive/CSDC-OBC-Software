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

/**
 * \page serial_quickstart Quick start guide for Serial Interface service
 *
 * This is the quick start guide for the \ref serial_group "Serial Interface module", with
 * step-by-step instructions on how to configure and use the serial in a
 * selection of use cases.
 *
 * The use cases contain several code fragments. The code fragments in the
 * steps for setup can be copied into a custom initialization function, while
 * the steps for usage can be copied into, e.g., the main application function.
 *
 * \section serial_use_cases Serial use cases
 * - \ref serial_basic_use_case
 * - \subpage serial_use_case_1
 *
 * \section serial_basic_use_case Basic use case - transmit a character
 * In this use case, the serial module is configured for:
 * - Using USARTD0
 * - Baudrate: 9600
 * - Character length: 8 bit
 * - Parity mode: Disabled
 * - Stop bit: None
 * - RS232 mode
 *
 * The use case waits for a received character on the configured USART and
 * echoes the character back to the same USART.
 *
 * \section serial_basic_use_case_setup Setup steps
 *
 * \subsection serial_basic_use_case_setup_prereq Prerequisites
 * -# \ref sysclk_group "System Clock Management (sysclk)"
 *
 * \subsection serial_basic_use_case_setup_code Example code
 * The following configuration must be added to the project (typically to a 
 * conf_serial.h file, but it can also be added to your main application file.)
 * \code
 *    #define USART_SERIAL                     &USARTD0
 *    #define USART_SERIAL_BAUDRATE            9600
 *    #define USART_SERIAL_CHAR_LENGTH         USART_CHSIZE_8BIT_gc
 *    #define USART_SERIAL_PARITY              USART_PMODE_DISABLED_gc
 *    #define USART_SERIAL_STOP_BIT            false
 * \endcode
 *
 * A variable for the received byte must be added:
 * \code uint8_t received_byte; \endcode
 *
 * Add to application initialization:
 * \code
 *    sysclk_init();
 *
 *    static usart_serial_options_t usart_options = {
 *       .baudrate = USART_SERIAL_BAUDRATE,
 *       .charlength = USART_SERIAL_CHAR_LENGTH,
 *       .paritytype = USART_SERIAL_PARITY,
 *       .stopbits = USART_SERIAL_STOP_BIT
 *    };
 *
 *    usart_serial_init(USART_SERIAL, &usart_options);
 * \endcode
 *
 * \subsection serial_basic_use_case_setup_flow Workflow
 * -# Initialize system clock:
 *   - \code sysclk_init(); \endcode
 * -# Create serial USART options struct:
 *   - \code
 *        static usart_serial_options_t usart_options = {
 *           .baudrate = USART_SERIAL_BAUDRATE,
 *           .charlength = USART_SERIAL_CHAR_LENGTH,
 *           .paritytype = USART_SERIAL_PARITY,
 *           .stopbits = USART_SERIAL_STOP_BIT
 *        };
 *     \endcode
 * -# Initialize the serial service:
 *   - \code usart_serial_init(USART_SERIAL, &usart_options);\endcode
 *
 * \section serial_basic_use_case_usage Usage steps
 *
 * \subsection serial_basic_use_case_usage_code Example code
 * Add to application C-file:
 * \code
 *    usart_serial_getchar(USART_SERIAL, &received_byte);
 *    usart_serial_putchar(USART_SERIAL, received_byte);
 * \endcode
 *
 * \subsection serial_basic_use_case_usage_flow Workflow
 * -# Wait for reception of a character:
 *   - \code usart_serial_getchar(USART_SERIAL, &received_byte); \endcode
 * -# Echo the character back:
 *   - \code usart_serial_putchar(USART_SERIAL, received_byte); \endcode
 */

/**
 * \page serial_use_case_1 Advanced use case - Send a packet of serial data
 *
 * In this use case, the USART module is configured for:
 * - Using USARTD0
 * - Baudrate: 9600
 * - Character length: 8 bit
 * - Parity mode: Disabled
 * - Stop bit: None
 * - RS232 mode
 *
 * The use case sends a string of text through the USART.
 *
 * \section serial_use_case_1_setup Setup steps
 *
 * \subsection serial_use_case_1_setup_prereq Prerequisites
 * -# \ref sysclk_group "System Clock Management (sysclk)"
 *
 * \subsection serial_use_case_1_setup_code Example code
 * The following configuration must be added to the project (typically to a 
 * conf_serial.h file, but it can also be added to your main application file.):
 * \code
 *    #define USART_SERIAL                     &USARTD0
 *    #define USART_SERIAL_BAUDRATE            9600
 *    #define USART_SERIAL_CHAR_LENGTH         USART_CHSIZE_8BIT_gc
 *    #define USART_SERIAL_PARITY              USART_PMODE_DISABLED_gc
 *    #define USART_SERIAL_STOP_BIT            false
 * \endcode
 *
 * Add to application initialization:
 * \code
 *    sysclk_init();
 *
 *    static usart_serial_options_t usart_options = {
 *       .baudrate = USART_SERIAL_BAUDRATE,
 *       .charlength = USART_SERIAL_CHAR_LENGTH,
 *       .paritytype = USART_SERIAL_PARITY,
 *       .stopbits = USART_SERIAL_STOP_BIT
 *    };
 *
 *    usart_serial_init(USART_SERIAL, &usart_options);
 * \endcode
 *
 * \subsection serial_use_case_1_setup_flow Workflow
 * -# Initialize system clock:
 *   - \code sysclk_init(); \endcode
 * -# Create USART options struct:
 *   - \code
 *        static usart_serial_options_t usart_options = {
 *           .baudrate = USART_SERIAL_BAUDRATE,
 *           .charlength = USART_SERIAL_CHAR_LENGTH,
 *           .paritytype = USART_SERIAL_PARITY,
 *           .stopbits = USART_SERIAL_STOP_BIT
 *        };
 *     \endcode
 * -# Initialize in RS232 mode:
 *   - \code usart_serial_init(USART_SERIAL_EXAMPLE, &usart_options); \endcode
 *
 * \section serial_use_case_1_usage Usage steps
 *
 * \subsection serial_use_case_1_usage_code Example code
 * Add to, e.g., main loop in application C-file:
 * \code
 *       usart_serial_write_packet(USART_SERIAL, "Test String", strlen("Test String"));
 * \endcode
 *
 * \subsection serial_use_case_1_usage_flow Workflow
 * -# Write a string of text to the USART:
 *   - \code usart_serial_write_packet(USART_SERIAL, "Test String", strlen("Test String")); \endcode
 */