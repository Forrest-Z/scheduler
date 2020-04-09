#pragma once
#pragma once
#include <string>

using namespace std;

typedef void (*outFunc)(string s); // function pointer to lock_print function

class EnterRoomTask {
public:
	void process(int robot_id);
	EnterRoomTask() :id(0), time_point(0),task_print(nullptr) {}
	EnterRoomTask(int id) :id(id), time_point(0), task_print(nullptr) {}
	EnterRoomTask(int id,int time_point) :id(id), time_point(time_point),task_print(nullptr) {}
	EnterRoomTask(int id, int time_point, outFunc lock_print) :id(id), time_point(time_point), task_print(lock_print) {}
	int getId() { return id; }
private:
	int id;
	int time_point;
	outFunc task_print;
};