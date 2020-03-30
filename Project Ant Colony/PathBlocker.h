#pragma once
#include<SFML/Graphics.hpp>

class PathBlocker : public sf::RectangleShape
{
public:
	PathBlocker(); //default constructor
	PathBlocker(sf::Vector2f initPos, sf::Color initColor, float init_length);
	~PathBlocker();
	
	void initPathBlocker(sf::Vector2f initPos, sf::Color initColor, float init_length);
};

