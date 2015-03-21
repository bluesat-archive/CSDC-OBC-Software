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

uint32_t cc1120_transmit (uint8_t data, uint8_t last) {
	return spi_write(SPI0, (uint16_t)data, SPI_DEVICE_CC1120, last);
}
uint32_t cc1120_receive (uint8_t *data) {
	uint8_t device_id = SPI_DEVICE_CC1120;
	return spi_read(SPI0, (uint16_t*)data, &device_id);
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
	
	uint8_t *current_buffer = data_buffer;
	
	cc1120_transmit(start_address | CC1120_READ | CC1120_BURST_MODE, 0);
	for (uint32_t y = 0; y < iterations-1; y++) {
		cc1120_transmit(CC1120_SNOP, 0);
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
void calibrate_frequency_synthesizer() {
	cc1120_transmit(CC1120_SCAL, 1);
}
void enable_rx_mode() {
	cc1120_transmit(CC1120_SRX, 1);
}
void enable_tx_mode() {
	cc1120_transmit(CC1120_STX, 1);
}
void enable_idle_mode() {
	cc1120_transmit(CC1120_SIDLE, 1);
}
void automatic_frequency_compensation() {
	// see FREQOFF1
	// only becomes active once in rx/tx mode
	cc1120_transmit(CC1120_AFC, 1);
}
void start_auto_rx_polling() {
	cc1120_transmit(CC1120_SWOR, 1);
}
void enter_sleep_on_CSn_deassert() {
	cc1120_transmit(CC1120_SPWD, 1);
}
void flush_rx_fifo() {
	cc1120_transmit(CC1120_SFRX, 1);
}
void flush_tx_fifo() {
	cc1120_transmit(CC1120_SFTX, 1);
}
void reset_eWOR_timer() {
	// Enhanced Wake on Radio
	cc1120_transmit(CC1120_SWORRST, 1);
}
void get_chip_status_byte() {
	cc1120_transmit(CC1120_SNOP, 1);
}

// Direct FIF0 Access



// Standard FIFO Access




