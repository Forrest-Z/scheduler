
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
//#include "global.h"

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

//
//static void generate_simulation() {
//	ofstream fs; 
//	fs.open("../simulation/door_status_room_3.txt"); // create a output stream, which open file  for writing
//	if (!fs.is_open()) {
//		cout << "unable to open file"<<endl;
//		return;
//	}
//	int start_year = 2020, start_month = 4, start_day = 10, start_hour = 8;
//	int time_interval = 1;
//	int num_of_measurement = 500;
//
//	time_t raw_time;
//	struct tm* time_info;
//	time(&raw_time); // get current time
//	time_info = localtime(&raw_time); // convert to a tm structure
//	time_info->tm_year = start_year - 1900;
//	time_info->tm_mon = start_month - 1;
//	time_info->tm_mday = start_day;
//	time_info->tm_hour = start_hour;
//	raw_time = mktime(time_info);  // convert tm structure to time_t
//
//	for (int i = 0; i < num_of_measurement; i++) {
//
//		// determin door closed/opened time
//		std::string status = (time_info->tm_hour > 12 && time_info->tm_hour < 18)? "opened" : "closed";  
//		
//		cout << status << " "<<ctime(&raw_time);
//		fs << status << " " << ctime(&raw_time);
//		time_info->tm_hour += time_interval;
//		raw_time = mktime(time_info); // automatically adjusts the time members if they are off-range
//	}
//	//fs << " ";
//	fs.close();
//}

int main()
{
	const int num_of_task = 30;
	const int num_of_robot = 2;
	signal(SIGINT, &sigint_handler);
	lock_print("main thread start\n");

	Dispatcher dispatcher = Dispatcher::GetDispatcher();
	dispatcher.init(num_of_robot, &lock_print);	 // create 2 robots and assign lock_print function
	
	for (int i = 0; i < num_of_task; i++) {
		Room_Time rt;
		rt.day_of_week = rand() % (5 - 1 + 1) + 1; // create a day( monday - friday)
		rt.room_id = rand() % (3 - 1 + 1) + 1; // create a room id(1-3)
		rt.time = rand() % (18 - 8 + 1) + 8; // create a time(8am -18pm)
		EnterRoomTask* task = new EnterRoomTask(&lock_print);
		task->setRoomAndTime(rt);
		Dispatcher::AddTask(task);
		Dispatcher::SortTaskQueue();
	}
	
	this_thread::sleep_for(chrono::seconds(5));

	Dispatcher::stop();
	cout << "Clean-up done.\n";
	return 0;
}
