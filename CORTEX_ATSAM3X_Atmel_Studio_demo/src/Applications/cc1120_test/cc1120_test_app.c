/*
 * cc1120_test_app.c
 *
 * Created: 14-02-2015 5:22:38 PM
 *  Author: Blue
 */ 

#include "FreeRTOS.h"				// Scheduler include files.
#include "task.h"					// Scheduler include files.
#include "cc1120_test_app.h"		// Application include file.

#include <asf.h>
#include <cc1120_drv.h>

#define intgSTACK_SIZE			configMINIMAL_STACK_SIZE				// number of variables available for this task
#define intgNUMBER_OF_TASKS		( 1 )									// 

static portTASK_FUNCTION_PROTO( vcc1120_TestTask, pvParameters );		// The task function.

/* Variables that are set to true within the calculation task to indicate
that the task is still executing.  The check task sets the variable back to
false, flagging an error if the variable is still false the next time it
is called. */
static volatile BaseType_t xTaskCheck[ intgNUMBER_OF_TASKS ] = { ( BaseType_t ) pdFALSE };		// 

void vStartcc1120_testTasks( UBaseType_t uxPriority )
{
	short sTask;
	
	for( sTask = 0; sTask < intgNUMBER_OF_TASKS; sTask++ )
	{
		xTaskCreate( vcc1120_TestTask, "cc1120_Test", intgSTACK_SIZE, ( void * ) &( xTaskCheck[ sTask ] ), uxPriority, ( TaskHandle_t * ) NULL );
	}
}
/*-----------------------------------------------------------*/

static portTASK_FUNCTION( vcc1120_TestTask, pvParameters ) 
{
	pvParameters = pvParameters;
	
	// LED to use for debugging
	gpio_configure_pin(PIO_PB27_IDX, (PIO_TYPE_PIO_OUTPUT_0 | PIO_DEFAULT));
	
	int y;
	
	uint16_t send_byte = 0;
	send_byte = send_byte;
	
	uint16_t read_byte = 0;
	read_byte = read_byte;
	
	for (;;) {
		
		uint32_t flag = cc1120_reset();
		flag = flag;
		
		for (y=0; y<1000; y++);		
	}
	
}