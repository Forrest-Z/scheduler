#include "Dispatcher.h"
//#include "global.h"
#include <thread>
#include<cmath>
#include <vector>

using namespace std;

mutex Dispatcher::robot_mutex;
mutex Dispatcher::task_mutex;
mutex Dispatcher::timer_mutex;
Dispatcher Dispatcher::dispatcher;
outFunc Dispatcher::dispatcher_print;
queue<Robot*> Dispatcher::robot_queue; // Store waiting robot
deque<EnterRoomTask*> Dispatcher::task_queue; // Store to do task 
vector<Robot*>Dispatcher::AllRobots; // Store robots created by dispatcher init process
vector<thread*> Dispatcher::threads; // Store threads created by dispatcher init process
thread* Dispatcher::timer_thread;
time_t Dispatcher::global_time;

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


void Dispatcher::StartTimer(time_t start_time) {
	SetGlobalTime(start_time);
	timer_thread = new thread(&Timer);
}

void Dispatcher::Timer() {
	dispatcher_print("Timer running\n");
	while (true) {
		IncreaseGlobalTime(10800); // increase global timer a hour
		this_thread::sleep_for(chrono::seconds(1));  // threadsleep for 2 second
	}
}

void Dispatcher::StopTimer() {
	dispatcher_print("Timer stoped\n");
	timer_thread->join();
}

void Dispatcher::CreateRandomTasks(int num,time_t start_time) {
	int cnt = 0;
	while(cnt < num) {
		Room_Time rt;
		char buf[50];
		long increase_sec = rand() * 8;
		int priority = rand()%4 +1; // priority 1-5
		rt.calendar_time = start_time + increase_sec;
		rt.room_id = rand() % 5; // create a room id(0-4)
		struct tm* t = localtime(&(rt.calendar_time));
		if (t->tm_wday > 0 && t->tm_wday < 6 && t->tm_hour > 9 && t->tm_hour < 20) {
			// filter task on Monday to Friday from 9 am to 20 pm
			EnterRoomTask* task = new EnterRoomTask(dispatcher_print);
			task->setRoomAndTime(rt);
			task->setTaskPriority(priority);
			Dispatcher::AddTask(task);
			ctime_s(buf, sizeof(buf), &rt.calendar_time);
			dispatcher_print("Create Task " + std::to_string(task->getTaskId()) + " priority " + 
				std::to_string(task->getPriority()) +	" room " + std::to_string(task->getRoomId()) + " " +  buf);
			cnt++;
		}
	}
}

bool Dispatcher::CompareTaskCost(pair<EnterRoomTask*, int> &pair1, pair<EnterRoomTask*, int> &pair2) {
	return pair1.second > pair2.second;  // hiher cost means lower periority 
}

int Dispatcher::CalculateTaskCostForRobot(EnterRoomTask* task, Robot* robot) {
	double second_diff;
	int cost, distance, occu;
	struct tm t;
	time_t carlendar_time = task->getCalendarTime();
	time_t global_time = GetGlobalTime();
	second_diff = difftime(carlendar_time, global_time); // second different between robot time and task time
	distance = abs(robot->getCurrentRoomId() - task->getRoomId()); // difference between room id 
	
	Occu_key key;
	localtime_s(&t, &carlendar_time); // find occupancy possibility in table
	key.room_id = task->getRoomId();
	key.hour = t.tm_hour;
	key.day_of_week = t.tm_wday;
	Occu_params& params = Util::occu_table[key];
	occu = params.occupancy_possibility;
	
	cost = (5- task->getPriority()) * 10 + (100 - robot->getBettery_level()) + distance + second_diff / 3600  +  100 - occu; // distance + hour different  + 100 -  occupancy possibility
	return cost;
}
bool Dispatcher::AssignTaskToRobot(Robot* robot) {
	
	vector<pair<EnterRoomTask*, int>> cost_vector;
	deque<EnterRoomTask*>::iterator it = task_queue.begin();
	char buf[100];
	while (it != task_queue.end()) {

		if ((*it)->getCalendarTime() >Dispatcher::GetGlobalTime()) { // only look at task which later than current time
			int cost = CalculateTaskCostForRobot(*it, robot);
			cost_vector.push_back(make_pair(*it, cost)); // put task and cost in a vector		
		}
		it++;
	}
	if (cost_vector.empty()) {
		return false; // if no available task ( all tasks are old tasks), return false
	}
	std::sort(cost_vector.begin(), cost_vector.end(), CompareTaskCost);
	pair<EnterRoomTask*, int> task_cost = cost_vector.back();
	
	sprintf(buf, "Robot %d(bettery  %d%%)  get task %d cost %d from centralized poor\n",
	robot->id, robot->getBettery_level(),task_cost.first->getTaskId(), task_cost.second);
	dispatcher_print(buf);
	robot->setTask(task_cost.first);
	
	// find task in task queue and delete it
	it = find(task_queue.begin(), task_queue.end(), task_cost.first);
	task_queue.erase(it);
	cost_vector.pop_back();
	return true;
	
}
bool Dispatcher::AddRobot(Robot* robot)
{
	bool wait = false;

	task_mutex.lock();

	if (!task_queue.empty()) {
		// Get every task in task queue
		wait = !AssignTaskToRobot(robot);
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

void Dispatcher::ReturnTask(EnterRoomTask* task) {
	
	struct tm* tmp = localtime(&(task->rt.calendar_time));
	if ((tmp->tm_hour ) > 17) {  // if it is already 17pm, run task at 8 am tomorrow
															// otherwise this task 3 hours later 
		tmp->tm_hour =  8;
		tmp->tm_mday++;
	}
	else {
		tmp->tm_hour +=3;
	}
	task->rt.calendar_time = mktime(tmp);
	AddTask(task);
}
void Dispatcher::AddTask(EnterRoomTask* task)
{
	robot_mutex.lock(); // allow only one thread to chage robot queue
	if (!robot_queue.empty()) {
		
		Robot* robot = robot_queue.front();
		//robot->setTask(task);
		//AssignTaskToRobot(robot); //assign a tassk to this robot
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
	dispatcher_print("Dispatcher stop\n");
	StopTimer();
	for (int i = 0; i < threads.size(); i++) {
		threads[i]->join();
	}
	
}


void Dispatcher::SetGlobalTime(time_t time) {
	timer_mutex.lock();
	global_time = time;
	timer_mutex.unlock();
}

void Dispatcher::IncreaseGlobalTime(time_t second) {
	timer_mutex.lock();
	global_time += second;
	dispatcher_print(ctime(&global_time));
	timer_mutex.unlock();
}

time_t Dispatcher::GetGlobalTime() {
	time_t ret;
	timer_mutex.lock();
	ret = global_time;
	timer_mutex.unlock();
	return ret;
}

