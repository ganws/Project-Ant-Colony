#pragma once

#include "Ant.h"
#include "Colony.h"
#include "Astar.h"
#include "Food.h"
#include "PathBlocker.h"
#include "PheroMatrix.h"
#include "Terrain.h"

#include <iostream>
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>

//store all enviroment systems' pointers

class World
{

public:
	World();
	~World();
	void createWorld(sf::Window &window);


private:
	static PheroMatrix* pheroSystem_ptr;
	static Terrain* terrainSystem_ptr;
	static std::vector<PathBlocker*> pathBlockerSystem_ptr;
	static std::vector<Food*> foodSystem_ptr;
	static Colony* colonySystem_ptr;
};

