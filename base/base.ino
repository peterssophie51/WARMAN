#include <AccelStepper.h>
#include <VarSpeedServo.h>

//ARM 1 NEMA 17
#define arm1DP 6   //arm 1 direction pin
#define arm1EP 13   //arm 1 enable pin
#define arm1SP 12   //arm 1 step pin

//ARM 2 NEMA 17
#define arm2DP 2   //arm 2 direction pin
#define arm2EP 4   //arm 2 enable pin
#define arm2SP 3   //arm 2 step pin

const int armStepsPerRev = 1600; //arm stepper motor steps per revolution
const int armDownSteps = armStepsPerRev * 0.95;  //CHANGES THE ROTATION OF THE ARNMS DOWN TILL IT GETS TO THE BOX
const int armHalfSteps = armStepsPerRev * 0.225;
const int armSpeed = 240; // CHANGES THE SPEED OF THE ARMS 
const int armAcceleration = 200;
const int armEndSteps = armStepsPerRev * 0.2;

//DRIVE NEMA 23
#define driveDP 5    //drive direction pin
#define driveEP 7   //drive enable pin
#define driveSP 10   //drive step pin
const int driveStepsPerRev = 800;   //drive stepper motor steps per revolution
float driveRevolutions = 6.685;   //revolutions drive stepper moves through
float driveSteps = driveStepsPerRev * driveRevolutions * -1;   //steps for drive stepper motor to take
const int driveSpeed = 4000;  //drive speed (steps per second)
const int driveAcceleration = 2000;  //drive acceleration (steps per second per second)
float furtherDriveRevolutions = 0.87;
float furtherDriveSteps = driveStepsPerRev * furtherDriveRevolutions * -1;
float retractSteps = furtherDriveSteps + driveSteps;
float backwardsRevolutions = driveRevolutions + furtherDriveRevolutions - 0.5;
float backwardsDriveSteps = backwardsRevolutions * driveStepsPerRev;

//EXTRUSION NEMA 23
#define extrusionDP 8   //extrusion direction pin
#define extrusionEP 11   //extrusion enable pin
#define extrusionSP 9   //extrusion step pin
const int extrusionStepsPerRev = 800 ;  //extrusion stepper motor steps per revolution
float extrusionRevolutions = 6.5;                                   ;  //revolutions extrusion stepper moves through
float extrusionSteps = extrusionStepsPerRev * extrusionRevolutions;   //steps for extrusion stepper motor to take
const int extrusionSpeed = 4000;  //extrusion speed (steps per second)
const int extrusionAcceleration = 1000;  //extrusion acceleration (steps per second per second)

#define motorInterfaceType 1
#define onSwitch A4
int onPresses = 0;

#define collectionLimitSwitch A0
#define armLimitSwitch A3
#define scoopPin A2

AccelStepper arm1Stepper = AccelStepper(motorInterfaceType, arm1SP, arm1DP);   //arm 1 stepper motor
AccelStepper arm2Stepper = AccelStepper(motorInterfaceType, arm2SP, arm2DP);   //arm 2 stepper motor
AccelStepper driveStepper = AccelStepper(motorInterfaceType, driveSP, driveDP);   //drive stepper motor
AccelStepper extrusionStepper = AccelStepper(motorInterfaceType, extrusionSP, extrusionDP);   //extrusion stepper motor
VarSpeedServo scoopServo;

enum {rotatingDown, halfUp, halfDown, pickRocks, rotatingUp, endScoop, stationary, forward, further, retract, end};
unsigned char systemState;
unsigned char prevState = 1;

#define ledPin A5

void setup() {
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
  arm1Stepper.setMaxSpeed(armSpeed);
  arm1Stepper.setSpeed(armSpeed);
  arm1Stepper.setAcceleration(armAcceleration);

  //setting arm 2 stepper motor speeds
  arm2Stepper.setMaxSpeed(armSpeed);
  arm2Stepper.setSpeed(armSpeed);
  arm2Stepper.setAcceleration(armAcceleration);

  //setting drive stepper motor speeds
  driveStepper.setMaxSpeed(driveSpeed);
  driveStepper.setAcceleration(driveAcceleration);

  //setting extrusion stepper motor speeds
  extrusionStepper.setMaxSpeed(extrusionSpeed);
  extrusionStepper.setAcceleration(extrusionAcceleration);
  
  pinMode(onSwitch, INPUT_PULLUP);
  pinMode(collectionLimitSwitch, INPUT_PULLUP);
  pinMode(armLimitSwitch, INPUT_PULLUP);
  pinMode(ledPin, OUTPUT);

}

void loop() {
  int onState = digitalRead(onSwitch);
  int collectionLimitState = digitalRead(collectionLimitSwitch);
  int armLimitState = digitalRead(armLimitSwitch);

  if (onState == LOW) {
    onPresses++;
  }

  if (onPresses > 0) {
    
    switch (systemState) {
      case rotatingDown:  
        digitalWrite(ledPin, HIGH);
        if (systemState != prevState) {
          scoopServo.attach(scoopPin);
          scoopServo.write(0, 255, true);
          arm1Stepper.enableOutputs();
          arm2Stepper.enableOutputs();
          driveStepper.disableOutputs();
          extrusionStepper.disableOutputs();
          arm1Stepper.setMaxSpeed(4000);
          arm2Stepper.setMaxSpeed(4000);
          arm1Stepper.setAcceleration(4000);
          arm2Stepper.setAcceleration(4000);
          arm1Stepper.move(-armDownSteps);
          arm2Stepper.move(armDownSteps);
          prevState = systemState;
        }
        if (arm1Stepper.distanceToGo() == 0 and arm2Stepper.distanceToGo() == 0) {
          systemState = halfUp;
        }
        break;
      case halfUp: 
        if (systemState != prevState) {
          arm1Stepper.setMaxSpeed(2000);
          arm2Stepper.setMaxSpeed(2000);
          arm1Stepper.setAcceleration(2000);
          arm2Stepper.setAcceleration(2000);
          arm1Stepper.move(armHalfSteps);
          arm2Stepper.move(-armHalfSteps);
          scoopServo.write(50, 40, false); //CHANGES THE SPEED AND ROTATION OF THE SERVO WHEN ROTATING AROUND
          prevState = systemState;
        }
        if (arm1Stepper.distanceToGo() == 0 and arm2Stepper.distanceToGo() == 0) {
          systemState = halfDown;
        }
        break;
      case halfDown:
        scoopServo.write(170, 27, false);
        arm1Stepper.setSpeed(-400);
        arm2Stepper.setSpeed(400);
        if (collectionLimitState == LOW) {
          arm1Stepper.setSpeed(0);
          arm2Stepper.setSpeed(0);
          systemState = pickRocks; //CHANGES THE SPEED AND ROTATION OF THE SERVO WHEN ROTATING THE EXTRA BIT AROUND
        }
        break;
      case pickRocks:
        scoopServo.write(180, 10, true); //CHANGES THE
        systemState = rotatingUp;
        break;
      case rotatingUp:
        scoopServo.write(110, 35, false);
        arm1Stepper.setMaxSpeed(1600);
        arm2Stepper.setMaxSpeed(1600);
        arm1Stepper.setSpeed(1600);
        arm2Stepper.setSpeed(-1600);
        if (armLimitState == LOW) {
          arm1Stepper.setSpeed(0);
          arm2Stepper.setSpeed(0);
          scoopServo.write(145, 200, true);
          systemState = endScoop;
        }
        break;
      case endScoop:
        scoopServo.write(100, 40, false);
        arm1Stepper.setMaxSpeed(4000);
        arm2Stepper.setMaxSpeed(4000);
        arm1Stepper.move(-armEndSteps);
        arm2Stepper.move(armEndSteps);
        extrusionStepper.enableOutputs();
        driveStepper.enableOutputs();
        systemState = stationary;
        break;
      case stationary:
        extrusionStepper.move(-extrusionSteps);
        driveStepper.move(driveSteps + furtherDriveSteps);
        systemState = forward;
        break;
      case forward:
        if (arm1Stepper.distanceToGo() == 0 and arm2Stepper.distanceToGo() == 0) {
          arm1Stepper.disableOutputs();
          arm2Stepper.disableOutputs();
        }
        if (driveStepper.distanceToGo() == furtherDriveSteps) {
          driveStepper.setMaxSpeed(270);
        }
        if (driveStepper.distanceToGo() == 0 and extrusionStepper.distanceToGo() == 0) {
          extrusionStepper.setAcceleration(4000);
          driveStepper.setMaxSpeed(4000);
          systemState = retract;
        }
        break;
      case retract:
        if (driveStepper.distanceToGo() == 0 and extrusionStepper.distanceToGo() == 0) {
          delay(1200);
          driveStepper.setMaxSpeed(4000);
          driveStepper.setAcceleration(4000);
          driveStepper.move(backwardsDriveSteps);
          extrusionStepper.move(extrusionSteps);
          systemState = end;
        }
        break;
      case end:
      if (driveStepper.distanceToGo() == 0 and extrusionStepper.distanceToGo() == 0) {
        driveStepper.disableOutputs();
        extrusionStepper.disableOutputs();
        digitalWrite(ledPin, LOW);
      }
      break;
    
    }
  }
  
  if (systemState == rotatingDown or systemState == halfUp or systemState == endScoop) {
    arm1Stepper.run();
    arm2Stepper.run();
  } else if (systemState == stationary or systemState == forward or systemState == further or systemState == retract or systemState == end) {
    arm1Stepper.run();
    arm2Stepper.run();
    driveStepper.run();
    extrusionStepper.run();
  } else {
    arm1Stepper.runSpeed();
    arm2Stepper.runSpeed();
  }

}