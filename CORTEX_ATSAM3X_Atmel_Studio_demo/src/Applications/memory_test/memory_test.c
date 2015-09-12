/*
 * cc1120_test_app.c
 *
 * Created: 14-02-2015 5:22:38 PM
 *  Author: Blue
 */ 
/* Library Includes */ 
#include <asf.h>
#include <m25p32_drv.h>

/* FreeRTOS Includes */
#include "FreeRTOS.h"
#include "task.h"



/* Other Includes */
#include "memory_test.h"

#define intgSTACK_SIZE			configMINIMAL_STACK_SIZE				// number of variables available for this task
#define intgNUMBER_OF_TASKS		( 1 )									// 

static portTASK_FUNCTION_PROTO( vm25p32_TestTask, pvParameters );		// The task function.


/* Variables that are set to true within the calculation task to indicate
that the task is still executing.  The check task sets the variable back to
false, flagging an error if the variable is still false the next time it
is called. */
static volatile BaseType_t xTaskCheck[ intgNUMBER_OF_TASKS ] = { ( BaseType_t ) pdFALSE };

void vStartm25p32_testTasks( UBaseType_t uxPriority )
{
	short sTask;
	
	for( sTask = 0; sTask < intgNUMBER_OF_TASKS; sTask++ )
	{
		xTaskCreate( vm25p32_TestTask, "m25p32_Test", intgSTACK_SIZE, ( void * ) &( xTaskCheck[ sTask ] ), uxPriority, ( TaskHandle_t * ) NULL );
	}
}
/*-----------------------------------------------------------*/

static portTASK_FUNCTION( vm25p32_TestTask, pvParameters ) 
{
	pvParameters = pvParameters;
		
	for (;;) {
        
        /*
         *  Sends a burst of data to write to the registers and tries to read
         *  it back.                                                            
         */
        m25p32_read_write_test();
		// CC1120_read_test();
	}
}

void m25p32_read_write_test(){
    uint8_t message[10] = "aaaa  aaaa";
    uint8_t recieved[10] = "";
    uint8_t dummy[] = "<>",newLine[] = "\n\r";
    uint32_t address = 0x0000aabb;
    m25p32PageWrite(message,address,1);
    m25p32PageRead(recieved,address,1);
    usart_serial_write_packet((usart_if)UART,message,10);
    usart_serial_write_packet((usart_if)UART,dummy,2);
    usart_serial_write_packet((usart_if)UART,recieved,1);
    usart_serial_write_packet((usart_if)UART,newLine,2);
    
    
}