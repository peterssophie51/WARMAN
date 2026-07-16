#include "AccelStepper.h"
#define directionPin 
#define stepPin
#define enablePin
#define motorInterfaceType 1
AccelStepper driveStepper = AccelStepper(motorInterfaceType, stepPin, directionPin);
const int stepsPerRev = 200;
float rotations = 2;
int count = 0;

void setup() {
  driveStepper.setMaxSpeed(200);
  driveStepper.setAcceleration(100);
  long stepsToMove = rotations * stepsPerRev;
  driveStepper.moveTo(stepsToMove);

}

void loop() {
  driveStepper.run();

  if (driveStepper.distanceToGo() == 0) {
    if (count == 0) {
      driveStepper.moveTo(0);
      count++;
    }
  }
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
*/