const int limitPin = A0;   // D2
int count = 0;

void setup() {
  pinMode(limitPin, INPUT_PULLUP);  // Enable internal pull-up
  Serial.begin(9600);
}
void loop() {
  int state = digitalRead(limitPin);
  if (state == LOW) {
    Serial.println("Limit switch TRIGGERED");
    count++;
    Serial.println(count);
  } 
  delay(100);  // simple debounce / slow down prints
}