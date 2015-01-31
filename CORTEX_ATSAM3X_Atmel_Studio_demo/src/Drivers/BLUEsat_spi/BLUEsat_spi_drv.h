/*
 * BLUEsat_spi_drv.h
 *
 * Created: 31-01-2015 1:14:12 PM
 *  Author: Blue
 */ 


#ifndef BLUESAT_SPI_DRV_H_
#define BLUESAT_SPI_DRV_H_

void configure_spi(void);
void spi_master_init(Spi *p_spi);
void spi_master_setup_device(
					Spi *p_spi,
					uint32_t device_id,
					uint32_t flags,
					uint32_t baud_rate
);
// removed 'board_spi_select_id_t sel_id' from the parameters 
// because it wasn't used in the function

void BLUEsat_spi_write_string (char* c);
#endif /* BLUESAT_SPI_DRV_H_ */