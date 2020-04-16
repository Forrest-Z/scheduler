#pragma once

#include "EnterRoomTask.h"
#include <condition_variable>
#include <mutex>
#include <map>
using namespace std;

typedef struct table_key{
	int room_id;
	int day_of_week;
	int time;
	friend bool operator < (table_key key1, table_key key2) {
		return key1.time > key2.time; // ealier time will be in the frount
	}
}Table_key;

typedef struct  {
	int closed_count;
	int opened_count;
}Table_value;

class Robot {

	typedef void (*outFunc)(string s); // function pointer to lock_print function

public:
	//Robot() :id(0), task(nullptr), robot_print(nullptr), has_task(false), running(true) { ulock = unique_lock<mutex>(mtx); }
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
private:
	int id;
	bool has_task; 
	bool running;
	EnterRoomTask* task;
	outFunc robot_print;
	condition_variable cv;
	mutex mtx;
	unique_lock<mutex> ulock;
	map<Table_key, Table_value> occ_info;
};