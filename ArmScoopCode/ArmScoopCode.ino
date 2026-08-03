#include <AccelStepper.h>
#include <MultiStepper.h>

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

#define motorInterfaceType 1 

#define onSwitch A0   //on/off switch analog pin


AccelStepper arm1Stepper = AccelStepper(motorInterfaceType, arm1SP, arm1DP);   //arm 1 stepper motor
AccelStepper arm2Stepper = AccelStepper(motorInterfaceType, arm2SP, arm2DP);   //arm 2 stepper motor

MultiStepper steppers;

void setup() {
  //arm 1 stepper motor enabling
  Serial.begin(9600);
  arm1Stepper.setEnablePin(arm1EP);
  arm1Stepper.setPinsInverted(false, false, true);
  arm1Stepper.disableOutputs();

  //arm 2 stepper motor enabling
  arm2Stepper.setEnablePin(arm2EP);
  arm2Stepper.setPinsInverted(false, false, true);
  arm2Stepper.disableOutputs();

  //setting arm 1 stepper motor speeds
  arm1Stepper.setMaxSpeed(50);
  arm1Stepper.setAcceleration(10);

  //setting arm 2 stepper motor speeds
  arm2Stepper.setMaxSpeed(50);
  arm2Stepper.setAcceleration(10);

  steppers.addStepper(arm1Stepper);
  steppers.addStepper(arm2Stepper);

  //setting on/off switch as input
  pinMode(onSwitch, INPUT_PULLUP);

}

void loop() {
   int onState = digitalRead(onSwitch);  //read for switch inputs

  if (onState == LOW) {
    arm1Stepper.enableOutputs();
    arm2Stepper.enableOutputs();
    long positions[2];
    positions[0] = 100;
    positions[1]= -100;
    steppers.moveTo(positions);
    steppers.runSpeedToPosition();

  } else {
    arm1Stepper.disableOutputs();
    arm2Stepper.disableOutputs();

  }

}