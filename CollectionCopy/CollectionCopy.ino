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

const int armStepsPerRev = 200; //arm stepper motor steps per revolution
const int armSteps = 50;
const int armSpeed = 30;

#define motorInterfaceType 1
#define onSwitch A0
#define collectionLimitSwitch A5
#define armLimitSwitch A3

#define scoopPin A2

AccelStepper arm1Stepper = AccelStepper(motorInterfaceType, arm1SP, arm1DP);   //arm 1 stepper motor
AccelStepper arm2Stepper = AccelStepper(motorInterfaceType, arm2SP, arm2DP);   //arm 2 stepper motor
VarSpeedServo scoopServo;

enum {rotatingDown, halfUp, halfDown, rotatingUp, end};
unsigned char collectionState;
unsigned char prevState = 0;

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

  //setting arm 1 stepper motor speeds
  arm1Stepper.setMaxSpeed(armSpeed);
  arm1Stepper.setSpeed(armSpeed);
  arm1Stepper.setAcceleration(10);

  //setting arm 2 stepper motor speeds
  arm2Stepper.setMaxSpeed(armSpeed);
  arm2Stepper.setSpeed(armSpeed);
  arm2Stepper.setAcceleration(10);

  scoopServo.attach(scoopPin);
  scoopServo.write(0, 150, true);
  
  pinMode(onSwitch, INPUT_PULLUP);
  pinMode(collectionLimitSwitch, INPUT_PULLUP);
  pinMode(armLimitSwitch, INPUT_PULLUP);

}

void loop() {
  int onState = digitalRead(onSwitch);
  int collectionLimitState = digitalRead(collectionLimitSwitch);
  int armLimitState = digitalRead(armLimitSwitch);

  if (onState == LOW) {
    arm1Stepper.enableOutputs();
    arm2Stepper.enableOutputs();

    switch (collectionState) {

      case rotatingDown:
        arm1Stepper.setSpeed(armSpeed);
        arm2Stepper.setSpeed(armSpeed);
        if (collectionLimitState == LOW) {
          arm1Stepper.setSpeed(0);
          arm2Stepper.setSpeed(0);
          collectionState = halfUp;
        }
        break;
      case halfUp:
        if (collectionState != prevState) {
          arm1Stepper.move(-armSteps);
          arm2Stepper.move(-armSteps);
          scoopServo.write(180, 8, false);
          prevState = collectionState;
        }
        if (arm1Stepper.distanceToGo() == 0 and arm2Stepper.distanceToGo() == 0) {
          collectionState = halfDown;
        }
        break;
      
      case halfDown:
        if (collectionState != prevState) {
          arm1Stepper.setSpeed(armSpeed);
          arm2Stepper.setSpeed(armSpeed);
          arm1Stepper.move(armSteps);
          arm2Stepper.move(armSteps);
          prevState = collectionState;
        }
        if (arm1Stepper.distanceToGo() == 0 and arm2Stepper.distanceToGo() == 0) {
          collectionState = rotatingUp;
        }
        break;
      case rotatingUp:
        if (collectionState != prevState) {
          scoopServo.write(160, 5, false);
        }
        arm1Stepper.setSpeed(-armSpeed);
        arm2Stepper.setSpeed(-armSpeed);
        if (armLimitState == LOW) {
          arm1Stepper.setSpeed(0);
          arm2Stepper.setSpeed(0);
          collectionState = end;
                  }
        break;
      case end:
        arm1Stepper.disableOutputs();
        arm2Stepper.disableOutputs();
    }

    if (collectionState == halfUp || collectionState == halfDown) {
      arm1Stepper.run();
      arm2Stepper.run();
    } else {
      arm1Stepper.runSpeed();
      arm2Stepper.runSpeed();
    }

  
  } else {
    arm1Stepper.disableOutputs();
    arm2Stepper.disableOutputs();
  }

}
