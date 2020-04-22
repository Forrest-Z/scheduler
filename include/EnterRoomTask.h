#pragma once
#pragma once

#include <string>
#include "global.h"
//#include "Robot.h"
using namespace std;

class Robot;

class EnterRoomTask {
public:
	//void process(int robot_id, bool status);
	bool process(int robot_id);
	//void process(Robot* robot);
	EnterRoomTask();
	EnterRoomTask(outFunc out);
	EnterRoomTask(outFunc out, Room_Time rt);
	void setRoomAndTime(Room_Time rt) { this->rt = rt;}
	void getRoomAndTime(Room_Time& rt) {rt = this->rt;}
	void setTaskId(int id) { this->task_id = task_id; }

	Room_Time rt;
	int task_id;
private:
	outFunc task_print;

};