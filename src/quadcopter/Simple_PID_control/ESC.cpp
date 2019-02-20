/* This file should handle all controlling of the ESCs */

/* includes */
#include "ESC.h"

/* Make sure pulsewidth is within valid ranges. */
int bounds_check_pulse (int16_t pulsew) 
{
    int result = 0;

    if (pulsew < MIN_PULSEWIDTH) 
    {
        result = MIN_PULSEWIDTH;
    } 
    else if (pulsew > MAX_PULSEWIDTH) 
    {
        result = MAX_PULSEWIDTH;
    } 
    else 
    {
        result = pulsew;   
    }
    return result;
}