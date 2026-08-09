#include <Servo.h>

#define servoPin A2
#define onSwitch A0

Servo servo;

int startPos = 0;
int endPos = 180;
int stepDelay = 25;

enum {start, rotated, end};
unsigned char state;

void setup() {
  servo.attach(servoPin);
}

void loop() {
  switch (state) {
    case start:
      servo.write(0);
      delay(2000);
      state = rotated;
      break;

    case rotated:
      for (int pos = startPos; pos <= endPos; pos += 1) {
        servo.write(pos);
        delay(stepDelay);
      }
      state = end;
      break;

    case end:
      // nothing, done
      break;
  }
}