#pragma once
#include "PheroMatrix.h"

PheroMatrix::PheroMatrix() {}
PheroMatrix::~PheroMatrix() {}

void PheroMatrix::initPheroMatrix(int world_width, int world_height, sf::Vector2u resolution_param)
{

	// throw an error if world dimension is not divisible by resolution
	if (static_cast<bool>(world_height % resolution_param.y + world_width % resolution_param.x))
		std::cout << "ERROR: Pheromone Matrix- World dimension must be perfectly divisible by resolution\n";

	//Initializatoin
	m_resolution.x = resolution_param.x;
	m_resolution.y = resolution_param.y;

	//calculate tile dimensions
	m_tile_height = world_height / m_resolution.y;
	m_tile_width = world_width / m_resolution.x;
	
	m_strengthmatrix.resize(m_resolution.x * m_resolution.y);

	m_tilevertices.setPrimitiveType(sf::Quads);
	m_tilevertices.resize(m_resolution.x * m_resolution.y * 4);

	for (unsigned int i = 0; i < m_resolution.x; i++)
		for (unsigned int j = 0; j < m_resolution.y; j++)
		{
			//get a pointer to current tile
			sf::Vertex* quad = &m_tilevertices[(i + j * m_resolution.x) * 4];

			//define 4 corners
			quad[0].position = sf::Vector2f(i * m_tile_width, j * m_tile_height);
			quad[1].position = sf::Vector2f((i + 1) * m_tile_width, j * m_tile_height);
			quad[2].position = sf::Vector2f((i + 1) * m_tile_width, (j + 1) * m_tile_height);
			quad[3].position = sf::Vector2f(i * m_tile_width, (j + 1) * m_tile_height);

			//set all to red
			quad[0].color = sf::Color::Red;
			quad[1].color = sf::Color::Red;
			quad[2].color = sf::Color::Red;
			quad[3].color = sf::Color::Red;

			//set all to transparent
			quad[0].color.a = 0;
			quad[1].color.a = 0;
			quad[2].color.a = 0;
			quad[3].color.a = 0;
		}
}

void PheroMatrix::setDecayRate(float set_value) { m_decay_rate = set_value; }
void PheroMatrix::pheromoneDecay(float dt)
{
	//update whole matrix
	float alpha;

	for (int i = 0; i < m_resolution.x; i++)
		for (int j = 0; j < m_resolution.y; j++)
		{
			{
				float& str = m_strengthmatrix[i + j * m_resolution.x];
				str = str - m_decay_rate * dt;
				if (str <= 0)
					str = 0.0;

				alpha = str / (m_initial_strength * 5); //maximum opacity= 5 stacked instances
				if (alpha > 1.0) alpha = 1.0;
				alpha *= 255;

				//visual update
				sf::Vertex* quad = &m_tilevertices[(i + j * m_resolution.x) * 4];
				quad[0].color.a = alpha;
				quad[1].color.a = alpha;
				quad[2].color.a = alpha;
				quad[3].color.a = alpha;
			}
		}
}

void PheroMatrix::addStrength(sf::Vector2f worldPos, float input_str)
{
	float maxStrength = 400;
	sf::Vector2u tilePos = mapCoordsToPos(worldPos); //convert world coordinate to tile position
	float& str = m_strengthmatrix[tilePos.x + tilePos.y * m_resolution.x];
	str = str + input_str;
	if (str >= maxStrength)
		str = maxStrength;
}

sf::Vector2u PheroMatrix::mapCoordsToPos(sf::Vector2f worldPos)
{
	sf::Vector2u tilePos;
	float x_tmp = worldPos.x;
	float y_tmp = worldPos.y;

	if (x_tmp < 0) x_tmp = 0.0;
	if (y_tmp < 0) y_tmp = 0.0;

	tilePos.x = static_cast<unsigned int>(x_tmp / m_tile_width);
	tilePos.y = static_cast<unsigned int>(y_tmp / m_tile_height);

	// subscript out of bound management

	if ((tilePos.x) >= m_resolution.x)
	{
		tilePos.x = (m_resolution.x - 1);
		//std::cout << "outbound correctin\n";
	}
	if ((tilePos.y) >= m_resolution.y)
	{
		tilePos.y = (m_resolution.y - 1);
		//std::cout << "outbound correctin\n";
	}

	//printf("worldPos[%f %f] -> tilePos[%d %d]\n", worldPos.x, worldPos.y, tilePos.x, tilePos.y);

	return tilePos;
}

float PheroMatrix::getStrengh(sf::Vector2f worldPos)
{
	sf::Vector2u tilePos = mapCoordsToPos(worldPos);
	return m_strengthmatrix[tilePos.x + tilePos.y * m_resolution.x];
}

void PheroMatrix::resetPheromone()
{
	for (int i = 0; i < m_resolution.x; i++)
		for (int j = 0; j < m_resolution.y; j++)
			m_strengthmatrix[i + j * m_resolution.x] = 0;

}