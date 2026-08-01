#include <Servo.h>

Servo servo;
int pos = 0;

void setup() {
  servo.attach(4);
  Serial.begin(9600);
}


void loop() {
  for (pos = 0; pos <= 180; pos += 1) {
    servo.write(pos);
    delay(15);
  }
  delay(2000);
  Serial.println("run");
  for (pos = 180; pos >= 0; pos -= 1) {
    servo.write(pos);
    delay(15);
  }
  Serial.println("stop");
}
