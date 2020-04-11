#include "Robot.h"
#include "Dispatcher.h"

using namespace std;

void Robot::run()
{
	while (running)
	{
		if (has_task) {
			
			task->process(id);

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
