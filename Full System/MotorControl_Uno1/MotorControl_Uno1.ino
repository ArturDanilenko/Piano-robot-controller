/*

The board receives 1 bit direction flag and 3 bit speed variable and 
depending on the values sends the motor in one of two direction or halts of speed = 0

The state machine represents the control system that would be used
to control a reluctance motor with 3 pole pairs.

*/
const int dirPin = 3; //direction pin

const short int speedwire1 = 5; //bits of an encoded number representing speed
const short int speedwire2 = 6;
const short int speedwire3 = 7;

const int c1Pin = 13; //pole pair 1
const int c2Pin = 12; //pole pair 2
const int c3Pin = 11; //pole pair 3

bool old_dir = 0;

short int state = 0; 

short int delay_length = 10;

bool C1 = LOW;
bool C2 = LOW;
bool C3 = LOW;

void setup() {
  pinMode(dirPin,INPUT);
  pinMode(speedwire1,INPUT);
  pinMode(speedwire2,INPUT);
  pinMode(speedwire3,INPUT);
  
  pinMode(c1Pin,OUTPUT);
  pinMode(c2Pin,OUTPUT);
  pinMode(c3Pin,OUTPUT);
}

void loop() {  
  choose_pins_to_enable(digitalRead(dirPin));
  delay_length = decode_speed(digitalRead(speedwire1),digitalRead(speedwire2),digitalRead(speedwire3));
  if(delay_length>0)   {
      digitalWrite(c1Pin, C1);
      digitalWrite(c2Pin, C2);
      digitalWrite(c3Pin, C3);
      delay(delay_length); //overloaded arduinos start acting unpredictably
  } else {
      digitalWrite(c1Pin, LOW);
      digitalWrite(c2Pin, LOW);
      digitalWrite(c3Pin, LOW);
  }
 }


/*
decode_speed:
Function decodes binary number received into a delay which allows the board to act as a PWM module
If all bits are low - returns 0 signaling to turn off the motor
otherwise will return a delay with maximum of 1 second (1000ms) or lowest of 125ms
Arguments:
  bitx - part of a 3 bit number forming a single bus 3b'bit1_bit2_bit3
*/

 
short int decode_speed(bool bit1, bool bit2, bool bit3){
  if (bit1+bit2+bit3>0) return 1000-(bit1*4+bit2*2+bit3)*125;
  else {
    return 0;
  }
}

/*
choose pins to enable:
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
