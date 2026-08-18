#include <FastAccelStepper.h>
#include <AVRStepperPins.h>


//DRIVE NEMA 23
#define driveDP 5    //drive direction pin
#define driveEP 7   //drive enable pin
#define driveSP 10   //drive step pin
const int driveStepsPerRev = 800;   //drive stepper motor steps per revolution
float driveRevolutions = 6.3;   //revolutions drive stepper moves through
float driveSteps = driveStepsPerRev * driveRevolutions * -1;   //steps for drive stepper motor to take
const int driveSpeed = 4000;  //drive speed (steps per second)
const int driveAcceleration = 4000;  //drive acceleration (steps per second per second)
float furtherDriveRevolutions = 1.25;
float furtherDriveSteps = driveStepsPerRev * furtherDriveRevolutions * -1;
float retractSteps = furtherDriveSteps + driveSteps;
float backwardsRevolutions = driveRevolutions + furtherDriveRevolutions;
float backwardsDriveSteps = backwardsRevolutions * driveStepsPerRev;

//EXTRUSION NEMA 23
#define extrusionDP 8   //extrusion direction pin
#define extrusionEP 11   //extrusion enable pin
#define extrusionSP 9   //extrusion step pin
const int extrusionStepsPerRev = 800 ;  //extrusion stepper motor steps per revolution
float extrusionRevolutions = 8.5;                                   ;  //revolutions extrusion stepper moves through
float extrusionSteps = extrusionStepsPerRev * extrusionRevolutions;   //steps for extrusion stepper motor to take
const int extrusionSpeed = 4000;  //extrusion speed (steps per second)
const int extrusionAcceleration = 1000;  //extrusion acceleration (steps per second per second)

#define motorInterfaceType 1
#define onSwitch A4
int onPresses = 0;

#define collectionLimitSwitch A0
#define armLimitSwitch A3
#define scoopPin A2

FastAccelStepperEngine engine = FastAccelStepperEngine();
FastAccelStepper *driveStepper = NULL;
FastAccelStepper *extrusionStepper = NULL;

enum {stationary, forward, further, retract, end};
unsigned char systemState;
unsigned char prevState = 1;

#define ledPin A5

void setup() {
   Serial.begin(9600);

   engine.init();
   driveStepper = engine.stepperConnectToPin(driveSP);
   if (driveStepper) {
    driveStepper->setDirectionPin(driveDP);
    driveStepper->setEnablePin(driveEP);
    driveStepper-> setAutoEnable(false);
    driveStepper->setSpeedInHz(4000);
    driveStepper->setAcceleration(4000);
    driveStepper->disableOutputs();
   }

   extrusionStepper = engine.stepperConnectToPin(extrusionSP);
   if (extrusionStepper) {
    extrusionStepper->setDirectionPin(extrusionDP);
    extrusionStepper->setEnablePin(extrusionEP);
    extrusionStepper->setAutoEnable(false);
    extrusionStepper->setSpeedInHz(4000);
    extrusionStepper->setAcceleration(4000);
    extrusionStepper->disableOutputs();
   }

  pinMode(onSwitch, INPUT_PULLUP);
  pinMode(collectionLimitSwitch, INPUT_PULLUP);
  pinMode(armLimitSwitch, INPUT_PULLUP);
  pinMode(ledPin, OUTPUT);

}

void loop() {
  int onState = digitalRead(onSwitch);
  int collectionLimitState = digitalRead(collectionLimitSwitch);
  int armLimitState = digitalRead(armLimitSwitch);
  long driveDistanceToGo = driveStepper->targetPos() - driveStepper->getCurrentPosition();
  long extrusionDistanceToGo = extrusionStepper->targetPos() - extrusionStepper->getCurrentPosition();
  Serial.println(extrusionDistanceToGo);


  if (onState == LOW) {
    onPresses++;
  }

  if (onPresses > 0) {
    
    switch (systemState) {
      case stationary:
        driveStepper->enableOutputs();
        extrusionStepper->enableOutputs();
        extrusionStepper->move(-extrusionSteps);
        systemState = forward;
        break;
      case forward:
        if (!driveStepper->isRunning()) {
          driveStepper->setSpeedInHz(600);
        }
        if (!driveStepper->isRunning() and !extrusionStepper->isRunning()) {
          extrusionStepper->setAcceleration(4000);
          systemState = retract;
        }
        break;
      case retract:
        if (!driveStepper->isRunning() and !extrusionStepper->isRunning()) {
          delay(1000);
          driveStepper->setSpeedInHz(2000);
          driveStepper->setAcceleration(2000);
          extrusionStepper->move(extrusionSteps);
          systemState = end;
        }
        break;
      case end:
        if (!driveStepper->isRunning() and !extrusionStepper->isRunning()) {
          driveStepper->disableOutputs();
          extrusionStepper->disableOutputs();
          digitalWrite(ledPin, LOW);
        }
      break;
    
    }
  }


}