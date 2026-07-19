#include "AccelStepper.h"
#define directionPin 3
#define stepPin 2
#define enablePin 4
#define motorInterfaceType 1
#define onOffSwitch A0

AccelStepper driveStepper = AccelStepper(motorInterfaceType, stepPin, directionPin);

const int stepsPerRev = 1600;
float rotations = -2;
long stepsToMove = rotations * stepsPerRev;

enum {stationary, run};
unsigned char driveState;

void setup() {
  pinMode(enablePin, OUTPUT);
  digitalWrite(enablePin, LOW);
  driveStepper.setMaxSpeed(200);
  driveStepper.setAcceleration(75);
  pinMode(onOffSwitch, INPUT_PULLUP);
  Serial.begin(9600);
}

void loop() {
  int onOffState = digitalRead(onOffSwitch);
  if (onOffState == LOW) {
    Serial.println("On");
    switch (driveState) {
      case stationary: 
        driveStepper.move(stepsToMove);
        driveState = run;
        break;
      case run:
        break;
    }

    driveStepper.run();
  } 
}