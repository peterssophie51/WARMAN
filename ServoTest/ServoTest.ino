#include <VarSpeedServo.h>

#define servoPin A2
#define onSwitch A0

VarSpeedServo servo;

int startPos = 0;
int endPos = 180;
int stepDelay = 25;

enum {start, rotated, end};
unsigned char state;

void setup() {
  servo.attach(servoPin);
  servo.write(0, 8, true);
}

void loop() {
  servo.write(180, 8, false);
  delay(3000);
}