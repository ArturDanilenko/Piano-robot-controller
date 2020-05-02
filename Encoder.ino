#define outputA 6
#define outputB 7

int counter =0;
int aState;
int aLastState;

void setup() {
  pinMode (outputA,INPUT);
  pinMode (outputB,INPUT);

  Serial.begin (9600);
  aLastState = digitalRead(outputA);  //read initial state of outputA
}

void loop() {
  aState = digitalRead(outputA);      //reads currrent state of outputA

  //if previous and current state are different, pulse has occured -> only update when moved
  if (aState != aLastState){
    //if outputB state is different to ouputA state, encoder is rotating clockwise
    if (digitalRead(outputB)!= aState){
      counter++;
    }
    else {
      counter--;
    }
    Serial.print("Position: ");
    Serial.println(counter);
  }
  //update previous state with current state
  aLastState=aState;
}
