#define outputA 11
#define outputB 3

short int currentPos_R;
//int currentPos_L;
short int desiredPos_R = 12;    //indicate desired position
//int desiredPos_L;

short int aState;
short int aLastState;
short int counter = 0;

void setup() {
  pinMode(outputA, INPUT);
  pinMode(outputB, INPUT);

  pinMode(6, OUTPUT);
  pinMode(7, OUTPUT);

   Serial.begin(9600);

  aLastState = digitalRead(outputA);  //initial state of A
}

void loop() {
  aState = digitalRead(outputA);  //current state of A

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
  
  currentPos_R = counter;

  if (currentPos_R < desiredPos_R){  //send right motor forward
    digitalWrite(6, HIGH);
    digitalWrite(7, LOW);
   // Serial.print("Forward\n");
  }
  else if (currentPos_R > desiredPos_R){  //send right motor forward
    digitalWrite(6, LOW);
    digitalWrite(7, HIGH);
    //Serial.print("Reverse\n");
  }
  else{
    //stop robot movement
    digitalWrite(6, LOW);
    digitalWrite(7, LOW);
    //Serial.print("STOP");
    //digitalWrite(Motor_L_forward, LOW);
    //digitalWrite(Motor_L_reverse, LOW);
  }
  
}
