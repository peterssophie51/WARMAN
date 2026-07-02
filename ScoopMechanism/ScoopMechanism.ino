#include "AccelStepper.h"

#define armDirectionPin 12
#define armEnablePin 8
#define armStepPin 10
#define scoopDirectionPin 5
#define scoopEnablePin 7
#define scoopStepPin 3

#define limitPin A0
int limitHits = 0;

#define switchPin A5

#define motorInterfaceType 1

bool scoopMovedBack = false;

AccelStepper armStepper = AccelStepper(motorInterfaceType, armStepPin, armDirectionPin);
AccelStepper scoopStepper = AccelStepper(motorInterfaceType, scoopStepPin, scoopDirectionPin);

void setup() {
  Serial.begin(9600);

  pinMode(armDirectionPin, OUTPUT);
  pinMode(armStepPin, OUTPUT);
  pinMode(armEnablePin, OUTPUT);

  pinMode(scoopDirectionPin, OUTPUT);
  pinMode(scoopStepPin, OUTPUT);
  pinMode(scoopEnablePin, OUTPUT);

  pinMode(limitPin, INPUT_PULLUP);
  pinMode(switchPin, INPUT_PULLUP);

  armStepper.setEnablePin(armEnablePin);
  scoopStepper.setEnablePin(scoopEnablePin);

  armStepper.setPinsInverted(false, false, true);
  scoopStepper.setPinsInverted(false, false, true);

  armStepper.setMaxSpeed(100);
  scoopStepper.setMaxSpeed(100);
  armStepper.setAcceleration(50);
  scoopStepper.setAcceleration(50);

  armStepper.enableOutputs();
  scoopStepper.enableOutputs();
}

void loop() {
  int limitState = digitalRead(limitPin);
  int switchState = digitalRead(switchPin);

  if (switchState == LOW) {
    armStepper.enableOutputs();
    scoopStepper.enableOutputs();

    if (limitHits == 0) {
      armStepper.setSpeed(50);
      armStepper.runSpeed();

      if (scoopMovedBack == false) {
        scoopStepper.move(-50);
        scoopMovedBack = true;
      }
    }

    if (limitState == LOW && limitHits == 0) {
      Serial.println("Hit First Time");
      armStepper.move(-50);
      scoopStepper.move(100);
      limitHits++;

    }
    if (limitHits == 1 && armStepper.distanceToGo() == 0) {
      armStepper.move(50);
      limitHits++;
    }

    if (limitHits == 2 and armStepper.distanceToGo() == 0 and scoopStepper.distanceToGo() == 0) {
      armStepper.move(-75);
      limitHits++;
    }

    if (limitHits == 3 and armStepper.distanceToGo() == 0) {
      scoopStepper.move(100);
      limitHits++;
    }

    if (limitHits == 4 and scoopStepper.distanceToGo() == 0) {
      scoopStepper.move(-100);
      armStepper.move(75);
      limitHits++;
    }

    armStepper.run();
    scoopStepper.run();

  } else {
    armStepper.disableOutputs();
    scoopStepper.disableOutputs();
    scoopMovedBack = false;
  }

}
