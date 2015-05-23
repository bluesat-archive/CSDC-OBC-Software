/*
 * interrupts.c
 *
 * Created: 23/05/2015 8:15:10 PM
 *  Author: Blue
 */ 
#include <asf.h>
#include "bluesat_conf.h"

void write_to_spi_buffer(uint8_t data) {
    spi_buffer_start[spi_buffer_index] = data;
    spi_buffer_index++;
    
    if (spi_buffer_index>250) {
        spi_buffer_index = 0;
    }
}

void SPI0_Handler (void) {

    uint8_t spi_p = SPI_Device_CC1120->id;
    // break and check it reaches here
    
    uint16_t read_data = 0;
    spi_read(SPI0, &read_data, &spi_p);
    
    read_data = read_data;
    write_to_spi_buffer(read_data);
    
    NVIC_ClearPendingIRQ(SPI0_IRQn);
}

void TWI0_Handler() {
    
}

void TWI1_Handler() {
    
}
