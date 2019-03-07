#ifndef CONSTANTS_H
#define CONSTANTS_H

const unsigned short FRAMERATE = 80;

const unsigned int SCRWIDTH = sf::VideoMode::getDesktopMode().width;
const unsigned int SCRHEIGHT = sf::VideoMode::getDesktopMode().height;
const unsigned int WORLDWIDTH = SCRWIDTH;
const unsigned int WORLDHEIGHT = SCRHEIGHT;

const unsigned int WINSCORE = 69;
const unsigned int LOSESCORE = 10;

//types for entities
//no updates
const unsigned char STATIC = 1 << 0;
//initiates collision
const unsigned char COLLIDES = 1 << 1;
//could collide with initiator, has onCollide() method
const unsigned char COLLIDABLE = 1 << 2;
/*
const unsigned char SHOOTS = 1 << 3;
const unsigned char CLEARABLE = 1 << 4;
const unsigned char CONTROLLABLE = 1 << 5;
const unsigned char HOSTILE = 1 << 6;
const unsigned char MOVABLE = 1 << 7;
*/
enum class Object_is {PLAYER, BLOCK, BULLET, PETOOH};

#endif
