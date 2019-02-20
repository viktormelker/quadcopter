/*
  This code tries to control the quadcopter at a specified value of Z accelerometer setting.
  It is loosely based on MPU605_Example and ESCTest
*/

#include "MPU6050_own.h"
#include "quadcopter.h"
#include "PIDRegulator.h"
#include "RC.h"
#include <Servo.h> 
#include <Wire.h>

Servo esc_servo_1, esc_servo_2, esc_servo_3, esc_servo_4;  // create servo object to control a servo 
                // a maximum of eight servo objects can be created 
accel_t_gyro_union accel_t_gyro;

flight_control_data_t control_data; /* This struct controls how we wish the quad to behave */
quad_motors_t motor_speeds = {0, 0, 0, 0};  /*  Output to engines [us]. */

void setup() 
{
    int error;
    uint8_t c;

    /* Start receiving data from the remote control */
    start_tracking_rc_input();

    // Initialize the flight control data
    control_data.pitch = 0;
    control_data.yaw = 0;
    control_data.roll = 0;
    control_data.thrust = 0;     // Set the value for hovering

    // Initialize the 'Wire' class for the I2C-bus.
    Wire.begin();

    // Setup ESC control
    esc_servo_1.attach(ARDUINO_ESC_PIN_1);
    esc_servo_2.attach(ARDUINO_ESC_PIN_2);
    esc_servo_3.attach(ARDUINO_ESC_PIN_3);
    esc_servo_4.attach(ARDUINO_ESC_PIN_4);

    // Set motors to not running
    esc_servo_1.write(0);
    esc_servo_2.write(0);
    esc_servo_3.write(0);
    esc_servo_4.write(0);

    Serial.begin(115200);
    Serial.println("Setup()");

    /* Test sensor connection */
    error = MPU6050_read (MPU6050_WHO_AM_I, &c, 1);
    Serial.print(("WHO_AM_I : "));
    Serial.print(c,HEX);
    Serial.print((", error = "));
    Serial.println(error,DEC);

    init_accelerometer();
    //init_MPU6050();
}

void loop() 
{
    static unsigned long start_time;
    static unsigned long stop_time, diff_time;

    start_time = millis();
    
    //  Turn of interupts in the main loop.
    ISR_on = false;  

    // read sensors
    read_sensor_values((uint8_t *) &accel_t_gyro);
    print_sensor_values();

    // read input and determine what the quad should do
    // (set control_data variable here !)

    // determine how motors should run
    calculate_engine_output(&accel_t_gyro, &control_data, &motor_speeds);
    print_motor_values(&motor_speeds);

    print_rc_inputs();

    // delay until time to run loop again
    stop_time = millis();
    diff_time = stop_time - start_time;
    print_loop_duration(diff_time);
    ISR_on = true;  // turn on interrupts again
    delay(MAIN_LOOP_INTERVAL_MS - diff_time);
}



void print_sensor_values()
{
    double dT;

    Serial.print(("Read accel, temp and gyro, error = "));
    // Print the raw acceleration values
    Serial.print(("accel x,y,z: "));
    Serial.print(accel_t_gyro.value.x_accel, DEC);
    Serial.print((", "));
    Serial.print(accel_t_gyro.value.y_accel, DEC);
    Serial.print((", "));
    Serial.print(accel_t_gyro.value.z_accel, DEC);
    Serial.println((""));

    dT = get_temperature(accel_t_gyro.value.temperature);
    Serial.print(("temperature: "));
    Serial.print(dT, 3);
    Serial.print((" degrees Celsius"));
    Serial.println((""));

    // Print the raw gyro values.
    Serial.print(("gyro x,y,z : "));
    Serial.print(accel_t_gyro.value.x_gyro, DEC);
    Serial.print((", "));
    Serial.print(accel_t_gyro.value.y_gyro, DEC);
    Serial.print((", "));
    Serial.print(accel_t_gyro.value.z_gyro, DEC);
    Serial.print((", "));
    Serial.println((""));
}

void print_motor_values(quad_motors_t *motor_speeds)
{
    Serial.print(("Top left motor: "));
    Serial.print(motor_speeds->top_left_motor, DEC);
    Serial.println((""));

    Serial.print(("Top right motor: "));
    Serial.print(motor_speeds->top_right_motor, DEC);
    Serial.println((""));

    Serial.print(("Bot left motor: "));
    Serial.print(motor_speeds->bot_left_motor, DEC);
    Serial.println((""));

    Serial.print(("Bot right motor: "));
    Serial.print(motor_speeds->bot_right_motor, DEC);
    Serial.println((""));
}

void print_loop_duration(unsigned long duration)
{
    Serial.print(("Main loop took following amount of milliseconds: "));
    Serial.print(duration, DEC);
    Serial.println((""));
}
