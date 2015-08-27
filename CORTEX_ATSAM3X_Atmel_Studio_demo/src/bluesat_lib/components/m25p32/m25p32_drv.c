/*
 * m25p32_drv.c
 *
 * Created: 8/21/2015 11:17:11 PM
 *  Author: jabez
 */ 
#include <asf.h>
#include <spi_master.h>

#include "bluesat_conf.h"
#include "m25p32_drv.h"

void m25p32BulkErase(){
    
    uint8_t command;
    //sending write enable command
    spi_select_device(SPI0,SPI_Device_Memory);
    command = WRITE_ENABLE;
    spi_write_packet(SPI0,&command,ONE_BYTE);
    spi_deselect_device(SPI0,SPI_Device_Memory);
    
    //sending bulk erase command
    spi_select_device(SPI0,SPI_Device_Memory);
    command = BULK_ERASE;
    spi_write_packet(SPI0,&command,ONE_BYTE);
    spi_deselect_device(SPI0,SPI_Device_Memory);
     
    //sending write disable command
    spi_select_device(SPI0,SPI_Device_Memory);
    command = WRITE_DISABLE;
    spi_write_packet(SPI0,&command,ONE_BYTE);
    spi_deselect_device(SPI0,SPI_Device_Memory);
}

void m25p32SectorErase(uint32_t addr){
    int8_t command;
    //sending write enable command
    spi_select_device(SPI0,SPI_Device_Memory);
    command = WRITE_ENABLE;
    spi_write_packet(SPI0,&command,ONE_BYTE);
    spi_deselect_device(SPI0,SPI_Device_Memory);
    
    //make sure that the address is only 3 bytes
    addr = (addr & 0x00FFFFFF);
    
    spi_select_device(SPI0,SPI_Device_Memory);
    command = SECTOR_ERASE;
    spi_write_packet(SPI0,&command,ONE_BYTE);
    //not sure about this<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
    spi_write_packet(SPI0,&addr,3 * ONE_BYTE);
    spi_deselect_device(SPI0,SPI_Device_Memory);
    
    
    //sending write disable command
    spi_select_device(SPI0,SPI_Device_Memory);
    command = WRITE_DISABLE;
    spi_write_packet(SPI0,&command,ONE_BYTE);
    spi_deselect_device(SPI0,SPI_Device_Memory);
}


void m25p32PageProgram(Byte* data, uint32_t addr,uint32_t dataSize){
    int8_t command;
    //sending write enable command
    spi_select_device(SPI0,SPI_Device_Memory);
    command = WRITE_ENABLE;
    spi_write_packet(SPI0,&command,ONE_BYTE);
    spi_deselect_device(SPI0,SPI_Device_Memory);
    
    addr = (addr & 0x00FFFFFF);
    
    spi_select_device(SPI0,SPI_Device_Memory);
    command = PAGE_PROGRAM;
    spi_write_packet(SPI0,&command,ONE_BYTE);
    spi_write_packet(SPI0,&addr,3 * ONE_BYTE);
    int i = dataSize -1;
    while(i >= 0){
        spi_write_packet(SPI0,&Byte[i],ONE_BYTE);
    }
    spi_deselect_device(SPI0,SPI_Device_Memory);
    
    //sending write disable command
    spi_select_device(SPI0,SPI_Device_Memory);
    command = WRITE_DISABLE;
    spi_write_packet(SPI0,&command,ONE_BYTE);
    spi_deselect_device(SPI0,SPI_Device_Memory);
}
}