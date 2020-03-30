#pragma once

#include "PathBlocker.h"
#include "GameSetting.h"
#include "PBunit.h"

#include <iostream>
#include <SFML/Graphics.hpp>

class PathBlockSystem
{
	typedef std::vector<PBunit> Pbcontainer;

public:
	Pbcontainer m_PBcontainer;

public:
	PathBlockSystem();
	~PathBlockSystem();
	void drawMapBoundary();
	void windowDraw(sf::RenderWindow& window);
	void addPBunit(sf::VertexArray vertexarray, sf::Vector2f worldPos);

};

