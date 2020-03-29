#include "Terrain.h"

Terrain::Terrain(float world_width, float world_height, sf::Vector2u resolution)
{
	this->m_resolution = resolution;
	this->m_totalTiles = resolution.x * resolution.y;
	this->m_world_height = world_height;
	this->m_world_width = world_width;

	m_tile_width = world_width / resolution.x;
	m_tile_height = world_height / resolution.y;

	m_pathCoeff.resize(m_resolution.x * m_resolution.y);
	m_collisionCoeff.resize(m_resolution.x * m_resolution.y);
	//init all coeff
	for (int i = 0; i < m_resolution.y; i++)
		for (int j = 0; j < m_resolution.y; j++)
		{

			m_pathCoeff[i + j * m_resolution.x] = 1;
			m_collisionCoeff[i + j * m_resolution.x] = 1;

			//====VISUAL=====//
			m_pathVisual.setPrimitiveType(sf::Quads);
			m_pathVisual.resize(m_resolution.x * m_resolution.y * 4);
			//get a pointer to current tile
			sf::Vertex* quad = &m_pathVisual[(i + j * m_resolution.x) * 4];

			float m_tile_width = m_world_width / m_resolution.x;
			float m_tile_height = m_world_height / m_resolution.y;

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
};

Terrain::~Terrain() {};

int Terrain::getTerrainCoeff(sf::Vector2f worldpos)
{
	sf::Vector2u coord = mapCoordsToPos(worldpos);
	return m_pathCoeff[coord.x + coord.y* m_resolution.x];
}

int Terrain::getCollisionCoeff(sf::Vector2f worldpos)
{
	sf::Vector2u coord = mapCoordsToPos(worldpos);
	return m_collisionCoeff[coord.x + coord.y * m_resolution.x];
}

sf::Vector2u Terrain::mapIndx2Coord(const int indx)
{
	int tmp_x = indx % m_resolution.x;
	int tmp_y = (indx - tmp_x) / m_resolution.x;
	return sf::Vector2u(tmp_x, tmp_y);
};

void Terrain::updateCoeff(PathBlocker& new_pathblocker)
{

	sf::Vector2f pb_size = new_pathblocker.getSize();
	
	//get 4 corners in tilePos
	sf::Vector2u topleft = mapCoordsToPos(new_pathblocker.getPosition() + sf::Vector2f(-pb_size.x / 2, -pb_size.y / 2));
	sf::Vector2u botright = mapCoordsToPos(new_pathblocker.getPosition() + sf::Vector2f(pb_size.x / 2, pb_size.y / 2));

	for (int x = topleft.x; x <= botright.x; x++)
		for (int y = topleft.y; y <= botright.y; y++)
		{
			m_pathCoeff[x + y * m_resolution.x] = 0;
			colorPath(x, y, sf::Color::Red, 255);
		}
}

void Terrain::updateAntCoeff(Ant& ant, int coeff)
{

	sf::Vector2f ant_size = ant.m_size;
	ant_size = scalarProduct(ant_size, 0.2f);

	//get 4 corners in tilePos
	sf::Vector2u topleft = mapCoordsToPos(ant.getPosition() + sf::Vector2f(-ant_size.x / 2, -ant_size.y / 2));
	sf::Vector2u botright = mapCoordsToPos(ant.getPosition() + sf::Vector2f(ant_size.x / 2, ant_size.y / 2));

	for (unsigned int x = topleft.x; x <= botright.x; x++)
		for (unsigned int y = topleft.y; y <= botright.y; y++)
		{
			m_collisionCoeff[x + y * m_resolution.x] = coeff;
		}
}

void Terrain::clearAntCoeff(Ant& ant)
{

	sf::Vector2f ant_size = ant.m_size;
	ant_size = scalarProduct(ant_size, 0.2);
	sf::Vector2f ant_las_pos = ant.getLastPos();

	//get 4 corners in tilePos
	sf::Vector2u topleft = mapCoordsToPos(ant_las_pos + sf::Vector2f(-ant_size.x / 2, -ant_size.y / 2));
	sf::Vector2u botright = mapCoordsToPos(ant_las_pos + sf::Vector2f(ant_size.x / 2, ant_size.y / 2));

	for (unsigned int x = topleft.x; x <= botright.x; x++)
		for (unsigned int y = topleft.y; y <= botright.y; y++)
		{
			m_collisionCoeff[x + y * m_resolution.x] = 1;
		}
}

sf::Vector2u Terrain::mapCoordsToPos(sf::Vector2f worldPos)
{
	sf::Vector2u pathPos;
	float x_tmp = worldPos.x;
	float y_tmp = worldPos.y;

	if (x_tmp < 0) x_tmp = 0.0;
	if (y_tmp < 0) y_tmp = 0.0;



	pathPos.x = static_cast<int>(x_tmp / m_tile_width);
	pathPos.y = static_cast<int>(y_tmp / m_tile_height);

	//  out-of-bound index error prevention
	if ((pathPos.x) >= m_resolution.x)
	{
		pathPos.x = (m_resolution.x - 1);
	}
	if ((pathPos.y) >= m_resolution.y)
	{
		pathPos.y = (m_resolution.y - 1);
	}

	//printf("worldPos[%f %f] -> tilePos[%d %d]\n", worldPos.x, worldPos.y, nodePos.x, nodePos.y);
	return pathPos;
}

//sf::Vector2f Astar::mapPosToCoord(Node* target_node)
//{
//	return sf::Vector2f(target_node->x * m_node_width + m_node_width / 2, target_node->y * m_node_height + m_node_height / 2);
//}


void Terrain::colorPath(int u, int v, sf::Color color_input, unsigned int alpha_input)
{
	sf::Vertex* quad = &m_pathVisual[(u + v * m_resolution.x) * 4];
	quad[0].color = color_input;
	quad[1].color = color_input;
	quad[2].color = color_input;
	quad[3].color = color_input;

	quad[0].color.a = alpha_input;
	quad[1].color.a = alpha_input;
	quad[2].color.a = alpha_input;
	quad[3].color.a = alpha_input;
	

}

