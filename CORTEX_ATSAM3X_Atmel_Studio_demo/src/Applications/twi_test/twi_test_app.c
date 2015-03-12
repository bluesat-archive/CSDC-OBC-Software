/*
 * twi_test_app.c
 *
 * Created: 13-03-2015 12:06:57 AM
 *  Author: Blue
 */ 

#include "FreeRTOS.h"
#include "task.h"
#include "twi_test_app.h"

#include <asf.h>
#include <comms_twi_drv.h>

#define intgSTACK_SIZE			configMINIMAL_STACK_SIZE	// number of variables available for this task
#define intgNUMBER_OF_TASKS		( 1 )						// Create one task

/* The task function. */
static portTASK_FUNCTION_PROTO( vBLUEsat_TWI_TestTask, pvParameters );

/* Variables that are set to true within the calculation task to indicate
that the task is still executing.  The check task sets the variable back to
false, flagging an error if the variable is still false the next time it
is called. */
static volatile BaseType_t xTaskCheck[ intgNUMBER_OF_TASKS ] = { ( BaseType_t ) pdFALSE };

/*-----------------------------------------------------------*/

void vStartBLUEsat_TWI_TestTasks( UBaseType_t uxPriority )
{
	short sTask;
	
	for( sTask = 0; sTask < intgNUMBER_OF_TASKS; sTask++ )
	{
		xTaskCreate( vBLUEsat_TWI_TestTask, "BLUEsat_TWI_Test", intgSTACK_SIZE, ( void * ) &( xTaskCheck[ sTask ] ), uxPriority, ( TaskHandle_t * ) NULL );
	}
}
/*-----------------------------------------------------------*/

static portTASK_FUNCTION( vBLUEsat_TWI_TestTask, pvParameters ) {
	
	volatile BaseType_t *pxTaskHasExecuted;

	/* Set a pointer to the variable we are going to set to true each
	iteration.  This is also a good test of the parameter passing mechanism
	within each port. */
	pxTaskHasExecuted = ( volatile BaseType_t * ) pvParameters;
	pxTaskHasExecuted = pxTaskHasExecuted;
			
	for (;;) {
	}
}