#pragma once

#include "Food.h"
#include <SFML/Graphics.hpp>

class FoodSystem
{
public:

	FoodSystem();
	~FoodSystem();

	void addFood(sf::Vector2f worldPos); //add a new food to system
	void removeFood(Food *food); //remove food from system

	void Draw(sf::RenderWindow &window);

private:
	std::vector<Food*> FoodContainer;
};

