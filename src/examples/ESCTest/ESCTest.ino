#include <Servo.h> 
 
#define ARDUINO_ESC_PIN_1 (2)
#define ARDUINO_ESC_PIN_2 (3)
#define ARDUINO_ESC_PIN_3 (4)
#define ARDUINO_ESC_PIN_4 (5)

Servo esc_servo_1, esc_servo_2, esc_servo_3, esc_servo_4;  // create servo object to control a servo 
                // a maximum of eight servo objects can be created 


void setup()
{
  esc_servo_1.attach(ARDUINO_ESC_PIN_1);
  esc_servo_2.attach(ARDUINO_ESC_PIN_2);
  esc_servo_3.attach(ARDUINO_ESC_PIN_3);
  esc_servo_4.attach(ARDUINO_ESC_PIN_4);

  Serial.begin(9600);
  Serial.println("Setup()");
  esc_servo_1.write(0);
  esc_servo_2.write(0);
  esc_servo_3.write(0);
  esc_servo_4.write(0);
}

void loop()
{
  static int current_speed = 0;
    
  current_speed = get_speed_from_remote(current_speed);
  
  current_speed = get_speed_from_serial(current_speed);

  set_speed(current_speed);
}

int get_speed_from_serial(int current_speed)
{
  int temp_speed;
  if (Serial.available() > 0)
  {
    temp_speed = Serial.parseInt();
    
    if ((temp_speed >= 0 ) && (temp_speed <= 180))
    {
      Serial.print("Setting motor speed: ");
      Serial.println(temp_speed, DEC);
      return temp_speed;
    }
    else
    {
      Serial.println("Speed has to be within the range [0, 180])");
      return current_speed;
    }
  }
  return current_speed;
}

int get_speed_from_remote(int current_speed)
{
  return current_speed;
}

void set_speed(int current_speed)
{
  esc_servo_1.write(current_speed);
  esc_servo_2.write(current_speed);
  esc_servo_3.write(current_speed);
  esc_servo_4.write(current_speed);
}
