#include "Handler.h"

void EventHandler::listen(sf::Event event) {
	if (active) handler(event, target);
}

void EventHandler::bind(Object* _target) {
	target = _target;
}

void EventHandler::enable() {
	active = true;
}

void EventHandler::disable() {
	active = false;
}

std::unordered_map <std::string, EventHandler*> EventManager::Handlers;

void EventManager::addEventHandler(EventHandler* handler, std::string name) {
	Handlers[name] = handler;
}

EventHandler* EventManager::getEventHandler(std::string name) {
	return Handlers[name];
}

void EventManager::removeEventHandler(std::string name) {
	delete Handlers[name];
	Handlers.erase(name);
}

void EventManager::runHandlers(sf::Event event) {
	std::unordered_map <std::string, EventHandler*>::iterator it;
	for (it = Handlers.begin(); it != Handlers.end(); it++) {
		it->second->listen(event);
	}
}