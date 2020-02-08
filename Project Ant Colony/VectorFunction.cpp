#pragma once
#include"VectorFunction.h"

float normVector (sf::Vector2f v)
{
	return sqrt(v.x*v.x + v.y*v.y);
}

float dotProduct(sf::Vector2f v1, sf::Vector2f v2)
{
	return (v1.x * v2.x) + (v1.y * v2.y);
}

float vectorAngle(sf::Vector2f v1, sf::Vector2f v2)
{
	float cosA = dotProduct(v1, v2) / (normVector(v1) * normVector(v2));
		return acos(-cosA);
}

sf::Vector2f unitVector(sf::Vector2f v)
{
	return v / sqrt(v.x * v.x + v.y * v.y);
}