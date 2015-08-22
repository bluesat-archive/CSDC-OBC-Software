/*
 * i2c_read.h
 *
 * Created: 22/08/2015 2:08:36 PM
 *  Author: David Sicong Zhou
 */ 


#ifndef I2C_READ_H_
#define I2C_READ_H_

	/* FreeRTOS+IO includes. */
	#include "FreeRTOS_IO.h"

	/* The size of the buffer into which data will be read. */
	#define BUFFER_SIZE        200

	/* The buffer itself. */
	const int8_t readBuffer[ 200 ] = { 0 };

	/* The Peripheral_Descriptor_t type is the FreeRTOS+IO equivalent of a
	descriptor. */
	void vReadExample( Peripheral_Descriptor_t xOpenPort )
	{
	size_t xBytesTransferred;

		/* The peripheral has already been opened and configured.  Read BUFFER_SIZE
		bytes into readBuffer. The syntax is the same here no matter which
		transfer mode is being used. */
		xBytesTransferred = FreeRTOS_read( xOpenPort, readBuffer, BUFFER_SIZE );

		/* xBytesTransferred will now hold the number of bytes read, which could be
		less than BUFFER_SIZE bytes if the configured read timeout expired before
		the requested amount of data was available. */
	}

#endif /* I2C_READ_H_ */