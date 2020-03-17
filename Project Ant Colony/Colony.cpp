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
	this->AntContainer[(AntContainer.size()-1)].initAnt(0.1, spawn_loc, ant_skin, pblocker_systm_ptr, food_systm_ptr);
	float init_faceangle{ static_cast<float>(rand() / (RAND_MAX / 360)) };
	this->AntContainer[AntContainer.size()-1].setRotation(init_faceangle);
	printf("Ant created! face angle = %f\n", init_faceangle);
	ant_num++;
}

void Colony::initColony(std::vector<PathBlocker>* ptr_copy, sf::Texture *ant_skin_main, PheroMatrix* phero_mat_input, SpatialPartition* partition_input, std::vector<Food>* food_system_input)
{
	this->AntContainer.reserve(sizeof(Ant)*1000);
	this->pblocker_systm_ptr = ptr_copy;
	this->food_systm_ptr = food_system_input;
	this->ant_skin = ant_skin_main;
	this->m_partition_ptr = partition_input;
	//this->pheromones_ptr = phr_ptr;
	this->m_pheromatrix_ptr = phero_mat_input;
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
		//std::cout << AntContainer[0].pblocker_systm_ptr;
		for (auto& n : AntContainer)
		{
			n.updateMovement(dt, pblocker_systm_ptr);
			//n.secretPheromon(dt, pheromones_ptr);
			n.secretPheromon2(dt, m_pheromatrix_ptr);
			sf::Vector2f tmp{};
			tmp = n.computeMovementMatrix(dt, m_pheromatrix_ptr);
			switch (n.getActivity())
			{
			case Activity::FORAGING:
				if (n.recallFoodLoc()== sf::Vector2f(0, 0)) //if ant cant recall
				{
				n.issue_move_command(tmp);
				}
				else
				{
					//if (n.m_state.CARRYING == false)
					n.issue_move_command(n.recallFoodLoc());
					//std::cout << "go back to food!\n";
				}

				break;
			case Activity::GATHERING:
				if (n.m_state.CARRYING)
				{
					n.issue_move_command(m_chole_position);
					if (n.getGlobalBounds().contains(sf::Vector2f(1000, 1000)))
					{
						n.m_state.CARRYING = false;
						n.switchActivity(Activity::FORAGING);
					}
				}
				break;
			default:
				break;
			}
		}

		//check collision via spatial partitioning
		//for (auto chckIndx : m_partition_ptr->checkIndex)
		//{
		//	for (auto &ant : m_partition_ptr->m_Partition[chckIndx].ant_ptr)
		//	{
		//		for (auto& p : *pblocker_systm_ptr)
		//		{
		//			if (p.getGlobalBounds().intersects(ant->getGlobalBounds()))
		//			{
		//				sf::Vector2f faceVector = ant->getPosition() - p.getPosition();
		//				//ant->issue_move_command(sf::Vector2f(500, 500));
		//			}
		//		}
		//	}
		//}
	}
}