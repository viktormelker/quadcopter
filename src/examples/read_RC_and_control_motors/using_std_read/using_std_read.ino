#include <Servo.h>

#define RC_1 44 // RC input pins.
#define RC_2 46
#define RC_3 48
/*#define RC_5 48 // Ev. för att säga till att hålla höjden.                  */
#define SERVO_1 13 // Servo pin.
#define SERVO_2 12 // Ok kanal?
#define SERVO_3 11 // Ok kanal?
#define SERVO_4 10 // Ok kanal?

// Create a servo object.
Servo myservo_1;
Servo myservo_2;
Servo myservo_3;
Servo myservo_4;
// Raw RC input, PPM, [us].
int pulsew_ch_1 = 0;
int pulsew_ch_2 = 0;
int pulsew_ch_3 = 0;
/*int pulsew_ch_5 = 0;                                                               */
// Scaled RC input [degrees].
int degrees_thrust = 0;
int degrees_pitch = 0;
int degrees_roll = 0;
/*bool keep_height = false;                                                                   */
//  Output to engines [degrees].
int engine_1 = 0; // Front left engine.
int engine_2 = 0; // Front right engine.
int engine_3 = 0; // Back right engine.
int engine_4 = 0; // Back left engine.

/* Calculates the output for each engine. */
void calculate_engine_thrust () {
  engine_1 = degrees_thrust - degrees_pitch + degrees_roll;
  engine_2 = degrees_thrust - degrees_pitch - degrees_roll;
  engine_3 = degrees_thrust + degrees_pitch - degrees_roll;
  engine_4 = degrees_thrust + degrees_pitch + degrees_roll;
}


/* Make sure input from RC is within valid ranges. */
int bounds_check_input (int pulsew) {
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

/* Returns boolean True if height is to be kept. */
/*bool keep_height (int pulsew) {
  bool result = False;
  pulsew = bounds_check_input (pulsew);
  if (pulsew > 1800) {
    result = True;
  }
  return result;
} /*

/* Scales pulsewidth in microseconds to degrees.
   1100 <= Input <= 1900.
   1100us => 0deg, 1900us => 180deg. 
   Output to be used in height control.
   Will be applied to all motors. */
int scale_vert_movement (int pulsew) {
  pulsew = bounds_check_input (pulsew);
  return 9 * pulsew / 80 - 135/4;
}

/* Scales pulsewidth in microseconds to degrees.
   1100 <= Input <= 1900.
   1100us => -90deg, 1500us => 0deg, 1900us => 90deg.
   Output to be used for pitch and roll. */
int scale_horiz_movement (int pulsew) {
  pulsew = bounds_check_input (pulsew);
  return 9 * pulsew / 40 - 675/2;
}

/* Invokes scaling of input. */
void scale_input() {  
  /*keep_height = scale_keep_height (pelsew_ch_5);
  if keep_height {
      degrees_pitch = 0;
      degrees_roll = 0;
      degrees_thrust beror på föregående läst  höjdvärde.
    } else {                                                                                                 */
  degrees_thrust = scale_vert_movement (pulsew_ch_1); 
  degrees_roll = scale_horiz_movement (pulsew_ch_2);
  degrees_pitch = scale_horiz_movement (pulsew_ch_3);
  /*}                                                                                                        */
}

/* Calculate high time. 
   Note: Program pauses when waiting. */
void read_RC_input() { 
  // Timeout 4200us ~= 2*2000 = 2 pulses should be received.
  pulsew_ch_1 = pulseIn(RC_1, HIGH, 42000);
  pulsew_ch_2 = pulseIn(RC_2, HIGH, 42000);
  pulsew_ch_3 = pulseIn(RC_3, HIGH, 42000);
  /*  pulsew_ch_5 = pulseIn(RC_3, HIGH, 4200);                                            */  
}


void setup() {
  Serial.begin(9600);
  Serial.println("Setup()");
  
  // Assign input pins.
  pinMode(RC_1, INPUT);
  pinMode(RC_2, INPUT);
  pinMode(RC_3, INPUT);
  /*pinMode(RC_5, INPUT);                                                                 */
  // Assign output pins.
  myservo_1.attach(SERVO_1);
  myservo_2.attach(SERVO_2);
  myservo_3.attach(SERVO_3);
  myservo_4.attach(SERVO_4);
  
  // Write 0 degrees to ESC, means fully counter clockwise.
  // But when using the RC remot 1500 is the mittenläge and
  // all pulsewidth below that (down to lowest @1100us)
  // does not give any signal at all. So test degrees and see 
  // when the motor start rotating!
  myservo_1.write(0);
  myservo_2.write(0);
  myservo_3.write(0);
  myservo_4.write(0);
}

void loop() {
  read_RC_input;
  scale_input;    
  calculate_engine_thrust;

  myservo_1.write(engine_1);
  myservo_2.write(engine_2);
  myservo_3.write(engine_3);
  myservo_3.write(engine_4);
  
  Serial.print("Ch1: ");
  Serial.print(pulsew_ch_1, DEC);
  Serial.print(" Ch2: ");
  Serial.print(pulsew_ch_2, DEC);
  Serial.print(" Ch3: ");
  Serial.print(pulsew_ch_3, DEC);
  Serial.print("\n");
  /*
  Serial.print("Engine 1 degrees: ");
  Serial.print(engine_1_out, DEC);
  Serial.print("Engine 2 degrees: ");
  Serial.print(engine_2_out, DEC);
  Serial.print("Engine 3 degrees: ");
  Serial.print(engine_3_out, DEC);
  Serial.print("Engine 4 degrees: ");
  Serial.print(engine_4_out, DEC);
 */ 
  delay(20);  // [ms] Servo won't be updated more often than this.
}
