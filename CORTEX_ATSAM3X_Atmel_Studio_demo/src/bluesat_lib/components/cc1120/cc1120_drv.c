/*
 * cc1120_drv.c
 *
 * Created: 14-02-2015 4:59:17 PM
 *  Author: Blue
 */
#include <asf.h>
#include <spi_master.h>
#include "cc1120_drv.h"
#include "bluesat_conf.h"

uint32_t cc1120_transmit (uint8_t data, uint8_t last) {
    last = last;
    spi_select_device(SPI0, SPI_Device_CC1120);
	spi_write_packet(SPI0, (uint8_t*)&data, 1);
    spi_deselect_device(SPI0, SPI_Device_CC1120);
    return 1;
}
uint32_t cc1120_receive (uint8_t *data) {    
    spi_select_device(SPI0, SPI_Device_CC1120);
	spi_read_packet(SPI0, data, 1);
    spi_deselect_device(SPI0, SPI_Device_CC1120);
    return 1;
}

uint32_t cc1120_reset () {
	
	// check to see SO go high, and then low again. Keep CSn low. See 3.2.2 of User Guide	
	
	// uint16_t spi_read_data = 0;
	uint32_t timeout = 500;
	cc1120_transmit(CC1120_SRES, 0);
	uint32_t test = pio_get(PIOA, PIO_PA25A_SPI0_MISO, 0);
	
	test = test;
	while (pio_get(PIOA, PIO_PA25A_SPI0_MISO, 0)) {
		cc1120_transmit(CC1120_SNOP, 1);
		if (timeout-- <= 0) {
			// timed out, failed to reset quickly
			return 0;
		} 
	}
	
	return 1;
}

void cc1120_read_register_address_space(uint8_t *data_buffer) {
	uint8_t start_burst_address = CC1120_IOCFG3;
	uint32_t burst_iterations = CC1120_PKT_LEN - CC1120_IOCFG3;		// ranges the whole register space
	cc1120_read_burst_register(start_burst_address, data_buffer, burst_iterations);
}

// Single Register Access
void cc1120_read_single(uint8_t start_address, uint8_t *data_buffer) {
	cc1120_transmit(start_address | CC1120_READ | CC1120_SINGLE_MODE, 0);
	cc1120_receive(data_buffer);
	//cc1120_transmit(CC1120_SNOP, 1);
}
void cc1120_write_single(uint8_t start_address, uint8_t *data_buffer) {
	cc1120_transmit(start_address | CC1120_WRITE | CC1120_SINGLE_MODE, 0);
	cc1120_transmit(*data_buffer, 1);
}

// Burst Register access
void cc1120_read_burst_register (uint8_t start_address, uint8_t *data_buffer, uint32_t iterations) {
	uint8_t spi_instance = 0001;
	uint8_t *current_buffer = data_buffer;
	
	cc1120_transmit(start_address | CC1120_READ | CC1120_BURST_MODE, 0);
    
	for (uint32_t y = 0; y < iterations-1; y++) {
		cc1120_transmit(CC1120_SNOP, 0);
		spi_read(SPI0, (uint16_t *)current_buffer, &spi_instance);
		current_buffer++;
	}
    
	cc1120_transmit(CC1120_SNOP, 1);
}
void cc1120_write_burst_register (uint8_t start_address, uint8_t *data_buffer, uint32_t iterations) { //works
	cc1120_transmit(start_address | CC1120_WRITE | CC1120_BURST_MODE, 0);
	for (uint32_t y = 0; y < iterations-1; y++) {
		cc1120_transmit(data_buffer[y], 0);
	}
	cc1120_transmit(data_buffer[iterations-1], 1);
}

// Burst Extended Register Access
void cc1120_read_single_extended(uint8_t address, uint8_t *data_buffer) {
	cc1120_transmit(CC1120_READ | CC1120_SINGLE_MODE | CC1120_EXTENDED, 0);
	cc1120_transmit(address, 1);
	address = address;
	data_buffer = data_buffer;
}
void cc1120_write_single_extended(uint8_t address, uint8_t *data_buffer) {
	cc1120_transmit(CC1120_WRITE | CC1120_SINGLE_MODE | CC1120_EXTENDED, 0);
	cc1120_transmit(address, 1);
	address = address;
	data_buffer = data_buffer;
}

// Burst Extended Register Access
void cc1120_read_burst_extended(uint8_t start_address, uint8_t *data_buffer) {
	cc1120_transmit(CC1120_READ | CC1120_BURST_MODE | CC1120_EXTENDED, 0);
	cc1120_transmit(start_address, 1);
	start_address = start_address;
	data_buffer = data_buffer;
}
void cc1120_write_burst_extended(uint8_t start_address, uint8_t *data_buffer) {
	cc1120_transmit(CC1120_WRITE | CC1120_BURST_MODE | CC1120_EXTENDED, 0);
	cc1120_transmit(start_address, 1);
	start_address = start_address;
	data_buffer = data_buffer;
}
/*
// Command Strobes
void reset_chip() {
	cc1120_transmit(CC1120_SRES, 1);
}
void toggle_frequency_synthesizer() {
	cc1120_transmit(CC1120_SFSTXON, 1);
}
void XOFF_on_CSn_deassert() {
	cc1120_transmit(CC1120_SXOFF, 1);
}
void reset_chip() {
	cc1120_transmit(CC1120_SCAL, 1);
}
void reset_chip() {
	cc1120_transmit(CC1120_SRX, 1);
}
void reset_chip() {
	cc1120_transmit(CC1120_STX, 1);
}
void reset_chip() {
	cc1120_transmit(CC1120_SIDLE, 1);
}
void reset_chip() {
	cc1120_transmit(CC1120_AFC, 1);
}
void reset_chip() {
	cc1120_transmit(CC1120_SWOR, 1);
}
void reset_chip() {
	cc1120_transmit(CC1120_SPWD, 1);
}
void reset_chip() {
	cc1120_transmit(CC1120_SFRX, 1);
}
void reset_chip() {
	cc1120_transmit(CC1120_SFTX, 1);
}
void reset_chip() {
	cc1120_transmit(CC1120_SWORRST, 1);
}
void reset_chip() {
	cc1120_transmit(CC1120_SNOP, 1);
}
*/
// Direct FIF0 Access



// Standard FIFO Access




