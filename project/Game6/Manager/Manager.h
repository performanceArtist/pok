#ifndef MANAGER_H
#define MANAGER_H

#include <iostream>
#include <vector>
#include <unordered_map>
#include <string>
#include <utility>

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

namespace Images {
	enum Index {
		PLAYER = 0, BULLET, BLOCK, PETOOH,
		PETOOH_WALK1, PETOOH_WALK2, PETOOH_WALK3, PETOOH_WALK4,
		IMAGES_SIZE
	};
}

namespace Animations {
	enum Index {
		PETOOH_WALK = 0,
		ANIMATIONS_SIZE
	};
}

namespace Sounds {
	enum Index {
		MMM = 0, SORRY, POK,
		SOUNDS_SIZE
	};
}

namespace Music {
	enum Index {
		MAIN = 0,
		MUSIC_SIZE
	};
}

class mySound {
public:
	void init(std::string path);
	sf::Sound sound;
	void playSound();
private:
	sf::SoundBuffer buffer;
};

class myMusic {
public:
	myMusic(std::string path);
	sf::Music music;
};

class Manager {
public:
	static void loadImages();
	static void loadImage(std::string path, Images::Index name);
	static void loadSounds();
	static void loadMusic();
	static sf::Texture images[Images::IMAGES_SIZE];
	static mySound sounds[Sounds::SOUNDS_SIZE];
	static std::pair <Images::Index*, size_t> animations[Animations::ANIMATIONS_SIZE];
	static myMusic* music[Music::MUSIC_SIZE];
	static sf::Sprite getSprite(Images::Index name);
	static sf::Texture* getTexture(Images::Index name);
private:
	static std::vector <sf::SoundBuffer> SBuffer;
};

#endif