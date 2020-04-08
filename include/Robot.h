#pragma once
#include "EnterRoomTask.h"

using namespace std;

class Robot {

	typedef void (*outFunc)(string s); // function pointer to print function

public:
	Robot() :id(0), task(nullptr), robot_print(nullptr) {}
	Robot(int id,EnterRoomTask* task) :id(id), task(task), robot_print(nullptr) {}
	Robot(int id, outFunc print) :id(id), task(nullptr), robot_print(print) {}
	Robot(int id, EnterRoomTask* task, outFunc print):id(id),task(task), robot_print(print){}
	void run();
	void setTask(EnterRoomTask* task) { this->task = task; }
private:
	int id;
	EnterRoomTask* task;
	outFunc robot_print;

};