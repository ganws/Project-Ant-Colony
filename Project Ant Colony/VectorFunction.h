#pragma once
#include <SFML/Graphics.hpp>
#include<math.h>

float dotProduct(sf::Vector2f v1, sf::Vector2f v2); // dotproduct
float normVector(sf::Vector2f v); //compute vector length
float vectorAngle(sf::Vector2f v1, sf::Vector2f v2); //angle between two vectors
sf::Vector2f scalarProduct(sf::Vector2f v, float scalar); //scalar product
sf::Vector2f unitVector(sf::Vector2f v); //compute unit vector of v1
float dist2Vec(sf::Vector2f a, sf::Vector2f b); //euclidean dist betweeen 2 points

inline sf::Vector2f& operator * (const sf::Vector2f& left, int right)
{
	sf::Vector2f tmp(right * left.x, right * left.y);
	return tmp;
}

inline sf::Vector2f& operator * (int left, const sf::Vector2f& right)
{
	sf::Vector2f tmp(left * right.x, left * right.y);
	return tmp;
}