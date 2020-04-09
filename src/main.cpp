
#include "EnterRoomTask.h"
#include "Robot.h"
#include "Dispatcher.h"
#include <mutex>
#include <iostream>
#include <chrono>
#include <csignal>
#include <queue>
#include <set>

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

static const int num_of_time_point = 5;  // 10 time point. e.g. time point 0 is 8 am, point 2 is 10 am usw.
static const int num_of_robot = 2;
static const int num_of_room = 5;


typedef struct room_info {
	int id;
	int  occupy_posibility;
	friend bool operator < (room_info a, room_info b)
	{
		return a.occupy_posibility > b.occupy_posibility;   // if a.posibility > b.posibility, a > b, a is near the top
	}
} Room_info;

typedef struct table {
	int time;
	Room_info room_info[num_of_room];
}Table_unit;


int main()
{

	Table_unit table_unit[num_of_time_point] = { // assume each room post a enter room task
							{8, {{ 0, 10}, {1,70},{2, 80},{3, 10},{4, 0}} },		 
							{11, {{ 0, 60}, {1,80},{2, 80},{3, 50},{4, 20}} },
							{14, {{ 0, 80}, {1,20},{2, 20},{3, 20},{4, 20}} },
							{17, {{ 0, 10}, {1,10},{2, 5},{3, 70},{4, 90}} },
							{20, {{ 0, 10}, {1,60},{2, 90},{3, 90},{4, 80}} },
		// Format		{time=20 ,{room id=0, occupy possibility=10%}, {room id=0, occupy possibility=10%}
	};


	signal(SIGINT, &sigint_handler);
	lock_print("main thread start\n");

	Dispatcher dispatcher = Dispatcher::GetDispatcher();
	dispatcher.SetOut(&lock_print);	 // assign lock_print function to dispatcher
	
	dispatcher.init(num_of_robot, &lock_print);	 // create 2 robots and assign lock_print function

	int cycle = 0;
	std::set<int> room_entered;

	while (cycle < num_of_time_point) {

		priority_queue<room_info> possibility_priority_queue; // put all possibility value in queue;
		for (int i = 0; i < num_of_room; i++) {
			possibility_priority_queue.push(table_unit[cycle].room_info[i]);
		}

		while(true){

				 //get the room with maximal possibility at this time point
				const room_info room = possibility_priority_queue.top();
				int id = room.id;
						if  (room_entered.find(id) == room_entered.end() ){
							// if the room is not entered.
							int time_point = table_unit[cycle].time;
							  lock_print("Please enter room " + std::to_string(id) + 
											" at time " + std::to_string(time_point) + "\n"); // print the room and time 
								EnterRoomTask* task = new EnterRoomTask(id,time_point, &lock_print);
								Dispatcher::AddTask(task);  // add task to dispatcher
								room_entered.insert(id);	//add room id to set
								break; // jump out troom.idhe loop
						}
						else {
							// if this room is already entered. remove it from priority queue
							possibility_priority_queue.pop();
						}

		}
		cycle++;
	}
	

	this_thread::sleep_for(chrono::seconds(20));

	// Cleanup.
	Dispatcher::stop();
	cout << "Clean-up done.\n";

	return 0;
}
