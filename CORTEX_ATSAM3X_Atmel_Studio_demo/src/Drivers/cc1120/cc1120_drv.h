/*
 * cc1120_drv.h
 *
 * Created: 14-02-2015 4:59:25 PM
 *  Author: Blue
 */ 


#ifndef CC1120_DRV_H_
#define CC1120_DRV_H_



#define CC1120_MAX_SPI_ACCESS_ADDRESS		0x3F


/* Access Types */
#define CC1120_READ			0x80	// 1000 0000
#define CC1120_WRITE		0x00	// 0000 0000

/* Header Access Masks */
#define CC1120_SINGLE_REGISTER				0xBF		// 1011 1111	
#define CC1120_BURST_REGISTER				0x		// 1111 1111
#define CC1120_SINGLE_EXTENDED_REGISTER		0x		
#define CC1120_BURST_EXTENDED_REGISTER		0x
#define CC1120_COMMAND_STROBE				0x
#define CC1120_DIRECT_FIFO					0x								// lol
#define CC1120_STANDARD_FIFO				0x									// will come back to fix this 
																	// soon
/* Configuration Registers */																					// promise
#define CC1120_IOCFG3                   0x00
#define CC1120_IOCFG2                   0x01
#define CC1120_IOCFG1                   0x02
#define CC1120_IOCFG0                   0x03
#define CC1120_SYNC3                    0x04
#define CC1120_SYNC2                    0x05
#define CC1120_SYNC1                    0x06
#define CC1120_SYNC0                    0x07
#define CC1120_SYNC_CFG1                0x08
#define CC1120_SYNC_CFG0                0x09
#define CC1120_DEVIATION_M              0x0A
#define CC1120_MODCFG_DEV_E             0x0B
#define CC1120_DCFILT_CFG               0x0C
#define CC1120_PREAMBLE_CFG1            0x0D
#define CC1120_PREAMBLE_CFG0            0x0E
#define CC1120_FREQ_IF_CFG              0x0F
#define CC1120_IQIC                     0x10
#define CC1120_CHAN_BW                  0x11
#define CC1120_MDMCFG1                  0x12
#define CC1120_MDMCFG0                  0x13
#define CC1120_SYMBOL_RATE2             0x14
#define CC1120_SYMBOL_RATE1             0x15
#define CC1120_SYMBOL_RATE0             0x16
#define CC1120_AGC_REF                  0x17
#define CC1120_AGC_CS_THR               0x18
#define CC1120_AGC_GAIN_ADJUST          0x19
#define CC1120_AGC_CFG3                 0x1A
#define CC1120_AGC_CFG2                 0x1B
#define CC1120_AGC_CFG1                 0x1C
#define CC1120_AGC_CFG0                 0x1D
#define CC1120_FIFO_CFG                 0x1E
#define CC1120_DEV_ADDR                 0x1F
#define CC1120_SETTLING_CFG             0x20
#define CC1120_FS_CFG                   0x21
#define CC1120_WOR_CFG1                 0x22
#define CC1120_WOR_CFG0                 0x23
#define CC1120_WOR_EVENT0_MSB           0x24
#define CC1120_WOR_EVENT0_LSB           0x25
#define CC1120_PKT_CFG2                 0x26
#define CC1120_PKT_CFG1                 0x27
#define CC1120_PKT_CFG0                 0x28
#define CC1120_RFEND_CFG1               0x29
#define CC1120_RFEND_CFG0               0x2A
#define CC1120_PA_CFG2                  0x2B
#define CC1120_PA_CFG1                  0x2C
#define CC1120_PA_CFG0                  0x2D
#define CC1120_PKT_LEN                  0x2E

/* Extended Configuration Registers */
#define CC1120_IF_MIX_CFG               0x2F00
#define CC1120_FREQOFF_CFG              0x2F01
#define CC1120_TOC_CFG                  0x2F02
#define CC1120_MARC_SPARE               0x2F03
#define CC1120_ECG_CFG                  0x2F04
#define CC1120_CFM_DATA_CFG             0x2F05
#define CC1120_EXT_CTRL                 0x2F06
#define CC1120_RCCAL_FINE               0x2F07
#define CC1120_RCCAL_COARSE             0x2F08
#define CC1120_RCCAL_OFFSET             0x2F09
#define CC1120_FREQOFF1                 0x2F0A
#define CC1120_FREQOFF0                 0x2F0B
#define CC1120_FREQ2                    0x2F0C
#define CC1120_FREQ1                    0x2F0D
#define CC1120_FREQ0                    0x2F0E
#define CC1120_IF_ADC2                  0x2F0F
#define CC1120_IF_ADC1                  0x2F10
#define CC1120_IF_ADC0                  0x2F11
#define CC1120_FS_DIG1                  0x2F12
#define CC1120_FS_DIG0                  0x2F13
#define CC1120_FS_CAL3                  0x2F14
#define CC1120_FS_CAL2                  0x2F15
#define CC1120_FS_CAL1                  0x2F16
#define CC1120_FS_CAL0                  0x2F17
#define CC1120_FS_CHP                   0x2F18
#define CC1120_FS_DIVTWO                0x2F19
#define CC1120_FS_DSM1                  0x2F1A
#define CC1120_FS_DSM0                  0x2F1B
#define CC1120_FS_DVC1                  0x2F1C
#define CC1120_FS_DVC0                  0x2F1D
#define CC1120_FS_LBI                   0x2F1E
#define CC1120_FS_PFD                   0x2F1F
#define CC1120_FS_PRE                   0x2F20
#define CC1120_FS_REG_DIV_CML           0x2F21
#define CC1120_FS_SPARE                 0x2F22
#define CC1120_FS_VCO4                  0x2F23
#define CC1120_FS_VCO3                  0x2F24
#define CC1120_FS_VCO2                  0x2F25
#define CC1120_FS_VCO1                  0x2F26
#define CC1120_FS_VCO0                  0x2F27
#define CC1120_GBIAS6                   0x2F28
#define CC1120_GBIAS5                   0x2F29
#define CC1120_GBIAS4                   0x2F2A
#define CC1120_GBIAS3                   0x2F2B
#define CC1120_GBIAS2                   0x2F2C
#define CC1120_GBIAS1                   0x2F2D
#define CC1120_GBIAS0                   0x2F2E
#define CC1120_IFAMP                    0x2F2F
#define CC1120_LNA                      0x2F30
#define CC1120_RXMIX                    0x2F31
#define CC1120_XOSC5                    0x2F32
#define CC1120_XOSC4                    0x2F33
#define CC1120_XOSC3                    0x2F34
#define CC1120_XOSC2                    0x2F35
#define CC1120_XOSC1                    0x2F36
#define CC1120_XOSC0                    0x2F37
#define CC1120_ANALOG_SPARE             0x2F38
#define CC1120_PA_CFG3                  0x2F39
#define CC1120_IRQ0M                    0x2F3F
#define CC1120_IRQ0F                    0x2F40 

/* Status Registers */
#define CC1120_WOR_TIME1                0x2F64
#define CC1120_WOR_TIME0                0x2F65
#define CC1120_WOR_CAPTURE1             0x2F66
#define CC1120_WOR_CAPTURE0             0x2F67
#define CC1120_BIST                     0x2F68
#define CC1120_DCFILTOFFSET_I1          0x2F69
#define CC1120_DCFILTOFFSET_I0          0x2F6A
#define CC1120_DCFILTOFFSET_Q1          0x2F6B
#define CC1120_DCFILTOFFSET_Q0          0x2F6C
#define CC1120_IQIE_I1                  0x2F6D
#define CC1120_IQIE_I0                  0x2F6E
#define CC1120_IQIE_Q1                  0x2F6F
#define CC1120_IQIE_Q0                  0x2F70
#define CC1120_RSSI1                    0x2F71
#define CC1120_RSSI0                    0x2F72
#define CC1120_MARCSTATE                0x2F73
#define CC1120_LQI_VAL                  0x2F74
#define CC1120_PQT_SYNC_ERR             0x2F75
#define CC1120_DEM_STATUS               0x2F76
#define CC1120_FREQOFF_EST1             0x2F77
#define CC1120_FREQOFF_EST0             0x2F78
#define CC1120_AGC_GAIN3                0x2F79
#define CC1120_AGC_GAIN2                0x2F7A
#define CC1120_AGC_GAIN1                0x2F7B
#define CC1120_AGC_GAIN0                0x2F7C
#define CC1120_CFM_RX_DATA_OUT          0x2F7D
#define CC1120_CFM_TX_DATA_IN           0x2F7E
#define CC1120_ASK_SOFT_RX_DATA         0x2F7F
#define CC1120_RNDGEN                   0x2F80
#define CC1120_MAGN2                    0x2F81
#define CC1120_MAGN1                    0x2F82
#define CC1120_MAGN0                    0x2F83
#define CC1120_ANG1                     0x2F84
#define CC1120_ANG0                     0x2F85
#define CC1120_CHFILT_I2                0x2F86
#define CC1120_CHFILT_I1                0x2F87
#define CC1120_CHFILT_I0                0x2F88
#define CC1120_CHFILT_Q2                0x2F89
#define CC1120_CHFILT_Q1                0x2F8A
#define CC1120_CHFILT_Q0                0x2F8B
#define CC1120_GPIO_STATUS              0x2F8C
#define CC1120_FSCAL_CTRL               0x2F8D
#define CC1120_PHASE_ADJUST             0x2F8E
#define CC1120_PARTNUMBER               0x2F8F
#define CC1120_PARTVERSION              0x2F90
#define CC1120_SERIAL_STATUS            0x2F91
#define CC1120_MODEM_STATUS1            0x2F92
#define CC1120_MODEM_STATUS0            0x2F93
#define CC1120_MARC_STATUS1             0x2F94
#define CC1120_MARC_STATUS0             0x2F95
#define CC1120_PA_IFAMP_TEST            0x2F96
#define CC1120_FSRF_TEST                0x2F97
#define CC1120_PRE_TEST                 0x2F98
#define CC1120_PRE_OVR                  0x2F99
#define CC1120_ADC_TEST                 0x2F9A
#define CC1120_DVC_TEST                 0x2F9B
#define CC1120_ATEST                    0x2F9C
#define CC1120_ATEST_LVDS               0x2F9D
#define CC1120_ATEST_MODE               0x2F9E
#define CC1120_XOSC_TEST1               0x2F9F
#define CC1120_XOSC_TEST0               0x2FA0  
                                        
#define CC1120_RXFIRST                  0x2FD2   
#define CC1120_TXFIRST                  0x2FD3   
#define CC1120_RXLAST                   0x2FD4 
#define CC1120_TXLAST                   0x2FD5 
#define CC1120_NUM_TXBYTES              0x2FD6  /* Number of bytes in TXFIFO */ 
#define CC1120_NUM_RXBYTES              0x2FD7  /* Number of bytes in RXFIFO */
#define CC1120_FIFO_NUM_TXBYTES         0x2FD8  
#define CC1120_FIFO_NUM_RXBYTES         0x2FD9  
                                                                                                                                                
/* DATA FIFO Access */
#define CC1120_SINGLE_TXFIFO            0x3F      /*  TXFIFO - Single access to Transmit FIFO */
#define CC1120_BURST_TXFIFO             0x7F      /*  TXFIFO - Burst access to Transmit FIFO  */
#define CC1120_SINGLE_RXFIFO            0xBF      /*  RXFIFO - Single access to Receive FIFO  */
#define CC1120_BURST_RXFIFO             0xFF      /*  RXFIFO - Burst access to Receive FIFO  */

#define CC1120_LQI_CRC_OK_BM            0x80
#define CC1120_LQI_EST_BM               0x7F

/* Command strobe registers */
#define CC1120_SRES                     0x30      /*  SRES    - Reset chip. */
#define CC1120_SFSTXON                  0x31      /*  SFSTXON - Enable and calibrate frequency synthesizer. */
#define CC1120_SXOFF                    0x32      /*  SXOFF   - Turn off crystal oscillator. */
#define CC1120_SCAL                     0x33      /*  SCAL    - Calibrate frequency synthesizer and turn it off. */
#define CC1120_SRX                      0x34      /*  SRX     - Enable RX. Perform calibration if enabled. */
#define CC1120_STX                      0x35      /*  STX     - Enable TX. If in RX state, only enable TX if CCA passes. */
#define CC1120_SIDLE                    0x36      /*  SIDLE   - Exit RX / TX, turn off frequency synthesizer. */
#define CC1120_AFC                      0x37      /*  AFC     - Automatic Frequency Correction */
#define CC1120_SWOR                     0x38      /*  SWOR    - Start automatic RX polling sequence (Wake-on-Radio) */
#define CC1120_SPWD                     0x39      /*  SPWD    - Enter power down mode when CSn goes high. */
#define CC1120_SFRX                     0x3A      /*  SFRX    - Flush the RX FIFO buffer. */
#define CC1120_SFTX                     0x3B      /*  SFTX    - Flush the TX FIFO buffer. */
#define CC1120_SWORRST                  0x3C      /*  SWORRST - Reset real time clock. */
#define CC1120_SNOP                     0x3D      /*  SNOP    - No operation. Returns status byte. */

/* Chip states returned in status byte */
#define CC1120_STATE_IDLE               0x00
#define CC1120_STATE_RX                 0x10
#define CC1120_STATE_TX                 0x20
#define CC1120_STATE_FSTXON             0x30
#define CC1120_STATE_CALIBRATE          0x40
#define CC1120_STATE_SETTLING           0x50
#define CC1120_STATE_RXFIFO_ERROR       0x60
#define CC1120_STATE_TXFIFO_ERROR       0x70

/* Access Functions */
uint32_t cc1120_write(uint8_t address);
uint32_t cc1120_read(uint8_t address);

/* Command Strobe Functions */
uint32_t cc1120_reset();




/*
rfStatus_t CC1120SpiReadReg(uint16 addr, uint8 *data, uint8 len);
rfStatus_t CC1120GetTxStatus(void);
rfStatus_t CC1120GetRxStatus(void);  
rfStatus_t CC1120SpiWriteReg(uint16 addr, uint8 *data, uint8 len);
rfStatus_t CC1120SpiWriteTxFifo(uint8 *pWriteData, uint8 len);
rfStatus_t CC1120SpiReadRxFifo(uint8 *pReadData, uint8 len);


*/

#endif /* CC1120_DRV_H_ */