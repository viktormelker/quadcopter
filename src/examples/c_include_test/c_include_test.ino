
int test;

void setup() {
  Serial.begin(115200); // Open serial monitor at 115200 baud to see ping results.
}

void loop() {
  test = myfun();
  Serial.print("Result = ");
}
