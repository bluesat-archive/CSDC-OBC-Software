/*
 * cc1120_drv.c
 *
 * Created: 14-02-2015 4:59:17 PM
 *  Author: Blue
 */
#include <asf.h>
#include <comms_uart_drv.h>
#include <comms_spi_drv.h>
#include "cc1120_drv.h"
#include "comms_drv.h"

uint32_t cc1120_write (uint16_t data, uint8_t last) {
	return spi_write(SPI0, data, SPI_DEVICE_CC1120, last);
	
}
uint32_t cc1120_read (uint16_t *data) {
	uint8_t device_id = SPI_DEVICE_CC1120;
	return spi_read(SPI0, data, &device_id);
	
}

uint32_t cc1120_reset () {
	
	// check to see SO go high, and then low again. Keep CSn low. See 3.2.2 of User Guide	
	
	// uint16_t spi_read_data = 0;
	uint32_t timeout = 500;
	cc1120_write(CC1120_SRES, 0);
	uint32_t test = pio_get(PIOA, PIO_PA25A_SPI0_MISO, 0);
	
	test = test;
	while (pio_get(PIOA, PIO_PA25A_SPI0_MISO, 0)) {
		cc1120_write(CC1120_SNOP, 1);
		if (timeout-- <= 0) {
			// timed out, failed to reset quickly
			return 0;
		} 
	}
	
	return 1;
}