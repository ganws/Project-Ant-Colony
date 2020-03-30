#pragma once
#include<sfml/Graphics.hpp>
#include<vector>
#include <iostream>

class PheroMatrix : public sf::Drawable
{
public:
	PheroMatrix();
	~PheroMatrix();
	void initPheroMatrix(float world_width, float world_height, sf::Vector2u resolution_param);
	void setDecayRate(float value_set);
	void pheromoneDecay(float dt);
	void addStrength(sf::Vector2f worldPos, float input_str);
	float getStrengh(sf::Vector2f worldPos);
	sf::Vector2u mapCoordsToPos(sf::Vector2f worldPos);
	void resetPheromone();

private:
	std::vector<float> m_strengthmatrix; //2d array

	int m_tile_width;
	int m_tile_height;

	float m_decay_rate{ 1.2f };
	float m_initial_strength{ 100.0f }; //initial strength of newly created particle

	sf::Vector2u m_resolution; //num of tiles *
	sf::VertexArray m_tilevertices; //to visualize the strength of each tile

private:

	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const
	{
		// draw the vertex array
		target.draw(m_tilevertices, states);
	}

};
