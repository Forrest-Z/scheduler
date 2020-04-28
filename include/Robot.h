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
	Robot::Robot();
	Robot::Robot(int id, outFunc lock_print);
	Robot::Robot(int id, EnterRoomTask* task, outFunc lock_print);
	void run();
	int getId() { return id; }
	void setTask(EnterRoomTask* task);
	void getCondition(condition_variable* &cv) { cv = &(this)->cv; }
	void Write_data_in_occ_map(int room_id, int time, int day_of_week);
	bool getDoorStatusFromFile(int room, int time, int day_of_week, const char* path);
	int bettery_level;
	int id;
	bool has_task;
	bool running;
	EnterRoomTask* task;
	int getCurrentRoomId() { return task->rt.room_id; }
	time_t getCurrentTime() { return task->rt.calendar_time; } // start time if does't have a current task
private:
	
	
	outFunc robot_print;
	condition_variable cv;
	mutex mtx;
	unique_lock<mutex> ulock;
};