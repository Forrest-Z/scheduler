#include "EnterRoomTask.h"
#include <iostream>
#include <string>
#include <thread>
#include <chrono>
#include "global.h"
#include "Dispatcher.h"
using namespace std;


void EnterRoomTask::process(int robot_id,bool& measure_door)
{
	char path[50];
	bool status;
	sprintf(path, "../simulation/door_status_room_%d.txt", rt.room_id);
	//task_print("time " + std::to_string(time_point) + ": robot "+ std::to_string(robot_id) + " enter room " + std::to_string(room_id)+"\n");
	status = getDoorStatusFromFile(rt, path);
	task_print("\n" + day_of_weeks[rt.day_of_week] +  " time " + std::to_string(rt.time) + ": robot " + std::to_string(robot_id) + " enter room " + std::to_string(rt.room_id) + ", door " + (status? "open" : "close") + "\n");

	// Door is open, add a task 
	if (status) {
		task_print(" Create a task on next " + day_of_weeks[rt.day_of_week] + " time " + std::to_string(rt.time) + " for room " + std::to_string(rt.room_id)+ "\n");
		Dispatcher::AddTask(new EnterRoomTask(task_print, rt));
	}
	
	measure_door = status;
}
 