#include "Robot.h"
#include "Dispatcher.h"
#include "global.h"
#include <fstream>
#include <sstream>

using namespace std;

bool Robot::getDoorStatusFromFile(int room, int time, int day_of_week, const char* path) {
	ifstream ifs; // steam to read from file
	ifs.open(path);
	if (!ifs.is_open()) {
		robot_print("Unable to open file " + std::string(path));
	}
	std::string status, wday, month, year, str, blank, hour_minute_second;
	int day_of_month, hour, minute, second;
	while (!ifs.eof()) {
		ifs >> status >> wday >> month >> day_of_month >> hour;
		ifs.ignore(); // ignore delimiter ":"
		ifs >> minute;
		ifs.ignore(); // ignore delimiter ":"
		ifs >> second >> year;
		int equal = day_of_weeks[day_of_week].compare(wday);
		if (time == hour && equal == 0) {	// 
			//robot_print("Door " + status + "\n");
			ifs.close();
			return status.compare("closed");
		}
	}
	ifs.close();
	return false;
}

void Robot::Write_data_in_occ_map(int room, int time, int day_of_week) {
	
	char path[50];
	sprintf(path, "../simulation/door_status_room_%d.txt",room);
	
	Table_key key;
	key.room_id = room;
	key.day_of_week = day_of_week;
	key.time = time;
	Table_value& value = occ_info[key];
	if (getDoorStatusFromFile(room,time,day_of_week,path)) {
		value.opened_count++;
	}
	else {
		value.closed_count++;
	}
}

void Robot::run()
{
	while (running)
	{
		if (has_task) {
			
			task->process(id);

			has_task = false;
		}

		if (Dispatcher::AddRobot(this)) {
			// When no task in task queue, wait that task thread run Disparture 
			while (!has_task && running) // wait dispatcher call robot set task function
			{			
				cv.wait(ulock);	// block the thread until it was assigned to a task.
			}
		}
	}

}
