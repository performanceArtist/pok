#ifndef GAME_H
#define GAME_H

#include <iostream>
#include <vector>
#include <ctime>

#include <SFML/Graphics.hpp>
#include <SFML/System/Clock.hpp>

#include "Constants.h"
#include "Manager/Manager.h"
#include "Text/Text.h"
#include "Task/Task.h"
#include "Animation/Animation.h"
#include "Entities/Entities.h"
#include "Events/Handler.h"
#include "Network/Server.h"

struct gameState {
	bool start, restart, pause, exit, won, lost, show_fps;
};

class Game 
{
public:
	Game();
	void startSingle();
	void startClient(Client* client);
	void startServer(Server* server);
	void init();
	Player* player;
private:
	void processWindowEvents(sf::Time ElapsedTime);
	void draw(sf::Time ElapsedTime);
	void update(sf::Time ElapsedTime);
	float getFPS(sf::Time ElapsedTime);
	void reset();
	gameState state;
	sf::RenderWindow window;
};

#endif /* GAME_H*/