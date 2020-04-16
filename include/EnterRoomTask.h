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

	int getRoomId() { return room_id; }
	void setRoom(int room);
	void setRoomAndTime(Room_Time rt) {
		this->rt = rt;
	}
	void getRoomAndTime(Room_Time& rt) {
		rt = this->rt;
	}
	bool operator < (EnterRoomTask const& other) {
		return this->rt.day_of_week < other.rt.day_of_week;
	}

private:
	int room_id;
	int time_point;
	outFunc task_print;
	Room_Time rt;
};