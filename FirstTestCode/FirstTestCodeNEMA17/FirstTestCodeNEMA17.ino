//EXTRUSION NEMA 23
#define extrusionDP 8   //extrusion direction pin
#define extrusionEP 10   //extrusion enable pin
#define extrusionSP 9   //extrusion step pin
const int extrusionStepsPerRev = 800 ;  //extrusion stepper motor steps per revolution
float extrusionRevolutions = 18.3;                                   ;  //revolutions extrusion stepper moves through
long extrusionSteps = extrusionStepsPerRev * extrusionRevolutions;   //steps for extrusion stepper motor to take
const int extrusionSpeed = 4000;  //extrusion speed (steps per second)
const int extrusionAcceleration = 4000;  //extrusion acceleration (steps per second per second)
#define onSwitch A4
int onPresses = 0;


void setup() {
  // declare the pins as outputs
  pinMode(extrusionSP, OUTPUT);
  pinMode(extrusionDP, OUTPUT);
  pinMode(extrusionEP, OUTPUT);
  digitalWrite(extrusionEP, LOW);
  pinMode(onSwitch, INPUT_PULLUP);
  

}

void loop() {
  // set the spinning direction, direction depends on wiring
  int onState = digitalRead(onSwitch);
  digitalWrite(extrusionEP, HIGH);

  if (onState == LOW) {
    onPresses++;
  }

  if (onPresses > 0) {
    for (int i = 0; i < extrusionSteps; i++) {
      digitalWrite(extrusionSP, HIGH);
      delayMicroseconds(1000);
      digitalWrite(extrusionEP, LOW);
      delayMicroseconds(1000);
    }
    delay(2000);
  }

