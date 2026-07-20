#include "AccelStepper.h"
#define armEnablePin 10
#define armStepPin 8
#define armDirectionPin 9
#define scoopEnablePin 13
#define scoopStepPin 11
#define scoopDirectionPin 12

#define motorInterfaceType 1
#define onOffSwitch A0

const int armStepsPerRevolution = 200;
const int scoopStepsPerRevolution = 200;

AccelStepper armStepper = AccelStepper(motorInterfaceType, armStepPin, armDirectionPin);
AccelStepper scoopStepper = AccelStepper(motorInterfaceType, scoopStepPin, scoopDirectionPin);

enum {stationary, moved, end};
unsigned char movedState;

void setup() {
  armStepper.setEnablePin(armEnablePin);
  armStepper.setPinsInverted(false, false, true);
  armStepper.disableOutputs();

  scoopStepper.setEnablePin(scoopEnablePin);
  scoopStepper.setPinsInverted(false, false, true);
  scoopStepper.disableOutputs();

  armStepper.setAcceleration(50);
  armStepper.setMaxSpeed(100);

  pinMode(onOffSwitch, INPUT_PULLUP);
}

void loop() {
  int onOffState = digitalRead(onOffSwitch);
  if (onOffState == LOW) {
    scoopStepper.enableOutputs();
    switch (movedState) {
      case stationary:
        scoopStepper.move(100);
        movedState = moved;
        break;
      case moved:
        break;
    }

  } else {
    scoopStepper.disableOutputs(); // allows a fresh 100-step move next time switch goes LOW
  }
  scoopStepper.run();
}