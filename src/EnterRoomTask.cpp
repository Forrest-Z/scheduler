#include "EnterRoomTask.h"
#include <iostream>
#include <string>
#include <thread>
#include <chrono>

using namespace std;

void EnterRoomTask::process(int robot_id)
{
	task_print("robot "+ std::to_string(robot_id) + " enter room " + std::to_string(id)+"\n");
	this_thread::sleep_for(chrono::seconds(2));
}