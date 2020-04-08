
#include "EnterRoomTask.h"
#include "Robot.h"
#include "Dispatcher.h"
#include <mutex>
#include <iostream>
#include <chrono>
#include <csignal>

using namespace std;

sig_atomic_t signal_caught = 0;
mutex cout_mutex;

// Allow one thread print text on stdout.
void print(string s) {
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
	int num_of_robot = 3;
	int num_of_task = 15;

	signal(SIGINT, &sigint_handler);
	print("main thread start\n");

	Dispatcher dispatcher = Dispatcher::GetDispatcher();
	dispatcher.SetOut(&print);	 // assign print function to dispatcher
	
	dispatcher.init(num_of_robot, &print);	 // create 2 robots and assign print function

	for (int i = 0; i <num_of_task; i++) {
		 EnterRoomTask* task = new EnterRoomTask(i, &print);
		 dispatcher.AddTask(task);
	}

	this_thread::sleep_for(chrono::seconds(5));

	// Cleanup.
	Dispatcher::stop();
	cout << "Clean-up done.\n";

	return 0;
}
