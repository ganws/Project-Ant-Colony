#pragma once
#include <SFML/Graphics.hpp>
#include<math.h>

float dotProduct(sf::Vector2f v1, sf::Vector2f v2); // dotproduct
float normVector(sf::Vector2f v); //compute vector length
float vectorAngle(sf::Vector2f v1, sf::Vector2f v2); //angle between two vectors
sf::Vector2f scalarProduct(sf::Vector2f v, float scalar); //scalar product
sf::Vector2f unitVector(sf::Vector2f v); //compute unit vector of v1