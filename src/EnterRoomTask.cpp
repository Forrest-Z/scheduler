#include "EnterRoomTask.h"
#include <iostream>
#include <string>
#include <thread>
#include <chrono>

using namespace std;

static int cnt = 0;
void EnterRoomTask::process(int robot_id)
{
	task_print("step" + std::to_string(cnt++)+": robot "+ std::to_string(robot_id) + " enter room " + std::to_string(id)+"\n");
	this_thread::sleep_for(chrono::seconds(3));
}