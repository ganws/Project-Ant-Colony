#pragma once

#include "Ant.h"
#include "Colony.h"

Colony::Colony() {}
Colony::~Colony() {}

void Colony::addAnt(sf::Vector2f spawn_loc)
{
	//Ant dummyAnt(pblocker_systm_ptr);
	//dummyAnt.initAnt(0.1, spawn_loc, ant_skin);
	this->AntContainer.push_back(Ant(pblocker_systm_ptr));
	AntContainer[AntContainer.size()-1].initAnt(0.1, spawn_loc, ant_skin);
	std::cout << "ant created!\n";
	ant_num++;
}

void Colony::initColony(std::vector<PathBlocker>* ptr_copy, sf::Texture *ant_skin_main)
{
	this->pblocker_systm_ptr = ptr_copy;
	ant_skin = ant_skin_main;
}

void Colony::drawColony(sf::RenderWindow* window)
{
	if (ant_num != 0)
	{
		for (auto n : AntContainer)
		{
			//std::cout << k << "\n";
			window->draw(n);
			//k++;
		}
	}
}

int Colony::getAntNum()
{
	return ant_num;
}