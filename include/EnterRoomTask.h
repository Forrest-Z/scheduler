#pragma once
#pragma once
#include <string>

using namespace std;

typedef void (*outFunc)(string s); // function pointer to print function

class EnterRoomTask {
public:
	void process(int robot_id);
	EnterRoomTask() :id(0), task_print(nullptr) {}
	EnterRoomTask(int id) :id(id), task_print(nullptr) {}
	EnterRoomTask(int id, outFunc print) :id(id), task_print(print) {}
	int getId() { return id; }
private:
	int id;
	outFunc task_print;
};