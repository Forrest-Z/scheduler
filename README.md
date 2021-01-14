Robot Scheduler CMake

A simple Visual studio CMake multi-thread program that simulate robot task scheduling.

An advance robot task scheduling project: [Master Thesis: Exploiting Knowledge of Room Occupation for the Scheduling of Navigation Tasks of a Fleet of Robots in Office Environments](https://github.com/xuanjiao/Robots_task_scheduling)

# Requirement

* MS Visual Studio 2019
* MS C++ CMake tools for Windows

# Installation

```

git clone https://github.com/xuanjiao/scheduler.git

```

# Explanation:

## Structure

The program consists of a main thread, multiple robot threads and a global task vector.

**Main Thread** It creates task and store them in Global task vector.

**Global task vector** Simulation of centralized pool

**Robot Thread**  Simulation of robot. It calculates the cost of each task in vector and chooses the one with lower cost


![robot](./image/scheduler-structure.png)

## Task cost calculation by Robot

Cost Function

~~~
cost = w1 * occupation probability+  w2 * hour different +  w3* distance + w4* task priority + w5 * battery level + w6

In this project, we assumed:
cost =   (100 - occupation probability) + hour different  + distance + 10 * (highest priority - task priority) + (100-battery level); 
~~~

## Detail work flow

![scheduler](./image/scheduler-detail_work_flow.png)

## Program result:

![result](./image/result_with_timer.png)
