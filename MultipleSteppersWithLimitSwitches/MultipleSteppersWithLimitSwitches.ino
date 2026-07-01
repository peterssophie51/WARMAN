#include "AccelStepper.h"

#define armDirectionPin 4
#define armEnablePin 2
#define armStepPin 6
#define scoopDirectionPin 10
#define scoopEnablePin 12
#define scoopStepPin 8

#define armLimitPin A2
#define scoopLimitPin A5

#define motorInterfaceType 1

AccelStepper armStepper = AccelStepper(motorInterfaceType, armStepPin, armDirectionPin);
AccelStepper scoopStepper = AccelStepper(motorInterfaceType, scoopStepPin, scoopDirectionPin);

void setup() {
  Serial.begin(9600);

  pinMode(armDirectionPin, OUTPUT);
  pinMode(armEnablePin, OUTPUT);
  pinMode(armStepPin, OUTPUT);
  pinMode(scoopDirectionPin, OUTPUT);
  pinMode(scoopEnablePin, OUTPUT);
  pinMode(scoopStepPin, OUTPUT);

  pinMode(armLimitPin, INPUT_PULLUP);
  pinMode(scoopLimitPin, INPUT_PULLUP);

  digitalWrite(armEnablePin,  LOW);
  digitalWrite(scoopEnablePin, LOW);

  armStepper.setMaxSpeed(50);
  armStepper.setAcceleration(10);
  scoopStepper.setMaxSpeed(50);
  scoopStepper.setAcceleration(10);

}

void loop() {
  int armState = digitalRead(armLimitPin);
  int scoopState = digitalRead(scoopLimitPin);

  digitalWrite(armDirectionPin, LOW);
  digitalWrite(scoopDirectionPin, LOW);

  if (armState == LOW) {
    Serial.println("Arm Stepper pressed");
    armStepper.stop();
    armStepper.move(200);
  }

  if (scoopState == LOW) {
    Serial.println("Scoop Stepper pressed");
    scoopStepper.stop();
    scoopStepper.move(200);

  }

  armStepper.run();
  scoopStepper.run();

}
