// define the output pins the arduino is plugged intoi
#define directionPin 9
#define stepPin 4
#define enablePin 7
#define stepsPerRevolution 200
const int limitPin = 11;
int count = 0;

void setup() {
  // declare the pins as outputs
  pinMode(stepPin, OUTPUT);
  pinMode(directionPin, OUTPUT);
  pinMode(enablePin, OUTPUT);
  digitalWrite(enablePin, LOW);
  pinMode(limitPin, INPUT_PULLUP);
  Serial.begin(9600);

}

void loop() {
  // set the spinning direction, direction depends on wiring
  int state = digitalRead(limitPin);
  digitalWrite(directionPin, LOW);
  if (state == LOW) {
      Serial.println("Limit switch TRIGGERED");
      count++;
      Serial.println(count);
      for (int i = 0; i < stepsPerRevolution; i++) {
        	digitalWrite(stepPin, HIGH);
          delayMicroseconds(4000);
          digitalWrite(stepPin, LOW);
          delayMicroseconds(4000);
  }
  }
  delay(100);

}
