

#include <stdlib.h>

/* Scheduler include files. */
#include "FreeRTOS.h"
#include "task.h"
#include "bluesat_blinky.h"
/*
static portTASK_FUNCTION_PROTO( vLed51, pvParameters );
static volatile BaseType_t xTaskCheck[ intgNUMBER_OF_TASKS ] = { ( BaseType_t ) pdFALSE };
	int x =1 ;
	uint32_t test = PIO_PB14_IDX;
	gpio_configure_pin(test, (PIO_TYPE_PIO_OUTPUT_0 | PIO_DEFAULT));
	uint32_t test = PIO_PB14_IDX;
	pio_toggle_pin(test);
	*/
	
/* As this is the minimal version, we will only create one task. */
#define intgNUMBER_OF_TASKS		( 1 )
	
#define intgSTACK_SIZE			configMINIMAL_STACK_SIZE
	
	
void vStartBluesat_BlinkyMathTasks( UBaseType_t uxPriority )
{
short sTask;

	for( sTask = 0; sTask < intgNUMBER_OF_TASKS; sTask++ )
	{
		xTaskCreate( vCompeteingBluesat_BlinkyTask, "Bluesat_Blinky", intgSTACK_SIZE, ( void * ) &( xTaskCheck[ sTask ] ), uxPriority, ( TaskHandle_t * ) NULL );
	}
}
/*-----------------------------------------------------------*/

static portTASK_FUNCTION( vCompeteingBluesat_BlinkyTask, pvParameters )
{
/* These variables are all effectively set to constants so they are volatile to
ensure the compiler does not just get rid of them. */
volatile long lValue;
short sError = pdFALSE;
volatile BaseType_t *pxTaskHasExecuted;

	/* Set a pointer to the variable we are going to set to true each
	iteration.  This is also a good test of the parameter passing mechanism
	within each port. */
	pxTaskHasExecuted = ( volatile BaseType_t * ) pvParameters;

	/* Keep performing a calculation and checking the result against a constant. */
	for( ;; )
	{
		static portTASK_FUNCTION_PROTO( vLed51, pvParameters );
		static volatile BaseType_t xTaskCheck[ intgNUMBER_OF_TASKS ] = { ( BaseType_t ) pdFALSE };
		int x =1 ;
		uint32_t test = PIO_PB14_IDX;
		gpio_configure_pin(test, (PIO_TYPE_PIO_OUTPUT_0 | PIO_DEFAULT));
		uint32_t test = PIO_PB14_IDX;
		pio_toggle_pin(test);
		
		
		
		if( 0 ) // there will never be errors
		{
			sError = pdTRUE;
		}

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
BaseType_t xAreBluesat_BlinkyTaskStillRunning( void )
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

