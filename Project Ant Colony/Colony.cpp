#pragma once

#include "Ant.h"
#include "Colony.h"

Colony::Colony() {}
Colony::~Colony() {}

void Colony::addAnt(sf::Vector2f spawn_loc)
{
	Ant dummyAnt(pblocker_systm_ptr);
	dummyAnt.initAnt(0.1, spawn_loc, ant_skin);
	this->AntContainer.push_back(dummyAnt);
}

void Colony::initColony(std::vector<PathBlocker>* ptr_copy)
{
	this->pblocker_systm_ptr = ptr_copy;

	//======LOAD TEXTURE========//
	if (!ant_skin.loadFromFile("walk.png"))
		std::cout << "unable to load skin. \n";
}	