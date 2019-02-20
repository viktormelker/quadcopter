/* includes */
#include "PIDRegulator.h"

/* global variables */

/* These variables sum the pitch, yaw and roll. They are used together 
    with the I parameter */
unsigned long pitch_sum = 0;
unsigned long yaw_sum = 0;
unsigned long roll_sum = 0;

flight_control_data_t last_state;

/* functions */
/*  This function should use the information in the indata variables to 
    calculate the desired speeds of the motors for the next time step */
void calculate_engine_output(accel_t_gyro_union *accel_t_gyro, 
        flight_control_data_t *control_data, quad_motors_t * motor_speeds)
{
    flight_control_data_t current_state, diff_state, derivative_state;
    get_current_state(accel_t_gyro, &current_state);
    get_flight_control_data_diff(&current_state, control_data, &diff_state);

    /* Generate the sum of pitch, yaw and roll. 
        Maybe these values should be decay over time so that they don't */
    pitch_sum += current_state.pitch;
    yaw_sum += current_state.yaw;
    roll_sum += current_state.pitch;

    /* Calculate/obtain the derivatives */
    get_flight_control_data_diff(&last_state, &current_state, &derivative_state);

    reset_motor_power(motor_speeds);    /* Set motor speeds to 0 */
    /* Add P part of motor power */
    add_motor_power(motor_speeds, THRUST_P_VALUE, &inc_thrust_correction);
    add_motor_power(motor_speeds, PITCH_P_VALUE * diff_state.pitch, &inc_pitch_correction);
    add_motor_power(motor_speeds, YAW_P_VALUE * diff_state.yaw, &inc_yaw_correction);
    add_motor_power(motor_speeds, ROLL_P_VALUE * diff_state.roll, &inc_roll_correction);
    /* Add I part of motor power */
    add_motor_power(motor_speeds, PITCH_I_VALUE * pitch_sum, &inc_pitch_correction);
    add_motor_power(motor_speeds, YAW_I_VALUE * yaw_sum, &inc_yaw_correction);
    add_motor_power(motor_speeds, ROLL_I_VALUE * roll_sum, &inc_roll_correction);
    /* Add D part of motor power */
    add_motor_power(motor_speeds, PITCH_D_VALUE * derivative_state.pitch, &inc_pitch_correction);
    add_motor_power(motor_speeds, YAW_D_VALUE * derivative_state.yaw, &inc_yaw_correction);
    add_motor_power(motor_speeds, ROLL_D_VALUE * derivative_state.roll, &inc_roll_correction);

    // save last state
    last_state.pitch = current_state.pitch;
    last_state.yaw = current_state.yaw;
    last_state.roll = current_state.roll;
    last_state.thrust = current_state.thrust;
}

