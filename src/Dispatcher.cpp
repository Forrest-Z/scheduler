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
	SetOut(out);
	CreateRobots(num);
}

void Dispatcher::CreateRobots(int num) {
	
	Robot* robot;
	thread* thread;
	for (int i = 0; i < num; i++) {
		robot = new Robot(i,dispatcher_print);
		AllRobots.push_back(robot);
		thread = new std::thread(&Robot::run, robot);
		threads.push_back(thread);
	}
	dispatcher_print("Dispatcher: Create " + std::to_string(num) + " robots\n");
}

void Dispatcher::CreateRandomTasks(int num,time_t start_time) {

	for (int i = 0; i < num; i++) {
		Room_Time rt;
		long increase_sec = rand() % (60 * 60 * 24 * 30) +(60*60);
		rt.calendar_time = start_time + increase_sec;
		// rt.day_of_week = rand() % (5 - 1 + 1) + 1; // create a day( monday - friday)
		rt.room_id = rand() % (3 - 1 + 1) + 1; // create a room id(1-3)
		struct tm* tmp1 = localtime(&(start_time));
		struct tm* tmp = localtime(&(rt.calendar_time));
			// rand() % (18 - 8 + 1) + 8; // create a time(8am -18pm)
		EnterRoomTask* task = new EnterRoomTask(dispatcher_print);
		task->setRoomAndTime(rt);
		Dispatcher::AddTask(task);
		Dispatcher::SortTaskQueue();
	}
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

