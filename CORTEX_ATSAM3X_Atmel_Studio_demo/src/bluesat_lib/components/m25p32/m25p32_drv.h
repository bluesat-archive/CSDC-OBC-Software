/*
 * m25p32_drv.h
 *
 * Created: 8/21/2015 11:07:33 PM
 *  Author: jabez
 */ 


#ifndef M25P32_DRV_H_
#define M25P32_DRV_H_

#define ONE_BYTE 1

//more Commands need to be defined
#define WRITE_ENABLE 0x06
#define WRITE_DISABLE 0x04
//#define READ_IDENTIFICATION_SMALL 0x9e  
//#define READ_IDENTIFICATION_LARGE 0x9f
#define READ_STATUS_REGISTER 0x05
#define WIRTE_STATUS_REGISTER 0x01
#define READ_DATA_SLOW 0x03
#define READ_DATA_FAST 0x08
#define PAGE_PROGRAM 0x02
#define SECTOR_ERASE 0xd8
#define BULK_ERASE 0xc7
#define DEEP_POWER_DOWN 0xb9
#define RELEASE_FROM_POWER_DOWN 0xab
#define READ_ELECTRONIC_SIGN 0xab

//Prototypes
void m25p32BulkErase();
void m25p32SectorErase(uint32_t addr);
void m25p32PageProgram(Byte* data, uint32_t addr);

#endif /* M25P32_DRV_H_ */