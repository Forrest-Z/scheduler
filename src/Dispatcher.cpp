#include "Dispatcher.h"
#include <thread>

using namespace std;

mutex Dispatcher::robot_mutex;
mutex Dispatcher::task_mutex;
Dispatcher Dispatcher::dispatcher;
outFunc Dispatcher::dispatcher_print;
queue<Robot*> Dispatcher::robot_queue; // Store waiting robot
queue<EnterRoomTask*> Dispatcher::task_queue; // Store to do task 
vector<Robot*>Dispatcher::AllRobots; // Store robots created by dispatcher init process
vector<thread*> Dispatcher::threads; // Store threads created by dispatcher init process

void Dispatcher::init(int num)
{
	Robot* robot;
	thread* thread;
	for (int i = 0; i < num; i++) {
		robot = new Robot(i);
		AllRobots.push_back(robot);
		thread = new std::thread(&Robot::run, robot);
		threads.push_back(thread);
	}
}

void Dispatcher::init(int num, outFunc robot_print)
{
	Robot* robot;
	thread* thread;
	for (int i = 0; i < num; i++) {
		robot = new Robot(i, robot_print );
		AllRobots.push_back(robot);
		thread = new std::thread(&Robot::run, robot);
		threads.push_back(thread);
	}

	dispatcher_print("Dispatcher: Create " + std::to_string(num) + " robots\n");
}


bool Dispatcher::AddRobot(Robot* robot)
{
	bool wait = false;
	task_mutex.lock();

	if (!task_queue.empty()) {
		
		EnterRoomTask* task = task_queue.front();
		robot->setTask(task);
		task_queue.pop();
		task_mutex.unlock();
	}
	else {
		// No available task in task queue. Add the robot in waiting queue
		task_mutex.unlock();
		robot_mutex.lock();
		robot_queue.push(robot);
		robot_mutex.unlock();
		wait = true;
	}

	return false;
}

void Dispatcher::AddTask(EnterRoomTask* task)
{
	robot_mutex.lock(); // allow only one thread to chage robot queue
	if (!robot_queue.empty()) {
		
		Robot* robot = robot_queue.front();
		robot->setTask(task); //assign a tassk to this robot

		condition_variable* cv;
		robot->getCondition(cv);
		cv->notify_one();  // unblock one waiting threads.
		robot_queue.pop(); // remove robot from queue	
		robot_mutex.unlock();
		
	}
	else {
		robot_mutex.unlock();
		// No available robot in robot queue. Add the task in waiting queue
		task_mutex.lock();		// allow only one thread access task queue
		task_queue.push(task);  // add task to task queue
		task_mutex.unlock();
	}
	
}

void Dispatcher::stop()
{
	for (int i = 0; i < threads.size(); i++) {
		threads[i]->join();
	}
}

