/* This file handles all communication between arduino
     and remote control + receiver */

/* includes */
#include "RC.h"
#include <Servo.h>
#include <Wire.h>


/* global variables */
//  Boolean True if interrupts shall do anything, shall be false within main loop.
volatile bool ISR_on = false;

// Help when calculating PPM time, [us].
volatile unsigned long ISR_1_high_time = 0;  //  Volatile because its used in interrupt.
volatile unsigned long ISR_2_high_time = 0;
volatile unsigned long ISR_3_high_time = 0;
volatile unsigned long ISR_4_high_time = 0;
// Raw RC input, PPM, [us].
volatile uint16_t pulsew_ch_1 = 0;
volatile uint16_t pulsew_ch_2 = 0;
volatile uint16_t pulsew_ch_3 = 0;
volatile uint16_t pulsew_ch_4 = 0;

/* functions */
/* Help function for interrupts when calculating PPM high time.*/
static void update_ISR_variables (const int pin, volatile unsigned long *high_time, volatile uint16_t *pulsew)
{
    volatile unsigned long curr_time;  
    curr_time = micros();  //  Time since system start.

    //  Only update variables when allowed to.
    if (ISR_on == true) 
    {
        if (digitalRead (pin) == HIGH) 
        {
            *high_time = curr_time;
        }
        else 
        {      
            //  Check for wrap around of internal clock.
            if (curr_time < *high_time) 
            {
                *pulsew = curr_time + sizeof(curr_time) - *high_time;
            } 
            else 
            {      
                *pulsew = curr_time - *high_time;
            }
        }
    }
}

/* Interrupt routines
   Calculates high time of input on a pin. */
static void ISR_1 () 
{
    update_ISR_variables (RC_1, &ISR_1_high_time, &pulsew_ch_1);
}

static void ISR_2 () 
{
    update_ISR_variables (RC_2, &ISR_2_high_time, &pulsew_ch_2);
}

static void ISR_3 () 
{
    update_ISR_variables (RC_3, &ISR_3_high_time, &pulsew_ch_3);
}

static void ISR_4 () 
{
    update_ISR_variables (RC_4, &ISR_4_high_time, &pulsew_ch_4);
}

void start_tracking_rc_input()
{
    // Assign interrupt (input from remote).
    attachInterrupt(RC_1, ISR_1, CHANGE);
    attachInterrupt(RC_2, ISR_2, CHANGE);
    attachInterrupt(RC_3, ISR_3, CHANGE);
    attachInterrupt(RC_4, ISR_4, CHANGE);
}

void print_rc_inputs()
{
    Serial.print("Ch1 out: ");
    Serial.print(pulsew_ch_1, DEC);
    Serial.println((""));
    Serial.print("Ch2 out: ");
    Serial.print(pulsew_ch_2, DEC);
    Serial.println((""));
    Serial.print("Ch3 out: ");
    Serial.print(pulsew_ch_3, DEC);
    Serial.println((""));
    Serial.print("Ch4 out: ");
    Serial.print(pulsew_ch_4, DEC);
    Serial.print("\n");
}