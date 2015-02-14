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

/* Scheduler include files. */
#include "FreeRTOS.h"
#include "task.h"

/* Demo program include files. */
#include "blink_test_app.h"

/* Atmel library includes. */
#include <asf.h>

/* The constants used in the calculation. */
#define intgSTACK_SIZE			configMINIMAL_STACK_SIZE

/* As this is the minimal version, we will only create one task. */
#define intgNUMBER_OF_TASKS		( 1 )

/* The task function. */
static portTASK_FUNCTION_PROTO( vBLUEsat_BlinkTask, pvParameters );

/* Variables that are set to true within the calculation task to indicate
that the task is still executing.  The check task sets the variable back to
false, flagging an error if the variable is still false the next time it
is called. */
static volatile BaseType_t xTaskCheck[ intgNUMBER_OF_TASKS ] = { ( BaseType_t ) pdFALSE };
	
	
#define PIO_register	0
#define GPIO_function	1

/*-----------------------------------------------------------*/

void vStartBLUEsat_BlinkTasks( UBaseType_t uxPriority )
{
	short sTask;
	
	for( sTask = 0; sTask < intgNUMBER_OF_TASKS; sTask++ )
	{
		xTaskCreate( vBLUEsat_BlinkTask, "BLUEsat_Blink", intgSTACK_SIZE, ( void * ) &( xTaskCheck[ sTask ] ), uxPriority, ( TaskHandle_t * ) NULL );
	}
}
/*-----------------------------------------------------------*/

static portTASK_FUNCTION( vBLUEsat_BlinkTask, pvParameters )
{
	short sError = pdFALSE;
	volatile BaseType_t *pxTaskHasExecuted;
	int x;
	int delay = 1000000;
	 
	// set to either PIO_register or GPIO_function
	short blink_method = GPIO_function;

	/* Set a pointer to the variable we are going to set to true each
	iteration.  This is also a good test of the parameter passing mechanism
	within each port. */
	pxTaskHasExecuted = ( volatile BaseType_t * ) pvParameters;
	
	
	// PIO_PB27 is the arduino due on-board LED, labeled as 'L' next to the green 'ON' LED
	
	if (blink_method == PIO_register) {
		// In order: enables, sets to output, disables pullup
		PIOB->PIO_PER	= PIO_PB27;
		PIOB->PIO_OER	= PIO_PB27;
		PIOB->PIO_PUDR	= PIO_PB27;
		
		while (1) {
			// Set Output Data Register 
			PIOB->PIO_SODR = PIO_PB27;
			for (x=0;x<delay;x++);
			// Clear Output Data Register
			PIOB->PIO_CODR = PIO_PB27;
			for (x=0;x<delay;x++);
		}
		
	} else if (blink_method == GPIO_function) {
		gpio_configure_pin(PIO_PB27_IDX, (PIO_TYPE_PIO_OUTPUT_0 | PIO_DEFAULT));
		
		while (1) {
			for (x=0;x<delay;x++);
			pio_toggle_pin(PIO_PB27_IDX);
		}
	} else {
		
		// shouldn't get to here
	
		/* Keep performing a calculation and checking the result against a constant. */
		for( ;; )
		{		
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
}