#include "global.h"
#include <fstream>
#include "stdio.h"
#include <time.h>

map<string, int>  Util::day_of_weeks_map;
map<string, int> Util::monthes_map;
std::string Util::day_of_weeks[7];
std::string monthes[13];
outFunc Util::global_print;

Util::Util(outFunc out){
	global_print = out;
	
	day_of_weeks_map.insert(pair<string, int>("Mon", 1));
	day_of_weeks_map.insert(pair<string, int>("Tue", 2));
	day_of_weeks_map.insert(pair<string, int>("Wed", 3));
	day_of_weeks_map.insert(pair<string, int>("Thu", 4));
	day_of_weeks_map.insert(pair<string, int>("Fri", 5));
	day_of_weeks_map.insert(pair<string, int>("Sat", 6));
	day_of_weeks_map.insert(pair<string, int>("Sun", 0));
	monthes_map.insert(pair<string, int>("Jan", 1));
	monthes_map.insert(pair<string, int>("Feb", 2));
	monthes_map.insert(pair<string, int>("Mar", 3));
	monthes_map.insert(pair<string, int>("Apr", 4));
	monthes_map.insert(pair<string, int>("May", 5));
	monthes_map.insert(pair<string, int>("Jun", 6));
	monthes_map.insert(pair<string, int>("Jul", 7));
	monthes_map.insert(pair<string, int>("Aug", 8));
	monthes_map.insert(pair<string, int>("Sep", 9));
	monthes_map.insert(pair<string, int>("Oct", 10));
	monthes_map.insert(pair<string, int>("Nov", 11));
	monthes_map.insert(pair<string, int>("Dec", 12));
}


void Util::Generate_simulation_door_status(int num_of_rooms, int num_of_measurements,time_t start_time) {
	for (int i = 0; i < num_of_rooms; i++) {
		ofstream fs;
		char path[50];
		sprintf(path, "../simulation/door_status_room_%d.txt", i);
		fs.open(path); // create a output stream, which open file  for writing
		if (!fs.is_open()) {
			global_print("unable to open file\n");
			return;
		}

		int time_interval = 1;
		struct tm* current_time = localtime(&start_time);
		time_t raw_time;
		for (int i = 0; i < num_of_measurements; i++) {

			// determin door closed/opened time
			std::string status = (current_time->tm_hour > 12 && current_time->tm_hour < 18) ? "opened" : "closed";

			//ctime(&raw_time);
			fs << status << " " << asctime(current_time);
			current_time->tm_hour += time_interval;
			
			raw_time = mktime(current_time); // automatically adjusts the time members if they are off-range
			current_time = localtime(&raw_time);
		}
		//fs << " ";
		fs.close();
	}
	
}

bool Util::GetDoorStatusFromFile(Room_Time rt, const char* path) {
	ifstream ifs; // steam to read from file
	ifs.open(path);
	if (!ifs.is_open()) {
		return false;
	}
	std::string status;
	int day_of_month, hour, minute, second;

	std::string month_str, wday_str;
	struct tm tm;
	while (!ifs.eof()) {
		ifs >> status >> wday_str >> month_str >> tm.tm_mday >> tm.tm_hour;
		tm.tm_mon = monthes_map[month_str] - 1;
		ifs.ignore(); // ignore delimiter ":"
		ifs >> tm.tm_min;
		ifs.ignore(); // ignore delimiter ":"
		ifs >> tm.tm_sec >> tm.tm_year;
		tm.tm_year -= 1900;
		struct tm* t = localtime(&rt.calendar_time);
		double diff = difftime(rt.calendar_time, mktime(&tm));
		if (diff < 1800) {  //  time different is small than 30min 
			ifs.close();
			return status.compare("closed");
		}
	}
	ifs.close();
	return false;
}