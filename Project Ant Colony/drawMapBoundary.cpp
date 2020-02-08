#include "drawMapBoundary.h"

void drawMapBoundary(std::vector<PathBlocker>* pblock_systm)
{
	PathBlocker dummyPB;
	float PB_length = dummyPB.getSize().x; //get length of path blocker
	
	int PBnum_top_botm = ceil( GameSetting::windowWidth / PB_length);
	int PBnum_left_right = ceil(GameSetting::windowHeight / PB_length)+2;

	//=========TOP============//
	PathBlocker* pb_top;
	pb_top = new PathBlocker[PBnum_top_botm];
	for (int n = 0; n < PBnum_top_botm; n++)
	{
	
		sf::Vector2f placment_pos = sf::Vector2f(n * PB_length+ PB_length / 2, -PB_length / 2);
		sf::Color PBColor = sf::Color::Red;
		(pb_top+n)->initPathBlocker(placment_pos, PBColor, PB_length);
		pblock_systm->push_back(*(pb_top + n));
	}

	//=========BOTTOm============//
	PathBlocker* pb_bot;
	pb_bot = new PathBlocker[PBnum_top_botm];
	for (int n = 0; n < PBnum_top_botm; n++)
	{

		sf::Vector2f placment_pos = sf::Vector2f(n * PB_length + PB_length / 2, GameSetting::windowHeight +  PB_length / 2);
		sf::Color PBColor = sf::Color::Red;
		(pb_bot + n)->initPathBlocker(placment_pos, PBColor, PB_length);
		pblock_systm->push_back(*(pb_bot + n));
	}

	//=========LEFT============//
	PathBlocker* pb_left;
	pb_left = new PathBlocker[PBnum_left_right];
	for (int n = 0; n < PBnum_left_right; n++)
	{

		sf::Vector2f placment_pos = sf::Vector2f(-PB_length / 2, PB_length*n-PB_length/2);
		sf::Color PBColor = sf::Color::Red;
		(pb_left + n)->initPathBlocker(placment_pos, PBColor, PB_length);
		pblock_systm->push_back(*(pb_left + n));
	}

	//=========RIGHT============//
	PathBlocker* pb_right;
	pb_right = new PathBlocker[PBnum_left_right];
	for (int n = 0; n < PBnum_left_right; n++)
	{

		sf::Vector2f placment_pos = sf::Vector2f(GameSetting::windowWidth+PB_length / 2, PB_length * n - PB_length / 2);
		sf::Color PBColor = sf::Color::Red;
		(pb_right + n)->initPathBlocker(placment_pos, PBColor, PB_length);
		pblock_systm->push_back(*(pb_right + n));
	}
}