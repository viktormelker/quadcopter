/* This file has information about the connection from 
	the arduino to the remote control and it's receiver */

#ifndef RC_H_
#define RC_H_

/* includes */
#include <stdint.h>

/* defines */
// RC input pins.
#define RC_1 42 
#define RC_2 44
#define RC_3 46
#define RC_4 48

/* structs */

/* variables */
extern volatile bool ISR_on;
extern volatile unsigned long ISR_1_high_time;
extern volatile unsigned long ISR_2_high_time;
extern volatile unsigned long ISR_3_high_time;
extern volatile unsigned long ISR_4_high_time;
extern volatile uint16_t pulsew_ch_1;
extern volatile uint16_t pulsew_ch_2;
extern volatile uint16_t pulsew_ch_3;
extern volatile uint16_t pulsew_ch_4;

/* functions */
extern void start_tracking_rc_input();
extern void print_rc_inputs();

#endif // RC_H_