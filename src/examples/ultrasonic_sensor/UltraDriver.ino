class UltraDriver
{
private:
  static const int MAX_DISTANCE = 400;
  static const int PULSE_LEN = 10;
  
public:
  int EchoPin;
  int TrigPin;
  int startTime;
  int deltaTime;
  
  ///////////////
  // Interface //
  ///////////////
  
  UltraDriver::UltraDriver(int echoPin, int trigPin)
  {
    EchoPin = echoPin;
    TrigPin = trigPin;
    
    startTime = 0;
    deltaTime = 0;
    
    pinMode(TrigPin, OUTPUT);
    pinMode(EchoPin, INPUT);
  }
  
  void UltraDriver::Start()
  {
    attachInterrupt(EchoPin, EchoPulse, CHANGE);
  }
  
  
  void UltraDriver::Stop()
  {
    detachInterrupt(EchoPin);
  }
  
  void UltraDriver::SendPulse()
  {
    digitalWrite(TrigPin, HIGH);
    delayMicroseconds(PULSE_LEN);
    digitalWrite(TrigPin, LOW); 
  }
  
  int UltraDriver::GetDistance()
  {
    // distance [cm] = time [us] * speed [cm/us]
    //
    // time = deltaTime / 2 [us]                                            (deltaTime is time back and forth)
    // speed = 340 [m/s] = 34000 [cm/s] = 0.0340 [cm/us] = 1/29 [cm/us]     (340 m/s is the speed of sound in air at room temperature)
    //
    // distance = deltaTime / (2 * 29) = deltaTime /58 [cm]
    
    int distance = deltaTime / 58;
    
    if (distance > MAX_DISTANCE)
    {
        distance = -1;
    }
    
    return distance;
  }
  
  ///////////////
  // Interrupt //
  ///////////////
private:
  void UltraDriver::EchoPulse()
  {
    int pulseValue = digitalRead(EchoPin);
    
    if (pulseValue == HIGH)
    {
        startTime = micros();
    }
    else
    {
        deltaTime = micros() - startTime;
    }
  }
}

UltraDriver ultraDriver(53,52);

void setup()
{
  ultraDriver.Start();
  
  Serial.begin(9600);
}


void loop()
{
  ultraDriver.SendPulse();
  
  delay(500);
  
  Serial.print("Distance: ");
  Serial.println(ultraDriver.GetDistance());
}
