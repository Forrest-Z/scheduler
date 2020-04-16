#include "EnterRoomTask.h"
#include <iostream>
#include <string>
#include <thread>
#include <chrono>

using namespace std;

void EnterRoomTask::process(int robot_id)
{
	task_print("time " + std::to_string(time_point) + ": robot "+ std::to_string(robot_id) + " enter room " + std::to_string(id)+"\n");
	
}