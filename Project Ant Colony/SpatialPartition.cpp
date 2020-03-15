#include "SpatialPartition.h"

void SpatialPartition::initSpatialPartition(int world_width, int world_height, sf::Vector2u resolution_param)
{
	// throw an error if world dimension is not divisible by resolution
	if (static_cast<bool>(world_height % resolution_param.y + world_width % resolution_param.x))
		std::cout << "ERROR: Pheromone Matrix- World dimension must be perfectly divisible by resolution\n";

	//Initializatoin
	m_resolution.x = resolution_param.x;
	m_resolution.y = resolution_param.y;

	//dimension of a partition unit
	m_partition_height = world_height / m_resolution.y;
	m_partition_width = world_width / m_resolution.x;
	
	m_Partition.resize(m_resolution.x * m_resolution.y);

	checkIndex.resize(m_resolution.x * m_resolution.y);
}

sf::Vector2u SpatialPartition::mapCoordsToPos(sf::Vector2f worldPos)
{
	sf::Vector2u tilePos;
	float x_tmp = worldPos.x;
	float y_tmp = worldPos.y;

	if (x_tmp < 0) x_tmp = 0.0;
	if (y_tmp < 0) y_tmp = 0.0;

	tilePos.x = static_cast<unsigned int>(x_tmp / m_partition_width);
	tilePos.y = static_cast<unsigned int>(y_tmp / m_partition_height);

	//  out-of-bound index error prevention
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

void SpatialPartition::updatePartition(Ant *ant, sf::Vector2f ant_worldPos)
{
	//add ant pointer to appropriate partition
	sf::Vector2u pos = mapCoordsToPos(ant_worldPos);
	m_Partition[pos.x + pos.y * m_resolution.x].ant_ptr.push_back(ant);
}

void SpatialPartition::clearPartition()
{
	for (int i=0;i < m_resolution.x ; i++)
		for (int j = 0; j < m_resolution.y; j++)
		{
			m_Partition[i + j * m_resolution.x].ant_ptr.clear();
		}	
}


void SpatialPartition::updateCheckIndex(std::vector<PathBlocker> *pbsystem)
{
	for (auto& n : *pbsystem)
	{
		float length = n.getSize().x;
		sf::Vector2f center = n.getPosition();

		//calculate all 4 corner positions
		sf::Vector2f cornersPos[4]{};
		cornersPos[0] = center + sf::Vector2f(-length / 2, -length / 2); //top left
		cornersPos[1] = center + sf::Vector2f(length / 2, -length / 2); //top right
		cornersPos[2] = center + sf::Vector2f(length / 2, length / 2); //bottom right
		cornersPos[3] = center + sf::Vector2f(-length / 2, length / 2); //bottom left

		//check all 4 corners 
		for (auto& n : cornersPos)
		{
			sf::Vector2u partitionPos;
			partitionPos = mapCoordsToPos(n);
			checkIndex.push_back(partitionPos.x + partitionPos.y* m_resolution.x); 
		}
	}

	//remove duplicated indeces
	std::sort(checkIndex.begin(), checkIndex.end());
	checkIndex.erase(std::unique(checkIndex.begin(), checkIndex.end()), checkIndex.end());

	for (auto& n : checkIndex)
		std::cout << n << "\n";
}

void SpatialPartition::updateAntStatus()
{
	for (int n : checkIndex)
	{
		for (auto& ant : m_Partition[n].ant_ptr)
		{
			ant->collision_check = true;
		}
	}
}