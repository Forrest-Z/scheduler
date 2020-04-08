// Scheduler.cpp: 定义应用程序的入口点。
//

#include "Task.h"
#include "Robot.h"
#include <mutex>
#include <iostream>

using namespace std;

mutex cout_mutex;

// Allow one thread print text on stdout.
void print(string s) {
	cout_mutex.lock();
	cout << s << endl;
	cout_mutex.unlock();
}

// When user type ctrl C, exit the program
void signal_handle() {
	print("main thread stop");
}

int main()
{
	print("main thread start");


	for (int i = 0; i < 50; i++) {
		Task task(i, &print);
		task.process();
	}

	return 0;
}
