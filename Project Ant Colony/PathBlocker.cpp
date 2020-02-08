#include "PathBlocker.h"

//default constructor
PathBlocker::PathBlocker()
{
	this->setSize(sf::Vector2f(20, 20));
	this->setFillColor(sf::Color::Transparent);
	this->setOrigin(getSize().x/2, getSize().y / 2);
}
PathBlocker::PathBlocker(sf::Vector2f initPos, sf::Color initColor, float initLength)
{
	this->setSize(sf::Vector2f(initLength, initLength));
	this->setFillColor(initColor);
	this->setOrigin(getSize().x / 2, getSize().y / 2);
	this->setPosition(initPos);

}


PathBlocker::~PathBlocker() {};

void PathBlocker::initPathBlocker(sf::Vector2f initPos, sf::Color initColor, float init_length)
{
	this->setSize(sf::Vector2f(init_length, init_length));
	this->setFillColor(initColor);
	this->setOrigin(getSize().x / 2, getSize().y / 2);
	this->setPosition(initPos);

}
