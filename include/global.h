#pragma once

#include <string>
#include <map>

//#include "Robot.h"
//#include "EnterRoomTask.h"

using namespace std;

typedef struct room_info {
	int id;
	int  occupy_posibility;
	friend bool operator < (room_info a, room_info b)
	{
		return a.occupy_posibility > b.occupy_posibility;   // if a.posibility > b.posibility, a > b, a is near the top
	}
} Room_info;

typedef struct Room_Time {
	int room_id;
	time_t calendar_time;
}Room_Time;

typedef struct {
	int closed_count;
	int opened_count;
}Door_cnt;

typedef void (*outFunc)(string s); // function pointer to lock_print function

class Util {
	
public:
	Util(outFunc out);
	static bool GetDoorStatusFromFile(Room_Time rt, const char* path);
	static map<string, int>  day_of_weeks_map;
	static map<string,int> monthes_map;
	static std::string day_of_weeks[7];
	static std::string monthes[13];
	static void Generate_simulation_door_status(int num_of_rooms, int num_of_measurements, time_t start_time);
	static time_t start_time;
private:
	static outFunc global_print;
};

