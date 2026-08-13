#include "AccelStepper.h"
#define extrusionDP 8   //extrusion direction pin
#define extrusionEP 10   //extrusion enable pin
#define extrusionSP 9   //extrusion step pin
const int extrusionStepsPerRev = 3200 ;  //extrusion stepper motor steps per revolution
float extrusionRevolutions = 5;  //revolutions extrusion stepper moves through
long extrusionSteps = extrusionStepsPerRev * extrusionRevolutions;   //steps for extrusion stepper motor to take
const int extrusionSpeed = 6400;  //extrusion speed (steps per second)
const int extrusionAcceleration = 4000;  //extrusion acceleration (steps per second per second)

#define motorInterfaceType 1
#define onSwitch A4
int onPresses = 0;
AccelStepper extrusionStepper = AccelStepper(motorInterfaceType, extrusionSP, extrusionDP); 

enum {stationary, extrude, retract, end};
unsigned char extrudeState;

void setup() {
  extrusionStepper.setEnablePin(extrusionEP);
  extrusionStepper.setPinsInverted(false, false, true);
  extrusionStepper.disableOutputs();

  extrusionStepper.setMaxSpeed(extrusionSpeed);
  extrusionStepper.setAcceleration(extrusionAcceleration);

  pinMode(onSwitch, INPUT_PULLUP);
}

void loop() {
  int onState = digitalRead(onSwitch);

  if (onState == LOW) {
    onPresses++;
  }

  if (onPresses > 0) {
    extrusionStepper.enableOutputs();
    switch (extrudeState) {
      case stationary:
        extrusionStepper.move(-extrusionSteps);
        extrudeState = extrude;
        break;
      case extrude:
        if (extrusionStepper.distanceToGo() == 0) {
          delay(3000);
          extrudeState = end;
        }
        break;
      case end:
        extrusionStepper.disableOutputs();
        break;
    }
    extrusionStepper.run();
  } else {
    extrusionStepper.disableOutputs();
  }

}
