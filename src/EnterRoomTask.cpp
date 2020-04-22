#include "EnterRoomTask.h"
#include <iostream>
#include <string>
#include <thread>
#include <chrono>
#include "global.h"
#include "Dispatcher.h"
#include <strstream>
using namespace std;

EnterRoomTask::EnterRoomTask() {
	this->task_print = nullptr;
	this->rt = { 0,Util::start_time };
	this->task_id = rand() % 900 + 100;
}

EnterRoomTask::EnterRoomTask(outFunc out, Room_Time rt) {
	this->task_print = out;
	this->rt = rt;
	this->task_id = rand() % 900 + 100;
}

EnterRoomTask::EnterRoomTask(outFunc out)
{
	this->task_print = out;
	this->rt = { 0,Util::start_time };
	this->task_id = rand() % 900 + 100;
}


void EnterRoomTask::process(int robot_id,bool& measure_door)
{
	char path[50], buf[100];
	bool status;
	strstream stream;
	sprintf(path, "../simulation/door_status_room_%d.txt", rt.room_id);
	status = Util::GetDoorStatusFromFile(rt, path);
	sprintf(buf,"[Robot %d] [task %.3d] [room %d] [door %s] %s\n",robot_id,
		this->task_id, rt.room_id, status ? "opened" : "closed", ctime(&rt.calendar_time));
	task_print(buf);
}
 