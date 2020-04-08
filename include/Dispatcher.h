#pragma once

#include "EnterRoomTask.h"
#include "Robot.h"
#include <queue>
#include <vector>
#include <thread>
#include <mutex>

using namespace std;

class Dispatcher {

	typedef void (*outFunc)(string s); // function pointer to print function
	
public:
	static void init(int num);  // Create num robot thread
	static void init(int num, outFunc robot_print);
	static bool AddRobot(Robot* robot);	// Add robot to robot queue
	static void AddTask(EnterRoomTask* task); // Add task to task queue
	static void SetOut(outFunc print) { dispatcher_print = print; }
	static Dispatcher GetDispatcher() { return dispatcher; }
	static void stop();
private:
	static mutex robot_mutex;
	static mutex task_mutex;
	static outFunc dispatcher_print;
	static queue<Robot*> robot_queue; // Store waiting robot
	static queue<EnterRoomTask*> task_queue; // Store to do task 
	static vector<Robot*> AllRobots; // Store robots created by dispatcher init process
	static vector<thread*> threads;
	static Dispatcher dispatcher;
};