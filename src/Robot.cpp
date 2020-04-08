#include "Robot.h"
#include "Dispatcher.h"

using namespace std;

void Robot::run()
{
	while (running)
	{
		if (has_task) {
			
			//robot_print("Robot " + std::to_string(id) + ": ");
			task->process(id);

			has_task = false;
		}

		if (Dispatcher::AddRobot(this)) {
			
			while (!has_task && running) // wait dispatcher call robot set task function
			{
				if (cv.wait_for(ulock, chrono::seconds(1)) == cv_status::timeout) {
					// We timed out, but we keep waiting unless the worker is
					// stopped by the dispatcher.
				}
			}
			// if a robot add itself to queue, wait for a signal from dispatcher
		
		}
	}

}
