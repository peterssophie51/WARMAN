#include "AccelStepper.h"

//ARM STEPPER MOTOR
#define armDP 9   //arm direction pin
#define armEP 10   //arm enable pin
#define armSP 8   //arm step pin
#define armStepsPerRev 200   //arm stepper motor steps per revolution 

#define driveDP 3     //drive direction pin
#define driveEP 4   //drive enable pin
#define driveSP 2   //drive step pin
#define driveStepsPerRev 1600   //drive stepper motor steps per revolution

#define extrusionDP 6   //extrusion direction pin
#define extrusionEP 7   //extrusion enable pin
#define extrusionSP 5   //extrusion step pin
#define extrusionStepsPerRev 6400   //extrusion stepper motor steps per revolution

#define scoopDP 12   //scoop direction pin
#define scoopEP 13   //scoop enable pin
#define scoopSP 11   //scoop step pin
#define scoopStepsPerRev 200   //scoop stepper motor steps per revolution

#define motorInterfaceType 1

AccelStepper armStepper = AccelStepper(motorInterfaceType, armSP, armDP);
AccelSteper driveStepper = AccelStepper(motorInterfaceType, driveSP, driveDP);
AccelStepper extrusionStepper = AccelStepper(motorInterfaceType, extrusionSP, extrusionDP);
AccelStepper scoopStepper = AccelStepper(motorInterfaceType, scoopSP, scoopDP);

enum {stationary, forward, backwards, end};
unsigned char systemState;

void setup() {
  Serial.begin(9600);

  armStepper.setEnablePin(armEP);
  driveStepper.setEnablePin(driveEP);
  extrusionStepper.setEnablePin(extrusionEP);
  scoopStepper.setEnablePin(scoopEP);

  armStepper.setPinsInverted(false, false, true);
  driveStepper.setPinsInverted(false, false, true);
  extrusionStepper.setPinsInverted(false, false, true);
  scoopStepper.setPinsInverted(false, false, true);

  armStepper.disableOutputs();
  driveSteppeer.disableOutputs();
  extrusionStepper.disableOutputs();
  scoopStepper.disableOutputs();
  
}

void loop() {
  
}



