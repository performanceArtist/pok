#ifndef SERVER_H
#define SERVER_H

#include <iostream>
#include <string>
#include <thread>
#include <mutex>

#include <SFML/System.hpp>
#include <SFML/Network.hpp>

#include "Packet.h"

struct clientInfo {
	std::string username;
	sf::IpAddress address;
	unsigned short port;
};

namespace serverState {
	const sf::Uint8 INIT = 0;
	const sf::Uint8 WAIT = 1;
	const sf::Uint8 START = 2;
	const sf::Uint8 INFO = 3;
	const sf::Uint8 CREATE_OBJECT = 4;
	const sf::Uint8 UPDATE_OBJECT = 5;
}

namespace serverPacket {
	struct info {
		std::string message;
	};

	struct updateObject {
		std::string username;
		sf::Uint16 x, y;
	};

}

namespace clientState {
	const sf::Uint8 INIT = 0;
	const sf::Uint8 UPDATE_STATE = 1;
}

namespace clientPacket {
	struct info
	{
		std::string username;
		std::string message;
	};

	struct updateState {
		std::string username;
		sf::Uint8 state;
	};
}

class Server {
public:
	Server(sf::IpAddress address, unsigned short port);
	void initPlayers();
	void gameInit();
	void gameStart(sf::Time ElapsedTime);
	void receiveUpdates();
	void sendUpdate(sf::Uint8 header, std::string username, float x, float y);
	static const int MAX_PLAYERS = 1;
private:
	sf::IpAddress address;
	unsigned short port;
	sf::UdpSocket socket;
	clientInfo clients[MAX_PLAYERS];
	std::mutex mutex;
};

class Player;

class Client {
public:
	Client(sf::IpAddress serverIp, unsigned short port, std::string username);
	void gameStart();
	void initClient();
	void receiveUpdates();
	void sendUpdate(sf::Uint8 state, Player* player, sf::IpAddress _address, unsigned short _port);
	serverPacket::updateObject players[Server::MAX_PLAYERS];
	std::string username;
	sf::IpAddress serverIp;
	unsigned short port;
private:
	sf::UdpSocket socket;
	std::mutex mutex;
};

#endif