#include "EnterRoomTask.h"
#include <iostream>
#include <string>
#include <thread>
#include <chrono>
#include "global.h"
#include "Dispatcher.h"
#include <strstream>
using namespace std;

EnterRoomTask::EnterRoomTask():EnterRoomTask(nullptr) {
}

EnterRoomTask::EnterRoomTask(outFunc out, Room_Time rt) {
	this->task_print = out;
	this->priority = 0;
	this->rt = rt;
	this->task_id = rand() % 900 + 100;
}

EnterRoomTask::EnterRoomTask(outFunc out)
{
	this->rt = { 0,Util::start_time };
	this->task_id = rand() % 900 + 100;
	this->task_print = out;
	this->priority = 0;
}


bool EnterRoomTask::process(int robot_id)
{
	char path[50], buf[100];
	bool status;
	strstream stream;
	sprintf_s(path, "../simulation/door_status_room_%d.txt", rt.room_id);
	status = Util::GetDoorStatusFromFile(rt, path);
	sprintf_s(buf,"[Robot %d] [task %.3d] [periority %d] [room %d] [door %s] %s",robot_id,
		this->task_id, this->priority,rt.room_id,status ? "opened" : "closed", ctime(&rt.calendar_time));
	task_print(buf);
	return status; // return door status
}
 