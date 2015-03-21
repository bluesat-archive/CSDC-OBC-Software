/*
 * BLUEsat_spi_drv.h
 *
 * Created: 31-01-2015 1:14:12 PM
 *  Author: Blue
 */ 


#ifndef BLUESAT_SPI_DRV_H_
#define BLUESAT_SPI_DRV_H_

#define SPI_DEVICE_0	0000
#define SPI_DEVICE_1	0001
#define SPI_DEVICE_2	0011
#define SPI_DEVICE_3	0111

#define SPI_DEVICE_0_BUFFER	0
#define SPI_DEVICE_1_BUFFER	1
#define SPI_DEVICE_2_BUFFER	2
#define SPI_DEVICE_3_BUFFER	3

void configure_spi();
void spi_master_configure(Spi *p_spi);
void spi_master_configure_device(Spi *p_spi, uint32_t device_id, uint32_t flags, uint32_t baud_rate);
void BLUEsat_spi_write_string (char* c, uint32_t peripheral_select);

void write_to_spi_buffer(uint8_t data);
#endif /* BLUESAT_SPI_DRV_H_ */