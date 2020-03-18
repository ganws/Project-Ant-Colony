#pragma once
#include<sfml/Graphics.hpp>
#include<vector>
#include <iostream>

class PheroMatrix : public sf::Drawable
{
public:
	PheroMatrix();
	~PheroMatrix();
	void initPheroMatrix(int world_width, int world_height, sf::Vector2u resolution_param);
	void setDecayRate(float value_set);
	void pheromoneDecay(float dt);
	void addStrength(sf::Vector2f worldPos, float input_str);
	float getStrengh(sf::Vector2f worldPos);
	sf::Vector2u mapCoordsToPos(sf::Vector2f worldPos);
	void resetPheromone();

private:
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const
	{
		// draw the vertex array
		target.draw(m_tilevertices, states);
	}

	int m_tile_width;
	int m_tile_height;
	float m_decay_rate{ 1.0 };
	float m_initial_strength{ 100.0f }; //initial strength of newly created particle
	sf::Vector2u m_resolution; //num of tiles *
	std::vector<float> m_strengthmatrix; //2d array
	sf::VertexArray m_tilevertices; //to visualize the strength of each tile
};
