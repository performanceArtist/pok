#ifndef HANDLER_H
#define HANDLER_H

#include "../Game.h"
#include <functional>

class EventHandler {
public:
	EventHandler(std::function<void(sf::Event, Object* target)> handler) :
		target(nullptr),
		handler(handler),
		active(true) {}
	
	void listen(sf::Event event);
	void bind(Object* _target);
	void enable();
	void disable();

private:
	std::function<void(sf::Event, Object* target)> handler;
	Object* target;
	bool active;
};

class EventManager {
public:
	static void addEventHandler(EventHandler*, std::string name);
	static EventHandler* getEventHandler(std::string name);
	static void removeEventHandler(std::string name);
	static void runHandlers(sf::Event event);
private:
	static std::unordered_map <std::string, EventHandler*> Handlers;
};
#endif
