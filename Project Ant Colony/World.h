#pragma once
#include "Ant.h"

//world class that contains all interactable objects

class World
{

public:

	std::vector<Ant> Colony;
	std::vector<PathBlocker> PathBlockers;
};

