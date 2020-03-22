#pragma once

#include <iostream>
#include <list>
#include <sfml/Graphics.hpp>

class Food : public sf::CircleShape
{
public:
	Food(sf::Vector2f spwn_loc, sf::Color color, float radius);
	~Food();
	void storePath(std::list<sf::Vector2f> shortest_path );

private:
	int m_amount{10};
	std::list <sf::Vector2f> m_Bestpath;
};

