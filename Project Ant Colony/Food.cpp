#include "Food.h"

Food::Food(sf::Vector2f spwn_loc, sf::Color color, float radius)
{
	this->setFillColor(color);
	this->setRadius(radius);
	this->setOrigin(radius, radius);
	this->setPosition(spwn_loc);
	m_amount = 100;
}

int Food::Harvested(int harvest_amount)
{
	m_amount = m_amount - harvest_amount;
	if (m_amount <= 0)
	{
		depleted = true;
		return m_amount;
	}
	return harvest_amount;
}

Food::~Food() {}

void Food::storePath(std::vector<sf::Vector2f> short_path)
{
	m_Bestpath =  short_path;
}