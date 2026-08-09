#include <Servo.h>
#include <SlowMotionServo.h>

#define scoopPin A2

SMSLinear scoopServo;

// Adjust this to comfortably exceed however long a full min->max sweep takes
// at setSpeed(2). If unsure, start high (e.g. 3000-5000ms) and shorten later.
const unsigned long moveDuration = 3000;

enum TestState { GOING_MAX, PAUSING, GOING_MIN, DONE };
TestState testState = GOING_MAX;
unsigned long stateStart = 0;

void setup() {
  Serial.begin(9600);

  scoopServo.setPin(scoopPin);
  scoopServo.setMinMax(544, 2400);
  scoopServo.setSpeed(2);

  Serial.println("Starting: goToMax()");
  scoopServo.goToMax();
  stateStart = millis();
}

void loop() {
  SlowMotionServo::update();

  switch (testState) {

    case GOING_MAX:
      if (millis() - stateStart >= moveDuration) {
        Serial.println("Assumed at max. Pausing 1s...");
        stateStart = millis();
        testState = PAUSING;
      }
      break;

    case PAUSING:
      if (millis() - stateStart >= 1000) {
        Serial.println("Starting: goToMin()");
        scoopServo.goToMin();
        stateStart = millis();
        testState = GOING_MIN;
      }
      break;

    case GOING_MIN:
      if (millis() - stateStart >= moveDuration) {
        Serial.println("Assumed at min. Test complete.");
        testState = DONE;
      }
      break;

    case DONE:
      break;
  }
}