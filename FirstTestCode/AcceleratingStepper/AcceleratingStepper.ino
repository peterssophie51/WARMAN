#include "AccelStepper.h"
#define directionPin 2
#define stepPin 5
#define enablePin 9
#define motorInterfaceType 1
#define stepsPerRevolution 200

AccelStepper stepper = AccelStepper(motorInterfaceType, stepPin, directionPin);

void setup() {
  // set max speed and acceleration
  pinMode(enablePin, OUTPUT);
  digitalWrite(enablePin, LOW);
  stepper.setMaxSpeed(500);
  stepper.setAcceleration(500);

}

void loop() {
  // put your main code here, to run repeatedly:
  stepper.moveTo(1000);
  stepper.runToPosition();
  delay(1000);
  stepper.moveTo(0);
  stepper.runToPosition();

  delay(10000);
}
