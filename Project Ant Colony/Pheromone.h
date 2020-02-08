#pragma once

#include<SFML/Graphics.hpp>
#include<iostream>
/*=======PHEROMONE DEFINITION
Pheromone is a super class object defined as a single point of pixel, with maximum strength H0=100 at point of creation,
decaying linearly with time at rate alpha [/s]: H(t) = H0 - alpha*t


*/

struct ChemicalLevel
{
	float concentration{0.0f};
	int chem_sign{ 1 }; //attractant=1, repellent=-1

	ChemicalLevel(float init_concentration, int init_a)
	{
		if (init_a != 1 && init_a != -1)
		{
			std::cout << "Error: Chemical Attractant constant must be either 1 or -1!"; //for debug
		}
		concentration = init_concentration;
		chem_sign = init_concentration;
	}
	ChemicalLevel() {};
};

class Pheromone //public sf::CircleShape
{
public:
	float m_pstrength{0.0f}; //strength of hormone
	float m_decayrate{ 10.0f }; //strength decay/sec
	bool m_active_status = true; //if pheromone still exists
	sf::Vector2f m_coord{}; //coordinate

	ChemicalLevel clvl_food; //when a food is found (attractant)
	ChemicalLevel clvl_forage; //when searching for food (repellent)
	ChemicalLevel clvl_danger; //when there is danger (repellent)

	//chemical level found in a pheromone particle that contains that information from secreting ant.
	// can be devided into attractant and repellent

	// member function
	Pheromone(); //constructor 
	~Pheromone(); //destructor

	//void secretePheromon(Activity ant_activity);

	//--getter and setter
	void setCoord(sf::Vector2f new_coord); //set coordinate
	sf::Vector2f getCoord(); //get coordinate
	void setStrength(float newStr);
	float getStrength();
	void resetChemlvl();

private:

};

