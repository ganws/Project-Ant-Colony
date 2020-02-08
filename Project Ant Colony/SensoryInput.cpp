#pragma once

#include "SensoryInput.h"
#include<iostream>
//SensoryInput::SensoryInput() {};

//constructor destructor
SensoryInput::SensoryInput() {};

//calculate sensory circle radius depending on set count and distance from origin
//position them uniformly around the ant
SensoryInput::SensoryInput(float r1, int count, sf::Vector2f ant_pos, int active_circle_num):
	m_sensory_circle(count)
{
	//std::cout << "Sensory input circles of n= " << count << " is succesfully initiated" << "\n";//debug
	m_count = count;
	float theta = constants::pi / count;
	float r2 = sin(theta) * r1; //calculate radius of one small circle
	m_active_num = active_circle_num;
	for (int n = 0; n < count; n++)
	{
		SensoryCircle& cs = m_sensory_circle[n];
		cs.setPosition(ant_pos);

		cs.m_angle_indx = 2 * constants::pi / count * n; 

		cs.m_offset_x = cos(cs.m_angle_indx) * r1; //origin x axis of small circle
		cs.m_offset_y = sin(cs.m_angle_indx) * r1; //origin y axis of small circle
		cs.setFillColor(sf::Color::Red);
		cs.setRadius(r2);

		//cs.m_angle_indx -= constants::pi / 2;
		cs.setOrigin(r2 / 2, r2 / 2);
		updateVisual();
		
	}
}

SensoryInput::~SensoryInput() {};

//center the sensory circles around (x,y).
void SensoryInput::setPos(float x, float y)
{
	
	for (int n = 0; n < m_count; n++)
	{
		SensoryCircle& cs = m_sensory_circle[n];
		cs.setPosition(x+cs.m_offset_x, y+cs.m_offset_y);
	}
}

//detect collision of pheromone particles for each sensory circles. sum up all the chem level.
void SensoryInput::detectPheromone(PheromoneSystem* p_system)
{
	//iterate all sensory circles
	for (int n = 0; n < m_count; n++)
	{
		SensoryCircle& cs = m_sensory_circle[n]; 

		//reset sum
		cs.sum_clvl_danger = 0;
		cs.sum_clvl_food = 1;
		cs.sum_clvl_forage = 0;

		//detect collision for all active pheromones
		for (auto m : p_system->m_active_index)
		{
			Pheromone& p = p_system->m_particle[m];
			
			if (cs.getGlobalBounds().contains(p_system->m_pvisual[m].getPosition()))
			{
				//std::cout << "collide!\n" ;//debug

				//sum all the chemical level
				cs.sum_clvl_danger += p.clvl_danger.concentration;
				cs.sum_clvl_food += p.clvl_food.concentration+20;
				cs.sum_clvl_forage += p.clvl_forage.concentration;

			}
		//	std::cout << cs.sum_clvl_food << "   ";
		}
		//std::cout << std::endl;
		
		cs.updateCi(); //update circle Ci (numerator in computing probability)
	}
	//updateVisual();
}

//vary circle transparency according to chem lvl
void SensoryInput::updateVisual()
{
	
	for (int n = 0; n < m_count; n++)
	{
		SensoryCircle& c = m_sensory_circle[n];

		float str_visual = c.sum_clvl_food / 10 * 255;
		if (str_visual >= 255) str_visual = 255;
		sf::Color tmp_color = sf::Color::Red;
		tmp_color.a = static_cast<sf::Uint8>(str_visual);
		c.setFillColor(tmp_color);
	}

	m_sensory_circle[m_facing_index].setFillColor(sf::Color::Green);

	
}

//update face index of circle that ant face
void SensoryInput::updateFaceIndex(float ant_face_angle)
{
	//make angle system consistent 
	ant_face_angle -= 90;
	if (ant_face_angle < 0)
		ant_face_angle = 360 + ant_face_angle;

	//initialize
	float current_diff = 0;
	float smallest_diff = 360;
	int tmp_indx = 0;

	//find the closest index
	for (int n = 0; n < m_count; n++)
	{
		current_diff = abs(double(m_sensory_circle[n].m_angle_indx)*180/constants::pi - ant_face_angle);
		if (current_diff < smallest_diff)
		{
			smallest_diff = current_diff;
			tmp_indx = n;
		}
	}
	m_facing_index = tmp_indx;

	//update active circles element
	active_circle_indx.clear(); //clear active index elements
	active_circle_indx.push_back(m_facing_index);
	//std::cout << active_circle_indx[0] << " "; //debug
	for (int i = 1; i <= m_active_num; i++) //left [m_active_num] circles
	{
		int tmp_indx2 = (m_facing_index + i)%m_count;
		active_circle_indx.push_back(tmp_indx2);
		//std::cout << tmp_indx2 << " ";//debug
	}

	for (int i = 1; i <= m_active_num; i++) //right [m_active_num] circles
	{
		int tmp_indx2 = (m_facing_index - i) % m_count;
		if (tmp_indx2 < 0)
			tmp_indx2 = m_count + tmp_indx2;
		active_circle_indx.push_back(tmp_indx2);
		//std::cout << tmp_indx2 << " ";//debug
	}
	//std::cout << std::endl;//debug

}

//set active circle to weight alpha = 1;
void SensoryInput::updateWeight(std::vector<PathBlocker>* path_blck_ptr)
{
	for (int n = 0; n < m_count; n++)
	{
		m_sensory_circle[n].coeff_weight = 0.1;
		m_sensory_circle[n].coeff_pathing = 1.0;
		//check pathblockers
		for (auto m = path_blck_ptr->begin(); m != path_blck_ptr->end(); m++)
			if (m_sensory_circle[n].getGlobalBounds().intersects((*m).getGlobalBounds()))
			{
				m_sensory_circle[n].coeff_pathing = 0.0;
				std::cout << "Path block detected" << "\n"; //debug
			}
	}
	for (auto n : active_circle_indx)
		m_sensory_circle[n].coeff_weight = 10;
		
}

void SensoryInput::lightUpCircle(int light_circle_indx)
{
	//clear all circle with color
	for (int n = 0; n < m_count; n++)
		m_sensory_circle[n].setFillColor(sf::Color::Transparent);

	//light up circle
	m_sensory_circle[light_circle_indx].setFillColor(sf::Color::Green);


}