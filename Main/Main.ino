#include"Motor.h"
#include"TimerOne.h"
const byte ledPin = 13;
const byte interruptPin = 2;
volatile byte state = LOW;
Motor *motor;
void setup() {

  Timer1.initialize(100000);
  Timer1.attachInterrupt(timerISR);
 
  pinMode(4, OUTPUT);
  pinMode(5, OUTPUT);
  pinMode(6, OUTPUT);
  
  pinMode(9, OUTPUT);
  pinMode(13, OUTPUT);
  
  pinMode(interruptPin, INPUT_PULLUP);
  pinMode(3, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(interruptPin), switchstate, CHANGE);
  attachInterrupt(digitalPinToInterrupt(3), blink, CHANGE);
  motor = new Motor();
  motor->setSpeed(1);
}

void loop() {
 /* if (motor->getSpeed()) {
    digitalWrite(4, HIGH);
    state = 1;
  }
  else {
    digitalWrite(4, LOW);
     digitalWrite(5, HIGH);
  }
 // delay(200);
  */
  digitalWrite(ledPin, state);
/*  digitalWrite(4,HIGH);
  delay(200);
  digitalWrite(4,LOW);
  digitalWrite(5,HIGH);
  delay(200);
  digitalWrite(5,LOW);
  digitalWrite(6,HIGH);
  delay(200);
  digitalWrite(6,LOW);*/
}
void blink() {
  state = !state;
}
void timerISR(){
  state = !state;
}
void switchstate() {
  state = 1;
  if (motor->getState()) {
    motor->turnOn();
   
  }
  else {
    digitalWrite(6, LOW);
    motor->turnOff();
  }
}
