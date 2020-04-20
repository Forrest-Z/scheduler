
#include "Dispatcher.h"
#include "global.h"
#include <fstream>
#include <sstream>

using namespace std;



void Robot::run()
{
	while (running)
	{
		if (has_task) {
			//task->getRoomAndTime(rt);
			//bool status = getDoorStatusFromFile(rt, "../simulation/door_status_room_1.txt"); // measure door status
			//task->process(id,status);
			bool result;
			task->process(id,result);
			has_task = false;
		}

		Dispatcher::AddRobot(this);

		while (!has_task && running) // wait dispatcher call robot set task function
		{			
				cv.wait(ulock);	// block the thread until it was assigned to a task.
		}
		
	}

}
