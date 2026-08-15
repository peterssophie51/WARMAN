#include "AccelStepper.h"
#define directionPin 5
#define stepPin 6
#define enablePin 7
#define motorInterfaceType 1
#define onOffSwitch A4

AccelStepper driveStepper = AccelStepper(motorInterfaceType, stepPin, directionPin);

const int stepsPerRev = 800;
float rotations = 10;
long stepsToMove = -rotations * stepsPerRev;
int onPresses = 0;

enum {stationary, forwards, backwards, end};
unsigned char driveState;

void setup() {
  driveStepper.setEnablePin(enablePin);
  driveStepper.setPinsInverted(false, false, true);
  driveStepper.disableOutputs();

  driveStepper.setMaxSpeed(4000);
  driveStepper.setAcceleration(1000);

  pinMode(onOffSwitch, INPUT_PULLUP);
}

void loop() {
  int onOffState = digitalRead(onOffSwitch);

  if (onOffState == LOW) {
    onPresses++;
  }
  
  if (onPresses > 0) {
    driveStepper.enableOutputs();
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
    driveStepper.run();
  } else {
    driveStepper.disableOutputs();
  }
}