#pragma once
#include<SFML/Graphics.hpp>

class tmpPhero:public sf::CircleShape
{
public:
	float m_strength{100.0f};

public:
	tmpPhero();
	~tmpPhero();
};

