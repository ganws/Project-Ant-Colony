#include "Astar.h"

Astar::Astar() {};
Astar::~Astar() {};

void Astar::initAstar(int world_width, int world_height, sf::Vector2u resolution, bool diagonal_move)
{
	this->m_resolution = resolution;
	this->m_totalNode = resolution.x * resolution.y;
	this->m_world_height = world_height;
	this->m_world_width = world_width;

	m_node_width = world_width / resolution.x;
	m_node_height = world_height / resolution.y;

	this->m_diagonal = diagonal_move;

	if (m_diagonal)
		m_num_neighbour = 8;

	//update all node coordinate
	m_Nodes.resize(m_totalNode);
	//for (int i = 0; i < m_totalNode; i++)
		//m_Nodes.push_back(Node());
	this->updateNeighbourNode();
}

sf::Vector2u Astar::mapIndx2Coord(const int indx)
{
	int tmp_x = indx % m_resolution.x;
	int tmp_y = (indx - tmp_x) / m_resolution.x;
	return sf::Vector2u(tmp_x, tmp_y);
}

void Astar::updateNeighbourNode()
{
	for (int j = 0; j < m_resolution.y; j++)
		for (int i = 0; i < m_resolution.x; i++)
		{
			Node* currentNode = &m_Nodes[i + j * m_resolution.x];
			currentNode->x = i;
			currentNode->y = j;

			if (j > 0)
				currentNode->neighbors.push_back(&m_Nodes[i + (j - 1) * m_resolution.x]); //north

			if (i < m_resolution.x - 1)
				currentNode->neighbors.push_back(&m_Nodes[(i + 1) + j * m_resolution.x]); //east

			if (j < m_resolution.y - 1)
				currentNode->neighbors.push_back(&m_Nodes[i + (j + 1) * m_resolution.x]); //south

			if (i > 0)
				currentNode->neighbors.push_back(&m_Nodes[(i - 1) + j * m_resolution.x]); //wst

			if (m_diagonal && j > 0 && i < m_resolution.x - 1)
				currentNode->neighbors.push_back(&m_Nodes[(i + 1) + (j - 1) * m_resolution.x]); //north east

			if (m_diagonal && j > 0 && i > 0)
				currentNode->neighbors.push_back(&m_Nodes[(i - 1) + (j - 1) * m_resolution.x]); //north west

			if (m_diagonal && j < m_resolution.y - 1 && i < m_resolution.x - 1)
				currentNode->neighbors.push_back(&m_Nodes[(i + 1) + (j + 1) * m_resolution.x]); //south east

			if (m_diagonal && j < m_resolution.y - 1 && i >0)
				currentNode->neighbors.push_back(&m_Nodes[(i - 1) + (j + 1) * m_resolution.x]); //south west

			currentNode->num_neighbour = currentNode->neighbors.size();

			//====VISUAL=====//

			m_nodeVisual.setPrimitiveType(sf::Quads);
			m_nodeVisual.resize(m_resolution.x * m_resolution.y * 4);
			//get a pointer to current tile
			sf::Vertex* quad = &m_nodeVisual[(i + j * m_resolution.x) * 4];

			float m_tile_width = m_world_width / m_resolution.x;
			float m_tile_height = m_world_height / m_resolution.y;

			//define 4 corners
			quad[0].position = sf::Vector2f(i * m_tile_width, j * m_tile_height);
			quad[1].position = sf::Vector2f((i + 1) * m_tile_width, j * m_tile_height);
			quad[2].position = sf::Vector2f((i + 1) * m_tile_width, (j + 1) * m_tile_height);
			quad[3].position = sf::Vector2f(i * m_tile_width, (j + 1) * m_tile_height);

			//set all to red
			quad[0].color = sf::Color::Transparent;
			quad[1].color = sf::Color::Transparent;
			quad[2].color = sf::Color::Transparent;
			quad[3].color = sf::Color::Transparent;

			//set all to transparent
			quad[0].color.a = 0;
			quad[1].color.a = 0;
			quad[2].color.a = 0;
			quad[3].color.a = 0;
		}
}

float Astar::computeHeuristic(Node* a, Node* b)
{
	//printf("a.x = %d, a.y = %d \n", a->x, a->y);
	//printf("b.x = %d, b.y = %d \n", b->x, b->y);
	float tmp  = sqrt((a->x - b->x) * (a->x - b->x) + (a->y - b->y) * (a->y - b->y));

	return  tmp;
}

std::list<Node*> Astar::computeNodePath(Node* start_node, Node* goal_node)
{
	//reset all nodes
	for (auto& n : m_Nodes)
	{
		n.globalCost = INFINITY;
		n.localCost = INFINITY;
		n.parent = nullptr;
		n.visited = false;
		colorNode(&n, sf::Color::Transparent);
	}


	//Init start node
	start_node->globalCost = computeHeuristic(start_node, goal_node);
	start_node->localCost = 0;

	Node* currentNode = start_node;
	std::list<Node*> ListNotTested{};
	ListNotTested.push_back(start_node);
	//std::cout << "size of list " << ListNotTested.size() << "\n";

	while (!ListNotTested.empty())
	{
		// sort list by global cost in acesnding order
		//ListNotTested.sort([](const Node* lhs, const Node* rhs) {return lhs->globalCost < rhs->globalCost; });

		std::vector<Node*> list_Vec;
		for (auto a : ListNotTested)
			list_Vec.push_back(a);

		//randomize list
		//unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
		//shuffle(list_Vec.begin(), list_Vec.end(), std::default_random_engine(seed));

		//ListNotTested.clear();
		//for (int m = 0; m < list_Vec.size(); m++)
		//	ListNotTested.push_back( list_Vec[m]);


		while (!ListNotTested.empty() && ListNotTested.front()->visited)
			ListNotTested.pop_front();

		if (ListNotTested.empty())
			break;

		currentNode = ListNotTested.front();
		currentNode->visited = true;

		//check all the neighbors
		//printf("checking node[%d][%d]\n", currentNode->x, currentNode->y);

		for (auto neighbor : currentNode->neighbors)
		{
			if (!neighbor->visited && !neighbor->obstacle)
				ListNotTested.push_back(neighbor);

			float localCost_tmp = currentNode->localCost + computeHeuristic(currentNode, neighbor);

			// update if it is a better node
			if (localCost_tmp < neighbor->localCost)
			{
				//update costs
				neighbor->parent = currentNode;
				neighbor->localCost = localCost_tmp;
				neighbor->globalCost = neighbor->localCost + computeHeuristic(neighbor, goal_node);
			}

			if (goal_node->parent != NULL)
				break;
			//printf("Neighbor[%d] localCost = %f, globalCost = %f \n", neighbor->x + neighbor-> y * m_resolution.x, neighbor->localCost, neighbor->globalCost);
		}
	}

	std::list<Node*> ShortestPath;
	Node* current_trace = goal_node;

	while (current_trace != nullptr)
	{
		ShortestPath.push_front(current_trace);
		//printf("trace: [%d]\n", current_trace->x+ current_trace->y*m_resolution.x);
		current_trace = current_trace->parent;
	}

	if (ShortestPath.front() == start_node)
	{
		std::cout << "SHORTEST DISTANCE = " << ShortestPath.size() -1 << "\n";
		for (auto p : ShortestPath)
		{
			//std::cout << p->x + p->y * m_resolution.x;
			if (p != goal_node)
				//std::cout << " >> ";
				colorNode(p, sf::Color::Cyan);
		}
	}
	else
	{
		std::cout << "PATH NOT FOUND!\n";
	}

	//VISUAL
	colorNode(start_node, sf::Color::Green);
	colorNode(goal_node, sf::Color::Yellow);

	return ShortestPath;
}

void Astar::updateObstacleNode(std::vector<PathBlocker>* pbsytem)
{
	for (auto& n : *pbsytem)
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
			sf::Vector2u nodePos;
			nodePos = mapCoordsToPos(n);
			m_Nodes[nodePos.x + nodePos.y * m_resolution.x].obstacle = true;
		}
	}
}

sf::Vector2u Astar::mapCoordsToPos(sf::Vector2f worldPos)
{
	sf::Vector2u nodePos;
	float x_tmp = worldPos.x;
	float y_tmp = worldPos.y;

	if (x_tmp < 0) x_tmp = 0.0;
	if (y_tmp < 0) y_tmp = 0.0;

	

	nodePos.x = static_cast<int>(x_tmp / m_node_width);
	nodePos.y = static_cast<int>(y_tmp / m_node_height);

	//  out-of-bound index error prevention
	if ((nodePos.x) >= m_resolution.x)
	{
		nodePos.x = (m_resolution.x - 1);
		//std::cout << "outbound correctin\n";
	}
	if ((nodePos.y) >= m_resolution.y)
	{
		nodePos.y = (m_resolution.y - 1);
		//std::cout << "outbound correctin\n";
	}

	//printf("worldPos[%f %f] -> tilePos[%d %d]\n", worldPos.x, worldPos.y, nodePos.x, nodePos.y);

	return nodePos;
}
sf::Vector2f Astar::mapPosToCoord(Node* target_node)
{
	return sf::Vector2f(target_node->x * m_node_width + m_node_width / 2, target_node->y * m_node_height + m_node_height / 2);
}


void Astar::colorNode(Node* target_node, sf::Color color_input)
{
	sf::Vertex* quad = &m_nodeVisual[(target_node->x + target_node->y * m_resolution.x)*4];
	quad[0].color = color_input;
	quad[1].color = color_input;
	quad[2].color = color_input;
	quad[3].color = color_input;
}

Node* Astar::getNode(sf::Vector2f worldPos)
{
	sf::Vector2u tmp = mapCoordsToPos(worldPos);
	return &m_Nodes[tmp.x + tmp.y * m_resolution.x];
}

std::vector<sf::Vector2f> Astar::getPathFromNode(std::list<Node*> path)
{
	std::vector<sf::Vector2f> path_wordcoord; //path in word coordninate
	for (auto n : path)
	{
		path_wordcoord.push_back(mapPosToCoord(n));
	}

	return path_wordcoord;
}

std::vector<sf::Vector2f> Astar::computePath(sf::Vector2f startPos, sf::Vector2f endPos)
{
	sf::Vector2u startIndx = this->mapCoordsToPos(startPos);
	sf::Vector2u endIndx = this->mapCoordsToPos(endPos);

	Node* startNode = getNode(startPos);
	Node* endNode = getNode(endPos);

	std::list<Node*> nodePath = computeNodePath(startNode, endNode);
	std::vector<sf::Vector2f> worldPath = getPathFromNode(nodePath);

	return worldPath;

}