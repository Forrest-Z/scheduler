
#include "Dispatcher.h"
#include "global.h"
#include <fstream>
#include <sstream>

using namespace std;

Robot::Robot() :Robot(0, nullptr) {
}
Robot::Robot(int id, outFunc lock_print): Robot(id, new EnterRoomTask(), lock_print) {
}
Robot::Robot(int id, EnterRoomTask* task, outFunc lock_print) {
	this->id = id;
	this->task = task;
	robot_print = lock_print;
	has_task = false;
	running = true;
	bettery_level = 100; 
	ulock = unique_lock<mutex>(mtx); 
}

void Robot::setTask(EnterRoomTask* task) { 
	this->task = task;
	this->has_task = true; 
}

void Robot::run()
{
	while (running)
	{
		if (has_task) {
			//task->getRoomAndTime(rt);
			//bool status = getDoorStatusFromFile(rt, "../simulation/door_status_room_1.txt"); // measure door status
			//task->process(id,status);
			while (task->getCalendarTime() > Dispatcher::GetGlobalTime()) {
				this_thread::sleep_for(chrono::microseconds(1000)); // wait for 1s
			}
			bool result = task->process(id);
			if (!result) {		
				Dispatcher::ReturnTask(task);
			}
			has_task = false;
		}
		if (Dispatcher::AddRobot(this)) {
			// When no task in task queue, wait that task thread run Disparture 
			while (!has_task && running) // wait dispatcher call robot set task function
			{
				cv.wait(ulock);	// block the thread until it was assigned to a task.
			}
		}
		
	}

}
