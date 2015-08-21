/*
 * m25p32_drv.c
 *
 * Created: 8/21/2015 11:17:11 PM
 *  Author: jabez
 */ 

#include "m25p32_drv.h"

//needs to be filled up
#define SPI_M25P32 ??

void m25p32BulkErase(){
    
    uint8_t command;
    //sending write enable command
    spi_select_device(SPI_M25P32,SPI_Device_Memory);
    command = WRITE_ENABLE;
    spi_write_packet(SPI_M25P32,&command,ONE_BYTE);
    spi_deselect_device(SPI_M25P32,SPI_Device_Memory);
    
    //sending bulk erase command
    spi_select_device(SPI_M25P32,SPI_Device_Memory);
    command = BULK_ERASE;
    spi_write_packet(SPI_M25P32,&command,ONE_BYTE);
    spi_deselect_device(SPI_M25P32,SPI_Device_Memory);
     
    
    //sending write disable command
    spi_select_device(SPI_M25P32,SPI_Device_Memory);
    command = WRITE_DISABLE;
    spi_write_packet(SPI_M25P32,&command,ONE_BYTE);
    spi_deselect_device(SPI_M25P32,SPI_Device_Memory);
}