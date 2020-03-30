#pragma once

#include "Ant.h"
#include "Food.h"
#include "PathBlocker.h"
#include "PathBlockSystem.h"
#include<iostream>
#include<vector>

class Colony;
class Ant;

// Divide world into partitions for collision optimization
// Every partition is esentially a container for ant pointers

// On initialization,
// construct()
// updateCheckIndex()


// On update,
// clearPartition()
// updatePartition()
// updateAntStatus()


struct PartitionUnit
{
	std::vector<Ant*> ant_ptr{200}; //allocate max 200 ant pointers (TEMPORARY!!)
};

class SpatialPartition
{
public:

	std::vector<unsigned int> checkIndex; //indeces of partitions for collision check
	std::vector<PartitionUnit> m_Partition; 

public:

	SpatialPartition();
	~SpatialPartition();
	void initSpatialPartition(int world_width, int world_height, sf::Vector2u resolution_setting);

	// find partitions that contains collision object
	void updateCheckIndex(std::vector<PathBlocker> *pbsystem); // for pblockers
	void updateCheckIndex(PathBlockSystem* pbsystem); // for pblockers (mew)
	void addCheckIndex(Food& newfood); // for newly added food object (temporary)

	void clearPartition(); // clear all ant pointers in all partitions
	void updatePartition(Ant *ant, sf::Vector2f ant_worldPos); // find ants that are in collision partition
	void updateAntStatus(); // update ant check bool

private:
	
	sf::Vector2u m_resolution;
	float m_partition_height;
	float m_partition_width;

private:
	sf::Vector2u mapCoordsToPos(sf::Vector2f worldPos);

};