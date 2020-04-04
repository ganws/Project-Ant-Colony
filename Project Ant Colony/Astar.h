#pragma once
#include "Node.h"
#include "PathBlocker.h"
#include "VectorFunction.h"

#include <random>
#include <chrono>
#include <algorithm>
#include <vector>
#include <iostream>
#include <list>
#include <SFML/Graphics.hpp>

class Astar : public sf::Drawable
{
public:
	Astar();
	~Astar();

	void initAstar(int world_width, int world_height, sf::Vector2u resolution, bool diagonal_move);
	void updateObstacleNode(std::vector<PathBlocker>* pblock_system);
	std::vector<sf::Vector2f> computePath(sf::Vector2f startPos, sf::Vector2f endPos);


private:
	std::vector <Node> tileset;
	bool m_diagonal{ false }; 
	int m_num_neighbour = { 4 };
	int m_totalNode = { 0 };
	sf::Vector2u m_resolution;
	sf::VertexArray m_nodeVisual; //to visualize the each node
	int m_node_width;
	int m_node_height;
	int m_world_width{0};
	int m_world_height{0};

private:
	void colorNode(Node* target_node, sf::Color color_init);
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const
	{
		// draw the vertex array
		target.draw(m_nodeVisual, states);
	}

	void updateNeighbourNode(); 

	float computeHeuristic(Node* current_node, Node* goal_node);
	sf::Vector2u mapIndx2Coord(const int indx);
	sf::Vector2u mapCoordsToPos(sf::Vector2f worldPos);
	sf::Vector2f mapPosToCoord(Node* target_node);
	Node* getNode(sf::Vector2f worldPos);
	std::list<Node*> computeNodePath(Node* start_node, Node* goal_node);
	std::vector<sf::Vector2f> getPathFromNode(std::list<Node*> path);
};

