#pragma once
#include <math.h>
#include <vector>
#include <SFML/Graphics.hpp>

struct Node
{
public:

	Node();
	~Node();
	std::vector <Node*> neighbors;
	Node* parent{nullptr};
	bool obstacle{false};
	bool visited{};
	int num_neighbour{ 0 };
	//coordinate
	int x;
	int y;

	float globalCost{ INFINITY }; //exact cost
	float localCost{ INFINITY }; //heuristic cost

private:

};

