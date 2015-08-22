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
#define BULK_ERASE 0xc7
#define SECTOR_ERASE 0xd8

//Prototypes
void m25p32BulkErase();
void m25p32SectorErase(uint32_t addr);

#endif /* M25P32_DRV_H_ */