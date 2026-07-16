// define the output pins the arduino is plugged intoi
#define directionPin 4
#define stepPin 2
#define enablePin 6
#define stepsPerRevolution 200
int steps = stepsPerRevolution * 10;

void setup() {
  // declare the pins as outputs
  pinMode(stepPin, OUTPUT);
  pinMode(directionPin, OUTPUT);
  pinMode(enablePin, OUTPUT);
  digitalWrite(enablePin, LOW);
  

}

void loop() {
  // set the spinning direction, direction depends on wiring
  digitalWrite(directionPin, HIGH);

  //spin the stepper motor in 1 revolution
  for (int i = 0; i < stepsPerRevolution; i++) {
    digitalWrite(stepPin, HIGH);
    delayMicroseconds(3000);
    digitalWrite(stepPin, LOW);
    delayMicroseconds(3000);
  }

}
