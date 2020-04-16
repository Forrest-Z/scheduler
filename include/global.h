#pragma once

#include <string>
#include <map>
#include <fstream>
//#include "Robot.h"
//#include "EnterRoomTask.h"

using namespace std;



static std::string day_of_weeks[7] = {
	"Sun",
	"Mon",
	"Tue",
	"Wed",
	"Thu",
	"Fri",
	"Sat"
};

static std::map<string, int>  day_of_weeks_map;


typedef struct Room_Time {
	int room_id;
	int day_of_week;
	int time;
	friend bool operator < (Room_Time rt1, Room_Time rt2) {

		return rt1.time > rt2.time; // ealier time will be in the frount
	}
}Room_Time;

typedef struct {
	int closed_count;
	int opened_count;
}Door_cnt;

static bool getDoorStatusFromFile(Room_Time rt, const char* path) {
	ifstream ifs; // steam to read from file
	ifs.open(path);
	if (!ifs.is_open()) {
		return false;
	}
	std::string status, wday, month, year, str, blank, hour_minute_second;
	int day_of_month, hour, minute, second;
	while (!ifs.eof()) {
		ifs >> status >> wday >> month >> day_of_month >> hour;
		ifs.ignore(); // ignore delimiter ":"
		ifs >> minute;
		ifs.ignore(); // ignore delimiter ":"
		ifs >> second >> year;
		int equal = day_of_weeks[rt.day_of_week].compare(wday);
		if (rt.time == hour && equal == 0) {	// 
			//robot_print("Door " + status + "\n");
			ifs.close();
			return status.compare("closed");
		}
	}
	ifs.close();
	return false;
}

