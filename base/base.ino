#include <AccelStepper.h>
#include <VarSpeedServo.h>

//ARM 1 NEMA 17
#define arm1DP 11   //arm 1 direction pin
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

//DRIVE NEMA 23
#define driveDP 5    //drive direction pin
#define driveEP 7   //drive enable pin
#define driveSP 6   //drive step pin
const int driveStepsPerRev = 800;   //drive stepper motor steps per revolution
float driveRevolutions = 10;   //revolutions drive stepper moves through
long driveSteps = driveStepsPerRev * driveRevolutions * -1;   //steps for drive stepper motor to take
const int driveSpeed = 4000;  //drive speed (steps per second)
const int driveAcceleration = 4000;  //drive acceleration (steps per second per second)
float furtherDriveRevolutions = 1.4;
long furtherDriveSteps = driveStepsPerRev * furtherDriveRevolutions * -1;
long retractSteps = furtherDriveSteps + driveSteps;
long backwardsRevolutions = driveRevolutions + furtherDriveRevolutions;
long backwardsDriveSteps = backwardsRevolutions * driveStepsPerRev;

//EXTRUSION NEMA 23
#define extrusionDP 8   //extrusion direction pin
#define extrusionEP 10   //extrusion enable pin
#define extrusionSP 9   //extrusion step pin
const int extrusionStepsPerRev = 800 ;  //extrusion stepper motor steps per revolution
float extrusionRevolutions = 18.3;                                   ;  //revolutions extrusion stepper moves through
long extrusionSteps = extrusionStepsPerRev * extrusionRevolutions;   //steps for extrusion stepper motor to take
const int extrusionSpeed = 4000;  //extrusion speed (steps per second)
const int extrusionAcceleration = 4000;  //extrusion acceleration (steps per second per second)

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
          scoopServo.write(0, 100, true);
          arm1Stepper.enableOutputs();
          arm2Stepper.enableOutputs();
          driveStepper.disableOutputs();
          extrusionStepper.disableOutputs();
          arm1Stepper.setMaxSpeed(400);
          arm2Stepper.setMaxSpeed(400);
          arm1Stepper.setAcceleration(400);
          arm2Stepper.setAcceleration(400);
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
          arm1Stepper.setMaxSpeed(armSpeed);
          arm2Stepper.setMaxSpeed(armSpeed);
          arm1Stepper.setAcceleration(armAcceleration);
          arm2Stepper.setAcceleration(armAcceleration);
          arm1Stepper.move(armHalfSteps);
          arm2Stepper.move(-armHalfSteps);
          scoopServo.write(170, 9, false); //CHANGES THE SPEED AND ROTATION OF THE SERVO WHEN ROTATING AROUND
          prevState = systemState;
        }
        if (arm1Stepper.distanceToGo() == 0 and arm2Stepper.distanceToGo() == 0) {
          systemState = halfDown;
        }
        break;
      case halfDown:
        arm1Stepper.setSpeed(-armSpeed);
        arm2Stepper.setSpeed(armSpeed);
        if (collectionLimitState == LOW) {
          arm1Stepper.setSpeed(0);
          arm2Stepper.setSpeed(0);
          systemState = pickRocks; //CHANGES THE SPEED AND ROTATION OF THE SERVO WHEN ROTATING THE EXTRA BIT AROUND
        }
        break;
      case pickRocks:
        scoopServo.write(180, 8, true); //CHANGES THE
        systemState = rotatingUp;
        break;
      case rotatingUp:
        scoopServo.write(120, 5, false);
        arm1Stepper.setSpeed(armSpeed);
        arm2Stepper.setSpeed(-armSpeed);
        if (armLimitState == LOW) {
          arm1Stepper.setSpeed(0);
          arm2Stepper.setSpeed(0);
          scoopServo.write(180, 6, false);
          delay(2000);
          systemState = endScoop;
        }
        break;
      case endScoop:
        scoopServo.write(100, 20, false);
        arm1Stepper.disableOutputs();
        arm2Stepper.disableOutputs();
        extrusionStepper.enableOutputs();
        driveStepper.enableOutputs();
        systemState = stationary;
        break;
      case stationary:
        extrusionStepper.move(-extrusionSteps);
        driveStepper.move(driveSteps);
        systemState = forward;
        break;
      case forward:
        if (driveStepper.distanceToGo() == 0 and extrusionStepper.distanceToGo() == 0) {
          systemState = further;
        }
        break;
      case further:
        driveStepper.setMaxSpeed(500);
        driveStepper.setAcceleration(500);
        driveStepper.move(furtherDriveSteps);
        systemState = retract;
        break;
      case retract:
        if (driveStepper.distanceToGo() == 0 and extrusionStepper.distanceToGo() == 0) {
          delay(1000);
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
  
  if (systemState == rotatingDown or systemState == halfUp) {
    arm1Stepper.run();
    arm2Stepper.run();
  } else if (systemState == stationary or systemState == forward or systemState == further or systemState == retract or systemState == end) {
    driveStepper.run();
    extrusionStepper.run();
  } else {
    arm1Stepper.runSpeed();
    arm2Stepper.runSpeed();
  }

}