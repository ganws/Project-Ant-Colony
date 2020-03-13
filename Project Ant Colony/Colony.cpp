#pragma once

#include "Ant.h"
#include "Colony.h"

Colony::Colony() {}
Colony::~Colony() {}

void Colony::addAnt(sf::Vector2f spawn_loc)
{
	//Ant dummyAnt(pblocker_systm_ptr);
	//dummyAnt.initAnt(0.1, spawn_loc, ant_skin);
	this->AntContainer.push_back(Ant());
	this->AntContainer[(AntContainer.size()-1)].initAnt(0.1, spawn_loc, ant_skin, pblocker_systm_ptr);
	std::cout << "ant created! with pblock_ptr:" << AntContainer[(AntContainer.size()-1)].pblocker_systm_ptr << "\n";
	ant_num++;
}

void Colony::initColony(std::vector<PathBlocker>* ptr_copy, sf::Texture *ant_skin_main, PheromoneSystem* phr_ptr)
{
	this->AntContainer.reserve(sizeof(Ant)*1000);
	this->pblocker_systm_ptr = ptr_copy;
	this->ant_skin = ant_skin_main;
	this->pheromones_ptr = phr_ptr;
	std::cout << "Colony Initialized with pblock add:" << pblocker_systm_ptr << "\n";
	std::cout << "size of Ant = " << sizeof(Ant) << "\n";
}

void Colony::drawColony(sf::RenderWindow* window)
{
	if (AntContainer.size()!=0)
	{
		for (auto &n : AntContainer)
		{
			//std::cout << k << "\n";
			//std::cout << "antdraw! \n";
			window->draw(n);
			n.drawSensoryCircle(window);
			//k++;
		}
	}
}

int Colony::getAntNum()
{
	return AntContainer.size();
}

void Colony::computeAntMove(float dt)
{
	if (AntContainer.size() != 0)
	{
		//std::cout << AntContainer[0].pblocker_systm_ptr;
		for (auto &n : AntContainer)
		{
			n.updateMovement(dt, pblocker_systm_ptr);
			n.secretPheromon(dt, pheromones_ptr);
			n.m_sensory_input.detectPheromone(pheromones_ptr);
			//std::cout << n.pblocker_systm_ptr << "\n";
			//std::cout << AntContainer.size(); "\n";
		}
	}
}