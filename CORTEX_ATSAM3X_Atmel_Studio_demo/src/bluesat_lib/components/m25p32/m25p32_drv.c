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

static void enableWrite(){
    uint8_t command;
    //sending write enable command
    spi_select_device(SPI0,SPI_Device_Memory);
    command = WRITE_ENABLE;
    spi_write_packet(SPI0,&command,ONE_BYTE);
    spi_deselect_device(SPI0,SPI_Device_Memory);
}
static void disableWrite(){
    uint8_t command;
    //sending write disable command
    spi_select_device(SPI0,SPI_Device_Memory);
    command = WRITE_DISABLE;
    spi_write_packet(SPI0,&command,ONE_BYTE);
    spi_deselect_device(SPI0,SPI_Device_Memory);
}

void m25p32BulkErase(){
    
    uint8_t command;
    enableWrite();
    
    //sending bulk erase command
    spi_select_device(SPI0,SPI_Device_Memory);
    command = BULK_ERASE;
    spi_write_packet(SPI0,&command,ONE_BYTE);
    spi_deselect_device(SPI0,SPI_Device_Memory);
     
    while(m25p32WriteRunning());
    disableWrite();
}

void m25p32SectorErase(uint32_t addr){
    uint8_t command, address[3];
    enableWrite();
    address[0] = (addr&0x00FF0000)>>16;
    address[1] = (addr&0x0000FF00)>>8;
    address[2] = (addr&0x000000FF);
    spi_select_device(SPI0,SPI_Device_Memory);
    command = SECTOR_ERASE;
    spi_write_packet(SPI0,&command,ONE_BYTE);
    spi_write_packet(SPI0,address,3 * ONE_BYTE);
    spi_deselect_device(SPI0,SPI_Device_Memory);
    
    //while(m25p32WriteRunning());
    disableWrite();
}

void m25p32PageWrite(Byte* data, uint32_t addr,uint32_t dataSize){
    uint8_t command,address[3];
    uint16_t i;
    //while(m25p32WriteRunning());
    enableWrite();
    
    address[0] = (addr&0x00FF0000)>>16;
    address[1] = (addr&0x0000FF00)>>8;
    address[2] = (addr&0x000000FF);
    
    spi_select_device(SPI0,SPI_Device_Memory);
    command = PAGE_PROGRAM;
    spi_write_packet(SPI0,&command,ONE_BYTE);
    spi_write_packet(SPI0,address,3 * ONE_BYTE);
    
    for(i = 0; i < dataSize; i++)
    {
        spi_write_packet(SPI0,&data[i],ONE_BYTE);
    }
    
    spi_deselect_device(SPI0,SPI_Device_Memory);
    
    //while(m25p32WriteRunning());
    disableWrite();
}

void m25p32PageRead(Byte* data,uint32_t addr,uint32_t dataLength){
    uint8_t command,address[3];
    
    address[0] = (addr&0x00FF0000)>>16;
    address[1] = (addr&0x0000FF00)>>8;
    address[2] = (addr&0x000000FF);
    
    spi_select_device(SPI0,SPI_Device_Memory);
    command = READ_DATA_SLOW;
    spi_write_packet(SPI0,&command,ONE_BYTE);
    spi_write_packet(SPI0,address,3*ONE_BYTE);
    spi_read_packet(SPI0,data,dataLength);
    spi_deselect_device(SPI0,SPI_Device_Memory);
}

uint8_t m25p32ReadStatusRegister(){
    uint8_t command;
    uint8_t status;
        
    command = READ_STATUS_REGISTER;
    spi_select_device(SPI0,SPI_Device_Memory);
    spi_write_packet(SPI0,&command,ONE_BYTE);
    spi_read_packet(SPI0,&status,ONE_BYTE);
    spi_deselect_device(SPI0,SPI_Device_Memory);
    
    return status;
    
}

Byte m25p32WriteRunning(){
    uint8_t ret = m25p32ReadStatusRegister();
    ret = ret&0x01;
    return ret;
}