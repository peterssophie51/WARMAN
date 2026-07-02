#include "AccelStepper.h"
#define directionPin 12
#define stepPin 10
#define enablePin 8
#define motorInterfaceType 1
AccelStepper stepper = AccelStepper(motorInterfaceType, stepPin, directionPin);

void setup() {
  // put your setup code here, to run once:
  pinMode(enablePin, OUTPUT);
  digitalWrite(enablePin, LOW);
  stepper.setMaxSpeed(100);
  stepper.setAcceleration(50);
}

void loop() {
  // put your main code here, to run repeatedly:
  stepper.moveTo(1600);
  stepper.runToPosition();
  delay(1000);
  stepper.moveTo(0);
  stepper.runToPosition();
  delay(10000);
}
