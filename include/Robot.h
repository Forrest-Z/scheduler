#pragma once
#include "EnterRoomTask.h"
#include <condition_variable>
#include <mutex>

using namespace std;

class Robot {

	typedef void (*outFunc)(string s); // function pointer to print function

public:
	Robot() :id(0), task(nullptr), robot_print(nullptr), has_task(false), running(true) { ulock = unique_lock<mutex>(mtx); }
	Robot(int id) :id(id), task(nullptr), robot_print(nullptr),has_task(false), running(true) { ulock = unique_lock<mutex>(mtx); }
	Robot(int id,EnterRoomTask* task) :id(id), task(task), robot_print(nullptr), has_task(false), running(true) { ulock = unique_lock<mutex>(mtx); }
	Robot(int id, outFunc print) :id(id), task(nullptr), robot_print(print), has_task(false), running(true) { ulock = unique_lock<mutex>(mtx); }
	Robot(int id, EnterRoomTask* task, outFunc print):id(id),task(task), robot_print(print), has_task(false), running(true) { ulock = unique_lock<mutex>(mtx); }
	void run();
	int getId() { return id; }
	void setTask(EnterRoomTask* task) { this->task = task; this->has_task = true; }
	void getCondition(condition_variable* &cv) { cv = &(this)->cv; }
private:
	int id;
	bool has_task; 
	bool running;
	EnterRoomTask* task;
	outFunc robot_print;
	condition_variable cv;
	mutex mtx;
	unique_lock<mutex> ulock;

};