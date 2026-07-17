#include "AccelStepper.h"
#define directionPin 10
#define stepPin 8
#define enablePin 12
#define motorInterfaceType 1
AccelStepper driveStepper = AccelStepper(motorInterfaceType, stepPin, directionPin);
const int stepsPerRev = 400;
float rotations = 3;
int count = 0;

void setup() {
  driveStepper.setMaxSpeed(60);
  driveStepper.setAcceleration(30);
  long stepsToMove = rotations * stepsPerRev;
  driveStepper.moveTo(stepsToMove);

}

void loop() {
  driveStepper.run();
}

/*
void setup() {
  pinMode(directionPin, OUTPUT);
  pinMode(stepPin, OUTPUT);
  long stepsToMove = rotations * stepsPerRev;

  digitalWrite(directionPin, HIGH);
  for (int i = 0; i < stepsToMove; i++) {
    digitalWrite(stepPin, HIGH);
    delayMicroseconds(3000);
    digitalWrite(stepPin, LOW);
    delayMicroseconds(3000);
  }

  digitalWrite(directionPin, LOW);
  for (int i = 0; i < stepsToMove; i++) {
    digitalWrite(stepPin, HIGH);
    delayMicroseconds(3000);
    digitalWrite(stepPin, LOW);
    delayMicroseconds(3000);
  }

}

void loop() {

}
*/