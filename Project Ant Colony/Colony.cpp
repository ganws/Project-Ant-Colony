#pragma once

#include "Ant.h"
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
	newant.initAnt(0.1, spawn_loc, ant_skin, pblocker_systm_ptr, food_systm_ptr, m_pheromatrix_ptr);
	float init_faceangle{ static_cast<float>(rand() / (RAND_MAX / 360)) };
	newant.setRotation(init_faceangle);
	newant.setRotation(init_faceangle);
	newant.rememberCholePos(m_chole_position);
	newant.m_colony = this;
	//printf("Ant created! face angle = %f\n", init_faceangle);
	ant_num++;
	std::cout << "Ant added! Size = " << ant_num << std::endl;
}

void Colony::initColony(std::vector<PathBlocker>* ptr_copy, sf::Texture* ant_skin_main, PheroMatrix* phero_mat_input, SpatialPartition* partition_input, std::vector<Food>* food_system_input)
{
	this->AntContainer.reserve(1000);
	this->pblocker_systm_ptr = ptr_copy;
	this->food_systm_ptr = food_system_input;
	this->ant_skin = ant_skin_main;
	this->m_partition_ptr = partition_input;
	//this->pheromones_ptr = phr_ptr;
	this->m_pheromatrix_ptr = phero_mat_input;
	std::cout << "Colony Initialized with pblock add:" << pblocker_systm_ptr << "\n";
	std::cout << "size of Ant = " << AntContainer.size() << "\n";
	this->m_totalResource = 0;
}

void Colony::drawColony(sf::RenderWindow* window, bool display_sensor)
{
	if (AntContainer.size() != 0)
	{
		for (auto& n : AntContainer)
		{
			//std::cout << k << "\n";
			//std::cout << "antdraw! \n";
			if (n.m_visible)
			{
				window->draw(n);
				window->draw(n.m_food_scrap);
			}
			if (display_sensor)
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
			//n.updateMovement(dt);
			//n.secretPheromon(dt, pheromones_ptr);
			sf::Vector2f tmp{};

			n.Update(dt);


			//tmp = n.computeMovementMatrix(dt, m_pheromatrix_ptr);
			//if (n.getActivity() == Activity::GATHERING)
			//n.issue_move_command(tmp);

			/*switch (n.getActivity())
			{
			case Activity::FORAGING:
				tmp = n.computeMovementMatrix(dt);
				break;

			case Activity::GATHERING:
				if (n.m_state.CARRYING)
				{
					n.secretPheromon2(dt, m_pheromatrix_ptr);
					tmp = n.computeMovement_colony(dt, m_pheromatrix_ptr, m_chole_position);
					if (n.getGlobalBounds().contains(m_chole_position))
					{
						n.m_state.CARRYING = false;
						n.switchActivity(Activity::FORAGING);
					}
				}
				break;
			default:
				break;
			}

			n.issue_move_command(tmp);*/
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