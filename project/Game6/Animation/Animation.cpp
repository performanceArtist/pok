#include "Animation.h"

Animation::Animation(std::pair <Images::Index*, size_t> _sprites, Object* _target):
	count(0),
	stop(false),
	timeout(80) 
{
	sprites = _sprites;
	target = _target;
	def = target->Sprite;
}

void Animation::start(sf::Time ElapsedTime) {
	currentTime += ElapsedTime.asMilliseconds();
	//std::cout << currentTime << " " << timeout << " " << count <<" Working...\n";

	if (!stop && currentTime >= timeout) {
		Images::Index* ids = sprites.first;
		size_t size = sprites.second;

		target->Sprite.setTexture(*Manager::getTexture(ids[count]));
		if (count + 1 == size) count = 0; else  count++;
		currentTime = 0;
	}
}

void Animation::end() {
	target->Sprite = def;
	currentTime = 0;
	stop = true;
}