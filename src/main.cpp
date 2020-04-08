
#include "EnterRoomTask.h"
#include "Robot.h"
#include "Dispatcher.h"
#include <mutex>
#include <iostream>

using namespace std;

mutex cout_mutex;

// Allow one thread print text on stdout.
void print(string s) {
	cout_mutex.lock();
	cout << s;
	cout_mutex.unlock();
}

// When user type ctrl C, exit the program
void signal_handle() {
	print("main thread stop");
}

int main()
{
	print("main thread start\n");

	
	Dispatcher dispatcher = Dispatcher::GetDispatcher();
	dispatcher.SetOut(&print); // assign print function to dispatcher
	dispatcher.init(5, &print); // create 10 robots and assign print function

	for (int i = 0; i < 50; i++) {
		 EnterRoomTask* task = new EnterRoomTask(i, &print);
		 dispatcher.AddTask(task);
		//robot->setTask(task);
		// robot->run();
	}

	return 0;
}
