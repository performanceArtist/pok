#include "../Game.h"
#include "Entities.h"

std::vector<Object*> Storage::Entities;

Object* Storage::getFirst(Object_is name) {
	std::vector <Object*>::iterator it;
	for (it = Entities.begin(); it != Entities.end(); it++) {
		if ((*it)->name == name) return (*it);
	}
	return nullptr;
}

Object* Storage::getLast(Object_is name) {
	int it;
	for (it = Entities.size(); it > 0; it--) {
		if (Entities[it]->name == name) return Entities[it];
	}
	return nullptr;
}

std::vector<Object*> Storage::getObjects(Object_is name) {
	std::vector <Object*>::iterator it;
	std::vector <Object*> result;

	for (it = Entities.begin(); it != Entities.end(); it++) {
		if ((*it)->name == name) result.push_back((*it));
	}
	return result;
}

mapLimits Object::checkWallCollision()
{
	mapLimits limits = { true, true, true, true };

	float x = this->Sprite.getPosition().x;
	float y = this->Sprite.getPosition().y;

	if (x >= WORLDWIDTH - this->width) limits.right = false;
	if (x <= 0) limits.left = false;
	if (y >= WORLDHEIGHT - this->height) limits.bottom = false;
	if (y <= 0) limits.top = false;

	return limits;
}

bool Object::checkSpriteCollision(Object* a)
{
	float x1 = this->Sprite.getPosition().x;
	float y1 = this->Sprite.getPosition().y;
	float x2 = a->Sprite.getPosition().x;
	float y2 = a->Sprite.getPosition().y;

	unsigned int w1 = this->width;
	unsigned int h1 = this->height;
	unsigned int w2 = a->width;
	unsigned int h2 = a->height;

	return ((x1 < x2 + w2) && (x1 + w1 > x2)
		&& (y1 < y2 + h2) && (y1 + h1 > y2));
}

float Player::xv = 8;

Player::Player():
	scored(0),
	missed(0)
{
	name = Object_is::PLAYER;
	type |= (COLLIDES | COLLIDABLE);
	state = objectState::NONE;
	Sprite = Manager::getSprite(Images::PLAYER);
	width = Sprite.getTexture()->getSize().x;
	height = Sprite.getTexture()->getSize().y;
	clear = false;
	upToDate = true;

	shoot = new Task([](Task* task) {
		if (task->target == nullptr) return;
		Player* player = (Player*)task->target;
		if (!(player->state & objectState::SHOOT)) return;
		Bullet* Bullet1 = new Bullet();
		Bullet1->Sprite.setPosition(player->Sprite.getPosition().x + player->width, player->Sprite.getPosition().y + player->height / 2);
		Storage::Entities.push_back(Bullet1);
		player->state &= ~objectState::SHOOT;
	}, 200);
	shoot->bind(this);
	TaskManager::addTask(shoot, "pl_shoot");
}

void Player::update(sf::Time ElapsedTime)
{
	if (state & objectState::MOVE_LEFT) Sprite.move(-xv, 0);
	if (state & objectState::MOVE_RIGHT) Sprite.move(xv, 0);
	if (state & objectState::MOVE_UP) Sprite.move(0, -xv);
	if (state & objectState::MOVE_DOWN) Sprite.move(0, xv);
	if (state != objectState::NONE) upToDate = false; else upToDate = true;
	
	state &= objectState::STOP;
}

void Player::onCollision(Object *with)
{
	if (with->name == Object_is::PETOOH) {
		Manager::sounds[Sounds::MMM].playSound();
		scored++;
	}
}

float Bullet::xv = 6;
float Bullet::yv = -4;

Bullet::Bullet()
{
	name = Object_is::BULLET;
	type |= (COLLIDES | COLLIDABLE);
	state = objectState::NONE;
	Sprite = Manager::getSprite(Images::BULLET);
	width = Sprite.getTexture()->getSize().x;
	height = Sprite.getTexture()->getSize().y;
	clear = false;
}

void Bullet::update(sf::Time ElapsedTime)
{
	mapLimits limits = this->checkWallCollision();
	if (!limits.right || !limits.left) {
		clear = true;
	}
	
	this->Sprite.move(xv, 0);
}

void Bullet::onCollision(Object *with) {
	if (with->name == Object_is::PETOOH) {
		Player* pl = (Player*)Storage::getFirst(Object_is::PLAYER);
		pl->scored++;
	}
	clear = true;
}

Static::Static(Object_is name, Images::Index image, bool collidable)
{
	name = name;
	type |= STATIC;
	if (collidable) type |= COLLIDABLE;
	Sprite = Manager::getSprite(image);
	width = Sprite.getTexture()->getSize().x;
	height = Sprite.getTexture()->getSize().y;
	clear = false;
}

Petooh::Petooh():
	xv(2)
{
	name = Object_is::PETOOH;
	type |= (COLLIDABLE);
	state = objectState::NONE;
	Sprite = Manager::getSprite(Images::PETOOH);
	width = Sprite.getTexture()->getSize().x;
	height = Sprite.getTexture()->getSize().y;
	clear = false;
	upToDate = false;
	walk = new Animation(Manager::animations[Animations::PETOOH_WALK], this);
}

void Petooh::update(sf::Time ElapsedTime) {
	if (state & objectState::MOVE_LEFT) {
		walk->start(ElapsedTime);
		Sprite.move(-xv, 0);
	}

	mapLimits limits = this->checkWallCollision();
	if (!limits.left) {
		Manager::sounds[Sounds::SORRY].playSound();
		Player* pl = (Player*)Storage::getFirst(Object_is::PLAYER);
		pl->missed++;
		clear = true;
	}
}

void Petooh::onCollision(Object *with)
{
	if (with->name == Object_is::PLAYER || with->name == Object_is::BULLET) {
		Manager::sounds[Sounds::POK].playSound();
		clear = true;
	}
}

void Camera::updateView(float x, float y)
{
	if ((x + SCRWIDTH / 2 >= WORLDWIDTH) || (x - SCRWIDTH / 2 <= 0)) x = Camera::View.getCenter().x;
	if ((y + SCRHEIGHT / 2 >= WORLDHEIGHT) || (y - SCRHEIGHT / 2 <= 0)) y = Camera::View.getCenter().y;

	Camera::View.setCenter(x, y);
}