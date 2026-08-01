#include "AccelStepper.h"

//ARM 1 NEMA 17
#define arm1DP 9   //arm 1 direction pin
#define arm1EP 10   //arm 1 enable pin
#define arm1SP 8   //arm 1 step pin
const int arm1StepsPerRev = 200;   //arm stepper motor steps per revolution 
float arm1Revolutions = 1;   //revolutions arm stepper moves through 
long arm1Steps = arm1StepsPerRev * arm1Revolutions;   //steps for arm stepper motor to take

//ARM 2 NEMA 17
#define arm2DP 12   //arm 2 direction pin
#define arm2EP 13   //arm 2 enable pin
#define arm2SP 11   //arm 2 step pin
const int arm2StepsPerRev = 200;   //arm 2 stepper motor steps per revolution
float arm2Revolutions = 1;  //revolutions = arm 2 stepper moves through
long arm2Steps = arm2StepsPerRev * arm2Revolutions;   //steps for arm 2 stepper motor to take

//DRIVE NEMA 23
#define driveDP 3     //drive direction pin
#define driveEP 4   //drive enable pin
#define driveSP 2   //drive step pin
const int driveStepsPerRev = 6400;   //drive stepper motor steps per revolution
float driveRevolutions = 2;   //revolutions drive stepper moves through
long driveSteps = driveStepsPerRev * driveRevolutions * -1;   //steps for drive stepper motor to take

//EXTRUSION NEMA 23
#define extrusionDP 6   //extrusion direction pin
#define extrusionEP 7   //extrusion enable pin
#define extrusionSP 5   //extrusion step pin
const int extrusionStepsPerRev = 6400 ;  //extrusion stepper motor steps per revolution
float extrusionRevolutions = 2;  //revolutions extrusion stepper moves through
long extrusionSteps = extrusionStepsPerRev * extrusionRevolutions;   //steps for extrusion stepper motor to take

#define motorInterfaceType 1 

AccelStepper arm1Stepper = AccelStepper(motorInterfaceType, arm1SP, arm1DP);   //arm 1 stepper motor
AccelStepper arm2Stepper = AccelStepper(motorInterfaceType, arm2SP, arm2DP);   //arm 2 stepper motor
AccelStepper driveStepper = AccelStepper(motorInterfaceType, driveSP, driveDP);   //drive stepper motor
AccelStepper extrusionStepper = AccelStepper(motorInterfaceType, extrusionSP, extrusionDP);   //extrusion stepper motor

enum {stationary, forward, backward, end};   //system states
unsigned char systemState;   //state to track system state

#define onSwitch A0   //on/off switch analog pin

void setup() {
  //set up connection with laptop for print testing
  Serial.begin(9600);

  //arm 1 stepper motor enabling
  arm1Stepper.setEnablePin(arm1EP);
  arm1Stepper.setPinsInverted(false, false, true);
  arm1Stepper.disableOutputs();

  //arm 2 stepper motor enabling
  arm2Stepper.setEnablePin(arm2EP);
  arm2Stepper.setPinsInverted(false, false, true);
  arm2Stepper.disableOutputs();

  //drive stepper motor enabling
  driveStepper.setEnablePin(driveEP);
  driveStepper.setPinsInverted(false, false, true);
  driveStepper.disableOutputs();

  //extrusion steper motor enabling
  extrusionStepper.setEnablePin(extrusionEP);
  extrusionStepper.setPinsInverted(false, false, true);
  extrusionStepper.disableOutputs();

  //setting arm 1 stepper motor speeds
  arm1Stepper.setMaxSpeed(200);
  arm1Stepper.setAcceleration(100);

  //setting arm 2 stepper motor speeds
  driveStepper.setMaxSpeed(2000);
  driveStepper.setAcceleration(1000);

  //setting drive stepper motor speeds
  driveStepper.setMaxSpeed(1600);
  driveStepper.setAcceleration(400);

  //setting extrusion stepper motor speeds
  extrusionStepper.setMaxSpeed(2000);
  extrusionStepper.setAcceleration(10000);

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
      case forward:   //once both nema 23 movements triggerred 
        if (driveStepper.distanceToGo() == 0 and extrusionStepper.distanceToGo() == 0) { //wait for both processes to happen
          delay(3000); //wait for 3 seconds
          systemState = backward;  //move onto next state
        }
        break;
      case backward:
        driveStepper.move(-driveSteps); //move drive and extrusions back in the opposite direction for the same amount
        extrusionStepper.move(-extrusionSteps);
        systemState = end; //finish the movement
        break;
    }

    //run whatever movement the steppers have programmed to them
    driveStepper.run();
    extrusionStepper.run();

  } else {
    //when switch turned off disable the stepper motors
    driveStepper.disableOutputs();
    extrusionStepper.disableOutputs();
  }
  
}



