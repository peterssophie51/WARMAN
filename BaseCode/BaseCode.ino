#include "AccelStepper.h"
#include "Servo.h"

//ARM 1 NEMA 17
#define arm1DP 11   //arm 1 direction pin
#define arm1EP 13   //arm 1 enable pin
#define arm1SP 12   //arm 1 step pin
const int arm1StepsPerRev = 200;   //arm stepper motor steps per revolution 
float arm1Revolutions = 1;   //revolutions arm stepper moves through 
long arm1Steps = arm1StepsPerRev * arm1Revolutions;   //steps for arm stepper motor to take

//ARM 2 NEMA 17
#define arm2DP 2   //arm 2 direction pin
#define arm2EP 4   //arm 2 enable pin
#define arm2SP 3   //arm 2 step pin
const int arm2StepsPerRev = 200;   //arm 2 stepper motor steps per revolution
float arm2Revolutions = 1;  //revolutions = arm 2 stepper moves through
long arm2Steps = arm2StepsPerRev * arm2Revolutions;   //steps for arm 2 stepper motor to take

//DRIVE NEMA 23
#define driveDP 5    //drive direction pin
#define driveEP 7   //drive enable pin
#define driveSP 6   //drive step pin
const int driveStepsPerRev = 6400;   //drive stepper motor steps per revolution
float driveRevolutions = 8;   //revolutions drive stepper moves through
long driveSteps = driveStepsPerRev * driveRevolutions * -1;   //steps for drive stepper motor to take
const int driveSpeed = 6400;  //drive speed (steps per second)
const int driveAcceleration = 3200;  //drive acceleration (steps per second per second)
float furtherDriveRevolutions = 2.15;
long furtherDriveSteps = driveStepsPerRev * furtherDriveRevolutions * -1;
long retractSteps = furtherDriveSteps + driveSteps;

//EXTRUSION NEMA 23
#define extrusionDP 8   //extrusion direction pin
#define extrusionEP 10   //extrusion enable pin
#define extrusionSP 9   //extrusion step pin
const int extrusionStepsPerRev = 6400 ;  //extrusion stepper motor steps per revolution
float extrusionRevolutions = 22.5;  //revolutions extrusion stepper moves through
long extrusionSteps = extrusionStepsPerRev * extrusionRevolutions;   //steps for extrusion stepper motor to take
const int extrusionSpeed = 6400;  //extrusion speed (steps per second)
const int extrusionAcceleration = 3200;  //extrusion acceleration (steps per second per second)

//SCOOP SERVO 
#define scoopSP A2
const int scoopSpeed = 5;

//SET SWITCHES
#define onSwitch A0
float onPresses = 0;
#define collectionLimitSwitch A5
#define armLimitSwitch A3

//SET STEPPER MOTOR TYPES
#define motorInterfaceType 1 

//SET ACCELSTEPPER AND SERVO OBJECTS
AccelStepper arm1Stepper = AccelStepper(motorInterfaceType, arm1SP, arm1DP);   //arm 1 stepper motor
AccelStepper arm2Stepper = AccelStepper(motorInterfaceType, arm2SP, arm2DP);   //arm 2 stepper motor
AccelStepper driveStepper = AccelStepper(motorInterfaceType, driveSP, driveDP);   //drive stepper motor
AccelStepper extrusionStepper = AccelStepper(motorInterfaceType, extrusionSP, extrusionDP);   //extrusion stepper motor

enum {stationary, forward, further, retract, end};   //system states
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
  arm2Stepper.setMaxSpeed(200);
  arm2Stepper.setAcceleration(100);

  //setting drive stepper motor speeds
  driveStepper.setMaxSpeed(driveSpeed);
  driveStepper.setAcceleration(driveAcceleration);

  //setting extrusion stepper motor speeds
  extrusionStepper.setMaxSpeed(extrusionSpeed);
  extrusionStepper.setAcceleration(extrusionAcceleration);

  //setting switches as input
  pinMode(onSwitch, INPUT_PULLUP);
  pinMode(collectionLimitSwitch, INPUT_PULLUP);
  pinMode(armLimitSwitch, INPUT_PULLUP);
}

void loop() {
  int onState = digitalRead(onSwitch);  //read for switch inputs

  if (onState == LOW) {
    onPresses++;
  }

  if (onPresses > 0) {

    driveStepper.enableOutputs();   //enable drive stepper
    extrusionStepper.enableOutputs();   //enable extrusion stepper
    
    switch (systemState) {
      case stationary:   //when first started, extend out extrusions and drive forward
        extrusionStepper.move(extrusionSteps);
        driveStepper.move(driveSteps);
        systemState = forward;   //move onto next state
        break;
      case forward:   //once both nema 23 movements triggerred 
        if (driveStepper.distanceToGo() == 0 and extrusionStepper.distanceToGo() == 0) { //wait for both processes to happen
          delay(2000); //wait for 3 seconds
          systemState = further;  //move onto next state
        }
        break;
      case further:
        driveStepper.move(furtherDriveSteps);
        if (driveStepper.distanceToGo() == 0) {
          delay(1000);
          systemState = retract;
        }
        break;
      case retract:
        driveStepper.move(-retractSteps);
        extrusionStepper.move(-extrusionSteps);
        systemState = end;
      case end:
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



