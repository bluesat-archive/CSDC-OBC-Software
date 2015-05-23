/*
 * BLUEsat_SPI_Test.c
 *
 * Created: 01-02-2015 2:41:32 PM
 *  Author: Blue
 */ 

/* Scheduler include files. */
#include "FreeRTOS.h"
#include "task.h"
#include <bluesat_conf.h>
#include "drivers_test_app.h"

#include <asf.h>
#include <spi_master.h>

#define intgSTACK_SIZE			configMINIMAL_STACK_SIZE	// number of variables available for this task
#define intgNUMBER_OF_TASKS		( 1 )						// Create one task

/* The task function. */
static portTASK_FUNCTION_PROTO( vBLUEsat_drivers_test_Task, pvParameters );

/* Variables that are set to true within the calculation task to indicate
that the task is still executing.  The check task sets the variable back to
false, flagging an error if the variable is still false the next time it
is called. */
static volatile BaseType_t xTaskCheck[ intgNUMBER_OF_TASKS ] = { ( BaseType_t ) pdFALSE };

/*-----------------------------------------------------------*/

void vStartBLUEsat_drivers_test_Task( UBaseType_t uxPriority )
{
	short sTask;
	
	for( sTask = 0; sTask < intgNUMBER_OF_TASKS; sTask++ )
	{
		xTaskCreate( vBLUEsat_drivers_test_Task, "BLUEsat_drivers_test", intgSTACK_SIZE, ( void * ) &( xTaskCheck[ sTask ] ), uxPriority, ( TaskHandle_t * ) NULL );
	}
}
/*-----------------------------------------------------------*/

static portTASK_FUNCTION( vBLUEsat_drivers_test_Task, pvParameters ) {
	
	volatile BaseType_t *pxTaskHasExecuted;

	/* Set a pointer to the variable we are going to set to true each
	iteration.  This is also a good test of the parameter passing mechanism
	within each port. */
	pxTaskHasExecuted = ( volatile BaseType_t * ) pvParameters;
	pxTaskHasExecuted = pxTaskHasExecuted;

	for (;;) {
        
        spi_select_device(SPI0, SPI_Device_CC1120);
        
        spi_write_packet(SPI0, (uint8_t*)"zzza", 4);
        
        spi_deselect_device(SPI0, SPI_Device_CC1120);
        
        for (int y = 0; y < 1000; y++);
        
        SPI_Device_CC1120 = SPI_Device_CC1120;
        
        // twi_probe(TWI0, 0xFF);
        // I have no clue how twi works here.
        // Running this just gets stuck in a loop where the twi
        // master_write is waiting for the transmission to be 'completed'.
        // who knows what exactly that means.
	}
}