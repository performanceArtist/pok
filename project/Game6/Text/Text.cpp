#include "Text.h"

Font Text::comicSans("data/Fonts/comic.ttf");

Font::Font(std::string path) {
	font.loadFromFile(path);
}

sf::Text Text::center(std::string text, unsigned int size, sf::Color color) {
	sf::Text txt(text, Text::comicSans.font);
	txt.setCharacterSize(size); 
	txt.setFillColor(color);
	sf::FloatRect textRect = txt.getLocalBounds();
	txt.setOrigin(textRect.left + textRect.width / 2.0f,
		textRect.top + textRect.height / 2.0f);
	txt.setPosition(sf::Vector2f(SCRWIDTH / 2.0f, SCRHEIGHT / 2.0f));
	return txt;
}

sf::Text Text::leftCorner(std::string text, unsigned int size, sf::Color color) {
	sf::Text txt(text, Text::comicSans.font);
	txt.setCharacterSize(size);
	txt.setFillColor(color);
	txt.setPosition(sf::Vector2f(0, 0));
	return txt;
}

sf::Text Text::getText(std::string text, unsigned int size, sf::Color color) {
	sf::Text txt(text, Text::comicSans.font);
	txt.setCharacterSize(size);
	txt.setFillColor(color);
	return txt;
}