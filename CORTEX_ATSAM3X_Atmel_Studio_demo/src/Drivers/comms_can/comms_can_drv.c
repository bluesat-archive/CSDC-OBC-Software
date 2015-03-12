/*
 * comms_can_drv.c
 *
 * Created: 12-03-2015 10:51:34 PM
 *  Author: Blue
 */ 
#include <asf.h>
#include "comms_can_drv.h"

void configure_can() {
	/*
	 *	All code copied from the Atmel Quickstart Guide:
	 *	http://asf.atmel.com/docs/3.4.1/sam3x/html/sam_can_quickstart.html
	 */
	
	// Define the CAN0 and CAN1 Transfer mailbox structure:
	can_mb_conf_t can0_mailbox;
	can_mb_conf_t can1_mailbox;

	// Enable the module clock for CAN0 and CAN1:
	pmc_enable_periph_clk(ID_CAN0);
	pmc_enable_periph_clk(ID_CAN1);

	// Initialize CAN0 and CAN1, baudrate is 1Mb/s:
	can_init(CAN0, sysclk_get_cpu_hz(), CAN_BPS_1000K);
	can_init(CAN1, sysclk_get_cpu_hz(), CAN_BPS_1000K);

	// Note:
	// The CAN transceiver should be configured before initializing the CAN module.
	// Reset all CAN0 and CAN1 mailboxes:
	can_reset_all_mailbox(CAN0);
	can_reset_all_mailbox(CAN1);

	// Initialize CAN1 mailbox 0 as receiver, frame ID is 0x07:
	can1_mailbox.ul_mb_idx = 0;
	can1_mailbox.uc_obj_type = CAN_MB_RX_MODE;
	can1_mailbox.ul_id_msk = CAN_MAM_MIDvA_Msk | CAN_MAM_MIDvB_Msk;
	can1_mailbox.ul_id = CAN_MID_MIDvA(0x07);
	can_mailbox_init(CAN1, &can1_mailbox);

	// Initialize CAN0 mailbox 0 as transmitter, transmit priority is 15:
	can0_mailbox.ul_mb_idx = 0;
	can0_mailbox.uc_obj_type = CAN_MB_TX_MODE;
	can0_mailbox.uc_tx_prio = 15;
	can0_mailbox.uc_id_ver = 0;
	can0_mailbox.ul_id_msk = 0;
	can_mailbox_init(CAN0, &can0_mailbox);

	// Prepare transmit ID, data and data length in CAN0 mailbox 0:
	can0_mailbox.ul_id = CAN_MID_MIDvA(0x07);
	can0_mailbox.ul_datal = 0x12345678;
	can0_mailbox.ul_datah = 0x87654321;
	can0_mailbox.uc_length = 8;
	can_mailbox_write(CAN0, &can0_mailbox);
}