#include "FoodSystem.h"

FoodSystem::FoodSystem() 
{
	//FoodContainer.reserve(100); //reserve some memory
};

FoodSystem::~FoodSystem() {};

void FoodSystem::addFood(sf::Vector2f worldPos)
{
	float foodsize{ 10.0f };
	FoodContainer.push_back(new Food(worldPos, sf::Color::Green, foodsize));
};
void FoodSystem::removeFood(Food* food)
{
	for (auto i = FoodContainer.begin(); i != FoodContainer.end(); ++i)
	{
		if (food == *i)
			FoodContainer.erase(i);
	}
}

void FoodSystem::Draw(sf::RenderWindow& window)
{
	for (auto &i : FoodContainer)
	{
		window.draw(*i);
	}
}