void setup() {
  // put your setup code here, to run once:
  int debug = 1;  // set debug to 1 for printouts on serial port
  int loop_frequency = 100;   // set this to the frequency with which the loop should be executed [Hz]
  int loop_delay = 1000/loop_frequency;  // loop duration in ms
  
  if (debug ==1)
  {
    Serial.begin(9600);
  }
}

void loop() {
  // put your main code here, to run repeatedly:
  
  int start_of_loop = //get current time from cpu
  int end_of_loop =   //calculate time to finish loop
  
  if (debug ==1)
  {
    Serial.write("starting loop at time : " );
  }
  
  // do something
  
  int temp_time =    //get current time from cpu
  
  if (temp_time<end_of_loop)
  {
    if (debug ==1)
    {
      Serial.write();
    }
    delay(end_of_loop-temp_time);
  }
  
  
  
  
}
