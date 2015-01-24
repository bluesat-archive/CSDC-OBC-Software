/*
 * bluesat_blink.h
 *
 * Created: 24-01-2015 1:26:11 PM
 *  Author: Blue
 */ 

#ifndef BLUESAT_BLINK_H
#define BLUESAT_BLINK_H

void vStartBluesat_BlinkTasks( UBaseType_t uxPriority );
BaseType_t xAreBluesat_BlinkTaskStillRunning( void );

#endif /* BLUESAT_BLINK_H_ */