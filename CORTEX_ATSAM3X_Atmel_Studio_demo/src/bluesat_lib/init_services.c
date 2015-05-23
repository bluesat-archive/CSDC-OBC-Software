/*
 * init_services.c
 *
 * Created: 23/05/2015 5:40:36 PM
 *  Author: Blue
 */ 

#include "FreeRTOS.h"
#include "spi_master.h"
#include "twi_master.h"
#include "BLUEsatConfig.h"

void configure_spi ( void ) {
    SPI_Device_Memory = ( struct spi_device * ) pvPortMalloc( sizeof( struct spi_device ) );
    SPI_Device_Memory->id = SPI_DEVICE_0;
        
    SPI_Device_CC1120 = ( struct spi_device * ) pvPortMalloc( sizeof( struct spi_device ) );
    SPI_Device_CC1120->id = SPI_DEVICE_1;
    
    spi_master_init(SPI0);
    spi_master_setup_device(SPI0, SPI_Device_Memory, POLARITY_FLAG, BAUD_RATE, 0);
    spi_master_setup_device(SPI0, SPI_Device_CC1120, POLARITY_FLAG, BAUD_RATE, 0);
    
    spi_buffer_index = 0;
    
}

void configure_twi ( void ) {
    twi_master_options_t *p_opt = (twi_master_options_t *)pvPortMalloc(sizeof(p_opt));
    p_opt->speed        = TWI0_SPEED;
    p_opt->chip         = 0;
    
    twi_master_setup (TWI0, p_opt);
}

void enable_interrupts ( void ) {
    
    spi_enable(SPI0);
    spi_enable_interrupt(SPI0, INTERRUPT_FLAGS);
}