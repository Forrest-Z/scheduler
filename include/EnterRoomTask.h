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
	int getRoomId() { return rt.room_id; }
	time_t getCalendarTime() { return rt.calendar_time; }
	void setTaskId(int id) { this->task_id = task_id; }
	int getTaskId() { return task_id; }
	Room_Time rt;
	int task_id;
private:
	outFunc task_print;

};