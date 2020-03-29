#pragma once

#include "PathBlocker.h"
#include "Ant.h"

#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>

class Ant;

class Terrain : public sf::Drawable
{
public:
	std::vector<int> m_pathCoeff;
	std::vector<int> m_collisionCoeff;

public:
	Terrain(float game_width, float game_height, sf::Vector2u resolution);
	~Terrain();
	void colorPath(int u, int v, sf::Color color_input, unsigned int alpha_input=255);
	sf::Vector2u mapIndx2Coord(const int indx);
	sf::Vector2u mapCoordsToPos(sf::Vector2f worldPos);
	void updateCoeff(PathBlocker& new_pathblocker);

	void updateAntCoeff(Ant& ant, int coeff);
	void clearAntCoeff(Ant& ant);

	int getTerrainCoeff(sf::Vector2f worldpos);
	int getCollisionCoeff(sf::Vector2f worldpos);

private:
	int m_totalTiles = { 0 };
	sf::Vector2u m_resolution;
	sf::VertexArray m_pathVisual; //to visualize the each node
	int m_tile_width;
	int m_tile_height;
	int m_world_width{ 0 };
	int m_world_height{ 0 };

private:
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const
	{
		// draw the vertex array
		target.draw(m_pathVisual, states);
	}
	
};

