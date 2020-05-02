#include"Motor.h"

void Motor::setSpeed(int speed_new) {
	speed_set = speed_new;
	if(state){
		speed_running = speed_set;
	}
}
int Motor::getSpeed(){
	return speed_running;
}
bool Motor::getState(){
	return state;
}
void Motor::turnOn(){
	state = 1;
	speed_running = speed_set;
}
void Motor::turnOff(){
	state = 0;
	speed_running = 0;
}
Motor::Motor(int initial_speed) {
	 speed_set = initial_speed;
	 state = 0;
	 speed_running = 0;
}
Motor::Motor() {
	 speed_set = 0;
	 state = 0;
	 speed_running = 0;
}
Motor::~Motor() {
}