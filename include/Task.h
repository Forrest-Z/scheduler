#include <string>

using namespace std;

class Task {
	typedef void (*outFunc)(string s); // function pointer to print function

public:
	void process();
	Task(int id,outFunc print):id(id),task_print(print){}
private:
	int id;
	outFunc task_print;
};