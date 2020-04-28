
#include "EnterRoomTask.h"
#include "Robot.h"
#include "Dispatcher.h"
#include <mutex>
#include <iostream>
#include <chrono>
#include <csignal>
#include <set>
#include <fstream>
#include <time.h>
#include "global.h"

using namespace std;

sig_atomic_t signal_caught = 0;
mutex cout_mutex;

// Allow one thread lock_print text on stdout.
void lock_print(string s) {
	cout_mutex.lock();
	cout << s;
	cout_mutex.unlock();
}

// When user type ctrl C, exit the program
void sigint_handler(int sig) {
	signal_caught = 1;
}

int main()
{
	struct tm start;

	start.tm_year =2020 - 1900; // 2020
	start.tm_mon = 4 - 1; // April
	start.tm_mday = 10;
	start.tm_hour = 8;
	start.tm_min = 0;
	start.tm_sec = 0;

	Util util(&lock_print);
	util.Generate_occupancy_table();
	util.Generate_simulation_door_status(5, 500,mktime(&start)); // create 500 measurements for 5 rooms

	signal(SIGINT, &sigint_handler);
	lock_print("main thread start\n");

	const int num_of_task = 10;
	const int num_of_robot = 2;

	Dispatcher dispatcher = Dispatcher::GetDispatcher();
	//dispatcher.init(num_of_robot, &lock_print);	 // create 2 robots and assign lock_print function
	dispatcher.SetOut(&lock_print);
	dispatcher.CreateRandomTasks(num_of_task, mktime(&start));
	dispatcher.CreateRobots(num_of_robot);
	this_thread::sleep_for(chrono::seconds(5));

	Dispatcher::stop();
	cout << "Clean-up done.\n";
	return 0;
}

