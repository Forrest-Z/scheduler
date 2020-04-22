#pragma once
#pragma once

#include <string>
#include "global.h"
//#include "Robot.h"
using namespace std;

class Robot;
typedef void (*outFunc)(string s); // function pointer to lock_print function

class EnterRoomTask {
public:
	//void process(int robot_id, bool status);
	void process(int robot_id, bool& measure_door);
	//void process(Robot* robot);
	EnterRoomTask(){}
	EnterRoomTask(outFunc lock_print) :task_print(lock_print){}
	EnterRoomTask(outFunc lock_print,Room_Time rt) :task_print(lock_print),rt(rt) {}
	void setRoom(int room);
	void setRoomAndTime(Room_Time rt) {
		this->rt = rt;
	}
	void getRoomAndTime(Room_Time& rt) {
		rt = this->rt;
	}
	Room_Time rt;
private:
	outFunc task_print;

};