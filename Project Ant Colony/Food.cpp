#include "Food.h"

Food::Food(sf::Vector2f spwn_loc, sf::Color color, float radius)
{
	this->setFillColor(color);
	this->setRadius(radius);
	this->setOrigin(radius, radius);
	this->setPosition(spwn_loc);
	m_amount = 10;
}

Food::~Food() {}