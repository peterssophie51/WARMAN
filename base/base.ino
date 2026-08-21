// a lot of these notes are for myself to remember what i wanna change and for u to understand
//there are a whole lot of constants that i made and didnt use because we were changing values all over
//i tried to comment how it is working
//probably not as complex as your code but its just a huge state machine which runs through lots
//i can try and explain it and match it up with what happens properly when we are back at uni, i imagine we will have a group meeting to catch you up to speed with mater 


//NOTES ON CHANGES I WANNA MAKE
//want to take out all the delay() with milliseconds because then it is not blocking and you call the loop() more consistently
//change the way the arm movements are called when the rocks are being deposited into the milk bottle
//use the library fastaccelstepper for the drive and extrusion which is kind of a weird library
//also want to find a way to link the servo and arm movements more precisely 
//if the fast library doesnt work i also want to try coding acceleration myself because then you can run more steps per second because you're not limited by the number of times loop() runs

#include <AccelStepper.h>
#include <VarSpeedServo.h> //library which lets you run servos based on position, with a given speed and make it blocking or not
//true is blocking false is non-blocking
//this is also somethhing i wanted to change, because i dont want blocking calls in the loop(), and i want to link up the servo and stepper simultaneously

//ARM 1 NEMA 17
#define arm1DP 6   //arm 1 direction pin
#define arm1EP 13   //arm 1 enable pin
#define arm1SP 12   //arm 1 step pin

//ARM 2 NEMA 17
#define arm2DP 2   //arm 2 direction pin
#define arm2EP 4   //arm 2 enable pin
#define arm2SP 3   //arm 2 step pin

const int armStepsPerRev = 1600; //arm stepper motor steps per revolution
float armDownSteps = armStepsPerRev * 0.95;  //steps to rotate arms down to box
float armHalfSteps = armStepsPerRev * 0.225  // steps to rotate arms up in middle of collection
const int armSpeed = 240; 
const int armAcceleration = 200;
float armEndSteps = armStepsPerRev * 0.2; //steps to take arms to finishing position

//DRIVE NEMA 23
#define driveDP 5    //drive direction pin
#define driveEP 7   //drive enable pin
#define driveSP 10   //drive step pin
const int driveStepsPerRev = 800;   //drive stepper motor steps per revolution
float driveRevolutions = 6.72;   //revolutions drive stepper moves through
float driveSteps = driveStepsPerRev * driveRevolutions * -1;   //steps for drive stepper motor to take
const int driveSpeed = 4000;  //drive speed (steps per second)
const int driveAcceleration = 1500;  //drive acceleration (steps per second per second)
float furtherDriveRevolutions = 0.9; //slower drive section for deposit
float furtherDriveSteps = driveStepsPerRev * furtherDriveRevolutions * -1;
float retractSteps = furtherDriveSteps + driveSteps; //how far robot drives back after finishing
float backwardsRevolutions = driveRevolutions + furtherDriveRevolutions - 0.5;
float backwardsDriveSteps = backwardsRevolutions * driveStepsPerRev;

//EXTRUSION NEMA 23
#define extrusionDP 8   //extrusion direction pin
#define extrusionEP 11   //extrusion enable pin
#define extrusionSP 9   //extrusion step pin
const int extrusionStepsPerRev = 800 ;  //extrusion stepper motor steps per revolution
float extrusionRevolutions = 6.45;                                   ;  //revolutions extrusion stepper moves through
float extrusionSteps = extrusionStepsPerRev * extrusionRevolutions;   //steps for extrusion stepper motor to take
const int extrusionSpeed = 4000;  //extrusion speed (steps per second)
const int extrusionAcceleration = 1000;  //extrusion acceleration (steps per second per second)

#define motorInterfaceType 1
#define onSwitch A4
int onPresses = 0;

#define collectionLimitSwitch A0
#define armLimitSwitch A3
#define scoopPin A2

AccelStepper arm1Stepper = AccelStepper(motorInterfaceType, arm1SP, arm1DP);   //arm 1 stepper motor
AccelStepper arm2Stepper = AccelStepper(motorInterfaceType, arm2SP, arm2DP);   //arm 2 stepper motor
AccelStepper driveStepper = AccelStepper(motorInterfaceType, driveSP, driveDP);   //drive stepper motor
AccelStepper extrusionStepper = AccelStepper(motorInterfaceType, extrusionSP, extrusionDP);   //extrusion stepper motor
VarSpeedServo scoopServo;

enum {rotatingDown, halfUp, halfDown, pickRocks, rotatingUp, endScoop, stationary, forward, further, retract, end};
unsigned char systemState;
unsigned char prevState = 1;

#define ledPin A5

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

  //drive stepper motor enabling
  driveStepper.setEnablePin(driveEP);
  driveStepper.setPinsInverted(false, false, true);
  driveStepper.disableOutputs();

  //extrusion steper motor enabling
  extrusionStepper.setEnablePin(extrusionEP);
  extrusionStepper.setPinsInverted(false, false, true);
  extrusionStepper.disableOutputs();

  //setting arm 1 stepper motor speeds
  arm1Stepper.setMaxSpeed(armSpeed);
  arm1Stepper.setSpeed(armSpeed);
  arm1Stepper.setAcceleration(armAcceleration);

  //setting arm 2 stepper motor speeds
  arm2Stepper.setMaxSpeed(armSpeed);
  arm2Stepper.setSpeed(armSpeed);
  arm2Stepper.setAcceleration(armAcceleration);

  //setting drive stepper motor speeds
  driveStepper.setMaxSpeed(driveSpeed);
  driveStepper.setAcceleration(driveAcceleration);

  //setting extrusion stepper motor speeds
  extrusionStepper.setMaxSpeed(extrusionSpeed);
  extrusionStepper.setAcceleration(extrusionAcceleration);
  
  pinMode(onSwitch, INPUT_PULLUP);
  pinMode(collectionLimitSwitch, INPUT_PULLUP); //limit switch on the collection arms
  pinMode(armLimitSwitch, INPUT_PULLUP);  //limit switch which recognises when arms rotated back to deposit rocks after milk bottle
  pinMode(ledPin, OUTPUT);

}

void loop() {
  int onState = digitalRead(onSwitch);
  int collectionLimitState = digitalRead(collectionLimitSwitch);
  int armLimitState = digitalRead(armLimitSwitch);

  if (onState == LOW) {
    onPresses++;
  }

  if (onPresses > 0) {
    
    switch (systemState) {
      //arms rotate down to collection box
      case rotatingDown:  
        digitalWrite(ledPin, HIGH);
        if (systemState != prevState) {
          scoopServo.attach(scoopPin);
          scoopServo.write(0, 255, true);
          arm1Stepper.enableOutputs();
          arm2Stepper.enableOutputs();
          driveStepper.disableOutputs();
          extrusionStepper.disableOutputs();
          arm1Stepper.setMaxSpeed(4000);
          arm2Stepper.setMaxSpeed(4000);
          arm1Stepper.setAcceleration(4000);
          arm2Stepper.setAcceleration(4000);
          arm1Stepper.move(-armDownSteps);
          arm2Stepper.move(armDownSteps);
          prevState = systemState;
        }
        if (arm1Stepper.distanceToGo() == 0 and arm2Stepper.distanceToGo() == 0) {
          systemState = halfUp;
        }
        break;
      //arms rotate up once in collection box, and servo rotates around to halfway
      case halfUp: 
        if (systemState != prevState) {
          arm1Stepper.setMaxSpeed(2200);
          arm2Stepper.setMaxSpeed(2200);
          arm1Stepper.setAcceleration(2200);
          arm2Stepper.setAcceleration(2200);
          arm1Stepper.move(armHalfSteps);
          arm2Stepper.move(-armHalfSteps);
          scoopServo.write(50, 40, false); 
          prevState = systemState;
        }
        if (arm1Stepper.distanceToGo() == 0 and arm2Stepper.distanceToGo() == 0) {
          systemState = halfDown;
        }
        break;
      //arms rotate down in collection box until limit switch clicked, and servo rotates around to pick up rocks
      case halfDown:
        scoopServo.write(170, 27, false);
        arm1Stepper.setSpeed(-400);
        arm2Stepper.setSpeed(400);
        if (collectionLimitState == LOW) {
          arm1Stepper.setSpeed(0);
          arm2Stepper.setSpeed(0);
          systemState = pickRocks; 
        }
        break;
      //tip servo further to collect rocks with small delay to make sure they stay in
      case pickRocks:
        scoopServo.write(180, 10, true); 
        delay(200);
        systemState = rotatingUp;
        break;
      //rotate arms back up until limit switch clicked and adjust servo as they rotate upwards to level it out
      case rotatingUp:
        scoopServo.write(110, 27, false);
        arm1Stepper.setMaxSpeed(1800);
        arm2Stepper.setMaxSpeed(1800);
        arm1Stepper.setSpeed(1700);
        arm2Stepper.setSpeed(-1700);
        //once limit switch clicked tip rocks into the milk bottle
        if (armLimitState == LOW) {
          arm1Stepper.setSpeed(0);
          arm2Stepper.setSpeed(0);
          scoopServo.write(132, 255, true);
          delay(500);
          systemState = endScoop;
        }
        break;
      //move arms to finishing state
      case endScoop:
        scoopServo.write(100, 40, false);
        arm1Stepper.setMaxSpeed(4000);
        arm2Stepper.setMaxSpeed(4000);
        arm1Stepper.move(-armEndSteps);
        arm2Stepper.move(armEndSteps);
        extrusionStepper.enableOutputs();
        driveStepper.enableOutputs();
        systemState = stationary;
        break;
      //drive and extrude forwards
      case stationary:
        extrusionStepper.move(-extrusionSteps);
        driveStepper.move(driveSteps + furtherDriveSteps);
        systemState = forward;
        break;
      case forward:
        //this is not good code and we will change this part here
        if (arm1Stepper.distanceToGo() == 0 and arm2Stepper.distanceToGo() == 0) {
          arm1Stepper.disableOutputs();
          arm2Stepper.disableOutputs();
        }
        //also not the smoothest, will investigate, changes to slower speed at a certain distance away from hopper so the rocks go in slower
        //cant do this using a setacceleration because then the distances are varied, as you have to use that acceleration to get to the speed-
        //-which changes the distance as this is not accounted for
        if (driveStepper.distanceToGo() == furtherDriveSteps) {
          driveStepper.setMaxSpeed(270);
        }
        //changes speed back to max once deposited but lowkeyjust realised this wasnt needed lmao
        if (driveStepper.distanceToGo() == 0 and extrusionStepper.distanceToGo() == 0) {
          extrusionStepper.setAcceleration(4000);
          driveStepper.setMaxSpeed(4000);
          systemState = retract;
        }
        break;
      //retract extrusions and drive at max speed
      case retract:
        if (driveStepper.distanceToGo() == 0 and extrusionStepper.distanceToGo() == 0) {
          delay(1200);
          driveStepper.setMaxSpeed(4000);
          driveStepper.setAcceleration(4000);
          driveStepper.move(backwardsDriveSteps);
          extrusionStepper.move(extrusionSteps);
          systemState = end;
        }
        break;
      //turn off led at the end once retracted
      case end:
      if (driveStepper.distanceToGo() == 0 and extrusionStepper.distanceToGo() == 0) {
        driveStepper.disableOutputs();
        extrusionStepper.disableOutputs();
        digitalWrite(ledPin, LOW);
      }
      break;
    
    }
  }
  
  //have to use .run() for move, .runSpeed() for continous movements until limit switches have hit, this could also be done better
  if (systemState == rotatingDown or systemState == halfUp or systemState == endScoop) {
    arm1Stepper.run();
    arm2Stepper.run();
  } else if (systemState == stationary or systemState == forward or systemState == further or systemState == retract or systemState == end) {
    arm1Stepper.run();
    arm2Stepper.run();
    driveStepper.run();
    extrusionStepper.run();
  } else {
    arm1Stepper.runSpeed();
    arm2Stepper.runSpeed();
  }

}