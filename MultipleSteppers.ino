#include "AccelStepper.h"
#define driveDirectionPin 9
#define driveStepPin 10
#define driveEnablePin 8
#define armDirectionPin 3
#define armStepPin 4
#define armEnablePin 5
#define motorInterfaceType 1

AccelStepper driveStepper = AccelStepper(motorInterfaceType, driveStepPin, driveDirectionPin);
AccelStepper armStepper = AccelStepper(motorInterfaceType, armStepPin, armDirectionPin);

void setup() {
  // put your setup code here, to run once:
  pinMode(driveEnablePin, OUTPUT);
  pinMode(armEnablePin, OUTPUT);
  digitalWrite(armEnablePin, LOW);
  digitalWrite(driveEnablePin, LOW);
  driveStepper.setMaxSpeed(200);
  driveStepper.setAcceleration(100);
  armStepper.setMaxSpeed(200);
  armStepper.setAcceleration(100);

  driveStepper.move(200);
  armStepper.move(400);
  

}

void loop() {
  // when both motors are done, restart
  if (driveStepper.distanceToGo() == 0 && armStepper.distanceToGo() == 0) {
    driveStepper.move(200);
    armStepper.move(400);
  }
  driveStepper.run();
  armStepper.run();
}
