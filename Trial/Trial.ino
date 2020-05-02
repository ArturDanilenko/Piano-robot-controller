const byte ledPin = 13;
const byte interruptPin = 2;
volatile byte state = LOW;
void setup() {
  /*int a = 2;
  int b = 3;
  pinMode(13, OUTPUT);*/
  pinMode(4, OUTPUT);
  pinMode(3, OUTPUT);
  pinMode(2, OUTPUT);
  
  pinMode(interruptPin, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(interruptPin), blink, CHANGE);
}

void loop() {
  digitalWrite(2,HIGH);
  delay(100);
  digitalWrite(2,LOW);
  delay(100);
  digitalWrite(3,HIGH);
  delay(100);
  digitalWrite(3,LOW);
  delay(100);
  digitalWrite(4,HIGH);
  delay(100);
  digitalWrite(4,LOW);
  delay(100);
//  digitalWrite(ledPin, state);
}
void blink() {
  state = !state;
}
