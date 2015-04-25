/*
 * cc1120_drv.c
 *
 * Created: 14-02-2015 4:59:17 PM
 *  Author: Blue
 */
#include <asf.h>
#include <comms_spi_drv.h>
#include "cc1120_drv.h"
// #include "comms_drv.h"

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
    // holds the SPI line open while receiving data
    cc1120_cs_SNOP();
    
    // 
    read_top_spi_buffer(data);
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
    // Sends a header byte to read a single byte to a single address
	cc1120_transmit(address | CC1120_READ | CC1120_SINGLE_MODE, 0);
    
    // Reads the data from the the address specified above
	cc1120_receive(data);
    
    // Closes the SPI connection
	cc1120_send_last_byte();		
}
void cc1120_write_single(uint8_t address, uint8_t *data) {
    // Sends a header byte to write a single byte to a single address
	cc1120_transmit(address | CC1120_WRITE | CC1120_SINGLE_MODE, 0);
    
	// Writes the data to the the address specified above
	cc1120_transmit(*data, 1);
}

/*
 *		EXTENDED REGISTER ACCESS - BURST 
 *
 *		Size: 2 + n bytes
 *			- header
 *			- extended address
 *			- data * n
 */
void cc1120_read_burst_register (uint8_t address, uint8_t *data, uint32_t iterations) {
    
	uint8_t *current_buffer = data;
	
    // Sends a header byte to read a stream of bytes starting from a given address
	cc1120_transmit(address | CC1120_READ | CC1120_BURST_MODE, 0);
    
    // Iterate over buffer
	for (uint32_t y = 0; y < iterations-1; y++) {        
        // saves the received data into buffer, and increments for next read
		cc1120_receive(++current_buffer);
        
		// current_buffer++;
	}
    
    // Closes the SPI connection
	cc1120_send_last_byte();	
}
void cc1120_write_burst_register (uint8_t address, uint8_t *data, uint32_t iterations) {
    
    // Sends a header byte to write a stream of bytes starting from a given address
	cc1120_transmit(address | CC1120_WRITE | CC1120_BURST_MODE, 0);
    
    // Iterate over buffer to transmit
	for (uint32_t y = 0; y < iterations-1; y++) {
        // Transmit data
		cc1120_transmit(data[y], 0);
	}
    
    // Transmits the last byte and ends communication with the board
	cc1120_transmit(data[iterations-1], 1);
}

/*
 *		EXTENDED REGISTER ACCESS - SINGLE 
 *
 *		Size: 3 bytes
 *			- header
 *			- extended address
 *			- data
 */
void cc1120_read_single_extended(uint8_t address, uint8_t *data) {
    
    // Sends a header byte to read a byte from an extended address
	cc1120_transmit(CC1120_READ | CC1120_SINGLE_MODE | CC1120_EXTENDED, 0);
    
    // Send the extended address to read from
	cc1120_transmit(address, 0);
	
    // Reads the most recent byte received into SPI
    cc1120_receive(data);
    
    // Finishes communication
	cc1120_send_last_byte();
}
void cc1120_write_single_extended(uint8_t address, uint8_t *data) {

    // Sends a header byte to write a byte to an extended address
	cc1120_transmit(CC1120_WRITE | CC1120_SINGLE_MODE | CC1120_EXTENDED, 0);
    
    // Send which extended address to write to
	cc1120_transmit(address, 0);
    
    // Write data to specified address
	cc1120_transmit(*data, 1);
}

/*
 *		EXTENDED REGISTER ACCESS - BURST 
 *
 *		Size: 2 + n bytes
 *			- header
 *			- extended address
 *			- data * n
 */
void cc1120_read_burst_extended(uint8_t address, uint8_t *data, uint32_t iterations) {
    
	uint8_t *current_buffer = data;
    
	// Sends a header byte to read a stream of bytes starting from an extended address
	cc1120_transmit(CC1120_READ | CC1120_BURST_MODE | CC1120_EXTENDED, 0);
    
    // Send the extended address to read from
	cc1120_transmit(address, 1);
    
    // Iterate over buffer to save data to
	for (uint32_t y = 0; y < iterations-1; y++) {        
        // save information into buffer
		cc1120_receive(++current_buffer);
	}
    
    // Finishes communication
    cc1120_send_last_byte();
}
void cc1120_write_burst_extended(uint8_t address, uint8_t *data, uint32_t iterations) {
        
    // Sends a header byte to write a stream of bytes starting from an extended address
	cc1120_transmit(CC1120_WRITE | CC1120_BURST_MODE | CC1120_EXTENDED, 0);
	
    // Send the extended address to write to
	cc1120_transmit(address, 0);
    
    // Iterate over data
	for (uint32_t y = 0; y < iterations-1; y++) {
		// Transmit data
        cc1120_transmit(data[y], 0);
	}
    
    // Send last byte
	cc1120_transmit(data[iterations-1], 1);
}

/*
 *		COMMAND STROBES
 *      
 *      Read the User Guide to know what these do
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
void cc1120_cs_SNOP() {
	cc1120_transmit(cc1120_SNOP, 1);
}

// Direct FIF0 Access

// Standard FIFO Access

/*
 *		EXTENDED FUNCTIONS
 */
void cc1120_read_register_address_space(uint8_t *data_buffer) {
    
    // Start of the configuration registers
	uint8_t start_burst_address = CC1120_IOCFG3;
    
    // Size of the memory for the configuration registers
	uint32_t burst_iterations = CC1120_PKT_LEN;
  
    // Read info
	cc1120_read_burst_register(start_burst_address, data_buffer, burst_iterations);
}

void cc1120_reset_to_bluesat_default() {
    
    // Will reset the chip and configure the board to BLUEsat's default
    cc1120_cs_reset_chip();
        
    // Start of the configuration registers
    uint8_t start_burst_address = CC1120_IOCFG3;
        
    // Size of the memory for the configuration registers
    uint32_t burst_iterations = CC1120_PKT_LEN;
    
    // 
    uint8_t configuration_registers[CC1120_PKT_LEN] = {
        0x00, // IOCFG3
        0x00, // IOCFG2
        0x00, // IOCFG1
        0x00, // IOCFG0
        0x00, // SYNC3
        0x00, // SYNC2
        0x00, // SYNC1
        0x00, // SYNC0
        0x00, // SYNC_CFG1
        0x00, // SYNC_CFG0
        0x00, // DEVIATION_M
        0x00, // MODCFG_DEV_E
        0x00, // DCFILT_CFG
        0x00, // PREAMBLE_CFG1
        0x00, // PREAMBLE_CFG0
        0x00, // FREQ_IF_CFG
        0x00, // IQIC
        0x00, // CHAN_BW
        0x00, // MDMCFG1
        0x00, // MDMCFG0
        0x00, // SYMBOL_RATE2
        0x00, // SYMBOL_RATE1
        0x00, // SYMBOL_RATE0
        0x00, // AGC_REF
        0x00, // AGC_CS_THR
        0x00, // AGC_GAIN_ADJUST
        0x00, // AGC_CFG3
        0x00, // AGC_CFG2
        0x00, // AGC_CFG1
        0x00, // AGC_CFG0
        0x00, // FIFO_CFG
        0x00, // DEV_ADDR
        0x00, // SETTLING_CFG
        0x00, // FS_CFG
        0x00, // WOR_CFG1
        0x00, // WOR_CFG0
        0x00, // WOR_EVENT0_MSB
        0x00, // WOR_EVENT0_LSB
        0x00, // PKT_CFG2
        0x00, // PKT_CFG1
        0x00, // PKT_CFG0
        0x00, // RFEND_CFG1
        0x00, // RFEND_CFG0
        0x00, // PA_CFG2
        0x00, // PA_CFG1
        0x00  // PKT_LEN
    };
    
    // cast type 
    uint8_t *buffer = (uint8_t*) configuration_registers;
    
    // Buffer with configuration information
    cc1120_write_burst_extended(start_burst_address, buffer, burst_iterations);
}


void cc1120_send_last_byte() {
    // This function exists to make it easier to read the code.
    // SPI will stay open until a last byte is sent, which tells the SPI
    // chip on the atmel to de-assert the CSn line and stop communication.
    // An easy way to do this is to just send a single SNOP command strobe, 
    // which does nothing but request a status byte from the cc1120 board.
    cc1120_cs_SNOP();
}