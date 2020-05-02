#include <Console.h>
#include <Bridge.h>
#include "math.h"
#include "definitions.h"

/*===================================================================================
Registers for the data passed from the text file.
===================================================================================*/
double stack[100];
double stack2[100]; 
short int stack_counter = 5; //for debugging is set to 5

//FLAG FOR END OF RECEVING
bool done = 0;

//INITIAL STATE
short int state = PLAY;

/*===================================================================================
ENCODER VARIABLES
===================================================================================*/

bool laststate_a; //encoder 1 output A
bool laststate_b; //encode 1 output B
bool laststate_c; //encoder 2 out A
bool laststate_d; //encode 2 out B

short int ordernum;
short int lastordernum;
short int ordernum2;
short int lastordernum2;

int position = 0;
int prevPos = 0; //FOR PRINTING
short int encoder2_pos = 0;
short int encoder2_pos_prev = 0;

double currentAngle_r = 0;
double currentAngle_l = 0;

/*====================================================================================
PID constants
===================================================================================*/

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

/*============================================================================
UNO control
===================================================================================*/

short int speedR = 20;
short int speedL = 20;
const short int dir_pin_1 = 12;
const short int dir_pin_2 = 13;

const short int speed_1_bit_1_pin = 2;
const short int speed_1_bit_2_pin = 3;
const short int speed_1_bit_3_pin = 4;
const short int speed_2_bit_1_pin = 5;
const short int speed_2_bit_2_pin = 6;
const short int speed_2_bit_3_pin = 7;

bool speed_1_bit_1_value = 0;
bool speed_1_bit_2_value = 0;
bool speed_1_bit_3_value = 1;
bool speed_2_bit_1_value = 0;
bool speed_2_bit_2_value = 0;
bool speed_2_bit_3_value = 1;
short int posDiff = 0;
//int currentPos_L;
short int desiredPos_R = 5;    //indicate desired position
//int desiredPos_L;

short int j = 0;
short int k = 0;
bool retract_flag1 = 0;
bool retract_flag2 = 0;
bool done_flag1 = 0;
bool done_flag2 = 0;
bool transition_flag = 0;
bool trans2_flag = 0;
bool change1_flag = 0;
bool change2_flag = 0;

/*======================================================================================
PIN CONFIGURATION AND INITIALIZATION OF SOME VARIABLES
=======================================================================================*/


void setup() { 
   //angle_in_l = analogRead(A0); 
   setpoint_l = 800;                          //set point determine setpoint

   //angle_in_r = analogRead(A1);
   setpoint_r = 600; 
   stack[0] = 11.25;
   stack[1] = 22.5;
   stack[2] = 33.75;
   stack[3] = 45;
   stack[4] = 33.75;
   pinMode(8, INPUT); //ENCODER 1 output A
   pinMode(9, INPUT); // encoder 1 output B

   pinMode(10, INPUT); //ENCODER 2 output A
   pinMode(11, INPUT); // encoder 2 output B

   pinMode(speed_1_bit_1_pin,OUTPUT);
   pinMode(speed_1_bit_2_pin,OUTPUT);
   pinMode(speed_1_bit_3_pin,OUTPUT);
   pinMode(speed_2_bit_1_pin,OUTPUT);
   pinMode(speed_2_bit_2_pin,OUTPUT);
   pinMode(speed_2_bit_3_pin,OUTPUT);

   pinMode(dir_pin_1,OUTPUT);
   pinMode(dir_pin_2,OUTPUT);
   
   pinMode(13,OUTPUT); //DEBUG LED
   Bridge.begin();
   Console.begin();
   while(!Console);
   Serial.begin(9600); // Turn the Serial Protocol ON

   laststate_a = digitalRead(8); //initialize the laststate to the current state of the encoder  //looks unused
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

   laststate_c = digitalRead(10); //initialize the laststate to the current state of the encoder 2
   laststate_d = digitalRead(11); 
   if(!digitalRead(10)&&digitalRead(11)){
       lastordernum2 = 0;
    }
    else if(digitalRead(10)&&!digitalRead(11)){
       lastordernum2 = 1;
    }
    else if(digitalRead(10)&&digitalRead(11)){
       lastordernum2 = 2;
    }
    else {
       lastordernum2 = 3;
    }

    //stack[0] = 1;
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
        digitalWrite(2, LOW);  //Lights up LED 2
        digitalWrite(3, LOW);  //Lights up LED 2
        digitalWrite(5, LOW);  //Lights up LED 2
        digitalWrite(6, LOW);  //Lights up LED 2
        if(digitalRead(4)){ //Checks if the button is not pressed
           digitalWrite(3, LOW); //lights up LED 1
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
        digitalWrite(13,HIGH);
        for(int i = 0; i <= stack_counter; i++){ //Array to store the uploaded notes
            switch((int)stack[i]){
              case 1:
                stack[i]=Note1;
                stack2[i]=Note1_l;
                break;

              case 2:
                stack[i]=Note2;
                stack2[i]=Note2_l;
                break;

              case 3:
                stack[i]=Note3;
                stack2[i]=Note3_l;
                break;

              case 4:
                stack[i]=Note4;
                stack2[i]=Note4_l;
                break;

              case 5:
                stack[i]=Note5;
                stack2[i]=Note5_l;
                break;

              case 6:
                stack[i]=Note6;
                stack2[i]=Note6_l;
                break;

              case 7:
                stack[i]=Note7;
                stack2[i]=Note7_l;
                break;

              case 8:
                stack[i]=Note8;
                stack2[i]=Note8_l;
                break;

              default:
                stack[i]=0;
                stack2[i]=0;
                break;
            }
        }
        /*if(currentAngle<stack[0]) direction = 1; //Sets initial direction of the motor
        else{
          direction = 0;
        }*/
        delay(10);
        state = PLAY;
        break;

      case PLAY: //Running through notes state
          /*========================================================
          THIS STATE RUNS THROUGH THE NOTES DECODED INTO A REGISTER
          ========================================================*/
          //if(direction){ //checks if the motor supposed to go up or down 
          delay(1);
            if(done_flag2||1){
              if (angle_out_r>0){
               // R_Motor_F_state = LOW;
                digitalWrite(dir_pin_1,LOW);
                speed_1_bit_1_value = 0;
                speed_1_bit_2_value = 0;
                speed_1_bit_3_value = 1;
                digitalWrite(speed_1_bit_1_pin,speed_1_bit_1_value);
                digitalWrite(speed_1_bit_2_pin,speed_1_bit_2_value);
                digitalWrite(speed_1_bit_3_pin,speed_1_bit_3_value);
               // R_Motor_R_state = HIGH;
              }
              else if (angle_out_r<0){
              //  R_Motor_F_state = HIGH;
                //R_Motor_R_state = LOW;
                digitalWrite(dir_pin_1,HIGH);
                speed_1_bit_1_value = 0;
                speed_1_bit_2_value = 0;
                speed_1_bit_3_value = 1;
                digitalWrite(speed_1_bit_1_pin,speed_1_bit_1_value);
                digitalWrite(speed_1_bit_2_pin,speed_1_bit_2_value);
                digitalWrite(speed_1_bit_3_pin,speed_1_bit_3_value);
              }
              else { //if the current angled surpassed the desired one. We mark this part as complete and move on to the next note.
                speed_1_bit_1_value = 0;
                speed_1_bit_2_value = 0;
                speed_1_bit_3_value = 0;
                digitalWrite(speed_1_bit_1_pin,speed_1_bit_1_value);
                digitalWrite(speed_1_bit_2_pin,speed_1_bit_2_value);
                digitalWrite(speed_1_bit_3_pin,speed_1_bit_3_value);
                done_flag2 = 1; //cheesing 
                done_flag1 = 1; //cheesing
                if(done_flag2&&done_flag1){
                  //Serial.print("12 done in 1 \n");
                  if(retract_flag1){
                    j++;
                    //Serial.print("j incremented \n");
                    retract_flag1 = 0;
                  } else {
                    retract_flag1 = 1;
                  }

                  if(retract_flag2){
                    k++;
                    //Serial.print("k incremented \n");
                    retract_flag2 = 0;
                  } else {
                    retract_flag2 = 1;
                  }
                  //Serial.print("Flags are cleared, transition raised \n");
                  done_flag2 = 0;
                  done_flag1 = 0;
                  transition_flag = 1;
                }

              //  R_Motor_F_state = LOW;
               // R_Motor_R_state = LOW;
                if(!transition_flag){
                  if(!done_flag1){
                    change1_flag = 1;
                  }else {
                    change1_flag = 0;
                  }
                  done_flag1 = 1;
                  if(change1_flag){
                    //Serial.print("flag 1 raised \n");
                  }
                }
              }
            }
            if(!done_flag2){
              if (angle_out_l>0){
                //L_Motor_F_state = HIGH;
                //L_Motor_R_state = LOW;
                digitalWrite(dir_pin_2,LOW);
                digitalWrite(speed_2_bit_1_pin,speed_2_bit_1_value);
                digitalWrite(speed_2_bit_2_pin,speed_2_bit_2_value);
                digitalWrite(speed_2_bit_3_pin,speed_2_bit_3_value);
              }
              else if (angle_out_l<0){
                //L_Motor_F_state = LOW;
                //L_Motor_R_state = HIGH;
                digitalWrite(dir_pin_2,LOW);
                digitalWrite(speed_2_bit_1_pin,speed_2_bit_1_value);
                digitalWrite(speed_2_bit_2_pin,speed_2_bit_2_value);
                digitalWrite(speed_2_bit_3_pin,speed_2_bit_3_value);
              }
              else { //if the current angled surpassed the desired one. We mark this part as complete and move on to the next note.
                speed_2_bit_1_value = 0;
                speed_2_bit_2_value = 0;
                speed_2_bit_3_value = 0;
                if(done_flag1&&done_flag2){
                  //Serial.print("12 done in 2 \n");
                  if(retract_flag2){
                    k++;
                    //Serial.print("k2 incremented \n");
                    retract_flag2 = 0;
                  } else {
                    retract_flag2 = 1;
                  }

                  if(retract_flag1){
                    j++;
                    //Serial.print("j2 incremented \n");
                    retract_flag1 = 0;
                  } else {
                    retract_flag1 = 1;
                  }
                  done_flag1 = 0;
                  done_flag2 = 0;
                  trans2_flag = 1;
                  //Serial.print("flags cleared in 2 \n");
                }
            // Serial.print("uwu");
               // L_Motor_F_state = LOW;
               // L_Motor_R_state = LOW;
                if(!done_flag2){
                    change2_flag = 1;
                  }else {
                    change2_flag = 0;
                  }
                if(!transition_flag&&!trans2_flag){
                  done_flag2 = 1;
                  if(change2_flag){
                    //Serial.print("flag 2 raised \n");
                  }
                  //Serial.print("flag 2 raised \n");
                }else {
                  transition_flag = 0;
                }
                transition_flag = 0;
                trans2_flag = 0;
                
              }  
            }else {
               // L_Motor_F_state = LOW;
               // L_Motor_R_state = LOW;
              }
         
          //state = RESET;
          if(j>stack_counter||k>stack_counter) state = RESET;
          break;

      case DEBUG:
      
       /*  if (position < desiredPos_R){  //send right motor forward
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
         }*/
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


    position = calculate_encoder_rotation(digitalRead(8), digitalRead(9), lastordernum, position);
    lastordernum = calculate_order_num(digitalRead(8), digitalRead(9));
    currentAngle_r = posToAngle(position);


   /*===========================================================================
   Encoder 2 code and CALCULATIONS
   ==================================================================================*/

   encoder2_pos = calculate_encoder_rotation(digitalRead(10), digitalRead(11), lastordernum2, encoder2_pos);
   lastordernum2 = calculate_order_num(digitalRead(10), digitalRead(11)); //MAKE LAST ORDERNUM NEW FOR THE NEXT CYCLE
   currentAngle_l = posToAngle(encoder2_pos);

   
  /*====================================================================
  PID AND ANGLE CALCULATIONS
  ======================================================================*/
  setpoint_r = set_coordinate(retract_flag1, stack[j]);
  setpoint_l = set_coordinate(retract_flag2, stack2[k]);
 
  angle_out_l = computePIDleft(currentAngle_l);
  angle_out_r = computePIDright(currentAngle_r);

   if(prevPos!=position||encoder2_pos_prev!=encoder2_pos&&1){ //PRINTING FUNCTION. WILL PRINT ONLY IF THE VALUES ARE DIFFERENT
      display_properties();
      prevPos = position;
      encoder2_pos_prev = encoder2_pos;
  }
}



/*===================================================================================
SET OF FUNCTIONS FOR ISR, PID AND ANGLES
===================================================================================*/

short int calculate_encoder_rotation(bool wire_A, bool wire_B, short int ordernum_prev,short int position_old){
  int short ordernum_current;
  ordernum_current = calculate_order_num(wire_A,wire_B);
  if(ordernum_prev==0){ //EXTRA CHECK TO COUNTER 0 TO 3 AND 3 TO 0 ROLLOVERS
      if(ordernum_current == 3){
         position_old --;
      } else if(ordernum_current == 1){
         position_old ++;
      }
    } 
  else if(ordernum_prev==3){
      if(ordernum_current==0){
        position_old++;
      }else if(ordernum_current==2){
        position_old--;
      }
    } 
  else {
      if (ordernum_current>ordernum_prev){
         position_old++;
      }else if(ordernum_current<ordernum_prev){
         position_old--;
      }
  }
  //lastordernum = ordernum_current; //MAKE LAST ORDERNUM NEW FOR THE NEXT CYCLE
  return position_old;
}

short int calculate_order_num(bool wire_A, bool wire_B){
  short int result;
  if(!wire_A&&!wire_B){ //00
       result = 0;
      } 
    else if(wire_A&&!wire_B){ //10
       result = 1;
      } 
    else if(wire_A&&wire_B){ //11
       result = 2;
      } 
    else {//01
       result = 3;
      }
    return result;
}

double set_coordinate(bool retract_flag, double value){
  if(retract_flag){
    return 0;
  }else {
    return value;
  }
}

void display_properties(){
      /*Console.print(position);
      Console.print("\t");

      Console.print(" Position 2 is: ");
      Console.print(encoder2_pos);
      Console.print("\t");*/

      Console.print("Angle Right  is: ");
      Console.print(currentAngle_r);
      Console.print("\t");

      Console.print(" Angle Left is: ");
      Console.print(currentAngle_l);
      Console.print("\n");

      /*Console.print("Current Stack Value: ");
      Console.print(stack[j]);
      Console.print("\t");

      Console.print("Current Stack k Value: ");
      Console.print(stack[k]);
      Console.print("\n");

      Console.print("Current State Value: ");
      Console.print(state);
      Console.print("\n");*/

     /* Console.print("Current direction Value: ");
      Console.print(direction);
      Console.print("\n");*/

      Console.print("Current j Value: ");
      Console.print(j);
      Console.print("\t");/*

      Console.print("Current k Value: ");
      Console.print(k);
      Console.print("\n");*/

      Console.print("Current state Value: ");
      Console.print(state);
      Console.print("\t");

      Console.print("Current PID Value: ");
      Console.print(angle_out_r);
      Console.print("\t");

      Console.print("Current PID left Value: ");
      Console.print(angle_out_l);
      Console.print("\n");

      Console.print("Current setpoint right Value: ");
      Console.print(setpoint_r);
      Console.print("\t");

      Console.print("Current setpoint left Value: ");
      Console.print(setpoint_l);
      Console.print("\n");

      Console.print("done flag 1: ");
      Console.print(done_flag1);
      Console.print("\n");

      Console.print("done flag 2: ");
      Console.print(done_flag2);
      Console.print("\n");

      Console.print("retract_flag1: ");
      Console.print(retract_flag1);
      Console.print("\n");

      Console.print("retract_flag2: ");
      Console.print(retract_flag2);
      Console.print("\n");

      Console.print("trans flag 1: ");
      Console.print(transition_flag);
      Console.print("\n");

}

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

        elapsed_time_r = (double)(current_time_r - previous_time_r);        //compute time elapsed from previous computatio

      
        error_r = setpoint_r - angle_in_r;                              // determine error
        cumulative_error_r += error_r * elapsed_time_r;                // compute integral
        rate_error_r = (error_r - prev_error_r)/elapsed_time_r;   // compute derivative
        double angle_out_r = Kp*error_r + Ki*cumulative_error_r + Kd*rate_error_r;                //PID output               
     
        prev_error_r = error_r;                                //remember current error
        previous_time_r = current_time_r;                        //remember current time
 
        return angle_out_r;
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
