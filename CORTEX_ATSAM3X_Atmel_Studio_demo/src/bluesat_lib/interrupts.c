/*
 * interrupts.c
 *
 * Created: 23/05/2015 8:15:10 PM
 *  Author: Blue
 */ 
#include <asf.h>
#include "bluesat_conf.h"
#include "FreeRTOS.h"

volatile uint32_t spi_sr_temp;
volatile uint32_t spi_dr_temp;
BaseType_t HigherPriorityTaskWoken;

void SPI0_Handler (void) 
{
    NVIC_ClearPendingIRQ(SPI0_IRQn);
    spi_sr_temp = spi_read_status(SPI0);
	xHigherPriorityTaskWoken = pdFALSE;
    if(spi_sr_temp & SPI_SR_OVRES )
    {
		// receive overrun - handle error
        spi_sr_temp = spi_sr_temp;
    }

    if(spi_sr_temp & SPI_SR_MODF)
    {
		// mode fault occurred - handle error
        spi_sr_temp = spi_sr_temp;
    }

    if(spi_sr_temp & SPI_SR_RDRF)
    {
		spi_dr_temp = spi_read_single(SPI0);
        if(errQUEUE_FULL == xQueueSendFromISR(SPI_TX_QUEUE, &spi_dr_temp, &HigherPriorityTaskWoken ))
		{
			// Queue full - handle error 
		}
    }
	
	if(spi_sr_temp & SPI_SR_TDRE)
	{
		if( pdTRUE != xQueueReceiveFromISR(SPI_TX_QUEUE, &spi_dr_temp, HigherPriorityTaskWoken ))
		{
			// Failed to get a byte from TX queue - handle it!
		}
		
		if(!(uxQueueMessagesWaitingFromISR(SPI_TX_QUEUE) > 0))
		{
			spi_disable_interrupt(SPI0, SPI_IDR_TXEMPTY);
		}
	}
	
	if(HigherPriorityTaskWoken == pdTRUE)
	{
		portYIELD_FROM_ISR(HigherPriorityTaskWoken)
	}


}

void TWI0_Handler() {
    
}

void TWI1_Handler() {
    
}
