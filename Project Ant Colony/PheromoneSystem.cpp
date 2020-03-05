#pragma once
#include "PheromoneSystem.h"
#include <iostream>
#include <SFML/Graphics.hpp>

PheromoneSystem::PheromoneSystem(unsigned int count):
	m_particle(count),
	m_pvisual(count),
	m_empty_indx(count),
	m_active_index(0)
{
	for (std::size_t i = 0; i < count; i++) //Index of empty vertex
	{
		m_empty_indx[i] = i;
		m_particle[i].m_active_status = false;
		m_particle[i].m_pstrength = 0.0f;
	}
}

void PheromoneSystem::decay(float dt)
{
	//std::cout << " active pheromone: " << m_active_index.size() << "\n";
	for (std::size_t i = 0; i < m_particle.size(); i++)
	{
		if (m_particle[i].m_active_status == true) 
		{

			Pheromone& p = m_particle[i];
			p.m_pstrength = p.m_pstrength - p.m_decayrate * dt;

			if (p.m_pstrength <= 0 && p.m_active_status == true)
			{
				p.m_pstrength = 0;
				destroyParticle(i);
			}
			//=======VISUAL UPDATE======
			float alpha = p.m_pstrength / m_initial_strength;

			
			sf::Color tmpColor = m_pvisual[i].getFillColor();
			tmpColor.a = static_cast<sf::Uint8>(alpha * 255);
			m_pvisual[i].setFillColor(tmpColor);

			//for (int n = 0; n < m_active_index.size(); n++)
			//	std::cout << m_active_index[n] << " ";
			//std::cout << std::endl;
			//std::cout<<m_empty_indx.size() << std::endl;
		}
		
	}
}

//change visual settings here
void PheromoneSystem::createParticle(sf::Vector2f spawn_pos, Activity ant_activity)
{
	int assignedIndx = m_empty_indx[0]; //assign new particle to index from list of empty index 
	Pheromone &p = m_particle[assignedIndx];
	p.setStrength(m_initial_strength); // strength is 100;
	p.setCoord(spawn_pos);
	p.m_active_status = true;

	//Chemical level
	p.resetChemlvl(); //reset all chemical concentration level to 0
	if (ant_activity == Activity::FORAGING) 
		p.clvl_forage.concentration = 1.0f; //secret when ant is foraging
	if (ant_activity == Activity::GATHERING)
		p.clvl_food.concentration = 1.0f; //secret when ant found food

	//---pheromone visual----
	sf::CircleShape& v = m_pvisual[assignedIndx];
	sf::Color pheromon_color(static_cast<sf::Uint8>(p.clvl_danger.concentration * 255), //R: danger lvl
							 static_cast<sf::Uint8>(p.clvl_food.concentration * 255), //G: food lvl
							 static_cast<sf::Uint8>(p.clvl_forage.concentration * 255)); //B: forage lvl
	v.setPosition(p.getCoord());
	v.setFillColor(pheromon_color);
	v.setRadius(2.0f); //size of visual
	v.setOrigin(v.getRadius() / 2, v.getRadius() / 2);
	//v.setOutlineColor(sf::Color::Transparent);
	

	//m_vertices[assignedIndx].color =  sf::Color::Red;
	//m_vertices[assignedIndx].position = spawn_pos;

	m_active_index.push_back(assignedIndx);
	m_empty_indx.erase(m_empty_indx.begin()); //remove assign index from empty_list because it is taken by newly created particle

}

void PheromoneSystem::destroyParticle(std::size_t index)
{
	m_active_index.erase(m_active_index.begin());
	m_empty_indx.push_back(index); //add the destroyed particle index to empty_list
	m_particle[index].m_active_status = false;
	//m_vertices[index].color.a = 0.0f;
	//std::cout << "Particle destroyed \n";
}
