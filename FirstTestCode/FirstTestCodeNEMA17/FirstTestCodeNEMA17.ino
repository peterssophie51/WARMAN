//EXTRUSION NEMA 23
#define extrusionDP PA15   //extrusion direction pin
#define extrusionSP PA9  //extrusion step pin
const int extrusionStepsPerRev = 1600 ;  //extrusion stepper motor steps per revolution
float extrusionRevolutions = 2;                                   ;  //revolutions extrusion stepper moves through
long extrusionSteps = extrusionStepsPerRev * extrusionRevolutions;   //steps for extrusion stepper motor to take
int onPresses = 0;


void setup() {
  // declare the pins as outputs
  pinMode(extrusionSP, OUTPUT);
  pinMode(extrusionDP, OUTPUT);

}

void loop() {
  for (int i = 0; i < extrusionSteps; i++) {
    digitalWrite(extrusionSP, HIGH);
    delayMicroseconds(300);
    digitalWrite(extrusionSP, LOW);
    delayMicroseconds(300);
  }
  delay(2000);
}

