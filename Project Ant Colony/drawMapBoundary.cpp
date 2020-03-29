#include "drawMapBoundary.h"

void drawMapBoundary(std::vector<PathBlocker>* pblock_systm)
{
	PathBlocker dummyPB;
	float PB_length = dummyPB.getSize().x; //get length of path blocker
	
	int PBnum_top_botm = static_cast<int>(ceil( GameSetting::worldWidth / PB_length));
	int PBnum_left_right = static_cast<int>(ceil(GameSetting::worldHeight / PB_length)+2);

	//=========TOP============//
	PathBlocker* pb_top;
	pb_top = new PathBlocker[PBnum_top_botm];
	for (int n = 0; n < PBnum_top_botm; n++)
	{
	
		sf::Vector2f placment_pos = sf::Vector2f(n * PB_length+ PB_length / 2, -PB_length / 2);
		sf::Color PBColor = sf::Color::Blue;
		(pb_top+n)->initPathBlocker(placment_pos, PBColor, PB_length);
		pblock_systm->push_back(*(pb_top + n));
	}

	//=========BOTTOm============//
	PathBlocker* pb_bot;
	pb_bot = new PathBlocker[PBnum_top_botm];
	for (int n = 0; n < PBnum_top_botm; n++)
	{

		sf::Vector2f placment_pos = sf::Vector2f(n * PB_length + PB_length / 2, GameSetting::worldHeight +  PB_length / 2);
		sf::Color PBColor = sf::Color::Blue;
		(pb_bot + n)->initPathBlocker(placment_pos, PBColor, PB_length);
		pblock_systm->push_back(*(pb_bot + n));
	}

	//=========LEFT============//
	PathBlocker* pb_left;
	pb_left = new PathBlocker[PBnum_left_right];
	for (int n = 0; n < PBnum_left_right; n++)
	{

		sf::Vector2f placment_pos = sf::Vector2f(-PB_length / 2, PB_length*n-PB_length/2);
		sf::Color PBColor = sf::Color::Blue;
		(pb_left + n)->initPathBlocker(placment_pos, PBColor, PB_length);
		pblock_systm->push_back(*(pb_left + n));
	}

	//=========RIGHT============//
	PathBlocker* pb_right;
	pb_right = new PathBlocker[PBnum_left_right];
	for (int n = 0; n < PBnum_left_right; n++)
	{

		sf::Vector2f placment_pos = sf::Vector2f(GameSetting::worldWidth+PB_length / 2, PB_length * n - PB_length / 2);
		sf::Color PBColor = sf::Color::Blue;
		(pb_right + n)->initPathBlocker(placment_pos, PBColor, PB_length);
		pblock_systm->push_back(*(pb_right + n));
	}
}