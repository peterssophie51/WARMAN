// define the output pins the arduino is plugged intoi
#define directionPin 2
#define stepPin 5
#define enablePin 9
#define stepsPerRevolution 200

void setup() {
  // declare the pins as outputs
  pinMode(stepPin, OUTPUT);
  pinMode(directionPin, OUTPUT);
  pinMode(enablePin, OUTPUT);
  digitalWrite(enablePin, LOW);

}

void loop() {
  // set the spinning direction, direction depends on wiring
  digitalWrite(directionPin, LOW);

  //spin the stepper motor in 1 revolution
  for (int i = 0; i < stepsPerRevolution; i++) {
    digitalWrite(stepPin, HIGH);
    delayMicroseconds(2000);
    digitalWrite(stepPin, LOW);
    delayMicroseconds(2000);
  }

   digitalWrite(directionPin, HIGH);
   delay(10000);
}
