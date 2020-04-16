
#include "Dispatcher.h"
#include "global.h"
#include <fstream>
#include <sstream>

using namespace std;

//
//void Robot::Write_data_in_occ_map(int room, int time, int day_of_week) {
//	
//	char path[50];
//	sprintf(path, "../simulation/door_status_room_%d.txt",room);
//	
//	Room_Time key;
//	key.room_id = room;
//	key.day_of_week = day_of_week;
//	key.time = time;
//	Door_cnt& value = occ_info[key];
//	if (getDoorStatusFromFile(room,time,day_of_week,path)) {
//		value.opened_count++;
//	}
//	else {
//		value.closed_count++;
//	}
//}

void Robot::run()
{
	while (running)
	{
		if (has_task) {
			//task->getRoomAndTime(rt);
			//bool status = getDoorStatusFromFile(rt, "../simulation/door_status_room_1.txt"); // measure door status
			//task->process(id,status);
			bool result;
			task->process(id,result);
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
