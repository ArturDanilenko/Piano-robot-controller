int Motor_R_forward = 5;
int Motor_R_reverse = 6;
int Motor_L_forward = 10;
int Motor_L_reverse = 9;

//int ENABLE_R = 4; 
//int ENABLE_L = 5; 

int currentPos_R;
int currentPos_L;
int desiredPos_R;
int desiredPos_L;

int encoder_count = 0;

void setup() {
  //pinMode(ENABLE_R, OUTPUT);
  pinMode(Motor_R_forward, OUTPUT);
  pinMode(Motor_R_reverse, OUTPUT);

  //pinMode(ENABLE_L, OUTPUT);
  pinMode(Motor_L_forward, OUTPUT);
  pinMode(Motor_L_reverse,OUTPUT);

  //pinModes for encoder
}

void loop() {
  //test both desired position
  currentPos_R = 12;
  desiredPos_R = 9;
  currentPos_L = 6;
  desiredPos_L = 9;
  /*
  //test left less than desired, right at desired 
  currentPos_R = 9;
  desiredPos_R = 9;
  currentPos_L = 9;
  desiredPos_L = 9;

  //test right less than desired position, left at desired 
  currentPos_R = 9;
  desiredPos_R = 9;
  currentPos_L = 9;
  desiredPos_L = 9;

  //test left greater than desired position, right at desired 
  currentPos_R = 9;
  desiredPos_R = 9;
  currentPos_L = 9;
  desiredPos_L = 9;

  //test right greater than desired position, left at desired 
  currentPos_R = 9;
  desiredPos_R = 9;
  currentPos_L = 9;
  desiredPos_L = 9;

  //test left greater than desired position, right greater than desired 
  currentPos_R = 9;
  desiredPos_R = 9;
  currentPos_L = 9;
  desiredPos_L = 9;

  //test right greater than desired position, left greater than desired 
  currentPos_R = 9;
  desiredPos_R = 9;
  currentPos_L = 9;
  desiredPos_L = 9;

  //test left lesser than desired position, right lesser than desired 
  currentPos_R = 9;
  desiredPos_R = 9;
  currentPos_L = 9;
  desiredPos_L = 9;

  //test right lesser than desired position, left lesser than desired 
  currentPos_R = 9;
  desiredPos_R = 9;
  currentPos_L = 9;
  desiredPos_L = 9;

  //test left greater than desired position, right lesser than desired 
  currentPos_R = 9;
  desiredPos_R = 9;
  currentPos_L = 9;
  desiredPos_L = 9;

  //test right greater than desired position, left lesser than desired 
  currentPos_R = 9;
  desiredPos_R = 9;
  currentPos_L = 9;
  desiredPos_L = 9;
  */
  
  if (currentPos_R < desiredPos_R){
    //send right motor forward
    digitalWrite(Motor_R_forward, HIGH);
    digitalWrite(Motor_R_reverse, LOW);
  }
  if (currentPos_R > desiredPos_R){
    //send right motor forward
    digitalWrite(Motor_R_forward, LOW);
    digitalWrite(Motor_R_reverse, HIGH);
  }
  if (currentPos_L < desiredPos_L){
    //send left motor forward
    digitalWrite(Motor_L_forward, HIGH);
    digitalWrite(Motor_L_reverse, LOW);
  }
  if (currentPos_L > desiredPos_L){
    //send left motor backward
    digitalWrite(Motor_R_forward, LOW);
    digitalWrite(Motor_R_reverse, HIGH);
  }
  else{
    //stop robot movement
    digitalWrite(Motor_R_forward, LOW);
    digitalWrite(Motor_R_reverse, LOW);
    digitalWrite(Motor_L_forward, LOW);
    digitalWrite(Motor_L_reverse, LOW);
  }
}
