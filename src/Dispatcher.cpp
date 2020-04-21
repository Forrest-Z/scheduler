#include "Dispatcher.h"
//#include "global.h"
#include <thread>
#include<cmath>
#include <vector>

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
		long increase_sec = rand() *79;
		rt.calendar_time = start_time + increase_sec;
		// rt.day_of_week = rand() % (5 - 1 + 1) + 1; // create a day( monday - friday)
		rt.room_id = rand() % (3 - 1 + 1) + 1; // create a room id(1-3)
		struct tm* tmp1 = localtime(&(start_time));
		struct tm* tmp = localtime(&(rt.calendar_time));
			// rand() % (18 - 8 + 1) + 8; // create a time(8am -18pm)
		EnterRoomTask* task = new EnterRoomTask(dispatcher_print);
		task->setRoomAndTime(rt);
		Dispatcher::AddTask(task);
	}
}


bool Dispatcher::CompareTaskCost(pair<EnterRoomTask*, int> &pair1, pair<EnterRoomTask*, int> &pair2) {
	return pair1.second > pair2.second;
}

void Dispatcher::AssignTaskToRobot(Robot* robot) {
	double second_diff;
	int cost, distance;
	vector<pair<EnterRoomTask*, int>> cost_vector;
	deque<EnterRoomTask*>::iterator it = task_queue.begin();
	while (it != task_queue.end()) {
		second_diff = difftime((*it)->rt.calendar_time, robot->getCurrentTime()); // second different between robot time and task time
		distance = abs(robot->getCurrentRoomId() - (*it)->rt.room_id); // difference between room id 
		cost = distance + second_diff / 3600; // distance + hour different
		cost_vector.push_back(make_pair((*it), cost)); // put task and cost in a vector
		it++;
	}
	std::sort(cost_vector.begin(), cost_vector.end(), CompareTaskCost);
	EnterRoomTask* task = cost_vector.back().first;
	robot->setTask(task);
	
	// find task in task queue and delete it
	it = find(task_queue.begin(), task_queue.end(), task);
	task_queue.erase(it);
	cost_vector.pop_back();
	
}
bool Dispatcher::AddRobot(Robot* robot)
{
	bool wait = false;
	
	task_mutex.lock();

	if (!task_queue.empty()) {
		// Get every task in task queue
		AssignTaskToRobot(robot);
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
		
		AssignTaskToRobot(robot); //assign a tassk to this robot
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

//
//void Dispatcher::SortTaskQueue() {
//
//	sort(task_queue.begin(), task_queue.end(),compare);
//}

