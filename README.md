It is a Visual studio CMake program




**Main Thread** It creates task and store them in Global task vector.

**Global task vector** Simulation of centrolized poor

**Robot Thread**  Simulation of robot. It caculates the cost of each task in vector and chooses the one with lower cost



### Structure:

![robot](./image/scheduler-structure.png)

### How to calculate cost:

Cost Function
~~~
cost =  ��1 * occupation possibility+  ��2 * hour different +  ��3* distance

cost =   (100 - occupation possibility) + hour different  + distance; 
~~~
Example: 
~~~
last task  task 607 room 0, Tue Apr 7 15:02:08 2020

calculate cost for task 453 room 1 Tue Apr 7 17:38:48 2020

global time 17

cost = (100-100) + (17:38 - 17 ) + 1 = 1
~~~
robot get a occupation possibility from global occupation map.

initial occupation map: 
* Occupation possibility = 100 ( 12 noon to 18 pm)
									= 0 (other time)
* Ocupation map detail

~~~
typedef struct Occu_key { // key
	int room_id;
	int day_of_week;
	int hour;
...
}

typedef struct Occu_params { // value
	int closed_count;
	int opened_count;
	int occupancy_possibility;
...
}
~~~

rule(?): 
* what decrease cost: office occupied, small hour different
* what increase cost: office not occupied, bigger hour different, longer distance

### TO DO

when robot find door is close, robot should update occupancy map (opened_count and possibility)_)
periority

cost =  ��1 * occupation possibility+  ��2 * hour different +  ��3* distance +  ��4 * bettery level +��5* Task periority

### Program result:

![result](./image/result_with_timer.png)


### Detail work flow

![scheduler](./image/scheduler-detail_work_flow.png)
