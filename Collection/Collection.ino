#include <AccelStepper.h>
#include <Servo.h>
#include <ServoEasing.hpp>

//ARM 1 NEMA 17
#define arm1DP 11   //arm 1 direction pin
#define arm1EP 13   //arm 1 enable pin
#define arm1SP 12   //arm 1 step pin

//ARM 2 NEMA 17
#define arm2DP 2   //arm 2 direction pin
#define arm2EP 4   //arm 2 enable pin
#define arm2SP 3   //arm 2 step pin

const int armStepsPerRev = 200; //arm stepper motor steps per revolution
const int armSteps = 30;

#define motorInterfaceType 1
#define onSwitch A0

#define scoopPin A2

#define collectionLimitSwitch A5
#define armLimitSwitch A3

AccelStepper arm1Stepper = AccelStepper(motorInterfaceType, arm1SP, arm1DP);   //arm 1 stepper motor
AccelStepper arm2Stepper = AccelStepper(motorInterfaceType, arm2SP, arm2DP);   //arm 2 stepper motor
ServoEasing scoopServo;

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
  arm1Stepper.setMaxSpeed(50);
  arm1Stepper.setSpeed(100);
  arm1Stepper.setAcceleration(10);

  //setting arm 2 stepper motor speeds
  arm2Stepper.setMaxSpeed(50);
  arm2Stepper.setSpeed(100);
  arm2Stepper.setAcceleration(10);
  

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
        Serial.println("Rotating down");
        arm1Stepper.setSpeed(15);
        arm2Stepper.setSpeed(15);
        if (collectionLimitState == LOW) {
          arm1Stepper.setSpeed(0);
          arm2Stepper.setSpeed(0);
          collectionState = halfUp;
        }
        break;
      case halfUp:
        Serial.println("Halfup");
        if (collectionState != prevState) {
          arm1Stepper.move(-armSteps);
          arm2Stepper.move(-armSteps);
          prevState = collectionState;
        }
        if (arm1Stepper.distanceToGo() == 0 and arm2Stepper.distanceToGo() == 0) {
          collectionState = halfDown;
        }
        break;
      
      case halfDown:
        Serial.println("Halfdown");
        arm1Stepper.setSpeed(15);
        arm2Stepper.setSpeed(15);
        if (collectionLimitState == LOW) {
          arm1Stepper.setSpeed(0);
          arm2Stepper.setSpeed(0);
          collectionState = rotatingUp;
        }
        break;
      case rotatingUp:
        Serial.println("Rotating up");
        arm1Stepper.setSpeed(-15);
        arm2Stepper.setSpeed(-15);
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

    if (collectionState == halfUp) {
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
