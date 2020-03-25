#pragma once

#include "Ant.h"
#include "Food.h"
#include "PathBlocker.h"
#include<iostream>
#include<vector>

class Colony;
class Ant;
struct PartitionUnit
{
	std::vector<Ant*> ant_ptr{200};
};

class SpatialPartition
{

public:

	std::vector<unsigned int> checkIndex;
	std::vector<PartitionUnit> m_Partition;

	void initSpatialPartition(int world_width, int world_height, sf::Vector2u resolution_setting);
	sf::Vector2u mapCoordsToPos(sf::Vector2f worldPos);
	void updatePartition(Ant *ant, sf::Vector2f ant_worldPos);
	void clearPartition();
	void updateCheckIndex(std::vector<PathBlocker> *pbsystem);
	void addCheckIndex(Food& newfood);
	void updateAntStatus();

private:
	
	sf::Vector2u m_resolution;
	float m_partition_height;
	float m_partition_width;

};