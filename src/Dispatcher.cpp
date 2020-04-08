#include "Dispatcher.h"

using namespace std;

Dispatcher Dispatcher::dispatcher;
outFunc Dispatcher::dispatcher_print;
queue<Robot*> Dispatcher::robot_queue; // Store waiting robot
queue<EnterRoomTask*> Dispatcher::task_queue; // Store to do task 
vector<Robot*>Dispatcher::AllRobots; // Store robots created by dispatcher init process

void Dispatcher::init(int num)
{
	for (int i = 0; i < num; i++) {
		Robot* robot = new Robot(i);
		AllRobots.push_back(robot);
	}
}

void Dispatcher::init(int num, outFunc robot_print)
{
	for (int i = 0; i < num; i++) {
		Robot* robot = new Robot(i, robot_print );
		AllRobots.push_back(robot);
		robot_queue.push(robot);
	}
	dispatcher_print("Dispatcher: Create " + std::to_string(num) + " robots\n");
}

bool Dispatcher::AddRobot(Robot* robot)
{
	return false;
}

void Dispatcher::AddTask(EnterRoomTask* task)
{
	if (!robot_queue.empty()) {
		Robot* robot = robot_queue.front();
		robot->setTask(task);
		robot->run();
		dispatcher_print(" robot " + std::to_string(robot->getId()) + "run task " + std::to_string(task->getId())+".\n");
	}
	else {
		dispatcher_print("no available robot");
	}
	//task_queue.push(task);
	
}

