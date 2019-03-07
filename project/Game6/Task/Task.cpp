#include "Task.h"

Task::Task(std::function<void(Task*)> callback, unsigned int timeout = 100)
	:timeout(timeout),
	elapsed(0),
	count(0),
	track(0),
	target(nullptr),
	callback(callback),
	active(true)
{}

void Task::run(unsigned int time) {
	if (active && elapsed >= timeout) {
		callback(this);
		elapsed = 0;
		count++;
		track++;
	}
	else {
		elapsed += time;
	}
}

void Task::executeImmediately() {
	callback(this);
}

void Task::disable() {
	elapsed = 0;
	active = false;
}

void Task::enable() {
	active = true;
}

bool Task::getStatus() {
	return active;
}

void Task::bind(void* _target) {
	target = _target;
}

void Task::setTimeout(unsigned int milliseconds) {
	timeout = milliseconds;
}

unsigned int Task::getTimeout() {
	return timeout;
}

unsigned int Task::getCount() {
	return count;
}

void::Task::setCallback(std::function<void(Task*)> _callback) {
	callback = _callback;
}

std::unordered_map <std::string, Task*> TaskManager::Tasks;

void TaskManager::addTask(Task* task, std::string name) {
	Tasks[name] = task;
}

Task* TaskManager::getTask(std::string name) {
	return Tasks[name];
}

void TaskManager::removeTask(std::string name) {
	delete Tasks[name];
	Tasks.erase(name);
}

void TaskManager::runTasks(unsigned int time) {
	std::unordered_map <std::string, Task*>::iterator it; 
	for(it = Tasks.begin(); it!=Tasks.end(); it++) {
		it->second->run(time);
	}
}