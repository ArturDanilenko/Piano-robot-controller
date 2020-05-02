#ifndef   __Motor__
#define   __Motor__

class Motor {
	int speed_set;
	int speed_running;
	bool state;
	
public:
	Motor();
	Motor(int);
	~Motor();
	void setSpeed(int);
	int getSpeed();
	bool getState();
	void turnOn();
	void turnOff();
	//void run(int);
};
#endif 

