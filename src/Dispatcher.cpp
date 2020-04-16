#include "Dispatcher.h"
//#include "global.h"
#include <thread>

using namespace std;

mutex Dispatcher::robot_mutex;
mutex Dispatcher::task_mutex;
Dispatcher Dispatcher::dispatcher;
outFunc Dispatcher::dispatcher_print;
queue<Robot*> Dispatcher::robot_queue; // Store waiting robot
deque<EnterRoomTask*> Dispatcher::task_queue; // Store to do task 
vector<Robot*>Dispatcher::AllRobots; // Store robots created by dispatcher init process
vector<thread*> Dispatcher::threads; // Store threads created by dispatcher init process

void Dispatcher::init(int num, outFunc out)
{
	day_of_weeks_map.insert(pair<string, int>("Mon", 1));
	day_of_weeks_map.insert(pair<string, int>("Tue", 2));
	day_of_weeks_map.insert(pair<string, int>("Wed", 3));
	day_of_weeks_map.insert(pair<string, int>("Thu", 4));
	day_of_weeks_map.insert(pair<string, int>("Fri", 5));
	day_of_weeks_map.insert(pair<string, int>("Sat", 6));
	day_of_weeks_map.insert(pair<string, int>("Sun", 0));

	Robot* robot;
	thread* thread;
	for (int i = 0; i < num; i++) {
		robot = new Robot(i,out);
		AllRobots.push_back(robot);
		thread = new std::thread(&Robot::run, robot);
		threads.push_back(thread);
	}
	dispatcher_print = out;
	dispatcher_print("Dispatcher: Create " + std::to_string(num) + " robots\n");
}


bool Dispatcher::AddRobot(Robot* robot)
{
	bool wait = false;
	task_mutex.lock();

	if (!task_queue.empty()) {
		
		EnterRoomTask* task = task_queue.front();
		robot->setTask(task);
		task_queue.pop_front();
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
		task_queue.push_back(task);  // add task to task queue
		task_mutex.unlock();
	}
	
}

void Dispatcher::stop()
{
	for (int i = 0; i < threads.size(); i++) {
		threads[i]->join();
	}
}

bool compare( EnterRoomTask* a, EnterRoomTask* b) {
	Room_Time rt1, rt2;
	a->getRoomAndTime(rt1);
	b->getRoomAndTime(rt2);
	return rt1<rt2;
}

void Dispatcher::SortTaskQueue() {

	sort(task_queue.begin(), task_queue.end(),compare);
}

