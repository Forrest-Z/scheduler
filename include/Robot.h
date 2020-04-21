#pragma once


#include "global.h"
#include <condition_variable>
#include <mutex>
#include <map>
#include "EnterRoomTask.h"

using namespace std;


class Robot {

	typedef void (*outFunc)(string s); // function pointer to lock_print function

public:
	Robot() :id(0), task(nullptr), robot_print(nullptr), has_task(false), running(true) { ulock = unique_lock<mutex>(mtx); }
	//Robot(int id) :id(id), task(nullptr), robot_print(nullptr),has_task(false), running(true) { ulock = unique_lock<mutex>(mtx); }
	//Robot(int id,EnterRoomTask* task) :id(id), task(task), robot_print(nullptr), has_task(false), running(true) { ulock = unique_lock<mutex>(mtx); }
	Robot(int id, outFunc lock_print) :id(id), task(nullptr), robot_print(lock_print), has_task(false), running(true) { ulock = unique_lock<mutex>(mtx); }
	Robot(int id, EnterRoomTask* task, outFunc lock_print):id(id),task(task), robot_print(lock_print), has_task(false), running(true) { ulock = unique_lock<mutex>(mtx); }
	void run();
	int getId() { return id; }
	void setTask(EnterRoomTask* task) { this->task = task; this->has_task = true; }
	void getCondition(condition_variable* &cv) { cv = &(this)->cv; }
	//void createOccupationMap();
	// void InitializeOccMap();
	void Write_data_in_occ_map(int room_id, int time, int day_of_week);
	bool getDoorStatusFromFile(int room, int time, int day_of_week, const char* path);
	//bool getDoorStatusFromFile(Room_Time rt, const char* path);

	int id;
	bool has_task;
	bool running;
	EnterRoomTask* task;
	int getCurrentRoomId() { return has_task ? task->room_id: 0; }
	int getCurrentTime() { return has_task ? task->rt.calendar_time : Util::start_time; } // start time if does't have a current task
private:
	
	
	outFunc robot_print;
	condition_variable cv;
	mutex mtx;
	unique_lock<mutex> ulock;
	map<Room_Time, Door_cnt> occ_info;
};