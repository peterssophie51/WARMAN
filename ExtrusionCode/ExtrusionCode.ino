#include "AccelStepper.h"
#define directionPin 6
#define stepPin 4
#define enablePin 8
#define motorInterfaceType 1
const int stepsPerRevolution = 200;
const int revolutions = 1;
long stepsToTake = stepsPerRevolution * revolutions * -1;
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
  stepper.moveTo(stepsToTake);
  stepper.runToPosition();

  stepper.moveTo(0);
  stepper.runToPosition();

}
