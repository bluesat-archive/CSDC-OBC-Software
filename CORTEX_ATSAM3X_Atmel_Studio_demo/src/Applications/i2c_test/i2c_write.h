/*
 * i2c_write.h
 *
 * Created: 22/08/2015 2:09:00 PM
 *  Author: David Sicong Zhou
 */ 


#ifndef I2C_WRITE_H_
#define I2C_WRITE_H_

	#include "i2c_config.h"

	/* FreeRTOS+IO includes. */
	#include "FreeRTOS_IO.h"

	/* The size of the buffer to read and write. is this unsafe? */
	#define BUFFER_SIZE		200

	/* The buffer itself. */
	const int8_t writeBuffer[ 200 ] = { 0 };

	/* The Peripheral_Descriptor_t type is the FreeRTOS+IO equivalent of a
	descriptor. */
	void vWriteExample( Peripheral_Descriptor_t xOpenPort )
	{
	size_t xBytesTransferred;
	BaseType_t xReturn;

		/* This port is configured to use the zero copy Tx transfer mode, so the 
		write mutex must be obtained before starting a new write operation.  Wait 
		a maximum of 200ms for the mutex - this task will not consume any CPU time 
		while it is waiting. */
		xReturn = FreeRTOS_ioctl( xOpenPort, iocltOBTAIN_WRITE_MUTEX, ( void * ) ( 200 / portTICK_PERIOD_MS ) );

		if( xReturn != pdFAIL )
		{
			/* The write mutex was obtained, so it is safe to perform a write.  This
			writes BUFFER_SIZE bytes from writeBuffer to the peripheral. */
			xBytesTransferred = FreeRTOS_write( xOpenPort, writeBuffer, BUFFER_SIZE );

			/* The actual peripheral transmission is performed by an interrupt, so, 
			in the particular case of using a zero copy transfer, xBytesTransferred 
			will be either 0, if the transfer could not be started, or equal to 
			BUFFER_SIZE.  Note however, that the interrupt may still be in the process 
			of actually transmitting the data, even though the function has returned.  
			The actual transmission of data will have completed when the mutex is
			available again. */
		}
	}

#endif /* I2C_WRITE_H_ */