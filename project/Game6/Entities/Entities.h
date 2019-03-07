#ifndef ENTITIES_H
#define ENTITIES_H

struct mapLimits
{
	bool left, right, top, bottom;
};

namespace objectState {
	const sf::Uint8 NONE = 0;
	const sf::Uint8 MOVE_RIGHT = 1 << 0;
	const sf::Uint8 MOVE_LEFT = 1 << 1;
	const sf::Uint8 MOVE_UP = 1 << 2;
	const sf::Uint8 MOVE_DOWN = 1 << 3;
	const sf::Uint8 SHOOT = 1 << 4;
	const sf::Uint8 CLEAR = 1 << 5;
	const sf::Uint8 STOP = ~(MOVE_RIGHT | MOVE_LEFT | MOVE_UP | MOVE_DOWN);
};

class Animation;

class Storage {
public:
	static std::vector<Object*> Entities;
	static Object* getFirst(Object_is name);
	static Object* getLast(Object_is name);
	static std::vector<Object*> getObjects(Object_is name);
};

class Object
{
public:
	virtual void update(sf::Time ElapsedTime) {}
	virtual void onCollision(Object *with) {}
	mapLimits checkWallCollision();
	bool checkSpriteCollision(Object* a);
	sf::Uint8 state;
	sf::Sprite Sprite;
	unsigned int width, height;
	bool clear;
	unsigned char type;
	Object_is name;
	bool upToDate;
};

class Static :public Object
{
public:
	Static(Object_is name, Images::Index image, bool collidable);
};

class Player :public Object
{
public:
	Player();
	void update(sf::Time ElapsedTime);
	void onCollision(Object *with);
	static float xv;
	int scored, missed;
	std::string username;
	Task* shoot;
};

class Bullet :public Object
{
public:
	Bullet();
	void update(sf::Time ElapsedTime);
	void onCollision(Object *with);
	static float xv, yv;
};

class Petooh :public Object
{
public:
	Petooh();
	void update(sf::Time ElapsedTime);
	void onCollision(Object *with);
	float xv;
	Animation* walk;
};

class Camera
{
public:
	static sf::View View;
	static void updateView(float x, float y);
};

#endif
