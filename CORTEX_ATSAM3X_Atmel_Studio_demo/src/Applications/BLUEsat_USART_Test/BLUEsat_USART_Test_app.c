/*
 * bluesat_blink.c
 *
 * Created: 24-01-2015 1:23:26 PM
 *  Author: Blue
 */ 


/*
 * Will blind an LED connected into PIN 53 on and off
 * PIN 53 on the arduino board is referenced as PIO_PB14_IDX
 */

/*
#include <stdio.h>
#include <stdlib.h>
*/

/* Scheduler include files. */
#include "FreeRTOS.h"
#include "task.h"

/* Demo program include files. */
#include "BLUEsat_USART_Test_app.h"

/* Atmel library includes. */
#include <asf.h>

/* The constants used in the calculation. */
#define intgSTACK_SIZE			configMINIMAL_STACK_SIZE

/* As this is the minimal version, we will only create one task. */
#define intgNUMBER_OF_TASKS		( 1 )

/* The settings for USART */

/* Misc defines. */
#define serINVALID_QUEUE				( ( QueueHandle_t ) 0 )
#define serNO_BLOCK						( ( TickType_t ) 0 )
#define serPMC_USART_ID					( BOARD_ID_USART )

/* The USART supported by this file. */
#define serUSART_PORT					( USART0 )
#define serUSART_IRQ					( USART0_IRQn )

/* Baud rate used by the comtest tasks. */
#define mainCOM_TEST_BAUD_RATE				( 115200 )

/* Every bit in the interrupt mask. */
#define serMASK_ALL_INTERRUPTS			( 0xffffffffUL )

static inline int uart_getchar(uint8_t *c);

static inline int uart_putchar(const uint8_t c);

static void configure_uart(void);

/* The task function. */
static portTASK_FUNCTION_PROTO( vBLUEsat_USART_TestTask, pvParameters );

/* Variables that are set to true within the calculation task to indicate
that the task is still executing.  The check task sets the variable back to
false, flagging an error if the variable is still false the next time it
is called. */
static volatile BaseType_t xTaskCheck[ intgNUMBER_OF_TASKS ] = { ( BaseType_t ) pdFALSE };

/*-----------------------------------------------------------*/

void vStartBLUEsat_USART_TestTasks( UBaseType_t uxPriority )
{
	short sTask;
	
	for( sTask = 0; sTask < intgNUMBER_OF_TASKS; sTask++ )
	{
		xTaskCreate( vBLUEsat_USART_TestTask, "BLUEsat_USART_Test", intgSTACK_SIZE, ( void * ) &( xTaskCheck[ sTask ] ), uxPriority, ( TaskHandle_t * ) NULL );
	}
}
/*-----------------------------------------------------------*/

static portTASK_FUNCTION( vBLUEsat_USART_TestTask, pvParameters )
{
	short sError = pdFALSE;
	volatile BaseType_t *pxTaskHasExecuted;

	/* Set a pointer to the variable we are going to set to true each
	iteration.  This is also a good test of the parameter passing mechanism
	within each port. */
	pxTaskHasExecuted = ( volatile BaseType_t * ) pvParameters;
	
	// letter to send
	char helloWorldString[] = "Hello World\n\r";
	
	// uint32_t ulChar;	

	//First attempt (failed)
	/*		
	
	// Initiates the System Clock Management (sysclk)
	// sysclk_init();
	
	// Sets up usart options 
	sam_usart_opt_t first_usart_options;
	
	first_usart_options.baudrate = 9600;
	first_usart_options.char_length = US_MR_CHRL_8_BIT;
	first_usart_options.parity_type = US_MR_PAR_EVEN;
	first_usart_options.stop_bits = US_MR_NBSTOP_1_BIT;
	
	Usart first_usart;
	usart_reset(&first_usart);
	
	// Finds the USART module input clock frequency.
	// I think
	uint32_t first_usart_ul_mck = sysclk_get_peripheral_hz();
	
	uint32_t first_usart_result = usart_init_rs232(&first_usart, &first_usart_options, first_usart_ul_mck);
		
	// to make it compile
	first_usart_result = first_usart_result;
	
	uint32_t first_usart_is_tx_ready = usart_is_tx_ready(&first_usart);
	uint32_t first_usart_is_tx_empty = usart_is_tx_empty(&first_usart);
	
	first_usart_is_tx_ready = first_usart_is_tx_ready;
	first_usart_is_tx_empty = first_usart_is_tx_empty;
	
	usart_enable_tx(&first_usart);
	
	uint32_t first_usart_write = 55555;
	uint32_t first_usart_write_result = usart_write( &first_usart, first_usart_write );
	
	first_usart_write_result = first_usart_write_result;
	
	*/
	
	// second attempt (failed)
	/*
	
	// First enable the USART Clock in the PMC
	// serPMC_USART_ID --> BOARD_ID_USART --> ID_USART0 --> (17)
	pmc_enable_periph_clk( serPMC_USART_ID );
	
	// USART options
	const sam_usart_opt_t xUSARTSettings =
	{
		mainCOM_TEST_BAUD_RATE,
		US_MR_CHRL_8_BIT,
		US_MR_PAR_NO,
		US_MR_NBSTOP_1_BIT,
		US_MR_CHMODE_NORMAL,
		0 // Only used in IrDA mode.
	};
	
	// Configure USART in serial mode.
	usart_init_rs232( serUSART_PORT, &xUSARTSettings, sysclk_get_cpu_hz() );
	
	// attempting to configure the pins
	//	Instance	Signal		I/O Line	Peripheral
	//	USART0		CTS0		PB26		A
	//	USART0		RTS0		PB25		A
	//	USART0		RXD0		PA10		A
	//	USART0		SCK0		PA17		B
	//	USART0		TXD0		PA11		A
	gpio_configure_pin( PIO_PB26A_CTS0, ( PIO_TYPE_PIO_PERIPH_A | PIO_DEFAULT ) );
	gpio_configure_pin( PIO_PB25A_RTS0, ( PIO_TYPE_PIO_PERIPH_A | PIO_DEFAULT ) );
	gpio_configure_pin( PIO_PA10A_RXD0, ( PIO_TYPE_PIO_PERIPH_A | PIO_DEFAULT ) );
	gpio_configure_pin( PIO_PA17B_SCK0, ( PIO_TYPE_PIO_PERIPH_B | PIO_DEFAULT ) );
	gpio_configure_pin( PIO_PA11A_TXD0, ( PIO_TYPE_PIO_PERIPH_A | PIO_PULLUP ) );	// To prevent the TXD line from falling when the USART 
																					// is disabled, the use of an internal pull up
																					// is mandatory.
	
	// Disable all the interrupts. 
	usart_disable_interrupt( serUSART_PORT, serMASK_ALL_INTERRUPTS );

	// Enable the receiver and transmitter.
	usart_enable_tx( serUSART_PORT );
	usart_enable_rx( serUSART_PORT );

	// Clear any characters before enabling interrupt.
	usart_getchar( serUSART_PORT, &ulChar );

	// Enable Rx end interrupt.
	usart_enable_interrupt( serUSART_PORT, US_IER_RXRDY );

	// Configure and enable interrupt of USART.
	NVIC_SetPriority( serUSART_IRQ, configLIBRARY_MAX_SYSCALL_INTERRUPT_PRIORITY );
	NVIC_EnableIRQ( serUSART_IRQ );
	
	*/
	
	SystemInit();
	
	configure_uart();
	
	int x;
	int y;
	
	/* Keep performing a calculation and checking the result against a constant. */
	for( ;; )
	{
		for (x = 0; helloWorldString[x] != '\0'; x++) {
			uart_putchar(helloWorldString[x]);
			for (y=0;y<1000;y++);
		}
		for (x=0;x<100000;x++);
		
		
		if( sError == pdFALSE )
		{
			/* We have not encountered any errors, so set the flag that show
			we are still executing.  This will be periodically cleared by
			the check task. */
			portENTER_CRITICAL();
				*pxTaskHasExecuted = pdTRUE;
			portEXIT_CRITICAL();
		}

		/* Yield in case cooperative scheduling is being used. */
		#if configUSE_PREEMPTION == 0
		{
			taskYIELD();
		}
		#endif
	}
	
}

static inline int uart_getchar(uint8_t *c)
{
	// Check if the receiver is ready
	if((UART->UART_SR & UART_SR_RXRDY) == 0)
	return 1;
	
	// Read the character
	*c = (uint8_t) UART->UART_RHR;
	return 0;
}

static inline int uart_putchar(const uint8_t c)
{
	// Check if the transmitter is ready
	if(!(UART->UART_SR & UART_SR_TXRDY))
	return 1;
	
	// Send the character
	UART->UART_THR = c;
	return 0;
}

void UART_Handler(void)
{
	uint8_t c;
	
	// Check if the interrupt source is receive ready
	if(UART->UART_IMR & UART_IMR_RXRDY)
	{
		if(uart_getchar(&c) == 0)
		{
			uart_putchar(c);
		}
	}
}

static void configure_uart(void)
{
	uint32_t ul_sr;
	
	// ==> Pin configuration
	// Disable interrupts on Rx and Tx
	PIOA->PIO_IDR = PIO_PA8A_URXD | PIO_PA9A_UTXD;
	
	// Disable the PIO of the Rx and Tx pins so that the peripheral controller can use them
	PIOA->PIO_PDR = PIO_PA8A_URXD | PIO_PA9A_UTXD;
	
	// Read current peripheral AB select register and set the Rx and Tx pins to 0 (Peripheral A function)
	ul_sr = PIOA->PIO_ABSR;
	PIOA->PIO_ABSR &= ~(PIO_PA8A_URXD | PIO_PA9A_UTXD) & ul_sr;
	
	// Enable the pull up on the Rx and Tx pin
	PIOA->PIO_PUER = PIO_PA8A_URXD | PIO_PA9A_UTXD;
	
	// ==> Actual uart configuration
	// Enable the peripheral uart controller
	PMC->PMC_PCER0 = 1 << ID_UART;
	
	// Reset and disable receiver & transmitter
	UART->UART_CR = UART_CR_RSTRX | UART_CR_RSTTX | UART_CR_RXDIS | UART_CR_TXDIS;
	
	// Set the baudrate to 115200
	UART->UART_BRGR = 45; // 84000000 / 16 * x = BaudRate (write x into UART_BRGR)
	
	// No Parity
	UART->UART_MR = UART_MR_PAR_NO;
	
	// Disable PDC channel requests
	UART->UART_PTCR = UART_PTCR_RXTDIS | UART_PTCR_TXTDIS;
	
	// Disable / Enable interrupts on end of receive
	UART->UART_IDR = 0xFFFFFFFF;
	NVIC_EnableIRQ((IRQn_Type) ID_UART);
	UART->UART_IER = UART_IER_RXRDY;
	
	// Enable receiver and trasmitter
	UART->UART_CR = UART_CR_RXEN | UART_CR_TXEN;
	
}