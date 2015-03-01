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

uint32_t cc1120_write (uint8_t address){
	address = address;
	return 1;
}
uint32_t cc1120_read (uint8_t address) {
	address = address;
	return 1;
}

uint32_t cc1120_reset () {
	uint16_t spi_read_data = 0;
	
	uint32_t spi_flag_write = spi_write(SPI0, CC1120_SRES, 0, 0);	
	spi_write(SPI0, CC1120_SNOP, 0, 0);	
	spi_write(SPI0, CC1120_SNOP, 0, 0);
	uint32_t spi_flag_read  = spi_read(SPI0, &spi_read_data, 0);
	
	// check to see SO go high, and then low again. Keep CSn low. See 3.2.2 of User Guide	
	spi_write(SPI0, CC1120_SNOP, 0, 1);
	
	if ((spi_flag_write || spi_flag_read) == SPI_ERROR_TIMEOUT) {
		uart_print_string_to_serial("CC1120 reset failed; spi_write failed.\n\r");
		return 0;
	} else {
		uart_print_string_to_serial("Success.\n\r");
	}
	
	return 1;
}