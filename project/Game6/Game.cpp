#include "Game.h"

sf::View Camera::View(sf::FloatRect(0, 0, SCRWIDTH, SCRHEIGHT));

Game::Game():
window(sf::VideoMode(sf::VideoMode::getDesktopMode()), "Game", sf::Style::Fullscreen),
player(nullptr)
{
	window.setFramerateLimit(FRAMERATE);
	window.setMouseCursorVisible(false);
	state = {false, false, false, false, false, false, false};
}

float Game::getFPS(sf::Time ElapsedTime)
{
	static float t1 = 0;
	static float fps = 0;
	static float count = 0;
	const float avg = 20;

	fps += 100.f / (ElapsedTime.asSeconds() - t1);
	t1 = ElapsedTime.asSeconds() * 100;
	count++;

	if (count == avg) {
		return fps / avg;
		fps = 0;
		count = 0;
	}
	else {
		return 0;
	}

}

void Game::init()
{
	Player* Player1 = new Player;
	Player1->Sprite.setPosition(150 + 0.f, WORLDHEIGHT/2 + 0.f);

	Storage::Entities.push_back(Player1);
	player = Player1;

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

		if (target->name == Object_is::PLAYER) {
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Enter))
			{
				target->state |= objectState::SHOOT;
			}
		}

	});
	plHandle->bind(Player1);
	EventManager::addEventHandler(plHandle, "player");

	//Bullet* Bullet1 = new Bullet();
	//Bullet1->Sprite.setPosition(SCRWIDTH/2 + 0.f, SCRHEIGHT - 20 + 0.f);
	//Storage::Entities.push_back(Bullet1);

	Static* Block1 = new Static(Object_is::BLOCK, Images::BLOCK, true);
	Block1->Sprite.setPosition(SCRWIDTH/2 + 0.f, SCRHEIGHT - 50 + 0.f);
	Storage::Entities.push_back(Block1);

	std::srand(static_cast<unsigned int>(std::time(nullptr)));

	Task* spawnPetooh = new Task([](Task* task) {
		if (task->track == 2 && task->getTimeout() > 800) {
			task->track = 0;
			task->setTimeout(task->getTimeout() - 100);
			//std::cout << task->getTimeout() << "\n";
		}

		Object *p;
		p = new Petooh();
		p->Sprite.setPosition(SCRWIDTH, std::rand() % (SCRHEIGHT - p->height));
		p->state |= objectState::MOVE_LEFT;
		//std::cout << p->Sprite.getPosition().y << " " << SCRHEIGHT << "\n";
		Storage::Entities.push_back(p);
	}, 2200);

	TaskManager::addTask(spawnPetooh, "spawnPetooh");

}

void Game::startSingle() {
	sf::Clock clock;

	Manager::music[Music::MAIN]->music.setLoop(true);
	Manager::music[Music::MAIN]->music.setVolume(10);
	Manager::music[Music::MAIN]->music.play();

	while (window.isOpen())
	{
		update(clock.restart());
		draw(clock.restart());
	}
}

void Game::startClient(Client* client) {
	sf::Clock clock;

	Manager::music[Music::MAIN]->music.setLoop(true);
	Manager::music[Music::MAIN]->music.setVolume(10);
	Manager::music[Music::MAIN]->music.play();

	while (window.isOpen())
	{
		Game::processWindowEvents(clock.restart());

		if (state.exit)
		{
			std::vector<Object*>().swap(Storage::Entities);
			window.close();
		}

		for (std::vector<Object*>::iterator it = Storage::Entities.begin(); it != Storage::Entities.end(); it++)
		{
			unsigned char type = (*it)->type;

			if ((*it)->name == Object_is::PLAYER) {
				if ((*it)->state != objectState::NONE) {
					client->sendUpdate((*it)->state, player, client->serverIp, client->port);
					(*it)->state = objectState::NONE;
				}
			}
		}

		window.clear(sf::Color(0, 0, 0));

		for (std::vector<Object*>::iterator it = Storage::Entities.begin(); it != Storage::Entities.end(); it++)
		{
			window.draw((*it)->Sprite);
		}
		window.display();
	}
}

void Game::startServer(Server* server) {

}

void Game::processWindowEvents(sf::Time ElapsedTime)
{
	sf::Event event;

	while (window.pollEvent(event) && event.type == sf::Event::KeyPressed)
	{
		switch(event.key.code)
		{
		case sf::Keyboard::Escape:
			{
				state.exit = true;
				break;
			}
		case sf::Keyboard::Space:
			{
				state.start = true;
				break;
			}
		case sf::Keyboard::R:
			{
				state.restart = true;
				break;
			}
		case sf::Keyboard::W:
			{
				state.won = true;
				break;
			}
		case sf::Keyboard::X:
		{
			state.lost = true;
			break;
		}
		case sf::Keyboard::C:
		{
			window.setMouseCursorVisible(true);
			break;
		}
		case sf::Keyboard::P:
		{
			//puppetMaster
			Object* temp = Storage::getFirst(Object_is::PETOOH);
			if (temp) {
				EventManager::getEventHandler("player")->bind(temp);
			}
			break;
		}
		case sf::Keyboard::B:
		{
			EventManager::getEventHandler("player")->bind(Game::player);
			break;
		}
		default:
		{
			EventManager::runHandlers(event);
		}
			
		}
	}
}

void Game::update(sf::Time ElapsedTime)
{
	if (state.pause) return;

	Camera::updateView(player->Sprite.getPosition().x, player->Sprite.getPosition().y);
	window.setView(Camera::View);
	Game::processWindowEvents(ElapsedTime);
	TaskManager::runTasks(ElapsedTime.asMilliseconds());

	for (std::vector<Object*>::iterator it = Storage::Entities.begin(); it != Storage::Entities.end();)
	{
		unsigned char type = (*it)->type;

		//no updates for static objects
		if (!(type & STATIC)) (*it)->update(ElapsedTime);

		//collision check
		if (type & COLLIDES) {
			for (std::vector<Object*>::iterator it1 = Storage::Entities.begin(); it1 != Storage::Entities.end(); it1++) {
				if ((*it1) != (*it) && (*it1)->type & COLLIDABLE) {
					if ((*it)->checkSpriteCollision(*it1)) {
						if (type & COLLIDABLE) (*it)->onCollision((*it1));
						(*it1)->onCollision((*it));
					}
				}
			}
		}

		if ((*it)->clear)
		{
			delete (*it);
			it = Storage::Entities.erase(it);
		}
		else it++;
	}


	if (player->scored >= WINSCORE && !state.lost) {
		state.won = true;
		TaskManager::removeTask("spawnPetooh");
	}

	if (player->missed >= LOSESCORE && !state.won) {
		state.lost = true;
		TaskManager::removeTask("spawnPetooh");
	}

	if (state.restart)
	{
		reset();
	}

	if (state.exit)
	{
		std::vector<Object*>().swap(Storage::Entities);
		window.close();
	}

}

void Game::draw(sf::Time ElapsedTime)
{
	window.clear(sf::Color(0, 0, 0));

	for(std::vector<Object*>::iterator it = Storage::Entities.begin(); it != Storage::Entities.end(); it++)
	{
		window.draw((*it)->Sprite);
	}

	window.setView(window.getDefaultView());

	std::string score = " Hit: ";
	score += std::to_string(player->scored);
	score += " Missed: " + std::to_string(player->missed);
	window.draw(Text::leftCorner(score, 30, sf::Color::Red));

	if (state.show_fps && getFPS(ElapsedTime) > 0) {
		std::string fps = "FPS:";
		fps += std::to_string(getFPS(ElapsedTime)) + " ";
		sf::Text fps_text = Text::getText(fps, 40, sf::Color::Red);
		fps_text.setPosition(SCRWIDTH - fps_text.getLocalBounds().width, 0);
		window.draw(fps_text);
	}

	if (state.lost) {
		window.draw(Text::center("kek pok", 120, sf::Color::Red));
	}

	if (state.won) {
		window.draw(Text::center("you're wiener", 120, sf::Color::Red));
	}

	window.display();
}

void Game::reset()
{
	/*
	Game::start = false;
	Game::restart = false;
	Game::pause = false;
	Game::exit = false;
	Game::won = false;
	Bullet::xv = 400;
	Bullet::yv = -400;*/
	//std::vector<Object*>().swap(Storage::Entities);
}
