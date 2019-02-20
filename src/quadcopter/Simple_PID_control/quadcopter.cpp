/* includes */
#include "quadcopter.h"

/* variables */
quad_motors_t inc_pitch_correction  = {1, 1, 0, 0};
quad_motors_t inc_yaw_correction    = {1, 1, -1, -1};
quad_motors_t inc_roll_correction   = {1, -1, 1, -1};
quad_motors_t inc_thrust_correction = {1, 1, 1, 1};

/* functions */
/* This function calculates the difference between state1 and
    state2 and returns it in diff_state */
void get_flight_control_data_diff(flight_control_data_t *state1, 
        flight_control_data_t *state2, flight_control_data_t *diff_state)
{
    diff_state->pitch = state1->pitch - state2->pitch;
    diff_state->yaw = state1->yaw - state2->yaw;
    diff_state->roll = state1->roll - state2->roll;
    diff_state->thrust = state1->thrust - state2->thrust;
}

/* This function returns the current pitch, yaw and roll angles */
void get_current_state(accel_t_gyro_union *accel_t_gyro,
            flight_control_data_t *current_state)
{
    current_state->pitch = 0;   //TBD
    current_state->yaw = 0;     //TBD
    current_state->roll = 0;    /* Need compass to get this values */  
    current_state->thrust = 0;
}

/* This function increases (or decreases) the motor_speeds variable 
    in the direction of the motor_vector by a specific amount */
void add_motor_power(quad_motors_t *motor_speeds, int16_t amount, 
        quad_motors_t *motor_vector)
{
    motor_speeds->top_left_motor += amount * motor_vector->top_left_motor;
    motor_speeds->top_right_motor += amount * motor_vector->top_right_motor;
    motor_speeds->bot_left_motor += amount * motor_vector->bot_left_motor;
    motor_speeds->bot_right_motor += amount * motor_vector->bot_right_motor;
}

/* This function sets all motor speeds to 0 */
void reset_motor_power(quad_motors_t *motor_speeds)
{
    motor_speeds->top_left_motor = 0;
    motor_speeds->top_right_motor = 0;
    motor_speeds->bot_left_motor = 0;
    motor_speeds->bot_right_motor = 0;
}

/* This function returns the angle corresponding to the input 
    gyro value 
    The formula is taken from / gotten from bayesianadventures.wordpress.com/2013/10/20/gyroscopes-accelerometers-and-the-complementary-filter/ */
/*static float get_angle_from_gyro(int16_t gyro_data)
{
    return gyro_data / GYRO_TO_ANGLE_1000_G;
}*/

/* This function returns the pitch and roll angles corresponding
    to the input accelerometer value 
    The formula used here is only valid when the system is
    not accelerating in any direction */
/*static void get_angles_from_acc(accel_t_gyro_union *accel_t_gyro, 
                            flight_control_data_t *res_angles)
{
    uint32_t hypo;

    // calculate the total acceleration vector (should be equal to 1g)
    hypo =

    res_angles->pitch = 
}*/

/* This function returns the angle corresponding to the input 
    accelerometer value 
    The formula is taken from / gotten from bayesianadventures.wordpress.com/2013/10/20/gyroscopes-accelerometers-and-the-complementary-filter/ */
/*static float get_angle_from_acc(int16_t acc_data)
{
    // This is a crappy estimation. Should use trigonometry instead
    return acc_data / ACC_TO_ANGLE;
}*/

/*void print_flight_control_data(flight_control_data_t *state1)
{
    Serial.print(("Pitch angle: "));
    Serial.print(state1->pitch);
    Serial.println((""));

    Serial.print(("Yaw angle: "));
    Serial.print(state1->yaw);
    Serial.println((""));

    Serial.print(("Roll angle: "));
    Serial.print(state1->roll);
    Serial.println((""));

    Serial.print(("Thrust: "));
    Serial.print(state1->thrust, DEC);
    Serial.println((""));
}*/
