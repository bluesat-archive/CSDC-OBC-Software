/*
 * serial_print_drv.c
 *
 * Created: 28/01/2015 09:53:42
 *  Author: John
 */

/* Atmel Library */
#include <asf.h>

#include "BLUEsat_serial_drv.h"

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

int uart_getchar(uint8_t *c)
{
	// Check if the receiver is ready
	if((UART->UART_SR & UART_SR_RXRDY) == 0)
	return 1;
	
	// Read the character
	*c = (uint8_t) UART->UART_RHR;
	return 0;
}

int uart_putchar(const uint8_t c)
{
	// Check if the transmitter is ready
	if(!(UART->UART_SR & UART_SR_TXRDY))
	return 1;
	
	// Send the character
	UART->UART_THR = c;
	return 0;
}

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