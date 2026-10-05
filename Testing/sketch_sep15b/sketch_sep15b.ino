#include <AccelStepper.h>

#define DP PA8
#define SP PA9

const int stepsPerRev = 800;
const int rev = 6.45;
float steps = -stepsPerRev * rev;

AccelStepper stepper = AccelStepper(1, SP, DP);

void setup() {
  Serial.begin(9600);
  stepper.setMaxSpeed(3000);
  stepper.setAcceleration(3000);
  stepper.move(steps);
}

void loop() {
  stepper.run();
}
