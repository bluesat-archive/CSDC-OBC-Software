/*
 * cc1120_drv.c
 *
 * Created: 14-02-2015 4:59:17 PM
 *  Author: John
 */

#include <asf.h>
#include <spi_master.h>

#include "bluesat_conf.h"
#include "cc1120_drv.h"

/* Private functions */
static uint8_t prvTrxSpiCmdStrobe(uint8_t cmd);

static uint8_t prvTrx8BitRegAccess(uint8_t accessType, uint8_t addrByte, uint8_t *pData, uint16_t len);
static uint8_t prvTrx16BitRegAccess(uint8_t accessType, uint8_t addrByte, uint8_t *pData, uint8_t len);
static void prvTrxComm (uint8_t command, uint8_t *pData, uint8_t len);


/*-----------------------------------------------------------*/

uint8_t cc1120RegAccess(uint8_t accessType, uint16_t addr, uint8_t *pData, uint8_t len) {
	
    uint8_t addrHigh = (uint8_t)(addr>>8);
    uint8_t addrLow  = (uint8_t)(addr & 0x00FF);
    uint8_t status = 0;

	if (!addrHigh) {
        // Regular Register Access
		status = prvTrx8BitRegAccess(accessType, addrLow, pData, len);
	} else {
        // Extended Register Access
		status = prvTrx16BitRegAccess(accessType, addrLow, pData, len);
	}

    return status;
}


// This function is a shortened version of all the other low level access functions.
// Much easier to understand in one reading and refactored, need to test it.
/*
static uint8_t prvTrxRegAccess(uint8_t accessType, uint16_t addr, uint8 *pData, uint8 len) {

    uint8_t addrHigh = (uint8_t)(addr>>8);
    uint8_t addrLow  = (uint8_t)(addr & 0x00FF);
    uint8_t status = 0;
    uint8_t command;

    // Start communication
    spi_select_device(SPI0, SPI_Device_CC1120);

    // Access either regular or extended address space
    if (!addrHigh) {
        // read/write/burst/single and address
        command = accessType | addrLow;

        // Send header, read status byte
        spi_write_packet(SPI0, &command, ONE_BYTE);
        spi_read_packet(SPI0, &status, ONE_BYTE);
    } else {
        // read/write/burst/single in extended address space
        command = accessType | CC1120_EXTENDED_ACCESS;

        // Send header, read status byte
        spi_write_packet(SPI0, &command, ONE_BYTE);
        spi_read_packet(SPI0, &status, ONE_BYTE);
        
        // Send extended address
        spi_write_packet(SPI0, &addrLow, ONE_BYTE);
        
    }

    // Transmit/Receive data
	if (accessType & CC1120_READ_ACCESS) {
    	spi_read_packet(SPI0, pData, len);
    } else if (accessType & CC1120_WRITE_ACCESS) {
    	spi_write_packet(SPI0, pData, len);
	}

    // End communication
    spi_deselect_device(SPI0, SPI_Device_CC1120);

    return status;
}
*/

uint8_t cc1120Reset() {
    return prvTrxSpiCmdStrobe(CC1120_SRES);
}

static uint8_t prvTrxSpiCmdStrobe(uint8_t cmd) {
	
	uint8_t command = cmd;
	uint8_t status = 0;
	
	spi_select_device(SPI0, SPI_Device_CC1120);
	spi_write_packet(SPI0, &command, ONE_BYTE);
	spi_read_packet(SPI0, &status, ONE_BYTE);
	spi_deselect_device(SPI0, SPI_Device_CC1120);
	
	return status;
}
/*-----------------------------------------------------------*/

static uint8_t prvTrx8BitRegAccess(uint8_t accessType, uint8_t addrByte, uint8_t *pData, uint16_t len) {
	
	uint8_t command = accessType | addrByte;
	uint8_t status = 0;
	
	spi_select_device(SPI0, SPI_Device_CC1120);
	spi_write_packet(SPI0, &command, ONE_BYTE);
	spi_read_packet(SPI0, &status, ONE_BYTE);
	prvTrxComm (command, pData, len);
	spi_deselect_device(SPI0, SPI_Device_CC1120);
	
	return status;
}
/*-----------------------------------------------------------*/

static uint8_t prvTrx16BitRegAccess(uint8_t accessType, uint8_t addrByte, uint8_t *pData, uint8_t len) {
	
	uint8_t command = accessType | CC1120_EXTENDED_ACCESS;
	uint8_t status = 0;
	
	// Open connection with SPI device
	spi_select_device(SPI0, SPI_Device_CC1120);
	
	// Send header, read status byte
	spi_write_packet(SPI0, &command, ONE_BYTE);
	spi_read_packet(SPI0, &status, ONE_BYTE);
	
	// Send extended address
	command = addrByte;
	spi_write_packet(SPI0, &command, ONE_BYTE);
	
	// Transmit/Receive data
	prvTrxComm (command, pData, len);
	
	// Close connection with SPI device
	spi_deselect_device(SPI0, SPI_Device_CC1120);
	
	return status;
}
/*-----------------------------------------------------------*/

static void prvTrxComm (uint8_t command, uint8_t *pData, uint8_t len) {
	if (command & CC1120_READ_ACCESS) {
		spi_read_packet(SPI0, pData, len);
	} else if (command & CC1120_WRITE_ACCESS) {
		spi_write_packet(SPI0, pData, len);
	}
}
/*-----------------------------------------------------------*/
