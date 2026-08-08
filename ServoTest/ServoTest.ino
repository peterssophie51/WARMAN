#include <Servo.h>
#include <AccelStepper.h>

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
#define onSwitch A0

Servo scoopServo;
AccelStepper arm1Stepper = AccelStepper(motorInterfaceType, arm1SP, arm1DP);   //arm 1 stepper motor
AccelStepper arm2Stepper = AccelStepper(motorInterfaceType, arm2SP, arm2DP);   //arm 2 stepper motor

enum {on, off};
unsigned char systemState;

void setup() {
  scoopServo.attach(A2);
  scoopServo.write(0);
}

void loop() {
  arm1Stepper.disableOutputs();
  arm2Stepper.disableOutputs();
  slowServoMovement(scoopServo, 0, 180, 15)

}

void slowServoMovement(Servo &servo, int startAngle, int endEngle, int stepDelay) {
  for (int pos = startAngle; pos <= endAngle; pos++) {
    servo.write(pos);
    delay(stepDelay);
  }
}
