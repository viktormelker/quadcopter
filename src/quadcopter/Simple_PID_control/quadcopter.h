/*  This file should handle everything which is general to any 
    type of quadcopter implementation */

#ifndef QUADCOPTER_H_
#define QUADCOPTER_H_

/* includes */
#include <stdint.h>
#include "MPU6050_own.h"

/* defines */
#define MAIN_LOOP_INTERVAL_MS      (2000)  /* This is the 
                interval at which the main loop is run. */
#define ARDUINO_ESC_PIN_1 (2)
#define ARDUINO_ESC_PIN_2 (3)
#define ARDUINO_ESC_PIN_3 (4)
#define ARDUINO_ESC_PIN_4 (5)

/* structs */
typedef struct 
{
    int16_t top_left_motor;
    int16_t top_right_motor;
    int16_t bot_left_motor;
    int16_t bot_right_motor;
} quad_motors_t;

typedef struct 
{
    int16_t pitch;  // Angle in degrees. Positive is nose pointing up
    int16_t yaw;    // Angle in degrees. Positive is right side lower than left  
    int16_t roll;   // TBD
    int16_t thrust; // TBD
} flight_control_data_t;

/* variables */
extern quad_motors_t inc_pitch_correction ;//= {1, 1, 0, 0};
extern quad_motors_t inc_yaw_correction ;//= {1, 1, -1, -1};
extern quad_motors_t inc_roll_correction ;//= {1, -1, 1, -1};
extern quad_motors_t inc_thrust_correction ;//= {1, 1, 1, 1};

/* functions */
extern void get_flight_control_data_diff(flight_control_data_t *state1, 
        flight_control_data_t *state2, flight_control_data_t *diff_state);
extern void get_current_state(accel_t_gyro_union *accel_t_gyro,
            flight_control_data_t *current_state);
extern void reset_motor_power(quad_motors_t *motor_speeds);
extern void add_motor_power(quad_motors_t *motor_speeds, int16_t amount, 
        quad_motors_t *motor_vector);

#endif // QUADCOPTER_H_