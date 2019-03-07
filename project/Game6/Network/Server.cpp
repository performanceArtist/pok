#include "Server.h"
#include "../Game.h"

sf::Packet& operator <<(sf::Packet& packet, const clientPacket::info& data)
{
	return packet << data.username << data.message;
}

sf::Packet& operator >>(sf::Packet& packet, clientPacket::info& data)
{
	return packet >> data.username >> data.message;
}

sf::Packet& operator <<(sf::Packet& packet, const serverPacket::info& data)
{
	return packet << data.message;
}

sf::Packet& operator >>(sf::Packet& packet, serverPacket::info data)
{
	return packet >> data.message;
}

sf::Packet& operator <<(sf::Packet& packet, const serverPacket::updateObject& data)
{
	return packet << data.username << data.x << data.y;
}

sf::Packet& operator >>(sf::Packet& packet, serverPacket::updateObject& data)
{
	return packet >> data.username >> data.x >> data.y;
}

sf::Packet& operator <<(sf::Packet& packet, const clientPacket::updateState& data)
{
	return packet << data.username << data.state;
}

sf::Packet& operator >>(sf::Packet& packet, clientPacket::updateState& data)
{
	return packet >> data.username >> data.state;
}

/*
template <typename T>
void formPacket(sf::Packet &out, sf::Uint8 header, T data) {
	out << header;
	out << data;
}
*/

Server::Server(sf::IpAddress address, unsigned short port):
	address(address),
	port(port)
{}

void Server::initPlayers() {
	if (socket.bind(port) != sf::Socket::Done) return;
	std::cout << "Server is listening to port " << port << "..." << std::endl;

	unsigned short count = 0;

	while (count < MAX_PLAYERS) {
		// Wait for a message
		sf::IpAddress sender;
		unsigned short senderPort;
		sf::Packet received;

		socket.receive(received, sender, senderPort);

		sf::Uint8 header;
		clientPacket::info info;

		received >> header;
		received >> info;
		std::cout << header << " " << info.username << ": " << info.message;

		if (header != clientState::INIT) continue;

		clients[count] = { info.username, sender, senderPort };

		serverPacket::info data;
		data.message = "Connected";
		sf::Packet out;
		out << serverState::INIT << data;
		socket.send(out, sender, senderPort);
		count++;
	}
}

void Server::gameInit() {
	sf::Packet st;
	st << serverState::START;

	for (int j = 0; j < Server::MAX_PLAYERS; j++) {
		Player* pl = new Player();
		pl->Sprite.setPosition(200 * (j + 1), 200 * (j + 1));
		pl->username = clients[j].username;
		Storage::Entities.push_back(pl);
		serverPacket::updateObject temp = { clients[j].username, 200*(j+1), 200*(j+1)};
		st << temp;
	}

	for (int i = 0; i < MAX_PLAYERS; i++) {
		socket.send(st, clients[i].address, clients[i].port);
	}

	Task* spawnPetooh = new Task([](Task* task) {
		if (task->track == 2 && task->getTimeout() > 800) {
			task->track = 0;
			task->setTimeout(task->getTimeout() - 100);
		}

		Object *p;
		p = new Petooh();
		p->Sprite.setPosition(SCRWIDTH, std::rand() % (SCRHEIGHT - p->height));
		p->state |= objectState::MOVE_LEFT;

		Storage::Entities.push_back(p);
	}, 2200);
}

void Server::gameStart(sf::Time ElapsedTime) {

	while (true) {
		//TaskManager::runTasks(ElapsedTime.asMilliseconds());

		for (std::vector<Object*>::iterator it = Storage::Entities.begin(); it != Storage::Entities.end(); it++)
		{
			(*it)->update(ElapsedTime);
			if (!(*it)->upToDate) {
				Object_is is = (*it)->name;
				switch (is) {
					case Object_is::PLAYER:
					{
						Player* temp = (Player*)(*it);
						sendUpdate(serverState::UPDATE_OBJECT, temp->username, temp->Sprite.getPosition().x, temp->Sprite.getPosition().y);
						(*it)->upToDate = true;
						break;
					}
					case Object_is::PETOOH:
					{
						//Petooh* temp = (Petooh*)(*it);
						//sendUpdate(serverState::CREATE_OBJECT, temp->username, temp->Sprite.getPosition().x, temp->Sprite.getPosition().y);
						//(*it)->upToDate = true;
						break;
					}
					default:
					{
						break;
					}
				}

			}
		}
	}
}

void Server::receiveUpdates() {
	std::cout << "Receiving updates...";
	while (true) {
		// Wait for a message
		sf::IpAddress sender;
		unsigned short senderPort;
		sf::Packet received;

		socket.receive(received, sender, senderPort);

		sf::Uint8 header;

		received >> header;

		switch (header) {
		case clientState::UPDATE_STATE:
		{
			std::cout << "Update received\n";
			clientPacket::updateState info;
			received >> info;

			mutex.lock();
			for (std::vector<Object*>::iterator it = Storage::Entities.begin(); it != Storage::Entities.end(); it++)
			{
				if ((*it)->name == Object_is::PLAYER) {
					Player* temp = (Player*)(*it);
					if (temp->username == info.username) {
						temp->state = info.state;
					}
				}
			}
			mutex.unlock();
			break;
		}
		}
	}
}

void Server::sendUpdate(sf::Uint8 header, std::string username, float x, float y) {
	sf::Packet send;
	send << header;
	serverPacket::updateObject temp = {username, x, y};
	send << temp;

	std::cout << "upd:" << username << " " << x << " " << y << "\n";
	for (int i = 0; i < MAX_PLAYERS; i++) {
		socket.send(send, clients[i].address, clients[i].port);
	}
}

Client::Client(sf::IpAddress serverIp, unsigned short port, std::string username) :
	serverIp(serverIp),
	port(port),
	username(username)
{}

void Client::initClient() {	
	clientPacket::info packet;
	packet.username = username;
	std::cout << "Enter message:\n";
	std::cin >> packet.message;

	sf::Packet send;
	send << clientState::INIT << packet;
	socket.send(send, serverIp, port);

	while (true) {
		sf::Packet in;
		socket.receive(in, serverIp, port);

		sf::Uint8 header;
		in >> header;

		switch(header) {
			case serverState::INIT:
				{
					serverPacket::info init;
					in >> init;
					std::cout << "Connected! From server: " << init.message << "\n";
					std::cout << "Waiting for players...\n";
					break;
				}
			case serverState::START:
				{
					std::cout << "Game start\n";
					std::cout << "Players:\n";
					for (int i = 0; i < Server::MAX_PLAYERS; i++) {
						in >> players[i];
						std::cout << players[i].username << "\n";
					}
					return;
					break;
				}
			default:
				std::cout << "Unknown packet.\n";
		}

	}
}

void Client::gameStart() {
	Manager::loadImages();
	Manager::loadSounds();
	Manager::loadMusic();

	Game game;

	for (int i = 0; i < Server::MAX_PLAYERS; i++) {
		Player* temp = new Player();
		temp->username = players[i].username;
		temp->Sprite.setPosition(players[i].x, players[i].y);
		if (players[i].username == username) {
			game.player = temp;
		}
		Storage::Entities.push_back(temp);
	}

	EventHandler* plHandle = new EventHandler([](sf::Event event, Object* target) {
		struct mapLimits limits = target->checkWallCollision();

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left) && limits.left)
		{
			target->state |= objectState::MOVE_LEFT;
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right) && limits.right)
		{
			target->state |= objectState::MOVE_RIGHT;
		}

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up) && limits.top)
		{
			target->state |= objectState::MOVE_UP;
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down) && limits.bottom)
		{
			target->state |= objectState::MOVE_DOWN;
		}

	});

	plHandle->bind(game.player);
	EventManager::addEventHandler(plHandle, "player");

	std::cout << "player:" << game.player->username << " " << game.player->Sprite.getPosition().x << " " << game.player->Sprite.getPosition().y;
	std::cout << Storage::Entities.size();
	game.startClient(this);
}

void Client::sendUpdate(sf::Uint8 state, Player* player, sf::IpAddress _address, unsigned short _port) {
	std::cout << "Update sent for " << username << " " << _address << ":" << _port << "\n";
	if (player->username != username) return;
	sf::Packet send;
	clientPacket::updateState pack = {player->username, state};
	send << clientState::UPDATE_STATE << pack;
	socket.send(send, _address, _port);
}

void Client::receiveUpdates() {
	sf::IpAddress sip = serverIp;
	unsigned short sport = port;

	while (true) {
		sf::Packet in;
		socket.receive(in, sip, sport);
		sf::Uint8 header;
		in >> header;

		switch (header) {
		case serverState::UPDATE_OBJECT:
		{
			serverPacket::updateObject upd;
			in >> upd;

			mutex.lock();
			for (std::vector<Object*>::iterator it = Storage::Entities.begin(); it != Storage::Entities.end(); it++)
			{
				Object_is is = (*it)->name;
				switch (is) {
					case Object_is::PLAYER:
					{
						Player* tm = (Player*)(*it);
						if (tm->username == upd.username) {
							tm->Sprite.setPosition(upd.x, upd.y);
						}
						break;
					}
					default:
					{
						break;
					}
				}
			}
			mutex.unlock();
		}
		default:
			break;
		}
	}
}