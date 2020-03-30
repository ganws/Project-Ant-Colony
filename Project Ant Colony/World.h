#pragma once

#include "GameSetting.h"
#include "Ant.h"
#include "Colony.h"
#include "Astar.h"
#include "Food.h"
#include "PathBlockSystem.h"
#include "drawMapBoundary.h"
#include "PheroMatrix.h"
#include "Terrain.h"
#include "FoodSystem.h"
#include "ResourceManager.h"

#include <iostream>
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>

//store all enviroment systems' pointers

class World
{

public:
	World();
	~World();
	void createWorld();
	void importResource(ResourceManager* resource);
	void Update(float dt);
	void Draw(sf::RenderWindow& window);


public:

	ResourceManager* GameResource;
	FoodSystem* foodSystem_ptr;
	SpatialPartition* spatialPartition_ptr;
	PheroMatrix* pheroSystem_ptr;
	Terrain* terrainSystem_ptr;
	PathBlockSystem* pblockerSystem_ptr;
	Colony* colonySystem_ptr;
};