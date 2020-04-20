#include "EnterRoomTask.h"
#include <iostream>
#include <string>
#include <thread>
#include <chrono>
#include "global.h"
#include "Dispatcher.h"
#include <strstream>

using namespace std;

void EnterRoomTask::process(int robot_id,bool& measure_door)
{
	char path[50], buf[100];
	bool status;
	strstream stream;
	sprintf(path, "../simulation/door_status_room_%d.txt", rt.room_id);
	status = Util::GetDoorStatusFromFile(rt, path);
		
	sprintf(buf,"room %d door %s %s\n",rt.room_id, status ? "opened" : "closed", ctime(&rt.calendar_time));
	task_print(buf);
}
 