#pragma once
#include "Ant.h"
#include "PathBlocker.h"

class Colony
{
public:

	//constructor and destructor
	Colony();
	~Colony();
	sf::Texture* ant_skin;
	std::vector<Ant> AntContainer;

	//method
	void addAnt(sf::Vector2f spwn_loc);
	void removeAnt();
	void computeAntMove();
	void initColony(std::vector<PathBlocker>* pb_ptr, sf::Texture *ant_skin_mian);
	void updateAntNum();
	void drawColony(sf::RenderWindow* window);
	int getAntNum();

private:
	int ant_num{0};
	std::vector<PathBlocker>* pblocker_systm_ptr{ nullptr };

};
