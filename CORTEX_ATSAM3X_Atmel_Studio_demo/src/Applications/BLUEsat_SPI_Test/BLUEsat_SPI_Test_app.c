/*
 * BLUEsat_SPI_Test.c
 *
 * Created: 01-02-2015 2:41:32 PM
 *  Author: Blue
 */ 

/* Scheduler include files. */
#include "FreeRTOS.h"
#include "task.h"

#include "BLUEsat_SPI_Test_app.h"

/* Atmel library includes. */
#include <asf.h>

/* The constants used in the calculation. */
#define intgSTACK_SIZE			configMINIMAL_STACK_SIZE

/* As this is the minimal version, we will only create one task. */
#define intgNUMBER_OF_TASKS		( 1 )

/* The task function. */
static portTASK_FUNCTION_PROTO( vBLUEsat_SPI_TestTask, pvParameters );

/* Variables that are set to true within the calculation task to indicate
that the task is still executing.  The check task sets the variable back to
false, flagging an error if the variable is still false the next time it
is called. */
static volatile BaseType_t xTaskCheck[ intgNUMBER_OF_TASKS ] = { ( BaseType_t ) pdFALSE };

/*-----------------------------------------------------------*/

void vStartBLUEsat_SPI_TestTasks( UBaseType_t uxPriority )
{
	short sTask;
	
	for( sTask = 0; sTask < intgNUMBER_OF_TASKS; sTask++ )
	{
		xTaskCreate( vBLUEsat_SPI_TestTask, "BLUEsat_SPI_Test", intgSTACK_SIZE, ( void * ) &( xTaskCheck[ sTask ] ), uxPriority, ( TaskHandle_t * ) NULL );
	}
}
/*-----------------------------------------------------------*/

static portTASK_FUNCTION( vBLUEsat_SPI_TestTask, pvParameters ) {
	
	volatile BaseType_t *pxTaskHasExecuted;

	/* Set a pointer to the variable we are going to set to true each
	iteration.  This is also a good test of the parameter passing mechanism
	within each port. */
	pxTaskHasExecuted = ( volatile BaseType_t * ) pvParameters;
	pxTaskHasExecuted = pxTaskHasExecuted;
	
	gpio_configure_pin(PIO_PB14_IDX, (PIO_TYPE_PIO_OUTPUT_0 | PIO_DEFAULT));
		
	for (;;) {
		BLUEsat_spi_write_string("Hello, from BLUEsat",0);
	}
	
}