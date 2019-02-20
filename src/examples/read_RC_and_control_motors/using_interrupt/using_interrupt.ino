#include <Servo.h>

#define RC_1 42 // RC input pins.
#define RC_2 44
#define RC_3 46
#define RC_4 48

#define SERVO_1 2 // Servo pin.
#define SERVO_2 3
#define SERVO_3 4
#define SERVO_4 5

// Create a servo object.
Servo myservo_1;
Servo myservo_2;
Servo myservo_3;
Servo myservo_4;
// Help when calculating PPM time, [us].
volatile unsigned long ISR_1_high_time = 0;  //  Volatile because its used in interrupt.
volatile unsigned long ISR_2_high_time = 0;
volatile unsigned long ISR_3_high_time = 0;
volatile unsigned long ISR_4_high_time = 0;
// Raw RC input, PPM, [us].
volatile int pulsew_ch_1 = 0;
volatile int pulsew_ch_2 = 0;
volatile int pulsew_ch_3 = 0;
volatile int pulsew_ch_4 = 0;
// Scaled and filtered axis values [us].
int thrust = 0;
int pitch = 0;
int roll = 0;
int yaw = 0;
//  Output to engines [us].
int engine_1_out = 0; // Front left engine.
int engine_2_out = 0; // Front right engine.
int engine_3_out = 0; // Back right engine.
int engine_4_out = 0; // Back left engine.

// Calibration values of the separate engines
int calibration_e1 = 0;
int calibration_e2 = 0;
int calibration_e3 = 0;
int calibration_e4 = 0;

//  Boolean True if interrupts shall do anything, shall be false within main loop.
volatile bool ISR_on = false;

/* Calculates the output for each engine. */
void calculate_engine_output () {
  engine_1_out = thrust - pitch + roll + yaw + calibration_e1;
  engine_2_out = thrust - pitch - roll - yaw + calibration_e2;
  engine_3_out = thrust + pitch - roll + yaw + calibration_e3;
  engine_4_out = thrust + pitch + roll - yaw + calibration_e4;
}


/* Make sure pulsewidth is within valid ranges. */
int bounds_check_pulse (int pulsew) {
  int result = 0;
  
  if (pulsew < 1100) {
    result = 1100;
  } else if (pulsew > 1900) {
    result = 1900;
  } else {
   result = pulsew;   
  }
  return result;
}

/* Scales height movement, affects all rotors equally.
   Output to be used in height control.
   Will be applied to all motors. */
int scale_vert_movement (int pulsew) {
  pulsew = bounds_check_pulse (pulsew);
  return pulsew;
}

/* Scales horizontal movement.
   Output to be used for pitch and roll. */
int scale_horiz_movement (int pulsew) {
  pulsew = bounds_check_pulse (pulsew);
  pulsew = pulsew - 1500;
  pulsew = pulsew * 1;  //  Should be 4.75 (1900-1500=400, 400*4.75=1900), but we want some slack.
  return pulsew;
}

/* Invokes scaling of output signals. */
void bounds_check_output() {
  engine_1_out = bounds_check_pulse (engine_1_out); 
  engine_2_out = bounds_check_pulse (engine_2_out); 
  engine_3_out = bounds_check_pulse (engine_3_out); 
  engine_4_out = bounds_check_pulse (engine_4_out); 
}

/* Invokes scaling of input. */
void scale_input() {
  thrust = scale_vert_movement  (pulsew_ch_3); 
  roll   = scale_horiz_movement (pulsew_ch_1);
  pitch  = scale_horiz_movement (pulsew_ch_2);
  yaw    = scale_horiz_movement (pulsew_ch_4);
}

/* Help function for interrupts when calculating PPM high time.*/
void update_ISR_variables (const int pin, volatile unsigned long *high_time, volatile int *pulsew)
{
  volatile unsigned long curr_time;  
  curr_time = micros();  //  Time since system start.
    
  //  Only update variables when allowed to.
  if (ISR_on == true) {
    if (digitalRead (pin) == HIGH) {
      *high_time = curr_time;
    }
    else {      
      //  Check for wrap around of internal clock.
      if (curr_time < *high_time) {
        *pulsew = curr_time + sizeof(curr_time) - *high_time;
      } 
      else {      
        *pulsew = curr_time - *high_time;
      }
    }
  }
}

/* Interrupt routines
   Calculates high time of input on a pin. */
void ISR_1 () {
  update_ISR_variables (RC_1, &ISR_1_high_time, &pulsew_ch_1);
}

void ISR_2 () {
  update_ISR_variables (RC_2, &ISR_2_high_time, &pulsew_ch_2);
}

void ISR_3 () {
  update_ISR_variables (RC_3, &ISR_3_high_time, &pulsew_ch_3);
}

void ISR_4 () {
  update_ISR_variables (RC_4, &ISR_4_high_time, &pulsew_ch_4);
}


void setup() {
  Serial.begin(9600);
  Serial.println("Setup()");
  
  // Assign interrupt (input from remote).
  attachInterrupt(RC_1, ISR_1, CHANGE);
  attachInterrupt(RC_2, ISR_2, CHANGE);
  attachInterrupt(RC_3, ISR_3, CHANGE);
  attachInterrupt(RC_4, ISR_4, CHANGE);
  // Assign output pins.
  myservo_1.attach(SERVO_1);
  myservo_2.attach(SERVO_2);
  myservo_3.attach(SERVO_3);
  myservo_4.attach(SERVO_4);
  
  // Write min value to ESC so that they are not moving.
  myservo_1.writeMicroseconds(1100);
  myservo_2.writeMicroseconds(1100);
  myservo_3.writeMicroseconds(1100);
  myservo_4.writeMicroseconds(1100);
}


void loop(){
  //  Turn of interupts in the main loop.
  ISR_on = false;   
  
  scale_input();
  calculate_engine_output();
  bounds_check_output();
  /*
  Serial.print("Ch1: ");
  Serial.print(pulsew_ch_1, DEC);
  Serial.print(" Ch2: ");
  Serial.print(pulsew_ch_2, DEC);
  Serial.print(" Ch3: ");
  Serial.print(pulsew_ch_3, DEC);
  */
  Serial.print("Ch1 out: ");
  Serial.print(engine_1_out, DEC);
  Serial.print("Ch2 out: ");
  Serial.print(engine_2_out, DEC);
  Serial.print("Ch3 out: ");
  Serial.print(engine_3_out, DEC);
  Serial.print("Ch4 out: ");
  Serial.print(engine_4_out, DEC);
  Serial.print("\n");

  myservo_1.writeMicroseconds(engine_1_out);
  myservo_2.writeMicroseconds(engine_2_out);
  myservo_3.writeMicroseconds(engine_3_out);
  myservo_4.writeMicroseconds(engine_4_out);
  
  //  Turn on interrupts again.
  ISR_on = true;
  delay(100);  // [ms].
}
