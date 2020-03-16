#pragma once
#include <iostream>
#include <sfml/Graphics.hpp>

class Food : public sf::CircleShape
{
public:
	Food(sf::Vector2f spwn_loc, sf::Color color, float radius);
	~Food();

private:
	int m_amount{10};
};

