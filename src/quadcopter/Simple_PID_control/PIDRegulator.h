#ifndef PIDREGULATOR_H_
#define PIDREGULATOR_H_

/* includes */
#include "MPU6050_own.h"
#include "quadcopter.h"

/* defines */
#define PITCH_P_VALUE       (0)
#define PITCH_I_VALUE       (1)
#define PITCH_D_VALUE       (0)
#define YAW_P_VALUE         (0) 
#define YAW_I_VALUE         (1)
#define YAW_D_VALUE         (0)
#define ROLL_P_VALUE        (0)
#define ROLL_I_VALUE        (0)
#define ROLL_D_VALUE        (0)
#define THRUST_P_VALUE      (1000)

/* structs */

/* variables */

/* functions */
extern void calculate_engine_output(accel_t_gyro_union *accel_t_gyro, 
        flight_control_data_t *control_data, quad_motors_t * motor_speeds);

#endif // PIDREGULATOR_H_