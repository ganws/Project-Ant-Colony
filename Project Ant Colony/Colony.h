#pragma once
#include "Ant.h"
#include "PathBlocker.h"

class Colony
{
public:
	int ant_num{};

	//constructor and destructor
	Colony();
	~Colony();

	//method
	void initColony();
	void addAnt(sf::Vector2f spwn_loc);
	void removeAnt();
	void computeAntMove();
	void initColony(std::vector<PathBlocker>* pb_ptr);

private:
	sf::Texture ant_skin;
	std::vector<Ant> AntContainer;
	std::vector<PathBlocker>* pblocker_systm_ptr{ nullptr };

};
