/*

The board receives 1 bit direction flag and 3 bit speed variable and 
depending on the values sends the motor in one of two direction or halts of speed = 0

The state machine represents the control system that would be used
to control a reluctance motor with 3 pole pairs.

*/
const int dirPin = 3;

const int c1Pin = 13;
const int c2Pin = 12;
const int c3Pin = 11;

bool old_dir = 0;
bool dir = HIGH;

short int state = 0; 

short int speed = 10;

bool C1 = LOW;
bool C2 = LOW;
bool C3 = LOW;

void setup() {
  pinMode(dirPin,INPUT);
  
  pinMode(c1Pin,OUTPUT);
  pinMode(c2Pin,OUTPUT);
  pinMode(c3Pin,OUTPUT);
}

void loop() {
  dir = digitalRead(dirPin);
  choose_pins_to_enable(dir);
  if(speed>0)   {
      digitalWrite(c1Pin, C1);
      digitalWrite(c2Pin, C2);
      digitalWrite(c3Pin, C3);
      delay(150); //overloaded arduinos start acting unpredictably
  } else {
      digitalWrite(c1Pin, LOW);
      digitalWrite(c2Pin, LOW);
      digitalWrite(c3Pin, LOW);
  }
 }


/*
The choose pins to enable:
This function will be called every main loop run given that the input speed is > 0
Depending on the state it chooses the pins to be enabled depending on the desired direction of the motor spin.
Arguments: 
  dir = represents the desired 
Global Variables used:
  state - used to keep track of the order in which pins are to be raised and lowered
  old_dir - upon the direction change will reverse the order of the pins taking 
into account motors mechanical drawbacks
*/


void choose_pins_to_enable(bool dir){
    if (dir != old_dir){ //in case the direction changes, the motor needs a little bit of time to slow down and reverse the direction
       C1 = LOW;
       C2 = LOW;
       C3 = LOW;
      old_dir = dir;
      delay(50);
     }
     else if (state == 0){ //starting position of the motor upon first run
        C1 = LOW;
        C2 = LOW;
        C3 = LOW;
        state = 1;
     }    
     else if (state == 1){  //turns first pair of poles on (this follows the initial start hence it doesnt matter which pole we enable. 
        C1 = HIGH;
        C2 = LOW;
        C3 = LOW;
        if (dir == HIGH){ //next pole will be dependant upon the direction 
          state = 2;
        }
        else if (dir == LOW){
          state = 6;               
        }
        else state = 0;
      }
      else if (state == 2){ // In order to smooth down the state transtion and remove wobbling in the motor, before transitioning to the next pole we have to enable two at the same time and only afterward disable the old one.
        C1 = HIGH;
        C2 = HIGH;
        C3 = LOW;
        if (dir == HIGH){
         state = 3;
        }
        else if (dir == LOW){
          state = 1;               
        }
        else state = 0;
      }
      else if (state == 3){         //S2
        C1 = LOW;
        C2 = HIGH;
        C3 = LOW;
        if (dir == HIGH){ 
          state = 4;
        }
        else if (dir == LOW){ 
          state = 2;                
        }
        else state = 0;
      }
      else if (state == 4){         //S23
        C1 = LOW;
        C2 = HIGH;
        C3 = HIGH;
        if (dir == HIGH){
          state = 5;
        }
        else if (dir == LOW){
          state = 3;                  
        }
        else state = 0;
     }
     else if (state == 5){         //S3
        C1 = LOW;
        C2 = LOW;
        C3 = HIGH;;
        if (dir == HIGH){
          state = 6;
        }
        else if (dir == LOW){
          state = 4;                  
        }
        else state = 0;
      }
      else if (state == 6){         //S31
        C1 = HIGH;
        C2 = LOW;
        C3 = HIGH;
        if (dir == HIGH){
          state = 1;
        }
        else if (dir == LOW){
          state = 5;                   
        }
        else state = 0;
      }
}
