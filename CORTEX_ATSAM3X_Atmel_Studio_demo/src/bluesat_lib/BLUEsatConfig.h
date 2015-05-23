/*
 * BLUEsatConfig.c
 *
 * Created: 23/05/2015 6:02:28 PM
 *  Author: Blue
 */ 

/* SPI configuration */
struct spi_device * SPI_Device_CC1120;
struct spi_device * SPI_Device_Memory;

#define SPI_DEVICE_0                            1110
#define SPI_DEVICE_1                            1101
#define SPI_DEVICE_2                            1011
#define SPI_DEVICE_3                            0111

#define DEFAULT_CHIP_ID                         SPI_DEVICE_0

#define POLARITY_FLAG							0						// Flags for the clock polarity and phase
#define BAUD_RATE								9600					// Baud rate
#define CONFIG_SPI_MASTER_DELAY_BS				0						// Delay before SPCK (in number of MCK clocks).
#define CONFIG_SPI_MASTER_DELAY_BCS				0						// Delay between chip selects (in number of MCK clocks).
#define CONFIG_SPI_MASTER_DELAY_BCT				0						// Delay between consecutive transfers (in number of MCK clocks).
#define CONFIG_SPI_MASTER_BITS_PER_TRANSFER		SPI_CSR_BITS_8_BIT		// Size of data transfer
#define INTERRUPT_FLAGS							SPI_IDR_RDRF            // Interrupt flags

#define TWI0_SPEED                              100000                  // Standard mode speed (100 KHz)
#define TWI0_CHIP                               0                       // Not used in Atmel's drivers, might not need

uint8_t spi_buffer_start[250];
uint32_t spi_buffer_index;


