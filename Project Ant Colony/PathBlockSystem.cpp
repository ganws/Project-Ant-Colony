#include "PathBlockSystem.h"

PathBlockSystem::PathBlockSystem() {};
PathBlockSystem::~PathBlockSystem() {};

void PathBlockSystem::drawMapBoundary()
{
	// Automatically draw map boundry with square units of pathblocker (PBunits)

	//Define square PBunit
	float PB_length = 20.0f; //get length of path blocker
	PBunit dummyPB(sf::VertexArray(sf::Quads,4) , PB_length); //define square PBunits
	//dummyPB.setOrigin(PB_length / 2.0f, PB_length / 2.0f);
	dummyPB.setColor(sf::Color::Blue);

	int PBnum_top_botm = static_cast<int>(ceil(GameSetting::worldWidth / PB_length));
	int PBnum_left_right = static_cast<int>(ceil(GameSetting::worldHeight / PB_length) + 2);

	//=========TOP============//
	for (int n = 0; n < PBnum_top_botm; n++)
	{
		sf::Vector2f placment_pos = sf::Vector2f(n * PB_length + PB_length / 2, -PB_length / 2);
		dummyPB.setPosition(placment_pos);
		m_PBcontainer.push_back(PBunit(dummyPB));
	}

	//=========BOTTOm============//
	for (int n = 0; n < PBnum_top_botm; n++)
	{
		sf::Vector2f placment_pos = sf::Vector2f(n * PB_length + PB_length / 2, GameSetting::worldHeight + PB_length / 2);
		dummyPB.setPosition(placment_pos);
		m_PBcontainer.push_back(PBunit(dummyPB));
	}

	//=========LEFT============//
	for (int n = 0; n < PBnum_left_right; n++)
	{
		sf::Vector2f placment_pos = sf::Vector2f(-PB_length / 2, PB_length * n - PB_length / 2);
		dummyPB.setPosition(placment_pos);
		m_PBcontainer.push_back(PBunit(dummyPB));
	}

	//=========RIGHT============//
	for (int n = 0; n < PBnum_left_right; n++)
	{
		sf::Vector2f placment_pos = sf::Vector2f(GameSetting::worldWidth + PB_length / 2, PB_length * n - PB_length / 2);
		dummyPB.setPosition(placment_pos);
		m_PBcontainer.push_back(PBunit(dummyPB));
	}

	std::cout << "PBContainersize = " << m_PBcontainer.size() << std::endl;
}
void PathBlockSystem::addPBunit(sf::VertexArray vertexarray, sf::Vector2f worldPos)
{
	int pb_length = 20;
	PBunit dummy(vertexarray, pb_length);
	dummy.setPosition(worldPos);
	dummy.setColor(sf::Color::Blue);
	m_PBcontainer.push_back(dummy);

}

void PathBlockSystem::windowDraw(sf::RenderWindow& window)
{
	for (auto& n : m_PBcontainer)
		window.draw(n);
}