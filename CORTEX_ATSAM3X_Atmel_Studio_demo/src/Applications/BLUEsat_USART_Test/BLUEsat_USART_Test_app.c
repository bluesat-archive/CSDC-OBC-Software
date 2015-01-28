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
#include "BLUEsat_USART_Test_app.h"

/* Atmel library includes. */
#include <asf.h>

/* BLUEsat library includes. */
#include <BLUEsat_library_drv.h>

/* The constants used in the calculation. */
#define intgSTACK_SIZE			configMINIMAL_STACK_SIZE

/* As this is the minimal version, we will only create one task. */
#define intgNUMBER_OF_TASKS		( 1 )

/* The settings for USART */

/* Misc defines. */
#define serINVALID_QUEUE				( ( QueueHandle_t ) 0 )
#define serNO_BLOCK						( ( TickType_t ) 0 )
#define serPMC_USART_ID					( BOARD_ID_USART )

/* The USART supported by this file. */
#define serUSART_PORT					( USART0 )
#define serUSART_IRQ					( USART0_IRQn )

/* Baud rate used by the comtest tasks. */
#define mainCOM_TEST_BAUD_RATE				( 115200 )

/* Every bit in the interrupt mask. */
#define serMASK_ALL_INTERRUPTS			( 0xffffffffUL )

/* The task function. */
static portTASK_FUNCTION_PROTO( vBLUEsat_USART_TestTask, pvParameters );

/* Variables that are set to true within the calculation task to indicate
that the task is still executing.  The check task sets the variable back to
false, flagging an error if the variable is still false the next time it
is called. */
static volatile BaseType_t xTaskCheck[ intgNUMBER_OF_TASKS ] = { ( BaseType_t ) pdFALSE };

/*-----------------------------------------------------------*/

void vStartBLUEsat_USART_TestTasks( UBaseType_t uxPriority )
{
	short sTask;
	
	for( sTask = 0; sTask < intgNUMBER_OF_TASKS; sTask++ )
	{
		xTaskCreate( vBLUEsat_USART_TestTask, "BLUEsat_USART_Test", intgSTACK_SIZE, ( void * ) &( xTaskCheck[ sTask ] ), uxPriority, ( TaskHandle_t * ) NULL );
	}
}
/*-----------------------------------------------------------*/

static portTASK_FUNCTION( vBLUEsat_USART_TestTask, pvParameters )
{
	short sError = pdFALSE;
	volatile BaseType_t *pxTaskHasExecuted;

	/* Set a pointer to the variable we are going to set to true each
	iteration.  This is also a good test of the parameter passing mechanism
	within each port. */
	pxTaskHasExecuted = ( volatile BaseType_t * ) pvParameters;
	
	// send
	char String[] = "Hello World\n\r- Serial Driver Test\n\r\n\r";
	
	SystemInit();
	
	configure_uart();
	
	int x;
	int y;
	
	/* Keep performing a calculation and checking the result against a constant. */
	for( ;; )
	{
		for (x = 0; String[x] != '\0'; x++) {
			uart_putchar(String[x]);
			for (y=0;y<1000;y++);
		}
		for (x=0;x<1000000;x++);
		
		
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