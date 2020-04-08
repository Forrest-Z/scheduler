#include "Task.h"
#include <iostream>
#include <string>

using namespace std;

void Task::process()
{
	task_print("enter room " + std::to_string(id) );
}