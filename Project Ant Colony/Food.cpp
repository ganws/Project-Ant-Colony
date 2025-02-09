#include "Food.h"

Food::Food(sf::Vector2f spwn_loc, sf::Color color, float radius)
{
	this->setFillColor(color);
	this->setRadius(radius);
	this->setOrigin(radius, radius);
	this->setPosition(spwn_loc);
	m_amount = 100;
}

Food::Food(const Food& copyfood)
{
	this->setFillColor(copyfood.getFillColor());
	this->setRadius(copyfood.getRadius());
	this->setOrigin(this->getRadius(), this->getRadius());
	this->setPosition(copyfood.getPosition());
	this->m_amount = copyfood.m_amount;
}

int Food::Harvested(int harvest_amount)
{
	m_amount = m_amount - harvest_amount;
	if (m_amount <= 0)
	{
		depleted = true;
		this->setFillColor(sf::Color::Transparent);
		return m_amount;
	}
	return harvest_amount;
}

Food::~Food() {}

void Food::storePath(std::vector<sf::Vector2f> short_path)
{
	m_Bestpath =  short_path;
}