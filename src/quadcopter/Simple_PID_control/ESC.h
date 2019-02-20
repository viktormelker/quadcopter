#ifndef ESC_H_
#define ESC_H_

/* includes */
#include <stdint.h>

/* defines */
#define MAX_PULSEWIDTH      (1900)
#define MIN_PULSEWIDTH      (1100)

/* structs */

/* variables */

/* functions */
extern int bounds_check_pulse (int16_t pulsew) ;

#endif // ESC_H_