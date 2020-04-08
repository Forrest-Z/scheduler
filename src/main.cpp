
#include "EnterRoomTask.h"
#include "Robot.h"
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
	print("main thread start");

	Robot* robot = new Robot(1,print);

	for (int i = 0; i < 50; i++) {
		 EnterRoomTask* task = new EnterRoomTask(i, &print);
		
		robot->setTask(task);
		robot->run();
	}

	return 0;
}
