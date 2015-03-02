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
#include "uart_test_app.h"

/* Atmel library includes. */
#include <asf.h>

/* BLUEsat driver includes. */
#include <comms_uart_drv.h>

/* The constants used in the calculation. */
#define intgSTACK_SIZE			configMINIMAL_STACK_SIZE

/* As this is the minimal version, we will only create one task. */
#define intgNUMBER_OF_TASKS		( 1 )

/* The task function. */
static portTASK_FUNCTION_PROTO( vBLUEsat_UART_TestTask, pvParameters );

/* Variables that are set to true within the calculation task to indicate
that the task is still executing.  The check task sets the variable back to
false, flagging an error if the variable is still false the next time it
is called. */
static volatile BaseType_t xTaskCheck[ intgNUMBER_OF_TASKS ] = { ( BaseType_t ) pdFALSE };

/*-----------------------------------------------------------*/

void vStartBLUEsat_UART_TestTasks( UBaseType_t uxPriority )
{
	short sTask;
	
	for( sTask = 0; sTask < intgNUMBER_OF_TASKS; sTask++ )
	{
		xTaskCreate( vBLUEsat_UART_TestTask, "BLUEsat_UART_Test", intgSTACK_SIZE, ( void * ) &( xTaskCheck[ sTask ] ), uxPriority, ( TaskHandle_t * ) NULL );
	}
}
/*-----------------------------------------------------------*/

static portTASK_FUNCTION( vBLUEsat_UART_TestTask, pvParameters )
{
	// simple test, don't need this. (not sure how to use it anyway...)
	pvParameters = pvParameters;
	
	for( ;; )
	{
		uart_write_string_to_buffer("Hello World");
		uart_write_string_to_buffer("- UART Driver Test");
		uart_write_string_to_buffer("");
		uart_push_buffer_to_serial();
	}
}