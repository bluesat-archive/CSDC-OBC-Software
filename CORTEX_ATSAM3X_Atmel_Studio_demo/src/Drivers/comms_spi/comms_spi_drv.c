/*
 * BLUEsat_spi_drv.c
 *
 * Created: 31-01-2015 1:14:04 PM
 *  Author: Blue
 */ 

#include <stdio.h>
#include <stdlib.h>

#include <asf.h>
#include <comms_spi_drv.h>

#define POLARITY_FLAG							0						// Flags for the clock polarity and phase
#define BAUD_RATE								115200					// Baud rate
#define CONFIG_SPI_MASTER_DELAY_BS				0						// Delay before SPCK (in number of MCK clocks).
#define CONFIG_SPI_MASTER_DELAY_BCS				0						// Delay between chip selects (in number of MCK clocks). 
#define CONFIG_SPI_MASTER_DELAY_BCT				0						// Delay between consecutive transfers (in number of MCK clocks).
#define CONFIG_SPI_MASTER_BITS_PER_TRANSFER		SPI_CSR_BITS_8_BIT		// Size of data transfer

#define INTERRUPT_FLAGS							SPI_IDR_RDRF

void configure_spi(){
	pio_set_peripheral(PIOA, PIO_PERIPH_A, PIO_PA25A_SPI0_MISO);		// enables MISO pin
	pio_set_peripheral(PIOA, PIO_PERIPH_A, PIO_PA26A_SPI0_MOSI);		// enables MOSI pin
	pio_set_peripheral(PIOA, PIO_PERIPH_A, PIO_PA27A_SPI0_SPCK);		// enables Clock pin
 	pio_set_peripheral(PIOA, PIO_PERIPH_A, PIO_PA28A_SPI0_NPCS0);		// enables slave select 0 on pin 10
	pio_set_peripheral(PIOA, PIO_PERIPH_A, PIO_PA29A_SPI0_NPCS1);		// enables slave select 1 on pin 4
	
	spi_master_configure(SPI0);											// setup arduino as SPI master
	spi_enable(SPI0);													// enables SPI
	
	spi_master_configure_device(SPI0, SPI_DEVICE_0, POLARITY_FLAG, BAUD_RATE);		// configures SPI for slave on pin 10
	spi_master_configure_device(SPI0, SPI_DEVICE_1, POLARITY_FLAG, BAUD_RATE);		// configures SPI for slave on pin 4 (CC1120)
	
	spi_enable_interrupt(SPI0, INTERRUPT_FLAGS);						// Set what what will trigger the SPI interrupt
	NVIC_EnableIRQ(SPI0_IRQn);											// Enables the SPI handler in the interrupt controller thing
}

void spi_master_configure(Spi *p_spi)
{
	spi_enable_clock(p_spi);
	spi_reset(p_spi);
	spi_set_master_mode(p_spi);
	spi_disable_mode_fault_detect(p_spi);
	spi_disable_loopback(p_spi);
	spi_set_variable_peripheral_select(p_spi);
	spi_disable_peripheral_select_decode(p_spi);
	spi_set_delay_between_chip_select(p_spi, CONFIG_SPI_MASTER_DELAY_BCS);
}

void spi_master_configure_device(Spi *p_spi, uint32_t device_id, uint32_t flags, uint32_t baud_rate) {
	spi_set_transfer_delay(p_spi, device_id, CONFIG_SPI_MASTER_DELAY_BS, CONFIG_SPI_MASTER_DELAY_BCT);
	spi_set_bits_per_transfer(p_spi, device_id, CONFIG_SPI_MASTER_BITS_PER_TRANSFER);
	spi_set_baudrate_div(p_spi, device_id, spi_calc_baudrate_div(baud_rate, sysclk_get_cpu_hz()));
	spi_configure_cs_behavior(p_spi, device_id, SPI_CS_KEEP_LOW);
	spi_set_clock_polarity(p_spi, device_id, flags >> 1);
	spi_set_clock_phase(p_spi, device_id, ((flags & 0x1) ^ 0x1));
}

void BLUEsat_spi_write_string (char* c, uint32_t peripheral_select) {
	for (uint32_t pos = 0; c[pos] != '\0'; pos++) {
		spi_write(SPI0, c[pos], peripheral_select, c[pos+1] == '\0');
	}
}

void write_to_spi_buffer(uint8_t data) {
	spi_buffer[spi_buffer_position] = data;
	spi_buffer_position++;
	
	if (spi_buffer_position > SPI_BUFFER_SIZE) {
		spi_buffer_position = 0;
	}
}

void read_top_spi_buffer(uint8_t *data) {
    *data = spi_buffer[spi_buffer_position];
}

void SPI0_Handler (void) {
    
    // temp solution, not sure exactly how we'll design SPI 
    // receive buffers with multiple slaves
	uint8_t spi_select = 2;
	uint16_t read_data = 0;	
    
    // Reads from the SPI receive register
	spi_read(SPI0, &read_data, &spi_select);
    
    // Write into the buffer
	write_to_spi_buffer(read_data);
	
	// commenting this out until we 
    // figure out if we need it
	// NVIC_ClearPendingIRQ(SPI0_IRQn);
}