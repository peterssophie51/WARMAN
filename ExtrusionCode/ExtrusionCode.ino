#include "AccelStepper.h"
#define directionPin 6
#define stepPin 5
#define enablePin 7
#define motorInterfaceType 1
#define onOffSwitch A0

const int stepsPerRevolution = 6400;
float revolutions = 1;
long stepsToTake = stepsPerRevolution * revolutions;
AccelStepper extrusionStepper = AccelStepper(motorInterfaceType, stepPin, directionPin);

enum {stationary, extrude, retract, end};
unsigned char extrudeState;

void setup() {
  extrusionStepper.setEnablePin(enablePin);
  extrusionStepper.setPinsInverted(false, false, true);
  extrusionStepper.disableOutputs();

  extrusionStepper.setMaxSpeed(800);
  extrusionStepper.setAcceleration(400);

  pinMode(onOffSwitch, INPUT_PULLUP);
}

void loop() {
  int onOffState = digitalRead(onOffSwitch);
  if (onOffState == LOW) {
    extrusionStepper.enableOutputs();
    switch (extrudeState) {
      case stationary:
        extrusionStepper.move(stepsToTake);
        extrudeState = extrude;
        break;
      case extrude:
        if (extrusionStepper.distanceToGo() == 0) {
          delay(3000);
          extrudeState = retract;
        }
        break;
      case retract:
        extrusionStepper.move(-stepsToTake);
        extrudeState = end;
        break;
      case end:
        break;
    }
    extrusionStepper.run();
  } else {
    extrusionStepper.disableOutputs();
  }

}
