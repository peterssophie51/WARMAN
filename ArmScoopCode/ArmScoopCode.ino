#include <AccelStepper.h>
#include <MultiStepper.h>

#define armStepper1DP 3
#define armStepper1EP 2
#define armStepper1SP 4

#define armStepper2DP 10
#define armStepper2EP 11
#define armStepper2SP 13                                                     

#define motorInterfaceType 1

AccelStepper armStepper1 = AccelStepper(motorInterfaceType, armStepper1SP, armStepper1DP);
AccelStepper armStepper2 = AccelStepper(motorInterfaceType, armStepper2SP, armStepper2DP);

MultiStepper armSteppers;

void setup() {
  Serial.begin(9600);

  armStepper1.setMaxSpeed(25);
  armStepper2.setMaxSpeed(25);

  armSteppers.addStepper(armStepper1);
  armSteppers.addStepper(armStepper2);

}

void loop() {
  long positions[2];
  positions[0] = 100;
  positions[1] = 100;

  armSteppers.moveTo(positions);
  armSteppers.runSpeedToPosition();
  delay(2000);

}