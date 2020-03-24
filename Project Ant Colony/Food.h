#pragma once

#include <iostream>
#include <list>
#include <sfml/Graphics.hpp>

class Food : public sf::CircleShape
{
public:
	bool depleted{false};

	Food(sf::Vector2f spwn_loc, sf::Color color, float radius);
	~Food();
	void storePath(std::vector<sf::Vector2f> shortest_path );
	int Harvested(int harvest_amount);

private:

	int m_amount{50};
	std::vector <sf::Vector2f> m_Bestpath;
	std::vector<Food>* container_ptr;
};

