#pragma once
#include <SFML/Graphics.hpp>
#include <iostream>

class ResourceManager
{
public:

	sf::Texture antTexture;

public:

	ResourceManager();
	~ResourceManager();

	void LoadResource();

};

