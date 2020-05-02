const int pwrPin = 5;
const int dirPin = 3;
const int holdPin = 2;

const int c1Pin = 13;
const int c2Pin = 12;
const int c3Pin = 11;

int old_dir;
int counter_dir = 0;
int counter_main = 0;
//int max_count = 2592;

bool pwr = HIGH;
bool dir = HIGH;
bool hold = LOW;

short int state = 0; 
short int laststate = 0;

bool C1 = LOW;
bool C2 = LOW;
bool C3 = LOW;

void setup() {
// put your setup code here, to run once:
  pinMode(pwrPin,INPUT);
  pinMode(dirPin,INPUT);
  pinMode(holdPin,INPUT);
  
  pinMode(c1Pin,OUTPUT);
  pinMode(c2Pin,OUTPUT);
  pinMode(c3Pin,OUTPUT);
  
  Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
//  pwr = digitalRead(pwrPin);
  dir = digitalRead(dirPin);
  Serial.print("qwq");
 // hold = digitalRead(holdPin);
  if(digitalRead(pwrPin)){
    if(state!=laststate){
   // Serial.println(state);
      laststate = state;
    }  
   /* if (counter_main <= max_count - 1){
      counter_main++;
    }
    else {*/
     counter_main = 0;
     if (pwr == LOW){
        C1 = LOW;
        C2 = LOW;
        C3 = LOW;
      state = 0;
     }
     else if (hold == HIGH){
      
     }  
      else if (dir != old_dir){
      counter_dir = 0;
       C1 = LOW;
       C2 = LOW;
       C3 = LOW;
     // while  (counter_dir <= max_count-1){
     //   counter_dir ++;
     // }
     // delay(10);
      counter_dir = 0;
      old_dir = dir;
      //state = 0;
     }
     else if (state == 0){
        C1 = LOW;
        C2 = LOW;
        C3 = LOW;
        if(pwr == HIGH) state = 1;
     }    
     else if (state == 1){         //S1
        C1 = HIGH;
        C2 = LOW;
        C3 = LOW;
        if (pwr == LOW) state = 0;
        else if (pwr == HIGH && dir == HIGH){
          state = 2;
        }
        else if (pwr == HIGH && dir == LOW){
          state = 6;               
        }
        else state = 0;
      }
      else if (state == 2){              //S12
        C1 = HIGH;
        C2 = HIGH;
        C3 = LOW;
        if (pwr == LOW) state = 0;
        else if (pwr == HIGH && dir == HIGH){
         state = 3;
        }
        else if (pwr == HIGH && dir == LOW){
          state = 1;               
        }
        else state = 0;
      }
      else if (state == 3){         //S2
        C1 = LOW;
        C2 = HIGH;
        C3 = LOW;
        if (pwr == LOW) state = 0;
        else if (pwr == HIGH && dir == HIGH){ 
          state = 4;
        }
        else if (pwr == HIGH && dir == LOW){ 
          state = 2;                
        }
        else state = 0;
      }
      else if (state == 4){         //S23
        C1 = LOW;
        C2 = HIGH;
        C3 = HIGH;
        if (pwr == LOW) state = 0;
        else if (pwr == HIGH && dir == HIGH){
          state = 5;
        }
        else if (pwr == HIGH && dir == LOW){
          state = 3;                  
        }
        else state = 0;
     }
     else if (state == 5){         //S3
        C1 = LOW;
        C2 = LOW;
        C3 = HIGH;;
        if (pwr == LOW) state = 0;
        else if (pwr == HIGH && dir == HIGH){
          state = 6;
        }
        else if (pwr == HIGH && dir == LOW){
          state = 4;                  
        }
        else state = 0;
      }
      else if (state == 6){         //S31
        C1 = HIGH;
        C2 = LOW;
        C3 = HIGH;
        if (pwr == LOW) state = 0;
        else if (pwr == HIGH && dir == HIGH){
          state = 1;
        }
        else if (pwr == HIGH && dir == LOW){
          state = 5;                   
        }
        else state = 0;
      }
      digitalWrite(c1Pin, C1);
      digitalWrite(c2Pin, C2);
      digitalWrite(c3Pin, C3);
      delay(150);
  } else {
      digitalWrite(c1Pin, LOW);
      digitalWrite(c2Pin, LOW);
      digitalWrite(c3Pin, LOW);
  }
 }
//
