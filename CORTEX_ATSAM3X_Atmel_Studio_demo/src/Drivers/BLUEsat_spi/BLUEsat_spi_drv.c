/*
 * BLUEsat_spi_drv.c
 *
 * Created: 31-01-2015 1:14:04 PM
 *  Author: Blue
 */ 

#include <asf.h>

#include "BLUEsat_spi_drv.h"

// necessary for spi_set_peripheral_chip_select_value()
// See: http://asf.atmel.com/docs/latest/sam3x/html/group__sam__drivers__spi__group.html#ga71131f0a9772ac93eeb6af52a1101ecc
#define 	spi_get_pcs(chip_sel_id)   ((~(1u<<(chip_sel_id)))&0xF)
#define 	DEFAULT_CHIP_ID   0

// See: http://asf.atmel.com/docs/latest/sam3x/html/group__sam__drivers__spi__group.html#gab1a11f2d8808eb355c9b1e43be00da96
#define CONFIG_SPI_MASTER_DELAY_BCS   0

// in place of the spi_device struct in the example code
#define DEVICE_ID 0

// Set polarity and phase flags
// polarity: http://asf.atmel.com/docs/latest/sam3x/html/group__sam__drivers__spi__group.html#ga5047dc4debab95407fa5b27360addc39
// phase: http://asf.atmel.com/docs/latest/sam3x/html/group__sam__drivers__spi__group.html#gae7f5adbf3062c0a834cfd8f98b0c1878
#define POLARITY_FLAG 0

// baudrate (guess)
#define BAUD_RATE 9600

// blug
// Default Configuration of SPI Master Delay BS.
// Delay before SPCK (in number of MCK clocks).
#define CONFIG_SPI_MASTER_DELAY_BS   0

// Default Configuration of SPI Master Delay BCT.
// Delay between consecutive transfers (in number of MCK clocks).
#define CONFIG_SPI_MASTER_DELAY_BCT   0

// Default Configuration of SPI Master Bits per Transfer Definition.
// 8 bits for transfer
#define CONFIG_SPI_MASTER_BITS_PER_TRANSFER   SPI_CSR_BITS_8_BIT


#define SPI_WRITE_DELAY 450

void configure_spi(Spi *p_spi){
	
	uint32_t ul_sr;
	
	///////////////////////////////////////////////////////////
	
	// configure pins
	
	// attempt to disable the interupts on MISO and MOSI
	PIOA->PIO_IDR = PIO_PA25A_SPI0_MISO | PIO_PA26A_SPI0_MOSI | PIO_PA27A_SPI0_SPCK;
	
	// disable the pins entirely
	PIOA->PIO_PDR = PIO_PA25A_SPI0_MISO | PIO_PA26A_SPI0_MOSI | PIO_PA27A_SPI0_SPCK;
	
	ul_sr = PIOA->PIO_ABSR;
	PIOA->PIO_ABSR &= ~(PIO_PA25A_SPI0_MISO | PIO_PA26A_SPI0_MOSI | PIO_PA27A_SPI0_SPCK) & ul_sr;
	
	// copied from:
	// http://asf.atmel.com/docs/latest/sam3x/html/sam_spi_quickstart.html
	spi_master_init(p_spi);
	spi_master_setup_device(p_spi, DEVICE_ID, POLARITY_FLAG,	BAUD_RATE);
	
	spi_enable(p_spi);
}

void spi_master_init(Spi *p_spi)
{
	spi_enable_clock(p_spi);
	spi_reset(p_spi);
	spi_set_master_mode(p_spi);
	spi_disable_mode_fault_detect(p_spi);
	spi_disable_loopback(p_spi);
	
	//
	spi_set_peripheral_chip_select_value(p_spi,	spi_get_pcs(DEFAULT_CHIP_ID));
	spi_set_fixed_peripheral_select(p_spi);
	spi_disable_peripheral_select_decode(p_spi);
	spi_set_delay_between_chip_select(p_spi, CONFIG_SPI_MASTER_DELAY_BCS);
}

void spi_master_setup_device(Spi *p_spi, uint32_t device_id, uint32_t flags, uint32_t baud_rate)
{
	spi_set_transfer_delay(p_spi, device_id, CONFIG_SPI_MASTER_DELAY_BS, CONFIG_SPI_MASTER_DELAY_BCT);
	spi_set_bits_per_transfer(p_spi, device_id, CONFIG_SPI_MASTER_BITS_PER_TRANSFER);
	spi_set_baudrate_div(p_spi, device_id,
	spi_calc_baudrate_div(baud_rate, sysclk_get_cpu_hz()));
	spi_configure_cs_behavior(p_spi, device_id, SPI_CS_KEEP_LOW);
	spi_set_clock_polarity(p_spi, device_id, flags >> 1);
	spi_set_clock_phase(p_spi, device_id, ((flags & 0x1) ^ 0x1));
}

void BLUEsat_spi_write_string (char* c) {
	uint32_t x;
	//uint32_t y;
	
	// enable GPIO low
	PIOB->PIO_CODR = PIO_PB14;
	
	// don't forget that \0
	for (x = 0; c[x] != '\0'; x++) {
		spi_write(SPI0,c[x],0,0);
	}
	
	// enable GPIO high
	PIOB->PIO_SODR = PIO_PB14;
}