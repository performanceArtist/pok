#ifndef TASK_H
#define TASK_H

#include <iostream>
#include <string>
#include <unordered_map>
#include <functional>

class Object;
class Player;

class Task {
public:
	Task(std::function<void(Task *)> callback, unsigned int timeout);
	void run(unsigned int time);
	void executeImmediately();
	bool getStatus();
	void disable();
	void enable();
	void setTimeout(unsigned int milliseconds);
	unsigned int getTimeout();
	unsigned int getCount();
	void setCallback(std::function<void(Task*)> _callback);
	void bind(void* _target);
	unsigned int track;
    void* target;
private:
	unsigned int timeout, elapsed, count;
	bool active;
	std::function<void(Task*)> callback;
};

class TaskManager {
public:
	static void addTask(Task* task, std::string name);
	static void removeTask(std::string name);
	static Task* getTask(std::string name);
	static void runTasks(unsigned int time);
private:
	static std::unordered_map <std::string, Task*> Tasks;
};
#endif