#include <VarSpeedServo.h>

#define servoPin 8

VarSpeedServo servo;

int startPos = 0;
int endPos = 180;
int stepDelay = 25;

enum {start, rotated, end};
unsigned char state;

void setup() {
  Serial.begin(9600);
  servo.attach(servoPin);
  servo.write(0, 8, true);
}

void loop() {
  Serial.println("Test");
  servo.write(180, 8, true);
  delay(3000);
  Serial.println("Works");
  servo.write(0, 8, true);
}