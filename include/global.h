#pragma once

#include <string>
#include <map>
#include <thread>

//#include "Robot.h"
//#include "EnterRoomTask.h"

using namespace std;

typedef struct Room_Time {
	int room_id;
	time_t calendar_time;
}Room_Time;

typedef struct Occu_params {
	int closed_count;
	int opened_count;
	int occupancy_possibility;

	bool operator < (const Occu_params &other) {
		return this->occupancy_possibility < other.occupancy_possibility;
	}
}Occu_params;

typedef struct Occu_key {
	int room_id;
	int day_of_week;
	int hour;
	friend bool operator < (const Occu_key& first,const  Occu_key &second) {
		int ret = false;
		if (first.room_id < second.room_id) { ret =  true;}
		else if (first.room_id == second.room_id){
			if (first.day_of_week < second.day_of_week) {ret = true; }
			else if(first.day_of_week == second.day_of_week){
				if (first.hour < second.hour) {ret=true; }
			}
		}
		return ret;
	}
}Occu_key;

typedef void (*outFunc)(string s); // function pointer to lock_print function

class Util {
	
public:
	Util(outFunc out);
	
	static bool GetDoorStatusFromFile(Room_Time rt, const char* path);
	static void Generate_simulation_door_status(int num_of_rooms, int num_of_measurements, time_t start_time);
	static void Generate_occupancy_table();
	
	static map<string, int>  day_of_weeks_map;
	static map<string,int> monthes_map;
	//static bool Compare(const Occu_key first, const Occu_key second);
	static map<Occu_key,Occu_params> occu_table;
	static std::string day_of_weeks[7];
	static std::string monthes[13];

	static time_t start_time;
	static const long TASK_DELAY_SEC = 10800; // 3 hours
private:
	static outFunc global_print;
	
};
