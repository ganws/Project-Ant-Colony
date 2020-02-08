#pragma once

#include "Pheromone.h"

Pheromone::Pheromone() //constructor
{
	//m_pstrength = 100.0f; //initial strength, H0
} //constructor

Pheromone::~Pheromone() {}; //destructor

void Pheromone::setCoord(sf::Vector2f new_coord)
{
	//this->setPosition(new_coord);
	this->m_coord = new_coord;
}

sf::Vector2f Pheromone::getCoord()
{
	return this->m_coord;
}

void Pheromone::setStrength(float new_str)
{
	this->m_pstrength = new_str;
}

float Pheromone::getStrength()
{
	return this->m_pstrength;
}

void Pheromone::resetChemlvl()
{
	this->clvl_danger.concentration = 0.0f;
	this->clvl_food.concentration = 0.0f;
	this->clvl_forage.concentration = 0.0f;
}
