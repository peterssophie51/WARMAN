#include "AccelStepper.h"

//ARM STEPPER MOTOR
#define armDP 9   //arm direction pin
#define armEP 10   //arm enable pin
#define armSP 8   //arm step pin
const int armStepsPerRev = 200;   //arm stepper motor steps per revolution 
float armRevolutions = 1;   //revolutions arm stepper moves through 
long armSteps = armStepsPerRev * armRevolutions;   //steps for arm stepper motor to take

#define driveDP 3     //drive direction pin
#define driveEP 4   //drive enable pin
#define driveSP 2   //drive step pin
const int driveStepsPerRev = 6400;   //drive stepper motor steps per revolution
float driveRevolutions = 1;   //revolutions drive stepper moves through
long driveSteps = driveStepsPerRev * driveRevolutions * -1;   //steps for drive stepper motor to take

#define extrusionDP 6   //extrusion direction pin
#define extrusionEP 7   //extrusion enable pin
#define extrusionSP 5   //extrusion step pin
const int extrusionStepsPerRev = 6400 ;  //extrusion stepper motor steps per revolution
float extrusionRevolutions = 1;  //revolutions extrusion stepper moves through
long extrusionSteps = extrusionStepsPerRev * extrusionRevolutions;   //steps for extrusion stepper motor to take

#define scoopDP 12   //scoop direction pin
#define scoopEP 13   //scoop enable pin
#define scoopSP 11   //scoop step pin
const int scoopStepsPerRev = 200;   //scoop stepper motor steps per revolution
float scoopRevolutions = 1;  //revolutions scoop stepper moves through
long scoopSteps = scoopStepsPerRev * scoopRevolutions;   //steps for scoop stepper motor to take

#define motorInterfaceType 1

AccelStepper armStepper = AccelStepper(motorInterfaceType, armSP, armDP);   //arm stepper motor
AccelStepper driveStepper = AccelStepper(motorInterfaceType, driveSP, driveDP);   //drive stepper motor
AccelStepper extrusionStepper = AccelStepper(motorInterfaceType, extrusionSP, extrusionDP);   //extrusion stepper motor
AccelStepper scoopStepper = AccelStepper(motorInterfaceType, scoopSP, scoopDP);   //scoop stepper motor

enum {stationary, forward, backward, end};   //system states
unsigned char systemState;   //state to track system state

#define onSwitch A0   //on/off switch analog pin

void setup() {
  //set up connection with laptop for print testing
  Serial.begin(9600);

  //arm stepper motor enabling
  armStepper.setEnablePin(armEP);
  armStepper.setPinsInverted(false, false, true);
  armStepper.disableOutputs();

  //drive stepper motor enabling
  driveStepper.setEnablePin(driveEP);
  driveStepper.setPinsInverted(false, false, true);
  driveStepper.disableOutputs();

  //extrusion steper motor enabling
  extrusionStepper.setEnablePin(extrusionEP);
  extrusionStepper.setPinsInverted(false, false, true);
  extrusionStepper.disableOutputs();

  //scoop stepper motor enabling
  scoopStepper.setEnablePin(scoopEP);
  scoopStepper.setPinsInverted(false, false, true);
  scoopStepper.disableOutputs();

  //setting arm stepper motor speeds
  armStepper.setMaxSpeed(200);
  armStepper.setAcceleration(100);

  //setting drive stepper motor speeds
  driveStepper.setMaxSpeed(1600);
  driveStepper.setAcceleration(400);

  //extrusion stepper motor speeds
  extrusionStepper.setMaxSpeed(1000);
  extrusionStepper.setAcceleration(400);

  //drive stepper motor speeds
  driveStepper.setMaxSpeed(300);
  driveStepper.setAcceleration(100);

  //setting on/off switch as input
  pinMode(onSwitch, INPUT_PULLUP);
}

void loop() {
  int onState = digitalRead(onSwitch);  //read for switch inputs

  if (onState == LOW) {
    driveStepper.enableOutputs();   //enable drive stepper
    extrusionStepper.enableOutputs();   //enable extrusion stepper
    
    switch (systemState) {
      case stationary:   //when first started, extend out extrusions and drive forward
        driveStepper.move(driveSteps);
        extrusionStepper.move(extrusionSteps);
        systemState = forward;   //move onto next state
        break;
      case forward:   //wait for both processes to happen
        if (driveStepper.distanceToGo() == 0 and extrusionStepper.distanceToGo() == 0) {
          delay(3000);
          systemState = backward;
        }
        break;
      case backward:
        driveStepper.move(-driveSteps);
        extrusionStepper.move(-extrusionSteps);
        systemState = end;
        break;
    }

    driveStepper.run();
    extrusionStepper.run();

  } else {
    driveStepper.disableOutputs();
    extrusionStepper.disableOutputs();
  }
  
}



