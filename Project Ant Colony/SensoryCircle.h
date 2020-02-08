#pragma once
#include<SFML/Graphics.hpp>
class SensoryCircle: public sf::CircleShape
{
public:
	float m_angle_indx{};

	//sum of all chem lvl in detected pheromones
	float sum_clvl_food{};
	float sum_clvl_forage{};
	float sum_clvl_danger{};

	// offset from the origin of ant
	float m_offset_x{};
	float m_offset_y{};

	int coeff_weight{0}; //weight (front area is bigger, resulting in higher chance of ant moving forward)
	int coeff_pathing{1}; //pathing coefficient: 1 if path is availabe, 0 if path is blocked.

	float Ci{};//numeratorb probability terms

public:
	void updateCi();
};

