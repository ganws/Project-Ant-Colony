#pragma once
#include"VectorFunction.h"

float normVector (sf::Vector2f v)
{
	if (isnan(sqrt(v.x * v.x + v.y * v.y)))
		std::cout << "SQRT NAN DETECTED\n";
	return sqrt(v.x*v.x + v.y*v.y);
}

float dotProduct(sf::Vector2f v1, sf::Vector2f v2)
{
	return (v1.x * v2.x) + (v1.y * v2.y);
}

float vectorAngle(sf::Vector2f v1, sf::Vector2f v2)
{
	float cosA = dotProduct(v1, v2) / (normVector(v1) * normVector(v2));
	if (cosA > 1.0f)
	{
		cosA = 0.9999f;
		std::cout << "NAN DETECTED and CORRECTED \n" ;
	}
	if (cosA < -1.0f)
	{
		cosA = -0.9999f;
		std::cout << "-NAN DETECTED and CORRECTED \n";
	}
	
	return acos(-cosA);
}

sf::Vector2f unitVector(sf::Vector2f v)
{
	return v / sqrt(v.x * v.x + v.y * v.y);
}

sf::Vector2f scalarProduct(sf::Vector2f v, float scalar)
{
	return sf::Vector2f(v.x * scalar, v.y * scalar);
}

float dist2Vec(sf::Vector2f a, sf::Vector2f b)
{
	return sqrt((a.x - b.x) * (a.x - b.x) + (a.y - b.y) * (a.y - b.y));
}
