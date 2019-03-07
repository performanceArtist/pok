#ifndef TEXT_H
#define TEXT_H

#include <string>

#include <SFML/Graphics.hpp>

#include "../Constants.h"

class Font {
public:
	Font(std::string path);
	sf::Font font;
};

class Text {
public:
	static Font comicSans;
	static sf::Text center(std::string text, unsigned int size, sf::Color color);
	static sf::Text leftCorner(std::string text, unsigned int size, sf::Color color);
	static sf::Text getText(std::string text, unsigned int size, sf::Color color);
};


#endif
