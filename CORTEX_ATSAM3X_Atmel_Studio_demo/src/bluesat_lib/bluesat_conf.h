/*
 * bluesat_config.h
 *
 * Created: 23/05/2015 7:38:21 PM
 *  Author: Blue
 */
#include <asf.h>
#include "queue.h"
#ifndef BLUESAT_CONFIG_H_
#define BLUESAT_CONFIG_H_


/* SPI devices */
struct spi_device * SPI_Device_CC1120;
struct spi_device * SPI_Device_Memory;

/*SPI Queue Handles*/

QueueHandle_t SPI_TX_QUEUE;
QueueHandle_t SPI_RX_QUEUE;

/* SPI */
#define SPI_DEVICE_0                            0000
#define SPI_DEVICE_1                            0001
#define SPI_DEVICE_2                            0011
#define SPI_DEVICE_3                            0111

#define SPI_DEVICE_MEMORY_CHIP_ID               SPI_DEVICE_0 
#define SPI_DEVICE_CC1120_CHIP_ID               SPI_DEVICE_1

#define POLARITY_FLAG							0						// Flags for the clock polarity and phase
#define BAUD_RATE								9600					// Baud rate
#define CONFIG_SPI_MASTER_DELAY_BS				0						// Delay before SPCK (in number of MCK clocks).
#define CONFIG_SPI_MASTER_DELAY_BCS				0						// Delay between chip selects (in number of MCK clocks).
#define CONFIG_SPI_MASTER_DELAY_BCT				0						// Delay between consecutive transfers (in number of MCK clocks).
#define CONFIG_SPI_MASTER_BITS_PER_TRANSFER		SPI_CSR_BITS_8_BIT		// Size of data transfer
#define SPI_INTERRUPT_FLAGS						SPI_IDR_RDRF            // Interrupt flags


#define SPI_TX_QUEUE_MAX_LENGTH					64
#define SPI_RX_QUEUE_MAX_LENGTH					64

/* TWI */
#define TWI0_SPEED                              100000
#define TWI0_CHIP                               0xFF
#define TWI0_INTERRUPT_FLAGS                    0

<<<<<<< Updated upstream
uint8_t spi_buffer_start[250];
uint32_t spi_buffer_index;
=======
/* UART */
#define CONF_UART_BAUDRATE                      9600
#define CONF_UART_PARITY                        UART_MR_PAR_NO
>>>>>>> Stashed changes

#endif /* BLUESAT_CONFIG_H_ */