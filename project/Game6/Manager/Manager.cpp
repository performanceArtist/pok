#include "Manager.h"

sf::Texture Manager::images[Images::IMAGES_SIZE];
mySound Manager::sounds[Sounds::SOUNDS_SIZE];
myMusic* Manager::music[Music::MUSIC_SIZE];
//Images::Index* Manager::animations[Animations::ANIMATIONS_SIZE];
std::pair <Images::Index*, size_t> Manager::animations[Animations::ANIMATIONS_SIZE];

void Manager::loadImages() {
	loadImage("data/Sprites/char.png", Images::PLAYER);
	loadImage("data/Sprites/projectile.png", Images::BULLET);
	loadImage("data/Sprites/block.png", Images::BLOCK);
	loadImage("data/Sprites/petooh/petooh.png", Images::PETOOH);
	loadImage("data/Sprites/petooh/walk/1.png", Images::PETOOH_WALK1);
	loadImage("data/Sprites/petooh/walk/2.png", Images::PETOOH_WALK2);
	loadImage("data/Sprites/petooh/walk/3.png", Images::PETOOH_WALK3);
	loadImage("data/Sprites/petooh/walk/4.png", Images::PETOOH_WALK4);

	Images::Index* walk = new Images::Index[4]{ Images::PETOOH_WALK1, Images::PETOOH_WALK2, Images::PETOOH_WALK3, Images::PETOOH_WALK4 };
	animations[Animations::PETOOH_WALK] = { walk, 4 };
}

void Manager::loadImage(std::string path, Images::Index name) {
	sf::Texture temp;
	temp.loadFromFile(path);
	images[name] = temp;
}

sf::Sprite Manager::getSprite(Images::Index name) {
	return sf::Sprite(images[name]);
}

sf::Texture* Manager::getTexture(Images::Index name) {
	return &images[name];
}

void Manager::loadSounds() {
	mySound mmm, sorry, pok;

	sounds[Sounds::MMM] = mmm;
	sounds[Sounds::MMM].init("data/Sounds/mmm.ogg");
	sounds[Sounds::SORRY] = sorry;
	sounds[Sounds::SORRY].init("data/Sounds/sorry.ogg");
	sounds[Sounds::POK] = pok;
	sounds[Sounds::POK].init("data/Sounds/pok.ogg");
	sounds[Sounds::POK].sound.setVolume(20);
}

void mySound::init(std::string path) {
	buffer.loadFromFile(path);
	sound.setBuffer(buffer);
}

void mySound::playSound() {
	sound.play();
}

myMusic::myMusic(std::string path) {
	music.openFromFile(path);
}

void Manager::loadMusic() {
	Manager::music[Music::MAIN] = new myMusic("data/Sounds/pillar.ogg");
}
