/*
 * BLUEsat_spi_drv.c
 *
 * Created: 31-01-2015 1:14:04 PM
 *  Author: Blue
 */ 

#include <stdio.h>
#include <stdlib.h>

#include <asf.h>
#include "comms_spi_drv.h"

// macro to select slave peripheral. 'chip_sel_id' is 0, 1, 2, or 3.
#define spi_get_pcs(chip_sel_id)				((~(1u<<(chip_sel_id)))&0xF)

#define DEFAULT_CHIP_ID							0						// Default peripheral chip
#define POLARITY_FLAG							0						// Flags for the clock polarity and phase
#define BAUD_RATE								9600					// Baud rate
#define CONFIG_SPI_MASTER_DELAY_BS				0						// Delay before SPCK (in number of MCK clocks).
#define CONFIG_SPI_MASTER_DELAY_BCS				0						// Delay between chip selects (in number of MCK clocks). 
#define CONFIG_SPI_MASTER_DELAY_BCT				0						// Delay between consecutive transfers (in number of MCK clocks).
#define CONFIG_SPI_MASTER_BITS_PER_TRANSFER		SPI_CSR_BITS_8_BIT		// Size of data transfer

void configure_spi(){
	pio_set_peripheral(PIOA, PIO_PERIPH_A, PIO_PA25A_SPI0_MISO);		// enables MISO pin
	pio_set_peripheral(PIOA, PIO_PERIPH_A, PIO_PA26A_SPI0_MOSI);		// enables MOSI pin
	pio_set_peripheral(PIOA, PIO_PERIPH_A, PIO_PA27A_SPI0_SPCK);		// enables Clock pin
 	pio_set_peripheral(PIOA, PIO_PERIPH_A, PIO_PA28A_SPI0_NPCS0);		// enables slave select 0 on pin 10
	pio_set_peripheral(PIOA, PIO_PERIPH_A, PIO_PA29A_SPI0_NPCS1);		// enables slave select 1 on pin 4
	
	spi_master_configure(SPI0);										// setup arduino as SPI master
	spi_enable(SPI0);													// enables SPI
	
	spi_master_configure_device(SPI0, 0, POLARITY_FLAG, BAUD_RATE);	// configures SPI for slave on pin 10
	spi_master_configure_device(SPI0, 1, POLARITY_FLAG, BAUD_RATE);	// configures SPI for slave on pin 4
}

void spi_master_configure(Spi *p_spi)
{
	spi_enable_clock(p_spi);
	spi_reset(p_spi);
	spi_set_master_mode(p_spi);
	spi_disable_mode_fault_detect(p_spi);
	spi_disable_loopback(p_spi);
	spi_set_peripheral_chip_select_value(p_spi,	spi_get_pcs(DEFAULT_CHIP_ID));
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
		spi_write(SPI0, c[pos], spi_get_pcs(peripheral_select), c[pos+1] == '\0');
	}
}