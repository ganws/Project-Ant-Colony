#pragma once
#include<SFML/Graphics.hpp>

class Animation
{
public:
	Animation(sf::Texture* texture, sf::Vector2u ImageCount, int imageRealCount, float switchTime);
	~Animation();

	void Update(float deltaTime);

public:
	sf::IntRect m_uvRect;

private:
	sf::Vector2u m_imageCount;
	int m_imageRealCount;
	int m_currentIndex;
	sf::Vector2u m_currentImage;

	float m_totalTime;
	float m_switchTime;
};

