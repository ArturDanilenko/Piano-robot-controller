#include"TimerOne.h"
#include "math.h"
//STATES
#define RESET 0
#define RCV 1
#define PLAY 2
#define DECODE 3
#define DEBUG 69

#define POSITION_TO_ANGLE_RATIO 11.25

/*==============================================
NOTES angle VALUES
===============================================*/
#define Note1 11.25
#define Note2 22.50
#define Note3 33.75
#define Note4 45
#define Note5 56.25
#define Note6 67.50
#define Note7 78.75
#define Note8 90


//REG FOR RECEIVED VALUES FROM TEXT
short int stack[300];
short int stack_counter = 0;

//FLAG FOR END OF RECEVING
bool done = 0;

//INITIAL STATE
short int state = RCV;

//===================================================================================
//ENCODER VARIABLES
bool laststate_a;
bool laststate_b;
short int ordernum;
short int lastordernum;
int position = 0;
int prevPos = 0; //FOR PRINTING
double currentAngle_r = 0;
double currentAngle_l = 0;

//====================================================================================
//PID constants
double Kp = 1;
double Ki = 0;
double Kd = 0;
 
unsigned long current_time_l;
unsigned long previous_time_l;
double elapsed_time_l;
double error_l;
double prev_error_l;
double angle_in_l, angle_out_l, setpoint_l;
double cumulative_error_l, rate_error_l;

double angle_in[2];
double xy_des[2];
//double angle_in_r;

unsigned long current_time_r;
unsigned long previous_time_r;
double elapsed_time_r;
double error_r;
double prev_error_r;
double angle_in_r, angle_out_r, setpoint_r;
double cumulative_error_r, rate_error_r;

//============================================================================
//pwm VARIABLES
volatile bool R_Motor_F_state = LOW;
volatile bool R_Motor_R_state = LOW;
short int Motor_R_forward = 2;
short int Motor_R_reverse = 3;
//int Motor_L_forward = 10;
//int Motor_L_reverse = 11;
short int speedR = 50;
short int speedL = 0;

short int motorCounter;

short int posDiff = 0;
//int currentPos_L;
short int desiredPos_R = 5;    //indicate desired position
//int desiredPos_L;

short int j = 0;
bool direction = 1;
/*======================================================================================
PIN CONFIGURATION AND INITIALIZATION OF SOME VARIABLES
=======================================================================================*/


void setup() { 
   //PWM INITS
   Timer1.initialize(1000);
   Timer1.attachInterrupt(timerISR);
  
   pinMode(Motor_R_forward, OUTPUT); //MOTOR DRIVERS
   pinMode(Motor_R_reverse, OUTPUT);
   //pid
   angle_in_l = analogRead(A0); 
   setpoint_l = 800;                          //set point determine setpoint

   angle_in_r = analogRead(A1);
   setpoint_r = 600; 

   pinMode(2, OUTPUT); //LED 1
   pinMode(3, OUTPUT); //LED 2
   pinMode(4, INPUT); //BUTTON FOR THE RESET

   pinMode(8, INPUT); //ENCODER output A
   pinMode(9, INPUT); // encoder output B

   pinMode(13,OUTPUT); //DEBUG LED

   Serial.begin(9600); // Turn the Serial Protocol ON

   laststate_a = digitalRead(8); //initialize the laststate to the current state of the encoder
   laststate_b = digitalRead(9); 
   if(!digitalRead(8)&&digitalRead(9)){
       lastordernum = 0;
    }
    else if(digitalRead(8)&&!digitalRead(9)){
       lastordernum = 1;
    }
    else if(digitalRead(8)&&digitalRead(9)){
       lastordernum = 2;
    }
    else {
       lastordernum = 3;
    }
}

void loop() {
    byte byteRead;


    /*==============================================================================================
    MAIN STATE MACHINE:
    1)RESET: Initial and completion state
    2) RCV: Receiving the text file state. Enables another device to upload a text file onto the board and store
    it in a stack that can be accessed later
    3) PLAY: AS OF RIGHT NOW RUNS THROUGH THE PATTERN OUTLINED IN THE stack
    LATER WILL BE TRANSLATED INTO ARM POSITIONS

    4)DEBUG: DEBUGGING STATE USED FOR TESTING SPECIFIC FEAUTURES.
    INACCESIBLE IN A REGULAR RUN
    =================================================================================================*/


    switch (state) { //State machine

      case RESET:  //Resting state/waiting to start or have the processing file started
        stack_counter = 0;
        digitalWrite(2, HIGH);  //Lights up LED 2
        if(digitalRead(4)){ //Checks if the button is not pressed
           digitalWrite(3, HIGH); //lights up LED 1
        }else {
          digitalWrite(3, LOW); //Turns LEDS off if the button is pressed
          digitalWrite(2, LOW);
          state = RCV;
          byteRead = 0;
        }
        break;

      case RCV: //Receiving text file state

        if (Serial.available()) {
          
           byteRead = Serial.read();  /* read the most recent byte */
           byteRead=byteRead-'0';   //You have to subtract '0' from the read Byte to convert from text to a number.
           Serial.write(byteRead);

           if(byteRead==0){ //0 will be terminating integer hence raise the done flag upon it
             // done = 1;
              digitalWrite(2, LOW); 
              digitalWrite(3, LOW); 
           }

           if(byteRead==9&&stack_counter!=0){ //9 will be terminating integer hence raise the done flag upon it
              state = DECODE;
           }

           if(byteRead>0&&byteRead!=9){ //9 is a terminating character
              //digitalWrite((byteRead + 1), HIGH); 
              stack[stack_counter] = byteRead;
              stack_counter++;
           }
        } 
        break;
      case DECODE:
      /*=========================================
      DECODES Numbers 1-8 into appropriate angles
      =========================================*/
        for(int i = 0; i <= stack_counter; i++){ //Array to store the uploaded notes
            switch(stack[i]){
              case 1:
                stack[i]=Note1;
                break;

              case 2:
                stack[i]=Note2;
                break;

              case 3:
                stack[i]=Note3;
                break;

              case 4:
                stack[i]=Note4;
                break;

              case 5:
                stack[i]=Note5;
                break;

              case 6:
                stack[i]=Note6;
                break;

              case 7:
                stack[i]=Note7;
                break;

              case 8:
                stack[i]=Note8;
                break;

              default:
                stack[i]=0;
                break;
            }
        }
        /*if(currentAngle<stack[0]) direction = 1; //Sets initial direction of the motor
        else{
          direction = 0;
        }*/
        state = PLAY;
        break;

      case PLAY: //Running through notes state
          /*========================================================
          THIS STATE RUNS THROUGH THE NOTES DECODED INTO A REGISTER
          ========================================================*/
          //if(direction){ //checks if the motor supposed to go up or down 
          delay(5000);
            if (currentAngle_r < setpoint_r - angle_out_r){
              R_Motor_F_state = LOW;
              R_Motor_R_state = HIGH;
            }
            else if (currentAngle_r > setpoint_r - angle_out_r){
              R_Motor_F_state = HIGH;
              R_Motor_R_state = LOW;
            }
            else { //if the current angled surpassed the desired one. We mark this part as complete and move on to the next note.
              j++;
              Serial.print("UWU");
              R_Motor_F_state = LOW;
              R_Motor_R_state = LOW;
            }
            
            if (currentAngle_l < setpoint_l - angle_out_l){
            //  R_Motor_F_state = LOW;
              //R_Motor_R_state = HIGH;
            }
            else if (currentAngle_l > setpoint_l - angle_out_l){
              //L_Motor_F_state = HIGH;
             // L_Motor_R_state = LOW;
            }
            else { //if the current angled surpassed the desired one. We mark this part as complete and move on to the next note.
             // j++;
             // L_Motor_F_state = LOW;
              //L_Motor_R_state = LOW;
              //if(currentAngle<stack[j]) direction = 1;
              //else{
               // direction = 0;
            }
          /*else {
            if (currentAngle > stack[j]){
              R_Motor_F_state = HIGH;
              R_Motor_R_state = LOW;
            }*/
            // else if angle_out_r > 0 left motor forwards
            /*else {
              j++;
              if(currentAngle<stack[j]) direction = 1;
              else{
                direction = 0;
              }*/ 
          //state = RESET;
          if(j>stack_counter) state = RESET;
          break;

      case DEBUG:
      
         if (position < desiredPos_R){  //send right motor forward
            R_Motor_F_state = HIGH;
            R_Motor_R_state = LOW;
         }
         else if (position > desiredPos_R){  //send right motor forward
            R_Motor_F_state = LOW;
            R_Motor_R_state = HIGH;
         }
         else{
            //stop robot movement
            R_Motor_F_state = LOW;
            R_Motor_R_state = LOW;
            //digitalWrite(Motor_L_forward, LOW);
            //digitalWrite(Motor_L_reverse, LOW);
         }
         break;

      default: 
          state = RESET;
          break;
    }



    //ENCODER READING
    /*===========================================================================================
    THERE ARE 4 POSSIBLE POSITIONS WHICH CAN BE ORGANIZED IN AN ORDER FROM 0 - 3
    AFTER, WE CAN CHECK IF THE NEW POSITION IS ORDER UP OR BEFORE FROM THE PREVIOUS ONE 
    AND USING THIS WE CAN DETERMINE IF WE ROTATED CW OR CCW
    ============================================================================================*/



    if(!digitalRead(8)&&!digitalRead(9)){ //00
       ordernum = 0;
      } else if(digitalRead(8)&&!digitalRead(9)){//10
       ordernum = 1;
      } else if(digitalRead(8)&&digitalRead(9)){//11
       ordernum = 2;
      } else {//01
       ordernum = 3;
      }
   if(lastordernum==0){ //EXTRA CHECK TO COUNTER 0 TO 3 AND 3 TO 0 ROLLOVERS
      if(ordernum == 3){
         position--;
      } else if(ordernum == 1){
         position ++;
      }
    } else if(lastordernum==3){
      if(ordernum==0){
        position++;
      }else if(ordernum==2){
        position--;
      }
    } else {
      if (ordernum>lastordernum){
         position++;
      }else if(ordernum<lastordernum){
         position--;
      }
      }
   lastordernum = ordernum; //MAKE LAST ORDERNUM NEW FOR THE NEXT CYCLE
   currentAngle_r = posToAngle(position);
   
  /*====================================================================
  PID AND ANGLE CALCULATIONS
  ======================================================================*/
  
  setpoint_r = stack[j];
 // setpoint_l = (double)stack[j];    //change to whatever it needs to be 
  angle_out_l = computePIDleft(currentAngle_l);
  analogWrite(A3,angle_out_l);

  angle_out_r = computePIDright(currentAngle_r);
  analogWrite(A4,angle_out_r);

   if(prevPos!=position){ //PRINTING FUNCTION. WILL PRINT ONLY IF THE VALUES ARE DIFFERENT
      Serial.print("Position is: ");
      Serial.print(position);
      Serial.print("\n");

      Serial.print("Angle is: ");
      Serial.print(currentAngle_r);
      Serial.print("\n");

      Serial.print("Current Stack Value: ");
      Serial.print(stack[j]);
      Serial.print("\n");

      Serial.print("Current State Value: ");
      Serial.print(state);
      Serial.print("\n");

      Serial.print("Current direction Value: ");
      Serial.print(direction);
      Serial.print("\n");

      Serial.print("Current j Value: ");
      Serial.print(j);
      Serial.print("\n");

      Serial.print("Current PID Value: ");
      Serial.print(angle_out_r);
      Serial.print("\n");
      prevPos = position;
      }




  /*===================================================================================
  MOTOR CONTROL USING ANGLES 
  =====================================================================================*/

  
}



/*===================================================================================
SET OF FUNCTIONS FOR ISR, PID AND ANGLES
===================================================================================*/



double computePIDleft(double angle_in_l){     
        current_time_l = millis();                //get current time
        elapsed_time_l = (double)(current_time_l - previous_time_l);        //compute time elapsed from previous computation
        
        error_l = setpoint_l - angle_in_l;                                // determine error
        cumulative_error_l += error_l * elapsed_time_l;                // compute integral
        rate_error_l = (error_l - prev_error_l)/elapsed_time_l;   // compute derivative
 
        double angle_out_l = Kp*error_l + Ki*cumulative_error_l + Kd*rate_error_l;                //PID output               
 
        prev_error_l = error_l;                                //remember current error
        previous_time_l = current_time_l;                        //remember current time
 
        return angle_out_l;
} 

double computePIDright(double angle_in_r){     
        current_time_r = millis();                //get current time
        Serial.print("current_time");
        Serial.println(current_time_r);
        elapsed_time_r = (double)(current_time_r - previous_time_r);        //compute time elapsed from previous computation
        
        Serial.print("elapsed_time:");
        Serial.println(elapsed_time_r);

      
        error_r = setpoint_r - angle_in_r;       
        Serial.print("Current error_r Value: ");
         Serial.print(error_r);
         Serial.print("\n");  

         Serial.print("Current setpoint_r Value: ");
         Serial.print(setpoint_r);
         Serial.print("\n"); 

         Serial.print("Current angle_in_r Value: ");
         Serial.print(angle_in_r);
         Serial.print("\n");                        // determine error
        cumulative_error_r += error_r * elapsed_time_r;                // compute integral
        rate_error_r = (error_r - prev_error_r)/elapsed_time_r;   // compute derivative
        double angle_out_r = Kp*error_r + Ki*cumulative_error_r + Kd*rate_error_r;                //PID output               
        Serial.print("Current angle_out_r Value: ");
         Serial.print(angle_out_r);
         Serial.print("\n"); 
        prev_error_r = error_r;                                //remember current error
        previous_time_r = current_time_r;                        //remember current time
 
        return angle_out_r;
} 

void timerISR(){
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
    digitalWrite(Motor_R_forward, LOW);
  }
}  

double invKin0(double x, double y){

  double xpos = x;
  double ypos = y;
  
  double B = acos( (20000 - pow(xpos,2) - pow(ypos,2)) / (20000));
  double n = (3.1415 - B ) / 2;

  double theta1 = n + atan2(ypos, xpos);
  double theta2 =  atan2(ypos, xpos) - n;

  //theta = [theta1*180/pi; theta2*180/pi];
  return theta1;
} 

double invKin1(double x, double y){

  double xpos = x;
  double ypos = y;
  
  double B = acos( (20000 - pow(xpos,2) - pow(ypos,2)) / (20000));
  double n = (3.1415 - B ) / 2;

  double  theta1 = n + atan2(ypos, xpos);
  double  theta2 =  atan2(ypos, xpos) - n;

  //theta = [theta1*180/pi; theta2*180/pi];
  return theta2;
} 

double posToAngle(int pos){
  return pos * POSITION_TO_ANGLE_RATIO;
}
