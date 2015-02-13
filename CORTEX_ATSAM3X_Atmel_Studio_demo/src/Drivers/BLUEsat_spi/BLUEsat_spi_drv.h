/*
 * BLUEsat_spi_drv.h
 *
 * Created: 31-01-2015 1:14:12 PM
 *  Author: Blue
 */ 


#ifndef BLUESAT_SPI_DRV_H_
#define BLUESAT_SPI_DRV_H_

/*
 *	Enables PINs.
 *	Sets up Arduino as SPI master.
 *	Starts SPI.
 *	Configures peripherals for slaves to listen on.
 */
void configure_spi(Spi *p_spi);

/*
 *	Configures SPI settings.
 *	Arduino is master.
 */
void spi_master_configure(Spi *p_spi);

/*
 *	Configures a peripheral with the arduino as master
 *	device_id is 0,1,2, or 3. Make sure the pin is enabled.
 */
void spi_master_configure_device(Spi *p_spi, uint32_t device_id, uint32_t flags, uint32_t baud_rate);

void BLUEsat_spi_write_string (char* c, uint32_t peripheral_select);
#endif /* BLUESAT_SPI_DRV_H_ */