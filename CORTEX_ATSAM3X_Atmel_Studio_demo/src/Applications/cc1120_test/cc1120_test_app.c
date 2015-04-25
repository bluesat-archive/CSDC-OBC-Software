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
#include <comms_uart_drv.h>
#include <comms_drv.h>

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
	
	uint16_t send_byte = 0;
	send_byte = send_byte;
	uint16_t read_byte = 0;
	read_byte = read_byte;
	
	//char buf[20];
	//buf[19] = 0;
		
	for (;;) {
		// cc1200_test_one();
		cc1120_burst_test();
		// CC1120_read_test();
	}
}

void cc1120_test_one() {
	uint16_t send_byte = 0;
	
	cc1120_transmit(CC1120_SRES, 1);				// reset chip
	for(int y = 0; y<1000; y++);
		
	send_byte = CC1120_IOCFG3 | CC1120_READ;	// send header byte
	cc1120_transmit(send_byte, 0);
	send_byte = cc1120_SNOP;					// read byte 
	cc1120_transmit(send_byte, 1);
		
	for(int y = 0; y<1000; y++);
		
	send_byte = CC1120_IOCFG3 | CC1120_WRITE;	// send header byte
	cc1120_transmit(send_byte, 0);
	send_byte = 0x55;							// write byte
	cc1120_transmit(send_byte, 1);
		
	for(int y = 0; y<1000; y++);
		
	send_byte = CC1120_IOCFG3 | CC1120_READ;	// send header byte 
	cc1120_transmit(send_byte, 0);
	send_byte = cc1120_SNOP;					// read byte 
	cc1120_transmit(send_byte, 1);
		
	for(int y = 0; y<50000; y++);
}

void cc1120_burst_test() {	
	
	cc1120_transmit(CC1120_SRES, 1);		// reset the chip
	for (int y = 0; y<1000; y++);			// delay
	
	uint8_t data_buf_tx[4];
	data_buf_tx[0] = 0x55;
	data_buf_tx[1] = 0x81;
	data_buf_tx[2] = 0x55;
	data_buf_tx[3] = 0x81;
	
	uint8_t data_buf_rx[4];
	data_buf_rx[0] = 'z';
	data_buf_rx[1] = 'z';
	data_buf_rx[2] = 'z';
	data_buf_rx[3] = 'z';
	
	cc1120_write_burst_register(CC1120_IOCFG3, data_buf_tx, 4);		// write buffer
	
	for (int y = 0; y<1000; y++);						// delay
	
	cc1120_read_burst_register(CC1120_IOCFG3, data_buf_rx, 4);	// read four bits
	
	for (int y = 0; y<10000; y++);			// large delay
}

void CC1120_read_test() {
	
	uint8_t send_byte;
	uint16_t new_data = 0;
	uint8_t spi_p = 0001;
	
	for (int y = 0; y<10000; y++);
	
	cc1120_transmit(CC1120_SRES, 1);				// reset chip
	for(int y = 0; y<1000; y++);
	
	send_byte = CC1120_IOCFG3 | CC1120_WRITE;	// send header byte
	cc1120_transmit(send_byte, 0);
	send_byte = 0x55;					// read byte 
	cc1120_transmit(send_byte, 1);				// reset chip
	for(int y = 0; y<1000; y++);
	
	send_byte = CC1120_IOCFG3 | CC1120_READ;	// send header byte
	cc1120_transmit(send_byte, 0);
	send_byte = cc1120_SNOP;
	
	while(spi_read(SPI0, &new_data, &spi_p));
	
	cc1120_transmit(send_byte, 1);
	
//	cc1120_receive(new_data);				
	for (int y = 0; y<1000; y++);			// delay
}

/*
int i2a(char *s, int n){
	div_t qr;
	int pos;

	if(n == 0) {
		return 0;
	}

	qr = div(n, 10);
	
	pos = i2a(s, qr.quot);
	
	s[pos] = qr.rem + '0';
	
	return pos + 1;
}

char* my_itoa(char *output_buff, int num){
	
	char *p = output_buff;
	
	// check if it is even or odd
	if (num < 0) {
		*p++ = '-';
		num *= -1;
	} else if (num == 0)	
	*p++ = '0';
	p[i2a(p, num)]='\0';
	
	return output_buff;
}
*/