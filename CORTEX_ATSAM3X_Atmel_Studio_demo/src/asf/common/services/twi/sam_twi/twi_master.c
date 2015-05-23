/*
 * twi_master.c
 *
 * Created: 23/05/2015 4:55:23 PM
 *  Author: Blue
 */ 
#include "FreeRTOS.h"
#include "twi_master.h"

#define TWI0_SPEED          100000      // Standard mode speed (100 KHz)
#define TWI0_CHIP           0           // Not used in Atmel's drivers, might not need

void configure_twi ( void )
{
    twi_master_options_t *p_opt = (twi_master_options_t *)pvPortMalloc(sizeof(p_opt));
    p_opt->speed        = TWI0_SPEED;
    p_opt->chip         = 0;    
    
    twi_master_setup (TWI0, p_opt);
}