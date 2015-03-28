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

/*
 *		SPI TX/RX
 *		
 *		Wrapper commands for sending and receiving over SPI
 */
uint32_t cc1120_transmit (uint8_t data, uint8_t last) {
	return spi_write(SPI0, (uint16_t)data, SPI_DEVICE_CC1120, last);
}

// read needs to be updated - it does not work
uint32_t cc1120_receive (uint8_t *data) {
	// uint8_t device_id = SPI_DEVICE_CC1120;
	// read from the SPI buffer, and save into data
	*data = spi_buffer[spi_buffer_position];
	
	// do error checking or something
	return 0;
}

/*
 *		REGISTER ACCESS - SINGLE 
 *
 *		Size: 2 - infinite bytes
 *			- header
 *			- data
 */
void cc1120_read_single(uint8_t address, uint8_t *data) {
	cc1120_transmit(address | CC1120_READ | CC1120_SINGLE_MODE, 0);
	cc1120_receive(data);
	cc1120_SNOP();		// finishes communication
}
void cc1120_write_single(uint8_t address, uint8_t *data) {
	cc1120_transmit(address | CC1120_WRITE | CC1120_SINGLE_MODE, 0);
	cc1120_transmit(*data, 1);
}
void cc1120_read_burst_register (uint8_t address, uint8_t *data, uint32_t iterations) {
	uint8_t *current_buffer = data;
	
	cc1120_transmit(address | CC1120_READ | CC1120_BURST_MODE, 0);
	for (uint32_t y = 0; y < iterations-1; y++) {
		// cc1120_SNOP();	// removed until tested function works without this line
		cc1120_receive(current_buffer);
		current_buffer++;
	}
	cc1120_SNOP();
}
void cc1120_write_burst_register (uint8_t address, uint8_t *data, uint32_t iterations) { //works
	cc1120_transmit(address | CC1120_WRITE | CC1120_BURST_MODE, 0);
	for (uint32_t y = 0; y < iterations-1; y++) {
		cc1120_transmit(data[y], 0);
	}
	cc1120_transmit(data[iterations-1], 1);
}

/*
 *		EXTENDED REGISTER ACCESS - SINGLE 
 *
 *		Size: 3 - infinite bytes
 *			- header
 *			- extended address
 *			- data
 */
void cc1120_read_single_extended(uint8_t address, uint8_t *data) {
	cc1120_transmit(CC1120_READ | CC1120_SINGLE_MODE | CC1120_EXTENDED, 0);
	cc1120_transmit(address, 0);
	cc1120_receive(data);
	cc1120_SNOP();
}
void cc1120_write_single_extended(uint8_t address, uint8_t *data) {
	cc1120_transmit(CC1120_WRITE | CC1120_SINGLE_MODE | CC1120_EXTENDED, 0);
	cc1120_transmit(address, 0);
	cc1120_transmit(*data, 1);
}
void cc1120_read_burst_extended(uint8_t address, uint8_t *data, uint32_t iterations) {
	uint8_t *current_buffer = data;
	cc1120_transmit(CC1120_READ | CC1120_BURST_MODE | CC1120_EXTENDED, 0);
	cc1120_transmit(address, 1);
	for (uint32_t y = 0; y < iterations-1; y++) {
		cc1120_receive(current_buffer);
		current_buffer++;
	}
	cc1120_transmit(data[iterations-1], 1);
}
void cc1120_write_burst_extended(uint8_t address, uint8_t *data, uint32_t iterations) {
	cc1120_transmit(CC1120_WRITE | CC1120_BURST_MODE | CC1120_EXTENDED, 0);
	cc1120_transmit(address, 0);
	for (uint32_t y = 0; y < iterations-1; y++) {
		cc1120_transmit(data[y], 0);
	}
	cc1120_transmit(data[iterations-1], 1);
}

/*
 *		COMMAND STROBES
 *
 *		Size: 1 byte
 */
void cc1120_cs_reset_chip() {
	cc1120_transmit(CC1120_SRES, 1);
}
void cc1120_cs_toggle_frequency_synthesizer() {
	cc1120_transmit(CC1120_SFSTXON, 1);
}
void cc1120_cs_XOFF_on_CSn_deassert() {
	cc1120_transmit(CC1120_SXOFF, 1);
}
void cc1120_cs_calibrate_frequency_synthesizer() {
	cc1120_transmit(CC1120_SCAL, 1);
}
void cc1120_cs_enable_rx_mode() {
	cc1120_transmit(CC1120_SRX, 1);
}
void cc1120_cs_enable_tx_mode() {
	cc1120_transmit(CC1120_STX, 1);
}
void cc1120_cs_enable_idle_mode() {
	cc1120_transmit(CC1120_SIDLE, 1);
}
void cc1120_cs_automatic_frequency_compensation() {
	cc1120_transmit(CC1120_AFC, 1);
}
void cc1120_cs_start_auto_rx_polling() {
	cc1120_transmit(CC1120_SWOR, 1);
}
void cc1120_cs_enter_sleep_on_CSn_deassert() {
	cc1120_transmit(CC1120_SPWD, 1);
}
void cc1120_cs_flush_rx_fifo() {
	cc1120_transmit(CC1120_SFRX, 1);
}
void cc1120_cs_flush_tx_fifo() {
	cc1120_transmit(CC1120_SFTX, 1);
}
void cc1120_cs_reset_eWOR_timer() {
	cc1120_transmit(CC1120_SWORRST, 1);
}
void cc1120_SNOP() {
	cc1120_transmit(CC1120_SNOP, 1);
}

// Direct FIF0 Access



// Standard FIFO Access





/*
 *		WRAPPER COMMANDS
 *
 *		Size: 1 byte
 */
void cc1120_read_register_address_space(uint8_t *data_buffer) {
	uint8_t start_burst_address = CC1120_IOCFG3;
	uint32_t burst_iterations = CC1120_PKT_LEN - CC1120_IOCFG3;		// ranges the whole register space
	cc1120_read_burst_register(start_burst_address, data_buffer, burst_iterations);
}

