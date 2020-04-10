#include "World.h"

World::World() {};
World::~World() {};

void World::createWorld()
{
	//CREATE PHEROMONE SYSTEM
	pheroSystem_ptr = new PheroMatrix;
	pheroSystem_ptr->initPheroMatrix(GameSetting::worldWidth, GameSetting::worldHeight, sf::Vector2u(300, 300));

	//PATH BLOCKERS SYSTEM
	pblockerSystem_ptr = new PathBlockSystem;
	pblockerSystem_ptr->drawMapBoundary();


	//SPATIAL PARTITIONING SYSTEM
	spatialPartition_ptr = new SpatialPartition;
	spatialPartition_ptr->initSpatialPartition(GameSetting::worldWidth, GameSetting::worldHeight, sf::Vector2u(20, 20));
	spatialPartition_ptr->updateCheckIndex(pblockerSystem_ptr);

	//FOOD SYSTEM
	foodSystem_ptr = new FoodSystem;

	//TERRAIN
	terrainSystem_ptr = new Terrain(GameSetting::worldWidth, GameSetting::worldHeight, sf::Vector2u(300, 300));
	for (auto& pb : pblockerSystem_ptr->m_PBcontainer)
		terrainSystem_ptr->updateCoeff(pb);

	//COLONY
	colonySystem_ptr = new Colony;
	colonySystem_ptr->initColony2(pblockerSystem_ptr, &GameResource->antTexture, pheroSystem_ptr, 
		spatialPartition_ptr, foodSystem_ptr, terrainSystem_ptr);

}

void World::importResource(ResourceManager* resource)
{
	GameResource = resource;
};

void World::Update(float dt)
{
	colonySystem_ptr->computeAntMove(dt);
	pheroSystem_ptr->pheromoneDecay(dt);

	//spatial partitioning update
	//partition.clearPartition(); //clear all partition of ant pointers
	//for (int i = 0; i < Colony1.getAntNum(); i++)
	//{
	//	Colony1.AntContainer[i].collision_check = false;
	//	partition.updatePartition(&Colony1.AntContainer[i], Colony1.AntContainer[i].getPosition()); //add ant pointer to partition
	//}
	//partition.updateAntStatus(); //update ant collision check bool status

}

void World::Draw(sf::RenderWindow &window)
{
	//window.draw(*pheroSystem_ptr);
	pblockerSystem_ptr->windowDraw(window);
	foodSystem_ptr->Draw(window);
	colonySystem_ptr->drawColony(window);

}
