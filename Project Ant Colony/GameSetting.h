#ifndef GAMESETTING_H
#define GAMESETTING_H

#include<SFML/Graphics.hpp>

namespace GameSetting
{
	extern const int windowHeight;
	extern const int windowWidth;

	extern const float GAMESPEED; //normal speed = 1.0
	extern const float FRAMERATE; //default = 60fps

	extern const sf::Vector2f nullVector;
	extern const sf::Vector2f oneVector;
}
#endif // !GAMESETTING_H
