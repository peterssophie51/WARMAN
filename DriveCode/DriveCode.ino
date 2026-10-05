#include "AccelStepper.h"
#define directionPin 5
#define stepPin 10
#define motorInterfaceType 1

AccelStepper driveStepper = AccelStepper(motorInterfaceType, stepPin, directionPin);

const int stepsPerRev = 1600;
float rotations = 3;
long stepsToMove = -rotations * stepsPerRev;

enum {stationary, forwards, backwards, end};
unsigned char driveState;

void setup() {
  
  driveStepper.setMaxSpeed(7000);
  driveStepper.setAcceleration(7000);

}

void loop() {
  switch (driveState) {
    case stationary: 
      driveStepper.move(stepsToMove);
      driveState = forwards;
      break;
    case forwards:
      if (driveStepper.distanceToGo() == 0) {
        delay(3000);
        driveState = backwards;
      }
      break;
    case backwards:
      driveStepper.move(-stepsToMove);
      driveState = end;
      break;
    case end:
      break;
  }
  
  Serial.println(driveStepper.speed());
  driveStepper.run();

}