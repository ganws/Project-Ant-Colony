#pragma once
#include "Ant.h"
#include "PathBlocker.h"
#include "PheroMatrix.h"
#include "SpatialPartition.h"
#include "FoodSystem.h"
#include "PathBlockSystem.h"
#include "Terrain.h"

class Colony;
class Ant;
class SpatialPartition;
class Terrain;

class Colony
{
public:

	sf::Texture* ant_skin;
	std::vector<Ant> AntContainer{};

	bool dispSensor{false};

public:

	Colony();
	~Colony();

	void addAnt(sf::Vector2f spwn_loc);
	void addAnt2(sf::Vector2f spwn_loc);

	void removeAnt();
	void computeAntMove(float dt);


	void initColony(std::vector<PathBlocker>* pb_ptr, sf::Texture *ant_skin_mian, PheroMatrix* pheromat_input, 
					SpatialPartition *partition_input, std::vector<Food>* food_system_input, Terrain* terrain_input);

	//new version
	void initColony(PathBlockSystem* pb_system, sf::Texture* ant_skin_mian, PheroMatrix* pheromat_input,
		SpatialPartition* partition_input, FoodSystem* food_system_input, Terrain* terrain_input);

	void updateAntNum();
	void drawColony(sf::RenderWindow &window);
	int getAntNum();
	sf::Vector2f getCholePos();
	void setCholePos(sf::Vector2f chole_pos_input);
	void addResourceAmount(int add_amount);
	int getResourceAmount();

private:
	int ant_num{0};
	std::vector<PathBlocker>* pblocker_systm_ptr{ nullptr };
	std::vector<Food>* food_systm_ptr{ nullptr };

	FoodSystem* m_foodSystm_ptr{ nullptr };
	PathBlockSystem* m_PBlockerSystm_ptr{ nullptr };
	PheroMatrix* m_pheromatrix_ptr{nullptr};
	SpatialPartition* m_partition_ptr{ nullptr };
	Terrain* m_terrain_system_ptr{ nullptr };



	sf::Vector2f m_chole_position{};
	int m_totalResource;

};
