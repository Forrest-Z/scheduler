#include "EnterRoomTask.h"
#include <iostream>
#include <string>

using namespace std;

void EnterRoomTask::process()
{
	task_print("enter room " + std::to_string(id) +"\n" );
}