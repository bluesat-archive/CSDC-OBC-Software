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

//#include <stdio.h>
#include <stdlib.h>

/* Scheduler include files. */
#include "FreeRTOS.h"
#include "task.h"

/* Demo program include files. */
#include "bluesat_blink.h"

/* Atmel library includes. */
#include <asf.h>

/* The constants used in the calculation. */
#define intgSTACK_SIZE			configMINIMAL_STACK_SIZE

/* As this is the minimal version, we will only create one task. */
#define intgNUMBER_OF_TASKS		( 1 )

/* The task function. */
static portTASK_FUNCTION_PROTO( vRepeatingBluesat_BlinkTask, pvParameters );

/* Variables that are set to true within the calculation task to indicate
that the task is still executing.  The check task sets the variable back to
false, flagging an error if the variable is still false the next time it
is called. */
static volatile BaseType_t xTaskCheck[ intgNUMBER_OF_TASKS ] = { ( BaseType_t ) pdFALSE };
	
uint32_t PIN_53 = PIO_PB14_IDX;

/*-----------------------------------------------------------*/

void vStartBluesat_BlinkTasks( UBaseType_t uxPriority )
{
short sTask;

	for( sTask = 0; sTask < intgNUMBER_OF_TASKS; sTask++ )
	{
		xTaskCreate( vRepeatingBluesat_BlinkTask, "Bluesat_Blink", intgSTACK_SIZE, ( void * ) &( xTaskCheck[ sTask ] ), uxPriority, ( TaskHandle_t * ) NULL );
	}
}
/*-----------------------------------------------------------*/

static portTASK_FUNCTION( vRepeatingBluesat_BlinkTask, pvParameters )
{
/* These variables are all effectively set to constants so they are volatile to
ensure the compiler does not just get rid of them. */
// volatile long lValue;
short sError = pdFALSE;
volatile BaseType_t *pxTaskHasExecuted;

	/* Set a pointer to the variable we are going to set to true each
	iteration.  This is also a good test of the parameter passing mechanism
	within each port. */
	pxTaskHasExecuted = ( volatile BaseType_t * ) pvParameters;

	/* Keep performing a calculation and checking the result against a constant. */
	for( ;; )
	{
		// Blinks an LED connect on PIN 53
			
		// Config PIO_PB14_IDX, which is PIN 53 on the arduino board

		gpio_configure_pin(PIN_53, (PIO_TYPE_PIO_OUTPUT_0 | PIO_DEFAULT));
			
		// Blinks the LED
		int x;
		while (1) {
			// Only meant for testing, this will not exit.
			x = 0;
			while (x < 300000) {
				x++;
			}
			pio_toggle_pin(PIN_53);
		}
		
		// 
		/*
		if( 0 )
		{
			sError = pdTRUE;
		}
		*/

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
/*-----------------------------------------------------------*/

/* This is called to check that all the created tasks are still running. */
BaseType_t xAreBluesat_BlinkTaskStillRunning( void )
{
BaseType_t xReturn = pdTRUE;
short sTask;

	/* Check the maths tasks are still running by ensuring their check variables 
	are still being set to true. */
	for( sTask = 0; sTask < intgNUMBER_OF_TASKS; sTask++ )
	{
		if( xTaskCheck[ sTask ] == pdFALSE )
		{
			/* The check has not incremented so an error exists. */
			xReturn = pdFALSE;
		}

		/* Reset the check variable so we can tell if it has been set by
		the next time around. */
		xTaskCheck[ sTask ] = pdFALSE;
	}

	return xReturn;
}

