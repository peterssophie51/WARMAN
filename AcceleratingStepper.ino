#include "AccelStepper.h"
#define directionPin 6
#define stepPin 5
#define enablePin 8
#define motorInterfaceType 1
AccelStepper stepper = AccelStepper(motorInterfaceType, stepPin, directionPin);

void setup() {
  // put your setup code here, to run once:
  pinMode(enablePin, OUTPUT);
  digitalWrite(enablePin, LOW);
  stepper.setMaxSpeed(200);
  stepper.setAcceleration(100);
}

void loop() {
  // put your main code here, to run repeatedly:
  stepper.moveTo(200);
  stepper.runToPosition();
  delay(1000);
  stepper.moveTo(0);
  stepper.runToPosition();
  delay(10000);
}
