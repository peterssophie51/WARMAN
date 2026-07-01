// define the output pins the arduino is plugged intoi
#include "AccelStepper.h"
#define directionPin 9
#define stepPin 4
#define enablePin 7
#define stepsPerRevolution 200
const int limitPin = A0;
int count = 0;
#define motorInterfaceType 1

AccelStepper stepper = AccelStepper(motorInterfaceType, stepPin, directionPin);

void setup() {
  // declare the pins as outputs
  pinMode(stepPin, OUTPUT);
  pinMode(directionPin, OUTPUT);
  pinMode(enablePin, OUTPUT);
  digitalWrite(enablePin, LOW);
  pinMode(limitPin, INPUT_PULLUP);
  Serial.begin(9600);
  stepper.setMaxSpeed(50);
  stepper.setAcceleration(10);
}

void loop() {
  // set the spinning direction, direction depends on wiring
  int state = digitalRead(limitPin);
  digitalWrite(directionPin, LOW);
  if (state == LOW) {
      Serial.println("Limit switch TRIGGERED");
      count++;
      Serial.println(count);
      stepper.move(200);
      stepper.runToPosition();
  }
  delay(100);

}
