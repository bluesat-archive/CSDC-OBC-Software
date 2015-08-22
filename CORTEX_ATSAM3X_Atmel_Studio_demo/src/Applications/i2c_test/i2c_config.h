/*
 * i2c_config.h
 *
 * Created: 22/08/2015 2:09:34 PM
 *  Author: David Sicong Zhou
 */ 


#ifndef I2C_CONFIG_H_
#define I2C_CONFIG_H_

	/* FreeRTOS+IO includes. */
	#include "FreeRTOS_IO.h"

	Peripheral_Descriptor_t xOpenAndConfigureI2CPort( void )
	{
	/* The Peripheral_Descriptor_t type is the FreeRTOS+IO equivalent of a descriptor. */
	Peripheral_Descriptor_t xI2CPort;

		/* Open the I2C0 port, storing the returned value as the port's descriptor.
		The peripherals that are actually available to be opened depends on the board
		support package being used.  The second parameter is not currently used and can
		be set to anything, although, for future compatibility, it is recommended that
		it is set to NULL.  By default, the port is opened with its transfer mode set 
		to polling. */
		xI2CPort = FreeRTOS_open( "/I2C0/", NULL );

		/* FreeRTOS_open() returns NULL when the open operation cannot complete.  Check 
		the return value is not NULL. */
		configASSERT( xI2CPort );

		/* Configure the port for zero copy Tx.  The third parameter is not used in 
		this case. */
		FreeRTOS_ioctl( xI2CPort, iocltUSE_ZERO_COPY_TX, NULL );

		/* Configure the same port for circular buffer Rx.  This time the third
		parameter is used, and defines the buffer size.
		FreeRTOS_ioctl( xI2CPort, iocltUSE_CIRCULAR_BUFFER_RX, ( void * ) 100 );

		/* Set the read timeout to 200ms.  This is the maximum time a FreeRTOS_read() 
		call will wait for the requested amount of data to be available.  As the port 
		is configured to use interrupts, the task performing the read is in the 
		Blocked state while the operation is in progress, so not consuming any CPU time. 
		An interrupt driven zero copy write does not require a timeout to be set. */
		FreeRTOS_ioctl( xI2CPort, iocltSET_RX_TIMEOUT, ( void * ) ( 200 / portTICK_PERIOD_MS ) );

		/* Set the I2C clock frequency to 400000. */
		FreeRTOS_ioctl( xI2CPort, iocltSET_SPEED, ( void * ) 400000 );

		/* Set the I2C slave address to 50. */
		FreeRTOS_ioctl( xI2CPort, iocltSET_I2C_SLAVE_ADDRESS, ( void * ) 50 );

		/* Return a handle to the open port, which can now be used in FreeRTOS_read()
		and FreeRTOS_write() calls. */
		return xI2CPort;
	}

#endif /* I2C_CONFIG_H_ */