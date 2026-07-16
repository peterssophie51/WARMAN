#include "AccelStepper.h"

//ARM STEPPER MOTOR
#define armDP   //arm direction pin
#define armEP   //arm enable pin
#define armSP   //arm step pin
#define armStepsPerRev   //arm stepper motor steps per revolution 

#define driveDP   //drive direction pin
#define driveEP   //drive enable pin
#define driveSP   //drive step pin
#define driveStepsPerRev   //drive stepper motor steps per revolution

#define extrusionDP   //extrusion direction pin
#define extrusionEP   //extrusion enable pin
#define extrusionSP   //extrusion step pin
#define extrusionStepsPerRev   //extrusion stepper motor steps per revolution

#define scoopDP
#define scoopEP
#define scoopSP
#define scoopStepsPerRev

#define motorInterfaceType 1

AccelStepper armStepper = AccelStepper(motorInterfaceType, armSP, armDP);
AccelSteper driveStepper = AccelStepper(motorInterfaceType, driveSP, driveDP);
AccelStepper extrusionStepper = AccelStepper(motorInterfaceType, extrusionSP, extrusionDP);
AccelStepper scoopStepper = AccelStepper(motorInterfaceType, scoopSP, scoopDP);


void setup() {
  Serial.begin(9600);

  pinMode(armDP, OUTPUT);
  pinMode(armEP, OUTPUT);
  pinMode(armSP, OUTPUT);

  pinMode(driveDP, OUTPUT);
  pinMode(driveEP, OUTPUT);
  pinMode(driveSP, OUTPUT);

  pinMode(extrusionDP, OUTPUT);
  pinMode(extrusionEP, OUTPUT);
  pinMode(extrusionSP, OUTPUT);

  pinMode(scoopDP, OUTPUT);
  pinMode(scoopEP, OUTPUT);
  pinMode(scoopSP, OUTPUT);

  armStepper.setEnablePin(armEP);
  driveStepper.setEnablePin(driveEP);
  extrusionStepper.setEnablePin(extrusionEP);
  scoopStepper.setEnablePin(scoopEP);

  armStepper.setPinsInverted(false, false, true);
  driveStepper.setPinsInverted(false, false, true)
  
}

void loop() {
  
}


//FUNCTION FOR LOWERING THE ARM AND SCOOP TO STARTING POSITION
void lowerArmToStartPosition() {

}

//FUNCTION TO MOVE THE ARM IN A COLLECTION MOVEMENT
void collectionArmMovement() {

}

//FUNCTION TO RETURN ARM TO FINAL POSITION
void moveArmToFinishPosition() {

}

//FUNCTION TO DRIVE SYSTEM
void drive(direction) {

}
