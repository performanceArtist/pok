#ifndef ANIMATION_H
#define ANIMATION_H

#include "../Game.h"
#include "../Entities/Entities.h"
#include <utility>

class Animation {
public:
	Animation(std::pair <Images::Index*, size_t>, Object* _target);
	void start(sf::Time ElapsedTime);
	void end();
private:
	Object* target;
	sf::Sprite def;
	std::pair <Images::Index*, size_t> sprites;
	int count;
	bool stop;
	float currentTime, timeout;
};
#endif
