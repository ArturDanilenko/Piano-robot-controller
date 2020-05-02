 #include"TimerOne.h"

bool R_Motor_F_state = 1;
short int speedR = 100;
short int motorCounter = 0;

void setup() {
    
  pinMode(5,OUTPUT);

  Timer1.initialize(1000);
  Timer1.attachInterrupt(timerISR);
  
}

void loop() {

}
void timerISR(){

  if(R_Motor_F_state){
    if(motorCounter<speedR){
      digitalWrite(5, HIGH);
      motorCounter++;
    }else if(motorCounter>=100){
      motorCounter = 0;
    }else {
      motorCounter++;
      digitalWrite(5, LOW);
    }
  }else if(R_Motor_R_state) {
    if(motorCounter<speedR){
      digitalWrite(Motor_R_reverse, HIGH);
      motorCounter++;
    }else if(motorCounter>=100){
      motorCounter = 0;
   }else {
      motorCounter++;
      digitalWrite(Motor_R_reverse, LOW);
    }
  }
  else {
    digitalWrite(Motor_R_reverse, LOW);
    digitalWrite(5, LOW);
  }
}  
