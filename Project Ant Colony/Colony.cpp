#pragma once

#include "Colony.h"

Colony::Colony() {}
Colony::~Colony() {}

void Colony::addResourceAmount(int add_amount)
{
	m_totalResource += add_amount;
}

int Colony::getResourceAmount()
{
	return m_totalResource;
}

void Colony::addAnt(sf::Vector2f spawn_loc)
{
	//Ant dummyAnt(pblocker_systm_ptr);
	//dummyAnt.initAnt(0.1, spawn_loc, ant_skin);

	this->AntContainer.push_back(Ant());
	Ant& newant = AntContainer[AntContainer.size() - 1];
	newant.initAnt(0.1f, spawn_loc, ant_skin, pblocker_systm_ptr, food_systm_ptr, m_pheromatrix_ptr);
	float init_faceangle{ static_cast<float>(rand() / (RAND_MAX / 360)) };
	newant.setRotation(init_faceangle);
	newant.setRotation(init_faceangle);
	newant.rememberCholePos(m_chole_position);
	newant.m_colony = this;
	newant.m_terrain_system_ptr = this->m_terrain_system_ptr;
	//printf("Ant created! face angle = %f\n", init_faceangle);
	ant_num++;
	std::cout << "Ant added! Size = " << ant_num << std::endl;
}


//NEW VERSION
void Colony::addAnt2(sf::Vector2f spawn_loc)
{
	//Ant dummyAnt(pblocker_systm_ptr);
	//dummyAnt.initAnt(0.1, spawn_loc, ant_skin);

	this->AntContainer.push_back(Ant());
	Ant& newant = AntContainer[AntContainer.size() - 1];
	newant.initAnt(0.1f, spawn_loc, ant_skin, pblocker_systm_ptr, food_systm_ptr, m_pheromatrix_ptr);
	float init_faceangle{ static_cast<float>(rand() / (RAND_MAX / 360)) };
	newant.setRotation(init_faceangle);
	newant.setRotation(init_faceangle);
	newant.rememberCholePos(m_chole_position);
	newant.m_colony = this;

	newant.m_foodSystem_ptr = this->m_foodSystm_ptr;
	newant.m_PblockerSystem_ptr = this->m_PBlockerSystm_ptr;
	newant.m_terrain_system_ptr = this->m_terrain_system_ptr;
	//printf("Ant created! face angle = %f\n", init_faceangle);
	ant_num++;
	std::cout << "Ant added! Size = " << ant_num << std::endl;
}

void Colony::initColony(std::vector<PathBlocker>* ptr_copy, sf::Texture* ant_skin_main, PheroMatrix* phero_mat_input,
						SpatialPartition* partition_input, std::vector<Food>* food_system_input, Terrain* terrain_sys_input)
{
	this->AntContainer.reserve(1000);
	this->pblocker_systm_ptr = ptr_copy;
	this->m_terrain_system_ptr = terrain_sys_input;
	this->food_systm_ptr = food_system_input;
	this->ant_skin = ant_skin_main;
	this->m_partition_ptr = partition_input;
	this->m_pheromatrix_ptr = phero_mat_input;
	std::cout << "Colony Initialized with pblock add:" << pblocker_systm_ptr << "\n";
	std::cout << "size of Ant = " << AntContainer.size() << "\n";
	this->m_totalResource = 0;
}

//NEW VERSION
void Colony::initColony(PathBlockSystem* pb_system, sf::Texture* skin, PheroMatrix* phero_system,
	SpatialPartition* partition_system, FoodSystem* food_system, Terrain* terrain_system)
{
	this->AntContainer.reserve(1000);
	this->m_PBlockerSystm_ptr = pb_system;
	this->m_terrain_system_ptr = terrain_system;
	this->m_foodSystm_ptr = food_system;
	this->ant_skin = skin;
	this->m_partition_ptr = partition_system;
	this->m_pheromatrix_ptr = phero_system;
	std::cout << "Colony Initialized with pblock add:" << pblocker_systm_ptr << "\n";
	std::cout << "size of Ant = " << AntContainer.size() << "\n";
	this->m_totalResource = 0;
}


void Colony::drawColony(sf::RenderWindow &window)
{
	if (AntContainer.size() != 0)
	{
		for (auto& n : AntContainer)
		{
			//std::cout << k << "\n";
			//std::cout << "antdraw! \n";
			if (n.m_visible)
			{
				window.draw(n);
				window.draw(n.m_food_scrap);
			}
			if (dispSensor)
				n.drawSensoryCircle(window);
			//k++;
		}
	}
}

int Colony::getAntNum()
{
	return AntContainer.size();
}

void Colony::setCholePos(sf::Vector2f chole_pos_input)
{
	m_chole_position = chole_pos_input;
}

sf::Vector2f Colony::getCholePos()
{
	return m_chole_position;
}

void Colony::computeAntMove(float dt)
{
	if (AntContainer.size() != 0)
	{
		for (auto& n : AntContainer)
		{
			this->m_terrain_system_ptr->clearAntCoeff(n);
			n.Update(dt);
			this->m_terrain_system_ptr->updateAntCoeff(n, 0);
		}
	}
}