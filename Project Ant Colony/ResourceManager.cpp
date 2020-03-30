#include "ResourceManager.h"

ResourceManager::ResourceManager() {};
ResourceManager::~ResourceManager() {};

void ResourceManager::LoadResource()
{
	if (!antTexture.loadFromFile("walk.png"))
		std::cout << "unable to load skin. \n";
}