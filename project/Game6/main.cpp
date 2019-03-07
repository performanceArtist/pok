#include "Game.h"
#include "Network/Server.h"

void singlePlayer() {
	Manager::loadImages();
	Manager::loadSounds();
	Manager::loadMusic();
	Game game;
	game.init();
	game.startSingle();
}

int main(int argc, char* argv[])
{	
	if (argc < 2) {
		std::cout << "Usage: <exe_path> single/server <port>/client <port>";
		return 1;
	}

	std::string mode = argv[1];

	
	if (argc == 2 && mode == "single") {
		singlePlayer();
	}

	if (argc == 3 && mode == "server") {

		unsigned short port = (unsigned short)atoi(argv[2]);

		Server s(sf::IpAddress::getLocalAddress(), port);
		s.initPlayers();
		s.gameInit();
		std::thread listen(&Server::receiveUpdates, &s);
		sf::Clock clock;
		s.gameStart(clock.restart());
		listen.join();
	}

	if (argc == 3 && mode == "client") {

		unsigned short port = (unsigned short)atoi(argv[2]);

		sf::IpAddress serverIp;
		do
		{
			std::cout << "Server address: ";
			std::cin >> serverIp;
		} while (serverIp == sf::IpAddress::None);

		std::string username;
		std::cout << "Enter your name:\n";
		std::cin >> username;
		
		Client c(serverIp, port, username);
		c.initClient();
		std::thread listen(&Client::receiveUpdates, &c);
		c.gameStart();
		listen.join();
	}
	
	return 0;
}
