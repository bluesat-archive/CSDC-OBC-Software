/*
 * serial_print_drv.c
 *
 * Created: 28/01/2015 09:53:42
 *  Author: John
 */

/* Atmel Library */
#include <asf.h>

#include "BLUEsat_serial_drv.h"


// I was running into errors where the arduino was pushing
// characters through the UART register too quickly and so
// I added a small delay with a for loop to fix this. Should
// try and solve it with a more friendly solution, but this
// works for now.
// Not actually sure what to base this delay off so just made
// it large enough that the characters stopped skipping.
// If the serial output looks like this:
/*
		Nwh_tn_l-iiT- u_psge(Heo
		iiT- upsgo_lHeo
					Hlo-iiTe warttr
		Hlo-iiT-  urs_e)Hlo-iiT- wart_r
		  psge(HeWo-iiT- urs_eia       Hlo-eirT
		 upinioa                Heo
		iiT-w ups_e)Hlo-ii e-wurs_e)Heo
		w __gs()                       -iiT- ups_eal
		Hlo-iiT- ursg_ra
		Hlo-iiT- ur_tn_leo
*/
// then PUTCHAR_DELAY needs to be higher
#define PUTCHAR_DELAY 1000

void configure_uart(void)
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

uint32_t uart_getchar(uint8_t *c)
{
	// Check if the receiver is ready
	if((UART->UART_SR & UART_SR_RXRDY) == 0)
	return 1;
	
	// Read the character
	*c = (uint8_t) UART->UART_RHR;
	return 0;
}

uint32_t uart_putchar(const uint8_t c)
{
	// Check if the transmitter is ready
	if(!(UART->UART_SR & UART_SR_TXRDY))
	return 1;
	
	// Send the character
	UART->UART_THR = c;
	return 0;
}

uint32_t uart_print_string_to_serial(char *c)
{
	uint32_t x;
	uint32_t y;
	
	// don't forget that \0
	for (x = 0; c[x] != '\0'; x++) {
		uart_putchar(c[x]);
		for (y=0;y<PUTCHAR_DELAY;y++);
	}
	
	return 0;
}
