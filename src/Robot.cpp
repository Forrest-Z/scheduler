#include "Robot.h"

using namespace std;

void Robot::run()
{
	robot_print("Robot " + std::to_string(id) + ": ");
	task->process();
}
