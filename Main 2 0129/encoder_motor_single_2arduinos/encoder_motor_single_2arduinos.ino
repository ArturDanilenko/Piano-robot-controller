#include"TimerOne.h"

#define outputA 11
#define outputB 3

volatile byte state = LOW;
volatile bool R_Motor_F_state = LOW;
volatile bool R_Motor_R_state = LOW;

short int Motor_R_forward = 10;
short int Motor_R_reverse = 9;
//int Motor_L_forward = 10;
//int Motor_L_reverse = 9;

short int currentPos_R;
//int currentPos_L;
short int desiredPos_R = 12;    //indicate desired position
//int desiredPos_L;

short int aState;
short int aLastState;
short int counter = 0;

short int speedR = 60;
short int speedL = 0;

short int motorCounter;

short int posDiff = 0;

void setup() {
  
  
  Timer1.initialize(1000);
  Timer1.attachInterrupt(timerISR);
  
  pinMode(Motor_R_forward, OUTPUT);
  pinMode(Motor_R_reverse, OUTPUT);

  pinMode(13, OUTPUT);//test pin

  //pinMode(Motor_L_forward, OUTPUT);
  //pinMode(Motor_L_reverse,OUTPUT);

  pinMode(6, INPUT);
  pinMode(7, INPUT);


//  Serial.begin(9600);
  
  aLastState = digitalRead(outputA);  //initial state of A
}

void loop() {
   
  /*aState = digitalRead(outputA);  //current state of A

  if (aState!= aLastState){   //if outputB state is different to ouputA state, encoder is rotating clockwise
     if (digitalRead(outputB)!= aState){
        counter++;
     }
     else{
        counter--;
     }
    Serial.print("Position: ");
    Serial.println(counter);
  }
  
  aLastState=aState;
  
  currentPos_R = counter;*/
  
  if (digitalRead(6)){  //send right motor forward
    R_Motor_F_state = HIGH;
    R_Motor_R_state = LOW;
   // Serial.print("GHDG");
  }
  else if (digitalRead(7)){  //send right motor forward
    R_Motor_F_state = LOW;
    R_Motor_R_state = HIGH;
    // Serial.print("GHDGfdiudfgb");
  }
  /*else if (currentPos_L < desiredPos_L){
    //send left motor forward
    digitalWrite(Motor_L_forward, HIGH);
    digitalWrite(Motor_L_reverse, LOW);
  }
  */
  /*else if (currentPos_L > desiredPos_L){
    //send left motor backward
    digitalWrite(Motor_R_forward, LOW);
    digitalWrite(Motor_R_reverse, HIGH);
  }
  */
  else{
    //stop robot movement
    digitalWrite(R_Motor_F_state, LOW);
    digitalWrite(R_Motor_R_state, LOW);
    //digitalWrite(Motor_L_forward, LOW);
    //digitalWrite(Motor_L_reverse, LOW);
  }
  digitalWrite(13,state);
}

void timerISR(){
  state = !state;
  if(R_Motor_F_state){
    if(motorCounter<speedR){
      digitalWrite(Motor_R_forward, HIGH);
      motorCounter++;
    }else if(motorCounter>=100){
      motorCounter = 0;
    }else {
      motorCounter++;
      digitalWrite(Motor_R_forward, LOW);
    }
  }else {
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
}
